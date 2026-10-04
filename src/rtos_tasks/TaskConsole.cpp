//
// Created by Anh Huynh on 29.9.2026.
//

#include "TaskConsole.h"

#include <sstream>

#include "UIEvent.h"
#define UART_NO 0
#define TX_PIN 0
#define RX_PIN 1
#define UART_SPEED 115200

TaskConsole::TaskConsole(EventGroupHandle_t event_grp, QueueHandle_t input_queue)
	:	ParentTask("Console Task", 512, tskIDLE_PRIORITY + 1),
		event_grp(event_grp),
		input_queue(input_queue)
{
}

void TaskConsole::task_runner()
{
	uart_ = std::make_unique<PicoOsUart>(UART_NO,TX_PIN,RX_PIN,UART_SPEED);
	char rx_char;
	std::string line;

	while (true) {
		EventBits_t uxBit = xEventGroupWaitBits(event_grp,CONSOLE_ACTIVATE_BIT,pdFALSE, pdFALSE,portMAX_DELAY);
		if ( (uxBit & CONSOLE_ACTIVATE_BIT) == CONSOLE_ACTIVATE_BIT)
		{
			if(int count = uart_->read(reinterpret_cast<uint8_t *> (&rx_char), 1, 30); count > 0) {
				xQueueSend(input_queue, &rx_char,0);
				//uart_->write(buffer, count);
			}
		}
		vTaskDelay(pdMS_TO_TICKS(1));
	}
}