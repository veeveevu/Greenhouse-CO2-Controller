//
// Created by Anh Huynh on 23.9.2026.
//

#include "EEPROM.h"


int EEPROM::write(const uint8_t *src, int payload_size, int wr_mem_addr)
{
	uint8_t data_reg[payload_size + 2];
	data_reg[0] = static_cast<uint8_t>(wr_mem_addr >> 8);
	data_reg[1] = static_cast<uint8_t>(wr_mem_addr);

	int index = 2;
	int src_index = 0;
	for (; index < payload_size + 2; index ++)
	{
		data_reg[index] = src[src_index];
		src_index++;
	}

	unsigned int bytes_sent = i2c->write(slave_address,data_reg,payload_size +2);
	sleep_ms(10);

	if (bytes_sent != payload_size)
	{
		return 1;
	}
	return 0;
}

int EEPROM::read(uint8_t *dest, int payload_size, int rd_mem_addr)
{
	uint8_t data_reg[2];
	data_reg[0] = static_cast<uint8_t>(rd_mem_addr >> 8);
	data_reg[1] = static_cast<uint8_t>(rd_mem_addr);

	unsigned int result = i2c->transaction(slave_address,data_reg,2,dest,payload_size);

	if (result < payload_size)
	{
		return 1;
	}
	return 0;
}
