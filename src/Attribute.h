#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>


class Attribute
{
public:
	Attribute(std::fstream& file);
	Attribute(std::string name, int type, int length, bool is_unique = false, bool has_index = false, bool is_primary = false);
	Attribute(const Attribute& attr) = default;
	virtual ~Attribute() = default;
	void save(std::fstream& file);
	std::string get_name();
	int get_type();
	int get_length();
	bool is_primary();
	bool is_unique();
	bool has_index();
	void set_primary_key();
	void set_unique();

private:
	std::string name;
	int type;
	int length;
	int bitmap;
};

