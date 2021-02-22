// https://gist.github.com/timepp/1f678e200d9e0f2a043a9ec6b3690635

#pragma once

#include <stdint.h>

struct crc32
{
	static void generate_table(uint32_t(&table)[256])
	{
		uint32_t polynomial = 0xEDB88320;
		for (uint32_t i = 0; i < 256; i++)
		{
			uint32_t c = i;
			for (size_t j = 0; j < 8; j++)
			{
				if (c & 1) {
					c = polynomial ^ (c >> 1);
				}
				else {
					c >>= 1;
				}
			}
			table[i] = c;
		}
	}

	static uint32_t update(uint32_t(&table)[256], uint32_t initial, const void* buf, size_t len)
	{
		uint32_t c = initial;// ^ 0xFFFFFFFF;
		const uint8_t* u = static_cast<const uint8_t*>(buf);
		for (size_t i = 0; i < len; ++i)
		{
			//c = table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
			c = table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
		}
		return c;// ^ 0xFFFFFFFF;
	}

	static uint32_t MonacoUpdate(uint32_t(&table)[256], const char * name)
	{
		// The version written from the dissassembly of Monaco

		uint32_t c = 0;
		for (size_t i = 0; i < name[i] != '\0'; ++i)
		{
			c = (c >> 8) ^ *(unsigned int*)((char*)&table + (((int)name[i] & 0xFFFFFFDF ^ c) & 0xFF * 4));
		}
		return c;// ^ 0xFFFFFFFF;
	}
};


// usage: the following code generates crc for 2 pieces of data
// uint32_t table[256];
// crc32::generate_table(table);
// uint32_t crc = crc32::update(table, 0, data_piece1, len1);
// crc = crc32::update(table, crc, data_piece2, len2);
// output(crc);