
#ifndef GREENHOUSE_TLS_COMMON_H
#define GREENHOUSE_TLS_COMMON_H

#include <cstddef>
#include <cstdint>

bool tls_https_request(const uint8_t *cert, size_t cert_len,
                       const char *server, const char *request,
                       char *response, size_t response_size, int timeout_s);


#endif //GREENHOUSE_TLS_COMMON_H