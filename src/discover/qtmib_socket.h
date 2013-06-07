#ifndef QTMIB_SOCKET_H
#define QTMIB_SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

int rx_open(uint16_t port);
uint32_t rx_packet(int sock);
void tx_packet(int sock, uint32_t ip, int port, int type, const char *community);

#ifdef __cplusplus
}
#endif

#endif