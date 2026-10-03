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

#if 0

void WifiManager::build_thingspeak_request(char *method, char *path, char *buffer, size_t buffer_size, const char *body)
{
	size_t body_len;
	if (body != "")
	{
		body_len = std::strlen(body);
	}

	if (body_len > 0)
	{
		return static_cast<void>(std::snprintf(buffer, buffer_size,
		                                       "%s %s HTTP/1.1\r\n"
		                                       "Host: api.thingspeak.com\r\n"
		                                       "User-Agent: PicoW\r\n"
		                                       "Accept: */*\r\n"
		                                       "Content-Length: %u\r\n"
		                                       "Content-Type: application/x-www-form-urlencoded\r\n"
		                                       "\r\n"
		                                       "%s",
		                                       method, path, body_len, body));
	}
	else
	{
		return static_cast<void>(std::snprintf(buffer, buffer_size,
		                                       "%s %s HTTP/1.1\r\n"
		                                       "Host: api.thingspeak.com\r\n"
		                                       "User-Agent: PicoW\r\n"
		                                       "Accept: */*\r\n"
		                                       "Content-Length: 0\r\n"
		                                       "Content-Type: application/x-www-form-urlencoded\r\n"
		                                       "\r\n",
		                                       method, path));
	}
}

void WifiManager::send_data(SensorReading data)
{
	char* method = "GET";

	char path[256];
	std::snprintf(path, sizeof(path),
		"/update?api_key=%s&field1=%.1f&field2=%.1f&field3=%.1f&field4=%.1f&field5=%d",
		THINGSPEAK_WRITE_KEY,data.co2_level_ppm, data.humidity_percent, data.temp_celsius, data.fan_pulse_counter,data.co2_set_point);

	char tx_buffer[512];
	unsigned char *buffer = new unsigned char[2048];

	build_thingspeak_request(method, path, tx_buffer, sizeof(tx_buffer));

	int rc = ip_stack->connect(TLS_CLIENT_SERVER, 80);
	if (rc == 0) {
		ip_stack->write((unsigned char *) (tx_buffer), strlen(tx_buffer), 1000);
		auto rv = ip_stack->read(buffer, 2048, 2000);
		buffer[rv] = 0;
		printf("rv=%d\n%s\n", rv, buffer);
		ip_stack->disconnect();
	}
	else {
		printf("rc from TCP connect is %d\n", rc);
	}
}

#endif
