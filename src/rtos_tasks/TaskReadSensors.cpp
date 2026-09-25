
#include "TaskReadSensors.h"

#include <memory>

#include "ModbusClient.h"
#include "PicoOsUart.h"


void modbus_task(void* param) {
    auto uart{std::make_shared<PicoOsUart>(UART_NR, UART_TX_PIN, UART_RX_PIN, BAUD_RATE, STOP_BITS)};
    auto client{std::make_shared<ModbusClient>(uart)};
}
