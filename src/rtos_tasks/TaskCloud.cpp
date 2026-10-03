#include "TaskCloud.h"

#include "cloud/WifiManager.h"
#include "cloud/secrets.h"

TaskCloud::TaskCloud(SystemStorage& storage)
    : ParentTask("Cloud connect to wifi, send/receive", 1024, tskIDLE_PRIORITY + 2),
      storage(storage) {}


void TaskCloud::task_runner() {
	wifi_manager = std::make_unique<WifiManager>(WIFI_ID, WIFI_PWD);

    //const uint8_t thingspeak_cert[] = THINGSPEAK_CERT;
    //const size_t thingspeak_cert_len = sizeof(thingspeak_cert);
    //ThingSpeak ts(THINGSPEAK_WRITE_KEY, THINGSPEAK_TALKBACK_ID, THINGSPEAK_TALKBACK_KEY, thingspeak_cert, thingspeak_cert_len);
	while (true) {
		/*
	    if (!wifi.is_connected()) {
	        if (!wifi.connect()) {
	            vTaskDelay(pdMS_TO_TICKS(1000));
	            continue;
	        }
	    }
		else
		{
			printf("Connected to Wifi\n");
		}

	    //gửi data mới nhất
	    //lệnh ts.send()

	    //check talkback coi có lệnh đổi setpoint?
	    //lệnh ts.receive_setpoint(sp)*/

	    //
	    vTaskDelay(pdMS_TO_TICKS(15000));
	}
}
