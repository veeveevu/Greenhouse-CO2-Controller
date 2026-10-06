//
// Created by Anh Huynh on 29.9.2026.
//

#ifndef GREENHOUSE_TASKCONSOLE_H
#define GREENHOUSE_TASKCONSOLE_H
#include <memory>


#include "FreeRTOS.h"
#include "event_groups.h"
#include "ParentTask.h"
#include "PicoOsUart.h"
#include "task.h"


class TaskConsole : public ParentTask
{
	public:
		TaskConsole(EventGroupHandle_t event_grp, QueueHandle_t input_queue);

		QueueHandle_t get_queue_handle();

		void          task_runner() override;
	private:
		std::shared_ptr<PicoOsUart> uart_;
		EventGroupHandle_t event_grp;
		QueueHandle_t input_queue;
};


#endif //GREENHOUSE_TASKCONSOLE_H