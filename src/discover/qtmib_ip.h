#ifndef QTMIB_IP_H
#define QTMIB_IP_H
#include <stdint.h>

#define RCP_PRINT_IP(A) \
((int) (((A) >> 24) & 0xFF)),  ((int) (((A) >> 16) & 0xFF)), ((int) (((A) >> 8) & 0xFF)), ((int) ( (A) & 0xFF))

static inline uint8_t mask2bits(uint32_t mask) {
	uint32_t tmp = 0x80000000;
	int i;
	uint8_t rv = 0;

	for (i = 0; i < 32; i++, tmp >>= 1) {
		if (tmp & mask)
			rv++;
		else
			break;
	}
	return rv;
}

// return 1 if error
static inline int atocidr(const char *str, uint32_t *ip, uint32_t *mask) {
	unsigned a, b, c, d, e;

	// extract ip
	int rv = sscanf(str, "%u.%u.%u.%u/%u", &a, &b, &c, &d, &e);
	if (rv != 5 || a > 255 || b > 255 || c > 255 || d > 255 || e > 32)
		return 1;
	*ip = a * 0x1000000 + b * 0x10000 + c * 0x100 + d;

	// extract mask
	uint32_t tmp;
	unsigned i;
	for (i = 0, *mask = 0, tmp = 0x80000000; i < e; i++, tmp >>= 1) {
		*mask |= tmp;
	}
	return 0;
}

#endif