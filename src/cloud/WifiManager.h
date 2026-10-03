#ifndef GREENHOUSE_WIFIMANAGGER_H
#define GREENHOUSE_WIFIMANAGGER_H

#define WIFI_TIMEOUT_MS 10000 //10s

class WifiManager {
public:
    WifiManager(const char *ssid_in, const char *pw_in);
    bool init();
    bool connect();
    bool is_connected() const;
private:
    char ssid[33];
    char password[64];
};

#endif //GREENHOUSE_WIFIMANAGGER_H