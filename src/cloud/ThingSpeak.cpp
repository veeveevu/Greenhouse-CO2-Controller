#include "ThingSpeak.h"

#include <cstring>

#include "secrets.h"
#include "lib/ipstack/tls_common.h"

ThingSpeak::ThingSpeak(
    const char* write_key_in,
    const char* talkback_key_in,
    const char* talkback_id_in,
    const uint8_t* root_certificate_in,
    size_t root_certificate_length_in
)
    : write_key(write_key_in),
      talkback_key(talkback_key_in),
      talkback_id(talkback_id_in),
      root_certificate(root_certificate_in),
      root_certificate_length(root_certificate_length_in) {}

bool ThingSpeak::post(const char* path, const char* body, char* response, size_t response_size) {
    char request[512];
    snprintf(request, sizeof(request),
             "POST %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n"
             "Content-Type: application/x-www-form-urlencoded\r\n"
             "Content-Length: %d\r\n"
             "\r\n"
             "%s",
             path, TLS_CLIENT_SERVER, (int)strlen(body), body);

    static const uint8_t cert[] = THINGSPEAK_CERT;
    return tls_https_request(cert, sizeof(cert), TLS_CLIENT_SERVER, request, response, response_size, 15);
}

bool ThingSpeak::send(int co2, float humidity, float temp, int fan_speed, int setpoint) {
    char body[160];

    snprintf(body, sizeof(body),
             "api_key=%s&field1=%d&field2=%.1f&field3=%.1f&field4=%d&field5=%d",
             write_key, co2, humidity, temp, fan_speed, setpoint);

    char resp[1024];

    if (!post("/update.json", body, resp, sizeof(resp))) {
        printf("[TS] send: connection failed\n");
        return false;
    }

    bool ok = strstr(resp, " 200 ") != nullptr && strstr(resp, "\"entry_id\"") != nullptr;
    printf("[TS] send %s\n", ok ? "OK" : "FAILED (kiem tra key / gui qua nhanh?)");
    return ok;
}

bool ThingSpeak::receive_setpoint(int &setpoint) {
    char path[96];
    snprintf(path, sizeof(path), "/talkbacks/%s/commands/execute.json", talkback_id);

    char body[64];
    snprintf(body, sizeof(body), "api_key=%s", talkback_key);

    char resp[1024];
    if (!post(path, body, resp, sizeof(resp))) {
        printf("[TS] receive: connection failed\n");
        return false;
    }

    //lấy từ response setpoint
	return false;

}

