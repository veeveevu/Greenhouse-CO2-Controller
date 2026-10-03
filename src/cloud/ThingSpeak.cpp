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

    snprintf(
        request,
        sizeof(request),
        "POST %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: %u\r\n"
        "\r\n"
        "%s",
        path,
        TLS_CLIENT_SERVER,
        (unsigned)strlen(body),
        body
    );

    //printf("[TS] request_len=%d, capacity=%u\n", request_len, (unsigned)sizeof(request));


    return tls_https_request(root_certificate, root_certificate_length, TLS_CLIENT_SERVER, request, response,
                             response_size, 15);
}

bool ThingSpeak::send(int co2, float humidity, float temp, int fan_speed, int setpoint) {
    char body[512];

    snprintf(body, sizeof(body),
             "api_key=%s&field1=%d&field2=%.1f&field3=%.1f&field4=%d&field5=%d",
             write_key, co2, humidity, temp, fan_speed, setpoint);

    char resp[1024];

    if (!post("/update.json", body, resp, sizeof(resp))) {
        printf("[TS] send: connection failed\n");
        return false;
    }

    //Dùng để check response của POST!
    //printf("[TS] POST response:\n%s\n", resp);

    bool ok = strstr(resp, " 200 ") != nullptr && strstr(resp, "\"entry_id\"") != nullptr;
    printf("[TS] send %s\n", ok ? "OK" : "FAILED");
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

    //check response
    //printf("[TS] TalkBack response:\n%s\n", resp);

    //{"executed_at":"2026-10-03T20:16:28Z","position":null,"id":65092491,"command_string":"20","created_at":"2026-10-03T20:12:21Z"}


    const char* key = strstr(resp, "\"command_string\"");

    //ko có key mới
    if (key == nullptr) {
        printf("[TS] No new TalkBack command\n");
        return false;
    }

    const char* colon = strchr(key, ':');
    const char* start = strchr(colon, '"');
    ++start;

    const char* end = strchr(start, '"');

    size_t command_length = (size_t)(end - start);

    char command[16];

    memcpy(command, start, command_length);
    command[command_length] = '\0';

    char* endptr = nullptr;
    long value = strtol(command, &endptr, 10);

    if (endptr == command || *endptr != '\0') {
        printf("[TS] Invalid command from ThingSpeak: [%s]\n", command);
        return false;
    }

    if (value < 0 || value > 1500) {
        printf("[TS] Setpoint out of range (%ld)\n", value);
        return false;
    }

    setpoint = static_cast<int>(value);
    printf("[TS] New setpoint = %d\n", setpoint);

    return true;

}
