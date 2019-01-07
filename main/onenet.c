/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "soc/rtc_cntl_reg.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "mqtt_client.h"
#include "os.h"
#include "onenet.h"
#include "sniffer.h"

static bool onenet_initialised = false;
static TaskHandle_t xOneNetTask = NULL;

extern station_info_t *g_station_list;
extern int s_device_info_num;
station_info_t *station_info_one = NULL;

void onenet_task(void *param)
{
    esp_mqtt_client_handle_t client = *(esp_mqtt_client_handle_t *)param;
	char  val_mac_timestamp_rssi[20];
	

    while (1) {
        char buf[128];
        s_device_info_num = 0;

        for (station_info_one = g_station_list->next; station_info_one; station_info_one = g_station_list->next) {
            g_station_list->next = station_info_one->next;
			
			/*transfer the value of mac,rssi and time to a sting for the convinience of excibition */
        	memset(buf, 0, sizeof(buf));
			/*combine mac data with rssi and timestamp for the convenience to exhibit*/
			/*the value of mac timestamp and rssi*/			
			sprintf(val_mac_timestamp_rssi,"%x%x%x%x%x%x_%d_%d",station_info_one->bssid[0],station_info_one->bssid[1],station_info_one->bssid[2],station_info_one->bssid[3],station_info_one->bssid[4],station_info_one->bssid[5],station_info_one->timestamp,station_info_one->rssi);
			/*buf get the message of mac time and rssi*/
			sprintf(&buf[3], "{\"check_pedestrian_flow\":\"%s\"}", val_mac_timestamp_rssi);
			uint16_t len = strlen(&buf[3]);
        	buf[0] = 0x03;
        	buf[1] = len >> 8;
        	buf[2] = len & 0xFF;
			esp_mqtt_client_publish(client, "$dp", buf, len + 3, 0, 0);
			free(station_info_one);
        }
		/*this function can ajust the publish time*/
        vTaskDelay((unsigned long long)ONENET_PUB_INTERVAL * 1000 / portTICK_RATE_MS);
    }
}

void onenet_start(esp_mqtt_client_handle_t client)
{
    if (!onenet_initialised) {
        xTaskCreate(&onenet_task, "onenet_task", 2048, &client, 6, &xOneNetTask);
        onenet_initialised = true;
    }
}

void onenet_stop(esp_mqtt_client_handle_t client)
{
    if (onenet_initialised) {
        if (xOneNetTask) {
            vTaskDelete(xOneNetTask);
        }

        onenet_initialised = false;
    }
}
