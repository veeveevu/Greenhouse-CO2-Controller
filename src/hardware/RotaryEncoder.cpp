//
// Created by Anh Huynh on 26.9.2026.
//

#include "RotaryEncoder.h"

void Encoder::irq_rotate_handler(uint gpio, uint32_t event_mask)
{
	if (gpio == rot_a.get_pin())
	{
		BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
		if (!rot_b.read())
		{
			xQueueSendFromISR(queue, &clockwise ,&pxHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		}
		else
		{
			xQueueSendFromISR(queue, &anticlockwise, &pxHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		}
	}
	if (gpio == rot_sw.get_pin())
	{
		TickType_t now = xTaskGetTickCountFromISR();
		BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

		if ( (now - last_press_time) >= pdMS_TO_TICKS(250))
		{
			last_press_time = now;
			xQueueSendFromISR(queue, &pressed, &pxHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		}
	}
}