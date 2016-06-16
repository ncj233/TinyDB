#include "RecordManager.h"

using namespace std;

RecordManager::RecordManager()
{
}

void RecordManager::createTable(std::string table_name)
{
	fstream file(table_name + string(".table"), ios::binary | ios::out);
	file << int(0);
}

void RecordManager::dropTable(std::string table_name)
{
	string file_name = table_name + string(".table");
	remove(file_name.c_str());
}

void RecordManager::insert_into(std::string table_name, std::vector<std::string> elements, vector<AttributeInfo> info)
{
	int pid = get_new_tid(table_name);
	int size = get_size(info);
	int offset = 4096 + (pid / (4096 / size)) * 4096 + (pid % (4096 / size))*size;
	fstream file(table_name + string(".table"), ios::binary | ios::out | ios::in);
	file.seekg(offset, ios::beg);
	file.put(1);
	int num_of_tuple;
	if (elements.size() == info.size())
		num_of_tuple = elements.size();
	else
		throw(ItemsNumberNotEq());
	for (int i = 0; i < num_of_tuple; i++) {
		int* num_int;
		float* num_float;
		char buf[4];
		switch (info[i].type) {
		case DataType::Integer:
			num_int = new int(stoi(elements[i]));
			buf[0] = *((char *)num_int);
			buf[1] = *((char *)num_int + 1);
			buf[2] = *((char *)num_int + 2);
			buf[3] = *((char *)num_int + 3);
			delete num_int;
			file.write(buf, 4);
			break;
		case DataType::Real:
			num_float = new float(stof(elements[i]));
			buf[0] = *((char *)num_float);
			buf[1] = *((char *)num_float + 1);
			buf[2] = *((char *)num_float + 2);
			buf[3] = *((char *)num_float + 3);
			delete num_float;
			file.write(buf, 4);
			break;
		case DataType::Character:
			file << elements[i];
			break;
		}
	}
}

vector<vector<string>> RecordManager::select(string table_name, vector<Comparison> comp, vector<AttributeInfo> info)
{
	vector<int> all_tid = select_tid(table_name, comp, info);
	vector<vector<string>> table;
	for (vector<int>::iterator it = all_tid.begin(); it != all_tid.end(); it++) {
		table.push_back(get_tuple(table_name, *it, info));
	}
	return table;
}

void RecordManager::delete_from(string table_name, vector<Comparison> comp, vector<AttributeInfo> info)
{
	vector<int> all_tid = select_tid(table_name, comp, info);
	for (vector<int>::iterator it = all_tid.begin(); it != all_tid.end(); it++) {
		delete_tuple(table_name, *it, info);
	}
}

bool RecordManager::compare(string parameter, Operation opt, string operand, DataType type)
{
	bool ans;
	if (type == DataType::Integer) {
		switch (opt) {
		case Operation::eq:
			ans = stoi(parameter) == stoi(operand);
			break;
		case Operation::ne:
			ans = stoi(parameter) != stoi(operand);
			break;
		case Operation::gt:
			ans = stoi(parameter) > stoi(operand);
			break;
		case Operation::ge:
			ans = stoi(parameter) >= stoi(operand);
			break;
		case Operation::lt:
			ans = stoi(parameter) < stoi(operand);
			break;
		case Operation::le:
			ans = stoi(parameter) <= stoi(operand);
			break;
		}
	}
	else if (type == DataType::Real) {
		switch (opt) {
		case Operation::eq:
			ans = stof(parameter) == stof(operand);
			break;
		case Operation::ne:
			ans = stof(parameter) != stof(operand);
			break;
		case Operation::gt:
			ans = stof(parameter) > stof(operand);
			break;
		case Operation::ge:
			ans = stof(parameter) >= stof(operand);
			break;
		case Operation::lt:
			ans = stof(parameter) < stof(operand);
			break;
		case Operation::le:
			ans = stof(parameter) <= stof(operand);
			break;
		}
	}
	else if (type == DataType::Character) {
		switch (opt) {
		case Operation::eq:
			ans = parameter == operand;
			break;
		case Operation::ne:
			ans = parameter != operand;
			break;
		case Operation::gt:
			ans = parameter > operand;
			break;
		case Operation::ge:
			ans = parameter >= operand;
			break;
		case Operation::lt:
			ans = parameter < operand;
			break;
		case Operation::le:
			ans = parameter <= operand;
			break;
		}
	}
	return ans;
}

vector<string> RecordManager::get_tuple(string table_name, int tid, vector<AttributeInfo> info)
{
	int size = get_size(info);
	int offset = 4096 + (tid / (4096 / size)) * 4096 + (tid % (4096 / size))*size;
	vector<string> tuple;
	fstream file(table_name + string(".table"), ios::in);
	file.seekg(offset, ios::beg);
	int is_not_delete = file.get();
	if (!is_not_delete)
		return tuple;
	vector<AttributeInfo>::iterator it;
	for (it = info.begin(); it != info.end(); it++) {
		string element;
		char *buf;
		int num_int;
		float num_float;
		switch (it->type) {
		case DataType::Integer:
			buf = new char[4];
			file.read(buf, 4);
			num_int = *((int*)buf);
			tuple.push_back(to_string(num_int));
			delete[]buf;
			break;
		case DataType::Real:
			buf = new char[4];
			file.read(buf, 4);
			num_float = *((float*)buf);
			tuple.push_back(to_string(num_float));
			delete[]buf;
			break;
		case DataType::Character:
			buf = new char[300];
			file.read(buf, it->length);
			buf[it->length] = '\0';
			tuple.push_back(string(buf));
			delete[]buf;
			break;
		}
	}
	return tuple;
}

vector<int> RecordManager::select_tid(string table_name, vector<Comparison> comp, vector<AttributeInfo> info)
{
	vector<int> has_select;
	int tuple_num = get_tid_num(table_name);
	for (int i = 0; i < tuple_num; i++) {
		vector<string> tuple = get_tuple(table_name, i, info);
		if (tuple.size() == 0)
			continue;
		if (check_tuple(tuple, comp, info))
			has_select.push_back(i);
	}
	return has_select;
}

bool RecordManager::check_tuple(vector<string> tuple, vector<Comparison> comp, vector<AttributeInfo> info)
{
	vector<Comparison>::iterator it;
	for (it = comp.begin(); it != comp.end(); it++) {
		int num = get_info_num(it->parameter, info);
		if (!compare(tuple[num], it->opt, it->operand, info[num].type))
			return false;
	}

	return true;
}

int RecordManager::get_info_num(string attribute_name, vector<AttributeInfo> info)
{
	int size = info.size();
	for (int i = 0; i < size; i++)
		if (attribute_name == info[i].name)
			return i;
	throw (AttributeInfoNotFound());
}

void RecordManager::delete_tuple(string table_name, int tid, vector<AttributeInfo> info)
{
	int size = get_size(info);
	int offset = 4096 + (tid / (4096 / size)) * 4096 + (tid % (4096 / size))*size;
	fstream file(table_name + string(".table"), ios::out | ios::in);
	file.seekg(offset, ios::beg);
	file.put(0);
}

int RecordManager::get_new_tid(string table_name)
{
	fstream file;
	file.open(table_name + string(".table"), ios::in);
	int tid;
	file >> tid;
	file.close();
	file.open(table_name + string(".table"), ios::out | ios::in);
	file << (tid + 1);
	file.close();
	return tid;
}

int RecordManager::get_tid_num(string table_name)
{
	fstream file(table_name + string(".table"), ios::in);
	int all_num;
	file >> all_num;
	return all_num;
}

int RecordManager::get_size(vector<AttributeInfo> info)
{
	vector<AttributeInfo>::iterator it;
	int size = 1;
	for (it = info.begin(); it != info.end(); it++)
		if (it->type == DataType::Character)
			size = size + it->length;
		else
			size = size + 4;
	return size;
}
