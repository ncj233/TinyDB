//
// Created by 孙耀珠 on 2016-06-16.
//

#ifndef TINYDB_DATABASE_H
#define TINYDB_DATABASE_H

#include "src/catalog/CatalogManager.h"
#include "src/record/RecordManager.h"

namespace TinyDB {

    class Database {
    public:
        Database()
        : catalog_manager(std::make_unique<CatalogManager>()),
          record_manager(std::make_unique<RecordManager>()) {}

        void create_table(std::string table_name);
        void drop_table(std::string table_name);
        void create_index(std::string index_name, std::string table_name, std::string column_name);
        void drop_index(std::string index_name);
        std::vector<std::vector<std::string>> select(std::string table_name, std::vector<Comparison> comparisons);
        void insert_into(std::string table_name, std::vector<std::string> values);
        void delete_from(std::string table_name, std::vector<Comparison> comparisons);

        void add_column(std::string table_name, std::string column_name, DataType type, int size, bool is_unique);
        void set_primary(std::string table_name, std::string column_name);

        class NumberOfValuesError : std::exception {};
        class SizeOfCharError : std::exception {};

    private:
        std::unique_ptr<CatalogManager> catalog_manager;
        std::unique_ptr<RecordManager> record_manager;
    };

}

#endif //TINYDB_DATABASE_H
