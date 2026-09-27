//
// Created by Anh Huynh on 27.9.2026.
//

#ifndef GREENHOUSE_PARENTTASK_H
#define GREENHOUSE_PARENTTASK_H


class ParentTask
{
	public:
		ParentTask(const char* name, uint16_t stack_depth, UBaseType_t priority)
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


#endif //GREENHOUSE_PARENTTASK_H