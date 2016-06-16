#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Attribute.h"

class Table
{
public:
	Table(std::fstream& file);
	Table(std::string table_name);

	void save(std::fstream& file);
	void add_attribute(std::string name, DataType type, int length, bool is_unique);
	bool has_attribute(std::string attribute_name);
	std::string get_name();
	bool has_primary_attribute();
	void set_primary(std::string attribute_name);
	void reset_index(std::string attribute_name);
	void set_index(std::string attribute_name);
	std::vector<std::string> get_all_attribute_name();
	AttributeInfo get_attribute_info(std::string attribute_name);
	std::vector<AttributeInfo> get_all_attribute_info();

	class AttributeNameExist : public std::exception {};
	class AttributeNotExist : public std::exception {};
	class HasPrimaryKey : public std::exception {};

private:
	Attribute& find_attribute(std::string attribute_name);		//you must check has_attribute() before
	Attribute& primary_attribute();

	std::string name;
	std::vector<Attribute> attribute;
};
