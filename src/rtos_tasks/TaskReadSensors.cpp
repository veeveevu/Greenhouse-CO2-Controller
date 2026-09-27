#include "TaskReadSensors.h"

#include <memory>

#include "ModbusClient.h"
#include "PicoOsUart.h"

SensorTask::SensorTask(const std::shared_ptr<ModbusClient>& client, QueueHandle_t sensor_queue)
        : handler(client), sensor_queue(sensor_queue) {}

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
    SensorReading data = handler.return_sensor_data();
    xQueueOverwrite(sensor_queue, &data);
    vTaskDelay(pdMS_TO_TICKS(READING_PERIOD_MS));
}
}
