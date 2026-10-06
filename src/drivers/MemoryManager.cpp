//
// Created by Anh Huynh on 23.9.2026.
//

#include "MemoryManager.h"

#include <cstring>
#include <iostream>

void MemoryManager::read_co2_setting(uint8_t *read_co2_dest)
{
	uint8_t dest[64];
	eeprom.read(dest,default_payload,co2_setting_addr);
	std::memcpy(read_co2_dest,dest,64);

}

int MemoryManager::read_network_setting(char *ssid, char *pwd)
{
	char buffer[default_payload];
	eeprom.read(reinterpret_cast<uint8_t *>(buffer),default_payload,network_setting_addr);
	std::cout << buffer;

	int parse_count = sscanf(buffer, "SSID: %32s PASSWORD: %32s\n", ssid, pwd);

	if (parse_count == 2)
	{
		printf("SSID: OK\n");
		printf("PASSWORD: OK\n");
	} else
	{
		printf("Parsing failed! EEPROM data format is invalid.\n");
	}
	return parse_count;
}

void MemoryManager::save_new_co2_setting(int new_co2_setting)
{
	uint8_t save_data[sizeof(int)]; //8 bytes
	std::memcpy(save_data,&new_co2_setting,sizeof(int));

	eeprom.write(save_data,default_payload,co2_setting_addr);
}

void MemoryManager::reset_network_setting()
{
	uint8_t save_data[64] = {0};
	eeprom.write(save_data,default_payload,network_setting_addr);
}
void MemoryManager::save_network_setting( char *ssid, char* pwd)
{
	char buffer[64];
	snprintf(buffer, 64, "SSID: %s PASSWORD: %s", ssid, pwd);
	uint8_t* save_data = reinterpret_cast<uint8_t*>(buffer);

	eeprom.write(save_data, default_payload, network_setting_addr);

}



