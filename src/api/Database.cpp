//
// Created by 孙耀珠 on 2016-06-16.
//

#include "Database.h"

using namespace TinyDB;
using namespace std;

void Database::create_table(string table_name) {
    catalog_manager->create_table(table_name);
    record_manager->create_table(table_name);
}

void Database::drop_table(string table_name) {
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
    return record_manager->select(table_name, comparisons, columns);
}

void Database::insert_into(string table_name, vector<string> values) {
    auto columns = catalog_manager->get_all_attribute_info(table_name);
    record_manager->insert_into(table_name, values, columns);
}

void Database::delete_from(string table_name, vector<Comparison> comparisons) {
    auto columns = catalog_manager->get_all_attribute_info(table_name);
    record_manager->delete_from(table_name, comparisons, columns);
}


void Database::add_column(string table_name, string column_name, DataType type, int size, bool is_unique) {
    catalog_manager->add_attribute(table_name, column_name, type, size, is_unique);
}

void Database::set_primary(string table_name, string column_name) {
    catalog_manager->set_primary(table_name, column_name);
}
