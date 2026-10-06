#ifndef GREENHOUSE_WIFIMANAGGER_H
#define GREENHOUSE_WIFIMANAGGER_H

#define WIFI_TIMEOUT_MS 10000 //10s
#include <memory>

#include "data_structs.h"
#include "IPStack.h"

class WifiManager {
public:
    WifiManager(const char *ssid_in, const char *pw_in);
	void build_thingspeak_request(char *method, char* path,  char* buffer, size_t buffer_size, const char* body = "");
	void send_data(SensorReading data);
		bool is_connected() const;

    void connect_new_wifi(const char *ssid, const char *pw);

private:
    const char *                   ssid;
    const char *                   password;
	std::unique_ptr<IPStack> ip_stack;

};

#endif //GREENHOUSE_WIFIMANAGGER_H