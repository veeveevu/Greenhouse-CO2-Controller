//
// Created by Anh Huynh on 29.9.2026.
//

#include "TaskConsole.h"

#include <sstream>
#define UART_NO 0
#define TX_PIN 0
#define RX_PIN 1
#define UART_SPEED 115200

TaskConsole::TaskConsole()
	:	ParentTask("Console Task", 512, tskIDLE_PRIORITY + 1)
{
}

void TaskConsole::task_runner()
{
	uart_ = std::make_unique<PicoOsUart>(UART_NO,TX_PIN,RX_PIN,UART_SPEED);
	uint8_t buffer[64];
	std::string line;
	while (true) {
		if(int count = uart_->read(buffer, 63, 30); count > 0) {
			uart_->write(buffer, count);
			buffer[count] = '\0';
			line += reinterpret_cast<const char *>(buffer);
			if(line.find_first_of("\n\r") != std::string::npos){
				uart_->send("\n");

				std::istringstream input(line);
				std::string cmd;
				input >> cmd;
				if(cmd == "co2" || cmd == "ppm") {
					uint32_t i = 0;
					input >> i;

					std::string message = cmd + std::to_string(i) + "\r\n";
					uart_->write(reinterpret_cast<const uint8_t*>(message.c_str()), message.length());
				}
				else if (cmd == "t") {
				}
				line.clear();
			}
		}
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
