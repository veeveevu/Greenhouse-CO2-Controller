#include "TaskCloud.h"

#include "cloud/WifiManager.h"
#include "cloud/secrets.h"

CloudTask::CloudTask(SystemStorage& storage, const std::shared_ptr<ThingSpeak>& thingspeak)
    : ParentTask("Cloud connect to wifi, send/receive", 1024, tskIDLE_PRIORITY + 2),
      storage(storage), thingspeak(thingspeak) {}


void CloudTask::task_runner() {
    if (cyw43_arch_init()) {
        printf("[Cloud] cyw43 init failed\n");
        vTaskDelete(nullptr);
    }
    cyw43_arch_enable_sta_mode();

    WifiManager wifi(WIFI_ID, WIFI_PWD);
    const uint8_t thingspeak_cert[] = THINGSPEAK_CERT;
    const size_t thingspeak_cert_len = sizeof(thingspeak_cert);
    ThingSpeak ts(THINGSPEAK_WRITE_KEY, THINGSPEAK_TALKBACK_ID, THINGSPEAK_TALKBACK_KEY, thingspeak_cert, thingspeak_cert_len);
while (true) {
    if (!wifi.is_connected()) {
        if (!wifi.connect()) {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
    }

    //gửi data mới nhất
    //lệnh ts.send()

    //check talkback coi có lệnh đổi setpoint?
    //lệnh ts.receive_setpoint(sp)

    //
    vTaskDelay(pdMS_TO_TICKS(15000));
}

}
