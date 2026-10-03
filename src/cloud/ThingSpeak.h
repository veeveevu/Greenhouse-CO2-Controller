#ifndef GREENHOUSE_THINGSPEAK_H
#define GREENHOUSE_THINGSPEAK_H
#include "IPStack.h"


class ThingSpeak {
public:
    ThingSpeak(const char* write_key,
               const char* talkback_key,
               const char* talkback_id,
               const uint8_t* root_certificate,
               size_t root_certificate_length);

    bool send(int co2, float humidity, float temp, int fan_speed, int setpoint);
    bool receive_setpoint(int &setpoint);

private:
    const char* write_key;
    const char* talkback_key;
    const char* talkback_id;

    const uint8_t* root_certificate;
    size_t root_certificate_length;

    bool post(const char *path, const char *body, char *response, size_t response_size);
};

#endif //GREENHOUSE_THINGSPEAK_H
