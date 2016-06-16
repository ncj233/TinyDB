#include "CatalogManager.h"
#include <cassert>

using namespace std;


CatalogManager::CatalogManager()
{
	fstream file;
	file.open("database", ios::binary | ios::in);
	if (!file.is_open())
		return;
	int index_num = file.get();
	int table_num = file.get();
	for (int i = 0; i < index_num; i++)
		index.push_back(IndexName(file));
	for (int i = 0; i < table_num; i++)
		table.push_back(Table(file));
	file.close();
}

CatalogManager::~CatalogManager()
{
	save();
}

void CatalogManager::save()
{
	fstream file;
	file.open("database", ios::binary | ios::out);
	assert(file.is_open());
	int index_num = index.size();
	int table_num = table.size();
	file.put((char)index_num);
	file.put((char)table_num);
	
	for (vector<IndexName>::iterator it = index.begin(); it != index.end(); it++) {
		it->save(file);
	}

	for (vector<Table>::iterator it = table.begin(); it != table.end(); it++) {
		it->save(file);
	}
	file.close();
}

void CatalogManager::create_table(std::string table_name)
{
	if (has_table(table_name))
		throw(TableExist());
	table.push_back(Table(table_name));
}

void CatalogManager::create_index(std::string index_name, std::string table_name, std::string attribute_name)
{
	if (has_index(index_name))
		throw(IndexNameExist());
	Table & table = find_table(table_name);
	table.set_index(attribute_name);
	index.push_back(IndexName(index_name, table_name, attribute_name));
}

void CatalogManager::add_attribute(std::string table_name, std::string attribute_name, DataType type, int length, bool is_unique)
{
	for (vector<Table>::iterator it = table.begin(); it != table.end(); it++) {
		if (table_name == it->get_name()) {
			it->add_attribute(attribute_name, type, length, is_unique);
			return;
		}
	}
	throw(InvalidTableName());
}

void CatalogManager::set_primary(std::string table_name, std::string attribute_name)
{
	if (!has_table(table_name))
		throw(TableNotExist());
	Table& table = find_table(table_name);
	table.set_primary(attribute_name);
}

void CatalogManager::drop_table(std::string table_name)
{
	if (!has_table(table_name))
		throw(TableNotExist());
	for (vector<Table>::iterator it = table.begin(); it != table.end(); it++) {
		if (table_name == it->get_name()) {
			table.erase(it);
			return;
		}
	}
}

void CatalogManager::drop_index(std::string index_name)
{
	if (!has_index(index_name))
		throw(IndexNotExist());
	for (vector<IndexName>::iterator it = index.begin(); it != index.end(); it++) {
		if (it->name_equal(index_name)) {
			Table & table = find_table(it->get_table_name());
			table.reset_index(it->get_attribute_name());
			index.erase(it);
			return;
		}
	}
}

vector<std::string> CatalogManager::get_attribute_name(std::string table_name)
{
	Table & table = find_table(table_name);
	return table.get_all_attribute_name();
}

vector<std::string> CatalogManager::all_index_on_table(std::string table_name)
{
	vector<std::string> all_index;
	for (vector<IndexName>::iterator it = index.begin(); it != index.end(); it++)
		if (it->get_table_name() == table_name)
			all_index.push_back(it->get_index_name());
	return all_index;
}

AttributeInfo CatalogManager::get_attribute_info(std::string table_name, std::string attribute_name)
{
	Table & table = find_table(table_name);
	return table.get_attribute_info(attribute_name);
}

std::vector<AttributeInfo> CatalogManager::get_all_attribute_info(std::string table_name)
{
	Table & t = find_table(table_name);
	return t.get_all_attribute_info();
}

bool CatalogManager::has_table(std::string name)
{
	for (vector<Table>::iterator it = table.begin(); it != table.end(); it++) {
		if (name == it->get_name()) {
			return true;
		}
	}
	return false;
}

bool CatalogManager::has_index(std::string name)
{
	for (vector<IndexName>::iterator it = index.begin(); it != index.end(); it++) {
		if (it->name_equal(name)) {
			return true;
		}
	}
	return false;
}

Table & CatalogManager::find_table(std::string table_name)
{
	for (vector<Table>::iterator it = table.begin(); it != table.end(); it++) {
		if (table_name == it->get_name()) {
			return *it;
		}
	}
	throw(TableNotExist());
}

