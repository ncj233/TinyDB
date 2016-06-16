#include "Table.h"
#include <cassert>

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
	const char *str = name.c_str();
	strncpy(buf, str, 64);
	file.write(buf, 64);

	file.put((char)attribute.size());

	vector<Attribute>::iterator it;
	for (it = attribute.begin(); it != attribute.end(); it++) {
		it->save(file);
	}

	delete[] buf;
}

void Table::add_attribute(std::string name, DataType type, int length, bool is_unique)
{
	if (has_attribute(name))
		throw(AttributeNameExist());
	attribute.push_back(Attribute(name, type, length, is_unique));
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

std::string Table::get_name()
{
	return name;
}

Attribute & Table::find_attribute(std::string attribute_name)
{
	vector<Attribute>::iterator it;
	for (it = attribute.begin(); it != attribute.end(); it++) {
		if (it->get_name() == attribute_name)
			return *it;
	}
	throw(AttributeNotExist());
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

void Table::set_primary(std::string attribute_name)
{
	if (!has_attribute(attribute_name))
		throw(AttributeNotExist());
	if (has_primary_attribute())
		throw(HasPrimaryKey());
	Attribute &attr = find_attribute(attribute_name);
	attr.set_primary_key();
	attr.set_unique();
	attr.set_index();
}

void Table::reset_index(std::string attribute_name)
{
	Attribute& attr = find_attribute(attribute_name);
	attr.reset_index();
}

void Table::set_index(std::string attribute_name)
{
	Attribute& attr = find_attribute(attribute_name);
	attr.set_index();
}

vector<std::string> Table::get_all_attribute_name()
{
	vector<std::string> all_name;
	for (vector<Attribute>::iterator it = attribute.begin(); it != attribute.end(); it++)
		all_name.push_back(it->get_name());
	return all_name;
}

AttributeInfo Table::get_attribute_info(std::string attribute_name)
{
	Attribute& attr = find_attribute(attribute_name);
	return attr.get_info();
}

std::vector<AttributeInfo> Table::get_all_attribute_info()
{
	vector<AttributeInfo> all_info;
	vector<Attribute>::iterator it;
	for (it = attribute.begin(); it != attribute.end(); it++) {
		all_info.push_back(it->get_info());
	}
	return all_info;
}
