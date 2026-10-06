//
// Created by Anh Huynh on 2.10.2026.
//

#ifndef GREENHOUSE_UIEVENT_H
#define GREENHOUSE_UIEVENT_H

enum class UIEvent {MENU, CO2_SETTING, SHOW_DATA, RESET, NETWORK, NEW_NETWORK, KNOWN_NETWORK};

enum class NetworkParam{SSID, PASSWORD, DONE};
enum class WiFiStatus{CONNECTING, CONNECT_SUCCESS, CONNECT_FAIL, IDLE, CONNECTED}; //CONNECT_SUCCESS means the attempt to connect is successful, CONNECTED means wifi is connected and ready for cloud task
#define CONSOLE_ACTIVATE_BIT (1 << 0)
#define WIFI_RECONNECT_BIT (1 << 1)
#define NEW_CO2_TALKBACK_BIT (1 << 2)
#endif //GREENHOUSE_UIEVENT_H