#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>

class IndexName
{
public:
	IndexName(std::fstream& file);
	IndexName(std::string index_name, std::string table_name, std::string attribute_name);

	void save(std::fstream& file);
	bool name_equal(std::string index_name);
	std::string get_table_name();
	std::string get_attribute_name();
	std::string get_index_name();

private:
	std::string index_name;
	std::string table_name;
	std::string attribute_name;
};

