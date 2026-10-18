#include <iostream>
#include <sstream>
#include "FreeRTOS.h"
#include "TaskController.h"
#include "task.h"
#include "semphr.h"
#include "hardware/gpio.h"
#include "PicoOsUart.h"
#include "ssd1306.h"

#include "SensorDataHandler.h"
#include "TaskConsole.h"
#include "TaskReadSensors.h"
#include "TaskUI.h"
#include "lib/modbus/ModbusRegister.h"
#include "IPStack.h"
#include "TaskCloud.h"
#include "cloud/secrets.h"
#include "cloud/ThingSpeak.h"
#include "cloud/WifiManager.h"

#include "hardware/timer.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}

// stack overflow check
extern "C" {
void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName ) {
    if (pcTaskName != NULL) panic("Stack overflow: %s",pcTaskName);
    else panic("Stack overflow of unnamed task");
}
}

#define UART_NR 1
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define BAUD_RATE 9600
#define STOP_BITS 2 // for real system (pico simualtor also requires 2 stop bits)

#define ROT_SW 12
#define ROT_A 10
#define ROT_B 11


int main() {
    stdio_init_all();
	printf("Boot\n");

	EventGroupHandle_t task_event_grp = xEventGroupCreate();

    auto uart = std::make_shared<PicoOsUart>(UART_NR,UART_TX_PIN,UART_RX_PIN,BAUD_RATE,STOP_BITS);
    auto client = std::make_shared<ModbusClient>(uart);

    auto actuator = std::make_shared<ActuatorController>(client);
	SystemStorage storage = SystemStorage();
	auto i2c_0 = std::make_shared<PicoI2C>(0, 100000);
	auto i2c_1 = std::make_shared<PicoI2C>(1, 400000);
	auto eeprom = std::make_shared<MemoryManager>(i2c_0);


    static SensorTask sensor_task(client,storage,i2c_1);
    static  ControllerTask controller_task(actuator, storage);
	static TaskCloud cloud_task(eeprom,storage, task_event_grp);
	static  TaskUI ui_task(eeprom,i2c_1,storage, task_event_grp, cloud_task.getTaskHandle());
	static TaskConsole console_task(task_event_grp, ui_task.get_input_queue_handle());


	static Encoder encoder(ROT_SW, ROT_A, ROT_B,ui_task.get_ui_queue_handle());

    sensor_task.start();
    controller_task.start();
	ui_task.start();
	console_task.start();
	cloud_task.start();

    vTaskStartScheduler();

    while (true)
    {
    }
}
