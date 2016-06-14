#include "Table.h"

using namespace std;


Table::Table(std::fstream & file)
{
	char *buf = new char[64];
	file.read(buf, 64);
	name = buf;
	int num = file.get();
	for (int i = 0; i < num; i++) {
		attribute.push_back(Attribute(file));
	}
	delete[]buf;
}

Table::Table(std::string table_name)
{
	name = table_name;
}

void Table::save(std::fstream & file)
{
	char *buf = new char[64];
	int len = name.length();
	const char *str = name.c_str();
	strncpy_s(buf, len + 1, str, 64);
	file.write(buf, 64);

	file.put((char)attribute.size());

	vector<Attribute>::iterator it;
	for (it = attribute.begin(); it != attribute.end(); it++) {
		it->save(file);
	}

	delete[] buf;
}

bool Table::add_attribute(std::string name, int type, int length, bool is_unique)
{
	if (has_attribute(name))
		return false;
	attribute.push_back(Attribute(name, type, length, is_unique));
	return true;
}

bool Table::has_attribute(std::string attribute_name)
{
	vector<Attribute>::iterator it;
	for (it = attribute.begin(); it != attribute.end(); it++) {
		if (it->get_name() == attribute_name)
			return true;
	}
	return false;
}

Attribute & Table::find_attribute(std::string attribute_name)
{
	vector<Attribute>::iterator it;
	for (it = attribute.begin(); it != attribute.end(); it++) {
		if (it->get_name() == attribute_name)
			return *it;
	}
	assert(0);
	return *it;
}

bool Table::has_primary_attribute()
{
	vector<Attribute>::iterator it;
	for (it = attribute.begin(); it != attribute.end(); it++) {
		if (it->is_primary())
			return true;
	}
	return false;
}

Attribute & Table::primary_attribute()
{
	vector<Attribute>::iterator it;
	for (it = attribute.begin(); it != attribute.end(); it++) {
		if (it->is_primary())
			return *it;
	}
	assert(false);
	return *it;
}

bool Table::set_primary(std::string attribute_name)
{
	if (!has_attribute(attribute_name))
		return false;
	if (has_primary_attribute())
		return false;
	Attribute &attr = find_attribute(attribute_name);
	attr.set_primary_key();
	attr.set_unique();
	return true;
}
