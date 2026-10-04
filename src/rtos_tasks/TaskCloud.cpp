#include "TaskCloud.h"

#include <iostream>

#include "UIEvent.h"
#include "cloud/WifiManager.h"
#include "cloud/secrets.h"

TaskCloud::TaskCloud(SystemStorage& storage, EventGroupHandle_t event_grp)
    : ParentTask("Cloud connect to wifi, send/receive", 2048, tskIDLE_PRIORITY + 1),
      storage(storage), event_grp(event_grp)
{}


void TaskCloud::task_runner() {
	std::cout << "Cloud starts\n";
    wifi_manager = std::make_unique<WifiManager>(WIFI_ID, WIFI_PWD);

    const uint8_t thingspeak_cert[] = THINGSPEAK_CERT;
    const size_t thingspeak_cert_len = sizeof(thingspeak_cert);
    ThingSpeak thingspeak(THINGSPEAK_WRITE_KEY, THINGSPEAK_TALKBACK_KEY, THINGSPEAK_TALKBACK_ID, thingspeak_cert,
                          thingspeak_cert_len);
	if (wifi_manager->is_connected())
	{

	}
    while (true) {
    	EventBits_t uxBits = xEventGroupWaitBits(event_grp,WIFI_RECONNECT_BIT,pdTRUE,pdFALSE,0);

    	//If event bit is set, then reconnect wifi to a different network

    	if (uxBits & WIFI_RECONNECT_BIT)
    	{
    		std::cout << "New WiFi connnecting\n";
    		NetworkSetting network_setting = storage.get_network_settings();
    		wifi_manager ->connect_new_wifi(network_setting.ssid, network_setting.pwd);
    		storage.update_wifi_status(wifi_manager->is_connected());
    	}
        SensorReading data = storage.get_data();
        int new_setpoint = data.co2_set_point;

        if (wifi_manager->is_connected()) {
            if (thingspeak.receive_setpoint(new_setpoint)) {
                storage.set_co2_point(new_setpoint);
                printf("[CLOUD] Set new setpoint = %d\n", new_setpoint);
            }

            data = storage.get_data();

            thingspeak.send(
                data.co2_level_ppm,
                data.humidity_percent,
                data.temp_celsius,
                data.fan_speed,
                data.co2_set_point
            );
        }
        vTaskDelay(pdMS_TO_TICKS(20000));
    }
}
