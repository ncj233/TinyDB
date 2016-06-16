#include <chrono>

#include "vendor/PEGTL/pegtl/input_error.hh"
#include "vendor/PEGTL/pegtl/parse_error.hh"

#include "src/api/Database.h"
#include "src/interpreter/Parser.h"

using namespace std;
using namespace TinyDB;

int main() {
    Database database;
    Parser parser(database);

    while (cin.good()) {
        string sql, line;
        while (cin.good() && sql.find(';') == string::npos) {
            if (sql.empty()) cout << "TinyDB> ";
            else cout << "     -> ";
            getline(cin, line);
            sql += line + " ";
        }
        if (line.empty()) break;
        try {
            auto start = chrono::system_clock::now();
            parser.exec(sql.substr(0, sql.find(';') + 1));
            auto end = chrono::system_clock::now();
            auto duration = chrono::duration<double>(end - start);
            cerr << "(" << duration.count() << "s)\n";
        } catch (Parser::Quit) {
            cout << "Bye\n";
            break;
        } catch (pegtl::input_error) {
            cerr << "Execfile error: cannot open the file.\n";
        } catch (pegtl::parse_error) {
            cerr << "Parse error: unrecognized syntax.";
        } catch (Database::NumberOfValuesError) {
            cerr << "The number of values to be inserted does not match schema.\n";
        } catch (Database::SizeOfCharError) {
            cerr << "For char(n), there should be 0 < n <= 255.\n";
        } catch (CatalogManager::TableExist) {
            cerr << "Table name exists.\n";
        } catch (CatalogManager::TableNotExist) {
            cerr << "Table name does not exist.\n";
        } catch (CatalogManager::IndexNameExist) {
            cerr << "Index name exists.\n";
        } catch (CatalogManager::IndexNotExist) {
            cerr << "Index name does not exist.\n";
        }
    }

    return 0;
}
