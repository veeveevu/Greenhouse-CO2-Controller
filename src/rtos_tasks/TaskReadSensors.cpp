#include "TaskReadSensors.h"

#include <memory>

#include "ModbusClient.h"
#include "PicoOsUart.h"

SensorTask::SensorTask(const std::shared_ptr<ModbusClient>& client, SystemStorage &storage,std::shared_ptr<PicoI2C> i2c)
        : ParentTask("Read modbus", 1024,  tskIDLE_PRIORITY + 2),
			pressure_sensor_(i2c),
			handler(client,pressure_sensor_), storage(storage) {}

void SensorTask::task_runner() {
	while (true) {
	    handler.sensors_read();

	    SensorReading data = handler.return_sensor_data();
		storage.update_data(data);
	    vTaskDelay(pdMS_TO_TICKS(READING_PERIOD_MS));
	}
}


