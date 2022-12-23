// SPDX-License-Identifier: GPL-2.0-only

/*
 * Utility for performing remote reset on ZyXEL networking devices
 *
 * Copyright (c) 2022, David Bauer <mail@david-bauer.net>
 */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <string.h>
#include <net/if.h>
#include <errno.h>
#include <sys/ioctl.h>

#define LLDP_DELAY	(250 * 1000)

char buf[] = {
	/* Destination - LLDP Multicast */
	0x01, 0x80, 0xc2, 0x00, 0x00, 0x0e,
	/* Source */
	0xd8, 0xec, 0xe5, 0x8f, 0x60, 0x14,
	/* Type */
	0x88, 0xcc,
	/* Chassis Subtype */
	0x02, 0x07, 0x04, 0xd8, 0xec, 0xe5, 0x8f, 0x60, 0x14,
	/* Port Subtype */
	0x04, 0x02, 0x07, 0x39,
	/* TTL */
	0x06, 0x02, 0x00, 0x78,
	/*Vendor specific -  Reset magic */
	0xfe, 0x06, 0x00, 0xa0, 0xc5, 0x80, 0x01,
	/* Trailer */
	0x52, 0x00, 0x00
};

int get_ifindex(int sock, const char *ifname)
{
	struct ifreq ifr;

	strncpy((char *)ifr.ifr_name, ifname, IFNAMSIZ);

	if (ioctl(sock, SIOCGIFINDEX, &ifr)) {
		printf("IOCTL error: %s\n", strerror(errno));
		return -1;
	}

	return ifr.ifr_ifindex;
}

int main(int argc, char *argv[])
{
	struct sockaddr_ll sll;
	int ifindex;
	int s_fd;

	if (argc < 2 || !strcmp("help", argv[1])) {
		printf("Usage: %s <ifname>\n", argv[0]);
		return 1;
	}

	s_fd = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if (s_fd < 0) {
		printf("Socket error: %s\n", strerror(errno));
		return 1;
	}

	ifindex = get_ifindex(s_fd, argv[1]);
	if (ifindex < 0) {
		return 1;
	}

	memset(&sll, 0, sizeof(sll));
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifindex;

	printf("Sending Reset command\n");
	while (1) {
		if (sendto(s_fd, buf, sizeof(buf), 0, (struct sockaddr *) &sll, sizeof(sll)) < 0) {
			printf("Error sending packet: %s\n", strerror(errno));
			return 1;
		}

		usleep(LLDP_DELAY);
	}

	close(s_fd);

	return 0;
}
