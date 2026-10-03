#ifndef GREENHOUSE_WIFIMANAGGER_H
#define GREENHOUSE_WIFIMANAGGER_H

#define WIFI_TIMEOUT_MS 10000 //10s
#include <memory>

#include "IPStack.h"

class WifiManager {
public:
    WifiManager(const char *ssid_in, const char *pw_in);
    bool init();
    bool connect();
    bool is_connected() const;
private:
    const char *                   ssid;
    const char *                   password;
	std::unique_ptr<IPStack> ip_stack;

};

#endif //GREENHOUSE_WIFIMANAGGER_H