//
// Created by 孙耀珠 on 2016-06-12.
//

#include "Parser.h"

#include <iostream>
#include <fstream>

#include "vendor/PEGTL/pegtl.hh"

using namespace TinyDB;
using namespace pegtl;

// Parser Rules
namespace TinyDB {

    template <typename T>
    struct padding
        : seq<T, star<space>> {};

    template <typename T>
    struct parentheses
        : if_must<padding<one<'('>>, padding<T>, padding<one<')'>>> {};


    struct number
        : plus<digit> {};

    struct integer
        : seq<opt<one<'+', '-'>>, number> {};

    struct real
        : seq<integer, opt<seq<one<'.'>, number>>> {};

    struct character
        : if_must<one<'\''>, star<not_one<'\''>>, one<'\''>> {};

    struct value
        : sor<integer, real, character> {};


    struct integer_type
        : padding<sor<pegtl_istring_t("int"), pegtl_istring_t("integer")>> {};

    struct real_type
        : padding<sor<pegtl_istring_t("float"), pegtl_istring_t("real")>> {};

    struct character_type
        : if_must<padding<sor<pegtl_istring_t("char"), pegtl_istring_t("character")>>,
                  parentheses<number>> {};

    struct data_type
        : sor<integer_type, real_type, character_type> {};


    struct table_name
        : identifier {};

    struct index_name
        : identifier {};

    struct column_name
        : identifier {};


    struct column_unique
        : pegtl_istring_t("UNIQUE") {};

    struct column_item
        : seq<padding<column_name>, padding<data_type>, opt<column_unique>> {};

    struct primary_key
        : seq<padding<pegtl_istring_t("PRIMARY")>, padding<pegtl_istring_t("KEY")>,
              parentheses<column_name>> {};

    struct column_list
        : list<sor<column_item, primary_key>, padding<one<','>>> {};


    struct comparison_operator
        : sor<one<'='>, istring<'<', '>'>, istring<'!', '='>,
              one<'<'>, one<'>'>, istring<'<', '='>, istring<'>', '='>> {};

    struct comparison
        : seq<padding<column_name>, padding<comparison>, padding<value>> {};

    struct condition
        : seq<padding<pegtl_istring_t("WHERE")>, list<comparison, padding<pegtl_istring_t("AND")>>> {};


    struct create_table
        : seq<padding<pegtl_istring_t("CREATE")>, padding<pegtl_istring_t("TABLE")>,
              parentheses<column_list>> {};

    struct drop_table
        : seq<padding<pegtl_istring_t("DROP")>, padding<pegtl_istring_t("TABLE")>,
              padding<table_name>> {};

    struct create_index
        : seq<padding<pegtl_istring_t("CREATE")>, padding<pegtl_istring_t("INDEX")>,
              padding<index_name>, padding<pegtl_istring_t("ON")>,
              padding<table_name>, parentheses<column_name>> {};

    struct drop_index
        : seq<padding<pegtl_istring_t("DROP")>, padding<pegtl_istring_t("INDEX")>,
              padding<index_name>> {};

    struct select_star_from
        : if_must<padding<pegtl_istring_t("SELECT")>, padding<one<'*'>>,
                  padding<pegtl_istring_t("FROM")>, padding<table_name>, opt<condition>> {};

    struct insert_into
        : if_must<padding<pegtl_istring_t("INSERT")>, padding<pegtl_istring_t("INTO")>,
                  padding<table_name>, padding<pegtl_istring_t("VALUES")>,
                  parentheses<list<padding<value>, padding<one<','>>>>> {};

    struct delete_from
        : if_must<padding<pegtl_istring_t("DELETE")>, padding<pegtl_istring_t("FROM")>,
                  padding<table_name>, opt<condition>> {};

    struct statement
        : must<sor<create_table, drop_table, create_index, drop_index,
                   select_star_from, insert_into, delete_from>> {};

    struct statements
        : star<seq<statement, padding<one<';'>>>> {};


    struct quit
        : padding<pegtl_istring_t("quit")> {};

    struct execfile
        : if_must<padding<pegtl_istring_t("execfile")>, padding<character>> {};

}

namespace TinyDB {

    class ParserState {
    public:
        ParserState(Database &db)
        : db(db) {};

        enum class DataType {
            Integer, Real, Character
        };

        Database &db;
        std::string integer, real, character;
        DataType data_type;
        std::string table_name, index_name, column_name;
        bool unique;
        std::string cmp_op;
    };

}

// Parser Actions
namespace TinyDB {

    template<typename Rule>
    struct action: nothing<Rule> {};

    template<> struct action<integer> {
        static void apply(const action_input &in, ParserState &state) {
            state.integer = in.string();
        }
    };

    template<> struct action<real> {
        static void apply(const action_input &in, ParserState &state) {
            state.real = in.string();
        }
    };

    template<> struct action<character> {
        static void apply(const action_input &in, ParserState &state) {
            state.character = in.string().substr(1, in.string().size() - 2);
        }
    };


    template<> struct action<integer_type> {
        static void apply(const action_input &in, ParserState &state) {
            state.data_type = ParserState::DataType::Integer;
        }
    };

    template<> struct action<real_type> {
        static void apply(const action_input &in, ParserState &state) {
            state.data_type = ParserState::DataType::Real;
        }
    };

    template<> struct action<character_type> {
        static void apply(const action_input &in, ParserState &state) {
            state.data_type = ParserState::DataType::Character;
        }
    };


    template<> struct action<table_name> {
        static void apply(const action_input &in, ParserState &state) {
            state.table_name = in.string();
        }
    };

    template<> struct action<index_name> {
        static void apply(const action_input &in, ParserState &state) {
            state.index_name = in.string();
        }
    };

    template<> struct action<column_name> {
        static void apply(const action_input &in, ParserState &state) {
            state.column_name = in.string();
        }
    };


    template<> struct action<column_unique> {
        static void apply(const action_input &in, ParserState &state) {
            state.unique = true;
        }
    };

    template<> struct action<column_item> {
        static void apply(const action_input &in, ParserState &state) {
            // Add column (column_name, data_type, unique)
            state.unique = false;
        }
    };


    template<> struct action<comparison_operator> {
        static void apply(const action_input &in, ParserState &state) {
            state.cmp_op = in.string();
        }
    };


    template<> struct action<quit> {
        static void apply(const action_input &in, ParserState &state) {
            throw Parser::Quit();
        }
    };

    template<> struct action<execfile> {
        static void apply(const action_input &in, ParserState &state) {
            auto filename = state.character;
            throw Parser::Execfile(filename);
        }
    };

//    template<> struct action<> {
//        static void apply(const action_input &in, ParserState &state) {
//
//        }
//    };

}

void Parser::exec(std::istream &stream) {
    ParserState state(db);
    try {
        parse_istream<statements, action>(stream, "", SIZE_T_MAX, state);
    } catch (Execfile &e) {
        std::ifstream f(e.filename);
        exec(f);
    }
}