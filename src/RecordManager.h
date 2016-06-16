#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "Attribute.h"

using std::vector;
using std::string;

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

	void createTable(std::string table_name);
	void dropTable(std::string table_name);
	void insert_into(std::string table_name, std::vector<std::string> elements, vector<AttributeInfo> info);
	//vector<vector<string>> select(string table_name, vector<Comparison> comp, vector<AttributeInfo> info);
	//void delete_from(string table_name, vector<Comparison> comp, vector<AttributeInfo> info);

	class ItemsNumberNotEq : public std::exception {};

//private:
	bool compare(string parameter, Operation opt, string operand, DataType type);
	vector<string> get_tuple(string table_name, int tid, vector<AttributeInfo> info);
	void delete_tuple(string table_name, int tid, vector<AttributeInfo> info);
	int get_new_tid(string table_name);
	int get_tid_num(string table_name);
	int get_size(vector<AttributeInfo> info);
};

