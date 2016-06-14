#include "Attribute.h"


Attribute::Attribute(std::fstream& file)
{
	char *buf = new char[67];
	file.read(buf, 67);
	name = buf;
	type = buf[64];
	length = buf[65];
	bitmap = buf[66];
	delete [] buf;
}

Attribute::Attribute(std::string name, int type, int length, bool is_unique, bool has_index, bool is_primary)
{
	this->name = name;
	this->type = type;
	this->length = length;
	bitmap = ((int)is_primary) | ((int)is_unique << 1) | ((int)has_index << 2);
}

/* Attribute::~Attribute()
{
} */

void Attribute::save(std::fstream& file)
{
	char *buf = new char[67];
	int len = name.length();
	const char *str = name.c_str();
	strncpy_s(buf, len+1, str, 64);
	buf[len] = 0;
	buf[64] = (char)type;
	buf[65] = (char)length;
	buf[66] = bitmap;
	file.write(buf, 67);
	delete [] buf;
}

std::string Attribute::get_name()
{
	return name;
}

int Attribute::get_type()
{
	return type;
}

int Attribute::get_length()
{
	return length;
}

bool Attribute::is_primary()
{
	return (bool)(bitmap & 0x1);
}

bool Attribute::is_unique()
{
	bool unique;
	if (bitmap & 0x2)
		unique = true;
	else
		unique = false;
	return unique;
}

bool Attribute::has_index()
{
	bool has_index;
	if (bitmap & 0x4)
		has_index = true;
	else
		has_index = false;
	return has_index;
}

void Attribute::set_primary_key()
{
	bitmap = bitmap | 0x1;
}

void Attribute::set_unique()
{
	bitmap = bitmap | 0x2;
}


