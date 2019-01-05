deps_config := \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/app_trace/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/aws_iot/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/bt/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/driver/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/esp32/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/esp_adc_cal/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/esp_http_client/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/ethernet/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/fatfs/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/freertos/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/heap/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/libsodium/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/log/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/lwip/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/mbedtls/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/nvs_flash/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/openssl/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/pthread/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/spi_flash/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/spiffs/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/tcpip_adapter/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/vfs/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/wear_levelling/Kconfig \
	/home/wuyuliang/esp/esp-iot-solution/components/__config/Kconfig.projbuild \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/wuyuliang/esp/esp-iot-solution/examples/check_pedestrian_flow/main/Kconfig.projbuild \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/wuyuliang/esp/esp-iot-solution/submodule/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
