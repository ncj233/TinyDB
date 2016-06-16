#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "src/catalog/Attribute.h"

enum class Operation {
	eq, ne, gt, lt, ge, le
};

struct Comparison {
	Comparison(std::string parameter, Operation opt, std::string operand) :
		parameter(parameter), opt(opt), operand(operand) {};
	std::string parameter;
	Operation opt;
	std::string operand;
};

class RecordManager
{
public:
	RecordManager();

	void create_table(std::string table_name);
	void drop_table(std::string table_name);
	void insert_into(std::string table_name, std::vector<std::string> elements, std::vector<AttributeInfo> info);
	std::vector<std::vector<std::string>> select(std::string table_name, std::vector<Comparison> comp, std::vector<AttributeInfo> info);
	void delete_from(std::string table_name, std::vector<Comparison> comp, std::vector<AttributeInfo> info);

	class ItemsNumberNotEq : public std::exception {};
	class AttributeInfoNotFound : public std::exception {};

private:
	bool compare(std::string parameter, Operation opt, std::string operand, DataType type);
	std::vector<std::string> get_tuple(std::string table_name, int tid, std::vector<AttributeInfo> info);
	std::vector<int> select_tid(std::string table_name, std::vector<Comparison> comp, std::vector<AttributeInfo> info);
	bool check_tuple(std::vector<std::string> tuple, std::vector<Comparison> comp, std::vector<AttributeInfo> info);
	int get_info_num(std::string attribute_name, std::vector<AttributeInfo> info);
	void delete_tuple(std::string table_name, int tid, std::vector<AttributeInfo> info);
	int get_new_tid(std::string table_name);
	int get_tid_num(std::string table_name);
	int get_size(std::vector<AttributeInfo> info);
};
