#include <ctime>
#include <stdio.h>
#include <cassert>
#include <iostream>
#include <stdint.h>
#include <math.h>

#define RAND48_SEED_0   (0x330e)
#define RAND48_SEED_1   (0xabcd)
#define RAND48_SEED_2   (0x1234)
#define RAND48_MULT_0   (0xe66d)
#define RAND48_MULT_1   (0xdeec)
#define RAND48_MULT_2   (0x0005)
#define RAND48_ADD      (0x000b)

unsigned short _rand48_seed[3] = {
	RAND48_SEED_0,
	RAND48_SEED_1,
	RAND48_SEED_2
};
unsigned short _rand48_mult[3] = {
	RAND48_MULT_0,
	RAND48_MULT_1,
	RAND48_MULT_2
};
unsigned short _rand48_add = RAND48_ADD;

void _dorand48(unsigned short xseed[3])
{
	unsigned long accu;
	unsigned short temp[2];

	accu = (unsigned long)_rand48_mult[0] * (unsigned long)xseed[0] +
		(unsigned long)_rand48_add;
	temp[0] = (unsigned short)accu;        /* lower 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += (unsigned long)_rand48_mult[0] * (unsigned long)xseed[1] +
		(unsigned long)_rand48_mult[1] * (unsigned long)xseed[0];
	temp[1] = (unsigned short)accu;        /* middle 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += _rand48_mult[0] * xseed[2] + _rand48_mult[1] * xseed[1] + _rand48_mult[2] * xseed[0];
	xseed[0] = temp[0];
	xseed[1] = temp[1];
	xseed[2] = (unsigned short)accu;
}
long nrand48(unsigned short xseed[3])
{
	_dorand48(xseed);
	return ((long)xseed[2] << 15) + ((long)xseed[1] >> 1);
}


int main()
{
	int seed = std::time(0);
	unsigned short xsubi[3] = { 0x330E, seed & 0xFFFF, seed >> 16 };
	unsigned int hash_tables[3][256];
	for (int i = 0; i < 256; ++i) {
		hash_tables[0][i] = nrand48(xsubi);
	}
	for (int i = 0; i < 256; ++i) {
		hash_tables[1][i] = nrand48(xsubi);
	}
	for (int i = 0; i < 256; ++i) {
		hash_tables[2][i] = nrand48(xsubi);
	}
	std::cout << "static unsigned int hash_tables[3][256] ={";
	std::cout << "{";
	for (int i = 0; i < 256; ++i)
	{
		std::cout << hash_tables[0][i] <<", ";
	}
	std::cout << "}, ";
	std::cout << "{";
	for (int i = 0; i < 256; ++i)
	{
		std::cout << hash_tables[1][i] << ", ";
	}
	std::cout << "}, ";
	std::cout << "{";
	for (int i = 0; i < 256; ++i)
	{
		std::cout << hash_tables[2][i] << ", ";
	}
	std::cout << "}}";

	std::cin.get();
	return 0;
}