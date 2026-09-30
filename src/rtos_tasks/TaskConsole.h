//
// Created by Anh Huynh on 29.9.2026.
//

#ifndef GREENHOUSE_TASKCONSOLE_H
#define GREENHOUSE_TASKCONSOLE_H
#include <memory>

#include "FreeRTOS.h"
#include "ParentTask.h"
#include "PicoOsUart.h"
#include "task.h"


class TaskConsole : public ParentTask
{
	public:
		TaskConsole();
		void task_runner() override;
	private:
		std::unique_ptr<PicoOsUart> uart_;

};


#endif //GREENHOUSE_TASKCONSOLE_H