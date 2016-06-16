#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <ctime>
#include <map>
#include <vector>

/*
 *for debug
 * */
class Table {
private:
    std::string table_name;
public:
    Table(const std::string& tb_name): table_name(tb_name) {};
    const std::string& get_name() { return table_name; };
};

class Index {
private:
    std::string index_name;
public:
    Index(const std::string& idx_name): index_name(idx_name) {};
    const std::string& get_name() { return index_name; };
};

/*
   Class Page
*/

class Page {
private:

     /*
      *private member objects
      * */
    char * const p;
    int lock, dirty, pid, last_use_time; //these variables can be manipulated by member functions only

    /*
     *private member functions
     * */

public:
    //static member objects
    static int capacity;

    //ctors
    Page();

    //interfaces
    char getc(int offset) const {
        return *(p+offset);
    }

    void set_lock(int mode) {
        this->lock = mode;
    }

    int get_lock() const {
        return this->lock;
    }

    void set_dirty(int mode) {
        this->dirty = mode;
    }

    int get_dirty() const{
        return this->dirty;
    }

    int correlate_with_file() const;

};


//declarations
namespace BufferManager {

    /*
       Some constants
     */

    const int PAGE_SIZE = 4096 * 1024;

    const int BUFFER_PAGE_LIMIT = 10;

    const int RECORD_PAGE = 1;
    const int INDEX_PAGE = 2;
    const int CATALOG_PAGE = 3;

    //error code
    const int CORRELATE_SUCCESS = 1;
    const int FILE_SIZE_TOO_SMALL = 2;

    /*
     *functors
     * */

    class comp_page_fctor {

    private:
    public:
        bool operator() (const Page *p1, const Page *p2) {
            return p1->last_use_time > p2->last_use_time;
        }
    }


    /*
       Interfaces for Users
    */

    //for table users
    const Page& pull(Table& tb, int pid);
    const Page& create(Table& tb, int user);
    int commit(Table& tb, int user);

    //for index users
    //const Page& pull(Index& idx, int pid, int* err_code);
    //const Page& create(Index& idx, int user, int* err_code);
    //int commit(Index& idx, int user, int* errcode);

    //for catalog users
    //const Page& pull(int pid, int* err_code);
    //const Page& create(int user, int* err_code);
    //int commit(int user, int* errcode);

    /*
        Internal ultilities
     */
    class BMExcep {
        private:
            int err_code;
        public:
            BMExcep(int ec): err_code(ec) {};
    }
}


namespace <{1:BufferManager}> {

    /*
        Definitions of some  maintainers
     */

    std::priority_queue<int, std::vector<Page*>, comp_page_fctor>
    std::map<std::string, std::map<int, Page&>& >& file_container = *(new std::map<std::string, std::map<int, Page&>& >);
    int& current_page_num = *(new int(0));

    /*
     *Definition of class Page
     * */
    int Page::capacity = PAGE_SIZE; //4KB

    Page::Page(): p(new char[PAGE_SIZE]()), lock(0), dirty(0), pid(-1), last_use_time(-1) {};

    Page::int correlate_with_file(const string& file_name, int pid) const {
        int return_code = CORRELATE_SUCCESS;
        std::ifstream ifs(file_name.c_str(), std::ios::in | std::ios::binary);

        ifs.seekg(0, std::ios::end);
        if (pid * PAGE_SIZE + PAGE_SIZE - 1 > ifs.tellg()) {
            return_code = FILE_SIZE_TOO_SMALL;
        }
        else {
            ifs.seekg(pid * PAGE_SIZE);
            ifs.read(this->p, PAGE_SIZE);
        }

        ifs.close();
        return return_code;
    }

    /*
     * Definitions of interface
     * */


    //const Page::Page& pull(Table& tb, int pid, int*err_code) {
    //}

    const Page& create(Table& tb, int _pid) {
        if (current_page_num < BUFFER_PAGE_LIMIT) {
            Page& new_page = *(new Page());
            new_page.set_lock();

            int err_code = new_page.correlate_with_file(tb.get_name(), _pid);
            if (err_code == CORRELATE_SUCCESS) {
                table_page_map.insert(new_page.pid)
            }
            else {
                throw BMExcep(err_code);
            }

            return new_page;
        }
        else { //need to do some swap (kick the least-recently-used page)

        }
    }

}


//debug
int main() {
    namespace BM = BufferManager;
    BM::Page p1(std::string("a.txt"), 0);
    for (int i=0; i<BM::PAGE_SIZE; ++i) std::cout << p1.getc(i);
}
