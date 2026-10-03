#include "WifiManager.h"

#include <cstring>
#include "IPStack.h"

WifiManager::WifiManager(const char *ssid_in, const char *pw_in) {
    std::strncpy(ssid, ssid_in, sizeof(ssid) - 1);
    ssid[sizeof(ssid) - 1] = '\0';
    std::strncpy(password, pw_in, sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0';
}
bool WifiManager::init() {

    if (cyw43_arch_init()) {
        printf("[Wifi] Failed to initialise.\n");
        return false;
    }
    cyw43_arch_enable_sta_mode();

    return true;
}

bool WifiManager::connect() {
    if (!init()) return false;

    printf("[Wifi] Connecting to Wi-fi...\n");

    int connect_result = cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, WIFI_TIMEOUT_MS);

    if (connect_result == 0) {
        printf("[Wifi] Connected.\n");
        return true;
    }
    else {
        printf("[Wifi] Failed to connect.\n");
        return false;
    }
}


bool WifiManager::is_connected() const {
    return cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA) == CYW43_LINK_UP;
}
