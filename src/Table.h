#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "Attribute.h"
#include <assert.h>

class Table
{
public:
	Table(std::fstream& file);
	Table(std::string table_name);
	Table(const Table& t) = default;
	~Table() = default;
	void save(std::fstream& file);
	bool add_attribute(std::string name, int type, int length, bool is_unique);
	bool has_attribute(std::string attribute_name);
	Attribute& find_attribute(std::string attribute_name);		//you must check has_attribute() before
	bool has_primary_attribute();
	Attribute& primary_attribute();
	bool set_primary(std::string attribute_name);


private:
	std::string name;
	std::vector<Attribute> attribute;
};

