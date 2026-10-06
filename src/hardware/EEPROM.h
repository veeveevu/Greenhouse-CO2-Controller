//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_EEPROM_H
#define GREENHOUSE_EEPROM_H


#include <memory>
#include "../lib/i2c/PicoI2C.h"


class EEPROM
{
	public:
		explicit EEPROM(std::shared_ptr<PicoI2C> i2c_0) : i2c(i2c_0){};
		EEPROM(const EEPROM &) = delete;
		int read( uint8_t *dest, int payload_size, int rd_mem_addr) ;
		int      write(const uint8_t *src, int payload_size, int wr_mem_addr) ;


	private:
		std::shared_ptr<PicoI2C> i2c;
		uint8_t slave_address = 0x50;

};

#endif //GREENHOUSE_EEPROM_H