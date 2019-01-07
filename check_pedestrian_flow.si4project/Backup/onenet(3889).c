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
    uint32_t 	val_stamp;
	//char 		val_mac[6];
	uint32_t 	val_temp=0,val=0;
	uint8_t 	val_rssi,val_mac_2=0;
	

    while (1) {
        //val =  s_device_info_num ;
        char buf[128];
        //memset(buf, 0, sizeof(buf));
        //sprintf(&buf[3], "{\"%s\":%d}", ONENET_DATA_STREAM, val);
        //uint16_t len = strlen(&buf[3]);
        //buf[0] = data_type_simple_json_without_time;
        //buf[1] = len >> 8;
        //buf[2] = len & 0xFF;

        //esp_mqtt_client_publish(client, "$dp", buf, len + 3, 0, 0);
        s_device_info_num = 0;

        for (station_info_one = g_station_list->next; station_info_one; station_info_one = g_station_list->next) {
            g_station_list->next = station_info_one->next;
			/*transfer the value of mac,rssi and time to a sting for the convinience of excibition */
        	memset(buf, 0, sizeof(buf));
			/*combine mac data with rssi for the convenience to exhibit*/
			/*mac*/
			
			//sprintf(val_mac,"%x%x%x%x%x%x",station_info_one->bssid[0],station_info_one->bssid[1],station_info_one->bssid[2],station_info_one->bssid[3],station_info_one->bssid[4],station_info_one->bssid[5]);
			val_stamp=station_info_one->timestamp;
			val_rssi=station_info_one->rssi;
		
			val_temp=(uint32_t)station_info_one->bssid[5];
			val=val_temp+val;
			val_temp=(uint32_t)station_info_one->bssid[4];
			val=(val_temp<<8)+val;
			val_temp=(uint32_t)station_info_one->bssid[3];
			val=(val_temp<<16)+val;
			val_temp=(uint32_t)station_info_one->bssid[2];
			val=(val_temp<<24)+val;

			
			val_temp=(uint32_t)station_info_one->bssid[1];
			val_mac_2=val_temp+val_mac_2;
			val_temp=(uint32_t)station_info_one->bssid[0];
			val_mac_2=(val_temp<<8)+val_mac_2;
			
			/*transfer the format of data*/
			//sprintf(&buf[3],"{\"datastreams\":[{\"id\":\"MAC\",\"datapoints\":[{\"value\":30}]}]}");
			sprintf(&buf[3],"{\"datastreams\":[{\"id\":\"MAC\", \"datapoints\":[{\"value\":%d}]},{\"id\":\"STAMP\", \"datapoints\":[{\"value\":%d}]},{\"id\":\"RSSI\", \"datapoints\":[{\"value\":%d}]}]}",val,val_stamp,val_rssi);
			//sprintf(&buf[3], "{\"MAC\":%d}", val);
			uint16_t len = strlen(&buf[3]);
        	buf[0] = 0x01;
        	buf[1] = len >> 8;
        	buf[2] = len & 0xFF;
			//printf("len=%x;buf[1]=%x;buf[2]=%x\n",len,buf[1],buf[2]);//test flag ...........
			esp_mqtt_client_publish(client, "$dp", buf, len + 3, 0, 0);
			free(station_info_one);
        }
		/*this function can ajust the publish time*/
        vTaskDelay((unsigned long long)ONENET_PUB_INTERVAL * 100 / portTICK_RATE_MS);
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
