/*
 * Copyright (C) 2013 RCP100 Team (rcpteam@yahoo.com)
 *
 * This file is part of qtmib project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "qtmib_socket.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int rx_open(uint16_t port) {
	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		return 0;
	}

	// allow reuse of local addresses in the next bind call
	int on = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,(char*)&on,sizeof(on)) < 0) {
		close(sock);
		return 0;
	}

	//pick up any UDP packet for port 53
	struct sockaddr_in s;
	s.sin_family = AF_INET;
	s.sin_port = htons(port);
	s.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr *)&s, sizeof(s)) < 0) {
		perror("binding socket"); // it could fail if the port is in use by somebody else
		close(sock);
		return 0;
	}

	// set the socket to non-blocking
	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
		close(sock);
		return 0;
	}

	return sock;
}		


uint32_t rx_packet(int sock) {
	uint8_t pkt[1500];
	int len;
	struct sockaddr_in fromaddr;
	unsigned int fromlen = sizeof(fromaddr);

	if ((len = recvfrom(sock, &pkt, sizeof(pkt), 0, (struct sockaddr *)&fromaddr, &fromlen)) == -1) {
		return 0;
	}

	uint32_t src_ip = ntohl(fromaddr.sin_addr.s_addr);
//	uint16_t src_port = ntohs(fromaddr.sin_port);

	return src_ip;
}

static uint8_t header[4] = {0x30, 0x29, 0x02, 0x01}; // 0x29 is the length of the packet assuming "public" community
static uint8_t request[30] = {
	0xa0, 0x1c, 0x02, 0x04, 0x4a, 0xd8, 0xff, 0x7c,
	0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x30, 0x0e,
	0x30, 0x0c, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x02,
	0x01, 0x01, 0x01, 0x00, 0x05, 0x00 };

void tx_packet(int sock, uint32_t ip, int port, int type, const char *community) {
	uint8_t pkt[1500];
	uint8_t *ptr = &pkt[0];
	
	memcpy(ptr, header, 4);
	// adjust length
#define LENPUBLIC 6 // this is the length of "public"
	ptr[1] = 0x29 - 6 + strlen(community);
	ptr += 4;
	*ptr++ = (uint8_t) type;
	*ptr++ = 4;
	int len = strlen(community);
	*ptr++ = (uint8_t) len;
	memcpy(ptr, community, len);
	ptr += len;
	memcpy(ptr, request, 30);
	len = 4 + 2 + len + 1 + 30;

	static struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	
	int txlen = sendto(sock, pkt, len, 0,
		(struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	if (txlen == -1) {
		perror("socket tx error\n");
		return;
	}
}
