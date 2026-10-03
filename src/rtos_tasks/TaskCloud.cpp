#include "TaskCloud.h"

#include "cloud/WifiManager.h"
#include "cloud/secrets.h"

TaskCloud::TaskCloud(SystemStorage& storage)
    : ParentTask("Cloud connect to wifi, send/receive", 2048, tskIDLE_PRIORITY + 2),
      storage(storage) {}


void TaskCloud::task_runner() {
    wifi_manager = std::make_unique<WifiManager>(WIFI_ID, WIFI_PWD);

    const uint8_t thingspeak_cert[] = THINGSPEAK_CERT;
    const size_t thingspeak_cert_len = sizeof(thingspeak_cert);
    ThingSpeak thingspeak(THINGSPEAK_WRITE_KEY, THINGSPEAK_TALKBACK_KEY, THINGSPEAK_TALKBACK_ID, thingspeak_cert,
                          thingspeak_cert_len);

    while (true) {
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
                data.fan_pulse_counter,
                data.co2_set_point
            );
        }
        vTaskDelay(pdMS_TO_TICKS(20000));
    }
}
