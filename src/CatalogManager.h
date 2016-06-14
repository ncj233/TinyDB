#pragma once

#include <iostream>
#include <fstream>
#include "Table.h"
#include "IndexName.h"
#include <assert>

class CatalogManager
{
public:
	CatalogManager();
	CatalogManager(const CatalogManager & cata) = default;
	virtual ~CatalogManager() = default;
	void save();
	bool create_table(std::string table_name);
	bool create_index(std::string index_name, std::string table_name, std::string attribute_name);
	bool has_table(std::string name);
	bool has_index(std::string name);
	bool drop_table(std::string table_name);
	bool drop_index(std::string index_name);


private:
	std::vector<Table> table;
	std::vector<IndexName> index;
};