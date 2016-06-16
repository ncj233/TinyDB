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
namespace TinyDB { namespace PEG {

    template <typename T>
    struct padding
        : seq<T, star<space>> {};

    template <typename T>
    struct parentheses
        : if_must<padding<one<'('>>, padding<T>, padding<one<')'>>> {};


    struct real
        : seq<opt<one<'+', '-'>>, plus<digit>, one<'.'>, plus<digit>> {};

    struct integer
        : seq<opt<one<'+', '-'>>, plus<digit>> {};

    struct character
        : if_must<one<'\''>, star<not_one<'\''>>, one<'\''>> {};

    struct value
        : sor<real, integer, character> {};

    struct insert_value
        : value {};


    struct integer_type
        : padding<sor<pegtl_istring_t("int"), pegtl_istring_t("integer")>> {};

    struct real_type
        : padding<sor<pegtl_istring_t("float"), pegtl_istring_t("real")>> {};

    struct character_type
        : if_must<padding<sor<pegtl_istring_t("char"), pegtl_istring_t("character")>>,
                  parentheses<integer>> {};

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

    struct create_table_name
        : seq<padding<pegtl_istring_t("CREATE")>, padding<pegtl_istring_t("TABLE")>, padding<table_name>> {};


    struct comparison_operator
        : sor<one<'='>, istring<'<', '>'>, istring<'!', '='>,
              istring<'<', '='>, istring<'>', '='>, one<'<'>, one<'>'>> {};

    struct comparison
        : seq<padding<column_name>, padding<comparison_operator>, padding<value>> {};

    struct condition
        : seq<padding<pegtl_istring_t("WHERE")>, list<comparison, padding<pegtl_istring_t("AND")>>> {};


    struct create_table
        : seq<create_table_name, parentheses<opt<column_list>>> {};

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
        : seq<padding<pegtl_istring_t("SELECT")>, padding<one<'*'>>,
              padding<pegtl_istring_t("FROM")>, padding<table_name>, opt<condition>> {};

    struct insert_into
        : seq<padding<pegtl_istring_t("INSERT")>, padding<pegtl_istring_t("INTO")>,
              padding<table_name>, padding<pegtl_istring_t("VALUES")>,
              parentheses<list<padding<insert_value>, padding<one<','>>>>> {};

    struct delete_from
        : seq<padding<pegtl_istring_t("DELETE")>, padding<pegtl_istring_t("FROM")>,
              padding<table_name>, opt<condition>> {};

    struct quit
        : padding<pegtl_istring_t("quit")> {};

    struct execfile
        : if_must<padding<pegtl_istring_t("execfile")>, padding<character>> {};

    struct statement
        : must<sor<create_table, drop_table, create_index, drop_index,
                   select_star_from, insert_into, delete_from, quit, execfile>> {};

    struct statements
        : star<seq<statement, padding<one<';'>>>> {};

} }


namespace TinyDB {

    class ParserState {
    public:
        ParserState(Database &db)
        : db(db) {};

        Database &db;
        std::string value;
        std::vector<std::string> inserted_values;
        DataType data_type;
        std::string table_name, index_name, column_name;
        bool is_unique;
        Operation cmp_op;
        std::vector<Comparison> cmp_vec;
    };

}

// Parser Actions
namespace TinyDB { namespace PEG {

    template<typename Rule>
    struct action: nothing<Rule> {};

    template<> struct action<real> {
        static void apply(const action_input &in, ParserState &state) {
            state.value = in.string();
        }
    };

    template<> struct action<integer> {
        static void apply(const action_input &in, ParserState &state) {
            state.value = in.string();
        }
    };

    template<> struct action<character> {
        static void apply(const action_input &in, ParserState &state) {
            state.value = in.string().substr(1, in.string().size() - 2);
        }
    };


    template<> struct action<integer_type> {
        static void apply(const action_input &in, ParserState &state) {
            state.data_type = DataType::Integer;
        }
    };

    template<> struct action<real_type> {
        static void apply(const action_input &in, ParserState &state) {
            state.data_type = DataType::Real;
        }
    };

    template<> struct action<character_type> {
        static void apply(const action_input &in, ParserState &state) {
            state.data_type = DataType::Character;
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
            state.is_unique = true;
        }
    };

    template<> struct action<primary_key> {
        static void apply(const action_input &in, ParserState &state) {
            state.db.set_primary(state.table_name, state.column_name);
        }
    };

    template<> struct action<column_item> {
        static void apply(const action_input &in, ParserState &state) {
            auto size = 0;
            if (state.data_type == DataType::Character) size = stoi(state.value);
            state.db.add_column(state.table_name, state.column_name, state.data_type, size, state.is_unique);
            state.is_unique = false;
        }
    };

    template<> struct action<create_table_name> {
        static void apply(const action_input &in, ParserState &state) {
            state.db.create_table(state.table_name);
        }
    };


    template<> struct action<comparison_operator> {
        static void apply(const action_input &in, ParserState &state) {
            if (in.string() == "=") state.cmp_op = Operation::eq;
            if (in.string() == "<>") state.cmp_op = Operation::ne;
            if (in.string() == "!=") state.cmp_op = Operation::ne;
            if (in.string() == "<") state.cmp_op = Operation::lt;
            if (in.string() == ">") state.cmp_op = Operation::gt;
            if (in.string() == "<=") state.cmp_op = Operation::le;
            if (in.string() == ">=") state.cmp_op = Operation::ge;
        }
    };

    template<> struct action<comparison> {
        static void apply(const action_input &in, ParserState &state) {
            state.cmp_vec.emplace_back(state.column_name, state.cmp_op, state.value);
        }
    };


    template<> struct action<insert_value> {
        static void apply(const action_input &in, ParserState &state) {
            state.inserted_values.push_back(state.value);
        }
    };


    template<> struct action<drop_table> {
        static void apply(const action_input &in, ParserState &state) {
            state.db.drop_table(state.table_name);
        }
    };

    template<> struct action<create_index> {
        static void apply(const action_input &in, ParserState &state) {
            state.db.create_index(state.index_name, state.table_name, state.column_name);
        }
    };

    template<> struct action<drop_index> {
        static void apply(const action_input &in, ParserState &state) {
            state.db.drop_index(state.index_name);
        }
    };

    template<> struct action<select_star_from> {
        static void apply(const action_input &in, ParserState &state) {
            auto table = state.db.select(state.table_name, state.cmp_vec);
            state.cmp_vec.clear();

            for (const auto& vec : table) {
                for (const auto& item : vec) {
                    std::cout << "\t" << item;
                }
                std::cout << std::endl;
            }
        }
    };

    template<> struct action<insert_into> {
        static void apply(const action_input &in, ParserState &state) {
            state.db.insert_into(state.table_name, state.inserted_values);
            state.inserted_values.clear();
        }
    };

    template<> struct action<delete_from> {
        static void apply(const action_input &in, ParserState &state) {
            state.db.delete_from(state.table_name, state.cmp_vec);
        }
    };


    template<> struct action<quit> {
        static void apply(const action_input &in, ParserState &state) {
            throw Parser::Quit();
        }
    };

    template<> struct action<execfile> {
        static void apply(const action_input &in, ParserState &state) {
            auto filename = state.value;
            throw Parser::Execfile(filename);
        }
    };

} }


void Parser::exec(std::string &&sql) {
    ParserState state(db);
    try {
        parse_string<PEG::statements, PEG::action>(sql, "SQL", state);
    } catch (Execfile &e) {
        execfile(std::move(e.filename));
    }
}

void Parser::execfile(std::string &&filename) {
    ParserState state(db);
    parse_file<PEG::statements, PEG::action>(filename, state);
}


//void Parser::exec(std::istream &stream) {
//    ParserState state(db);
//    try {
//        parse_istream<PEG::statements, PEG::action>(stream, "SQL", SIZE_T_MAX, state);
//    } catch (Execfile &e) {
//        std::ifstream f(e.filename);
//        exec(f);
//    }
//}
