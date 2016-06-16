#pragma once
#include <iostream>
#include <fstream>
#include <string>

enum class DataType {
	Integer, Real, Character
};

struct AttributeInfo {
	AttributeInfo(std::string name, DataType type, int length, bool has_index) :
		name(name), type(type), length(length), has_index(has_index) {};

	std::string name;
	DataType type;
	int length;
	bool has_index;
};

class Attribute
{
public:
	Attribute(std::fstream& file);
	Attribute(std::string name, DataType type, int length, bool is_unique = false, bool has_index = false, bool is_primary = false);

	void save(std::fstream& file);
	std::string get_name();
	DataType get_type();
	int get_length();
	bool is_primary();
	bool is_unique();
	bool has_index();
	void set_primary_key();
	void set_unique();
	void set_index();
	void reset_index();
	AttributeInfo get_info();

	class CreateIndexNotOnUnique : public std::exception {};

private:
	std::string name;
	DataType type;
	int length;
	int bitmap;
};