//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_GENERICTASKHANDLER_H
#define GREENHOUSE_GENERICTASKHANDLER_H
#include "FreeRTOS.h"
#include "task.h"

class GenericTaskHandler
{
	public:
		GenericTaskHandler(const char* name, uint16_t stack_depth, UBaseType_t priority)
			: name(name), stack_depth(stack_depth), priority(priority)
		{}
		void start()
		{
			xTaskCreate(
			task_handler,
			name,
			stack_depth,
			(void*) this,
			priority,
			&handle
			);
		}
		virtual ~GenericTaskHandler()
		{
			vTaskDelete(handle);
		};
		virtual void task_runner() = 0;
	private:
		const char *name;
		uint16_t    stack_depth;
		UBaseType_t priority;
		static void task_handler(void* param)
		{
			auto instance = static_cast<GenericTaskHandler*> (param);
			instance -> task_runner();
		}
	protected:
		TaskHandle_t handle;
};


#endif //GREENHOUSE_GENERICTASKHANDLER_H