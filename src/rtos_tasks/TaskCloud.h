#ifndef GREENHOUSE_TASKCLOUD_H
#define GREENHOUSE_TASKCLOUD_H

#include "ActuatorController.h"
#include "ModbusClient.h"

#include "ActuatorController.h"
#include "data_structs.h"
#include "ParentTask.h"
#include "storage/SystemStorage.h"
#include "cloud/secrets.h"
#include "cloud/ThingSpeak.h"


class CloudTask : public ParentTask {
public:
    CloudTask(
        SystemStorage &storage,
        const std::shared_ptr<ThingSpeak> &thingspeak
    );
private:
    void task_runner() override;

    void process_command(const char *command);

    SystemStorage &storage;
    std::shared_ptr<ThingSpeak> thingspeak;
};

#endif //GREENHOUSE_TASKCLOUD_H