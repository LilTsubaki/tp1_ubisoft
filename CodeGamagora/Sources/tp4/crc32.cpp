#include <string.h>

static unsigned long crc32_tab[] = {
	0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
	0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
	0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
	0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c,
};

unsigned long crc32(const void *buf, size_t size)
{
	unsigned long crc = 0xffffffff;
	const unsigned char* p;

	p = (unsigned char*)buf;

	unsigned long b;
	while (size--)
	{
		b = (*p) & 0xff;
		crc = (crc >> 4) ^ crc32_tab[(crc ^ b) & 0xf];
		crc = (crc >> 4) ^ crc32_tab[(crc ^ (b >>= 4)) & 0xf];
		//crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
		p++;
	}

	return ~crc;
}


unsigned long crc32OfString(const char *string_in)
{
	return crc32( string_in, strlen( string_in ) );
}

