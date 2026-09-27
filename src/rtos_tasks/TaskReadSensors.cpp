#include "TaskReadSensors.h"

#include <memory>

#include "ModbusClient.h"
#include "PicoOsUart.h"

SensorTask::SensorTask(const std::shared_ptr<ModbusClient>& client, QueueHandle_t sensor_queue, SystemStorage &storage)
        : ParentTask("Read modbus", 1024,  tskIDLE_PRIORITY + 2),
			handler(client), sensor_queue(sensor_queue), storage(storage) {}

void SensorTask::task_runner() {
while (true) {
    handler.sensors_read();
    SensorReading data = handler.return_sensor_data();
	storage.update_data(data);
    //xQueueOverwrite(sensor_queue, &data);
    vTaskDelay(pdMS_TO_TICKS(READING_PERIOD_MS));
}
}
