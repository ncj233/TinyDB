//
// Created by 孙耀珠 on 2016-06-16.
//

#include "Database.h"

using namespace TinyDB;
using namespace std;

static void format(string &str, int len) {
    if (str.size() < len) str += string(len - str.size(), ' ');
    else str = str.substr(0, static_cast<size_t>(len));
}

void Database::create_table(string table_name) {
    catalog_manager->create_table(table_name);
    record_manager->create_table(table_name);
}

void Database::drop_table(string table_name) {
    auto indexes = catalog_manager->all_index_on_table(table_name);
    for (const auto &index : indexes) {
        catalog_manager->drop_index(index);
    }
    record_manager->drop_table(table_name);
    catalog_manager->drop_table(table_name);
}

void Database::create_index(string index_name, string table_name, string column_name) {
    catalog_manager->create_index(index_name, table_name, column_name);
}

void Database::drop_index(string index_name) {
    catalog_manager->drop_index(index_name);
}

vector<vector<string>> Database::select(string table_name, vector<Comparison> comparisons) {
    auto columns = catalog_manager->get_all_attribute_info(table_name);
    for (auto &cmp : comparisons) {
        auto info = catalog_manager->get_attribute_info(table_name, cmp.parameter);
        if (info.type != DataType::Character) continue;
        format(cmp.operand, info.length);
    }
    return record_manager->select(table_name, comparisons, columns);
}

void Database::insert_into(string table_name, vector<string> values) {
    auto columns = catalog_manager->get_all_attribute_info(table_name);
    if (columns.size() != values.size()) throw NumberOfValuesError();
    for (auto i = 0; i < values.size(); i++) {
        if (columns[i].type != DataType::Character) continue;
        format(values[i], columns[i].length);
    }
    record_manager->insert_into(table_name, values, columns);
}

void Database::delete_from(string table_name, vector<Comparison> comparisons) {
    auto columns = catalog_manager->get_all_attribute_info(table_name);
    record_manager->delete_from(table_name, comparisons, columns);
}


void Database::add_column(string table_name, string column_name, DataType type, int size, bool is_unique) {
    if (type == DataType::Character && (size <= 0 || size > 255)) throw SizeOfCharError();
    catalog_manager->add_attribute(table_name, column_name, type, size, is_unique);
}

void Database::set_primary(string table_name, string column_name) {
    catalog_manager->set_primary(table_name, column_name);
}
