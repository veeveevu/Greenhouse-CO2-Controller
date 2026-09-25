#include "TaskReadSensors.h"

#include <memory>

#include "ModbusClient.h"
#include "PicoOsUart.h"

SensorTask::SensorTask(const std::shared_ptr<ModbusClient>& client, QueueHandle_t out_queue)
        : handler(client), out_queue(out_queue) {}

void SensorTask::start() {
    xTaskCreate(task_entry, "Read modbus", 1024, this, tskIDLE_PRIORITY + 2, nullptr);
}

void SensorTask::task_entry(void* param) {

    auto* self = static_cast<SensorTask*>(param);
    self->run();
}

void SensorTask::run() {
while (true) {
    handler.sensors_read();
    sensorData data = handler.return_sensor_data();
    xQueueOverwrite(out_queue, &data);
    vTaskDelay(pdMS_TO_TICKS(5000));
}
}
