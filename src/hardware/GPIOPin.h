//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_GPIOPIN_H
#define GREENHOUSE_GPIOPIN_H
#include <cstdint>


class GPIOPin
{
	public:
		explicit GPIOPin(int pin, bool input = true, bool pullup = true, bool invert = false);
		GPIOPin(const GPIOPin &) = delete;
		~GPIOPin();
		bool read() const;
		void write(bool value) const;
		explicit operator bool() const;
		int get_pin();
	private:
		static std::uint32_t pins_in_use;
		int pin;
		bool is_dormant;
		bool is_input;
};

#endif //GREENHOUSE_GPIOPIN_H