#include "WifiManager.h"

#include <cstring>
#include "IPStack.h"
#include "secrets.h"

WifiManager::WifiManager(const char *ssid_in, const char *pw_in) : ssid(ssid_in), password(pw_in) {
    ip_stack = std::make_unique<IPStack>(ssid, password);
}

bool WifiManager::is_connected() const
{
	return ip_stack->is_connected();
}

void WifiManager::connect_new_wifi(const char* ssid, const char* pw)
{
	ip_stack->connect_to_wifi(ssid, pw);
}
