#pragma once
#include <fstream>

#define GLbyte char

struct lwrite
{
	unsigned long value;
	unsigned      size;
	lwrite(unsigned long value, unsigned size) :
		value(value), size(size)
	{ }
};

inline std::ostream& operator << (std::ostream& outs, const lwrite& v);
void StoreIMG(GLbyte* data, int width, int height);