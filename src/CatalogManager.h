#pragma once

#include <iostream>
#include <fstream>
#include "Table.h"
#include "IndexName.h"
#include <assert.h>
#include <exception>

class CatalogManager
{
public:
	CatalogManager();
	~CatalogManager();

	void save();
	void create_table(std::string table_name);
	void create_index(std::string index_name, std::string table_name, std::string attribute_name);
	void add_attribute(std::string table_name, std::string attribute_name, DataType type, int length, bool is_unique);
	void set_primary(std::string table_name, std::string attribute_name);
	void drop_table(std::string table_name);
	void drop_index(std::string index_name);
	std::vector<std::string> get_attribute_name(std::string table_name);

	std::vector<std::string> all_index_on_table(std::string table_name);
	AttributeInfo get_attribute_info(std::string table_name, std::string attribute_name);
	std::vector<AttributeInfo> get_all_attribute_info(std::string table_name);

	class TableExist: public std::exception {};
	class InvalidTableName: public std::exception{};
	class IndexNameExist: public std::exception {};
	class TableNotExist: public std::exception {};
	class IndexNotExist: public std::exception {};

private:
	bool has_table(std::string name);
	bool has_index(std::string name);
	Table& find_table(std::string table_name);

	std::vector<Table> table;
	std::vector<IndexName> index;
};

