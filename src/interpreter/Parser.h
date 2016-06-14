//
// Created by 孙耀珠 on 2016-06-12.
//

#ifndef TINYDB_PARSER_H
#define TINYDB_PARSER_H

#include <string>
#include <exception>

namespace TinyDB {

    class Database {};

    class Parser {
    public:
        Parser(Database &db) : db(db) {};

        void exec(std::istream &);

        class Execfile: public std::exception {
        public:
            Execfile(std::string filename) : filename(filename) {};
            std::string filename;
        };
        class Quit: public std::exception {};

    private:
        Database &db;
    };

}

#endif //TINYDB_PARSER_H
