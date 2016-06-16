#include "IndexName.h"

IndexName::IndexName(std::fstream & file)
{
	char *buf = new char[64 * 3];
	file.read(buf, 64 * 3);
	index_name = buf;
	table_name = buf + 64;
	attribute_name = buf + 128;
	delete []buf;
}

IndexName::IndexName(std::string index_name, std::string table_name, std::string attribute_name)
{
	this->index_name = index_name;
	this->table_name = table_name;
	this->attribute_name = attribute_name;
}

void IndexName::save(std::fstream & file)
{
	char *buf = new char[64 * 3];
	const char *str;
	str = index_name.c_str();
	strncpy(buf, str, 64);
	str = table_name.c_str();
	strncpy(buf + 64, str, 64);
	str = attribute_name.c_str();
	strncpy(buf + 128, str, 64);
	file.write(buf, 192);
	delete[]buf;
}

bool IndexName::name_equal(std::string index_name)
{
	return index_name == this->index_name;
}

std::string IndexName::get_table_name()
{
	return table_name;
}

std::string IndexName::get_attribute_name()
{
	return attribute_name;
}

std::string IndexName::get_index_name()
{
	return index_name;
}
