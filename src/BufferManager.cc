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
       Class Page
    */

    class Page {
    private:
        char * const p;
        int lock, dirty;
        int last_use_time;
    public:
        //static member varibles
        static int capacity;

        //ctors
        Page(const std::string& file_name, int pid_);

        //interfaces
        char getc(int offset) const {
            return *(p+offset);
        }

        void set_lock() {
            lock = 1;
        }

        void unset_lock() {
            lock = 0;
        }
    };

    /*
       Interfaces for Users
    */

    //for table users
    Page& pull(Table& tb, int pid, int* err_code);
    Page& create(Table& tb, int user, int* err_code);
    int commit(Table& tb, int user, int* errcode);

    //for index users
    //Page& pull(Index& idx, int pid, int* err_code);
    //Page& create(Index& idx, int user, int* err_code);
    //int commit(Index& idx, int user, int* errcode);

    //for catalog users
    //Page& pull(int pid, int* err_code);
    //Page& create(int user, int* err_code);
    //int commit(int user, int* errcode);

    /*
        Internal ultilities
     */
}



namespace BufferManager
{

    /*
        Definitions of some  maintainers
     */

    std::priority_queue<int, std::vector<Page*>, >

    std::map<int, Page*>& table_page_map = *(new std::map<int, Page*>());
    std::map<int, Page*>& index_page_map = *(new std::map<int, Page*>());
    std::map<int, Page*>& catalog_page_map = *(new std::map<int, Page*>());

    int& current_table_page_num = *(new int(0));
    int& current_index_page_num = *(new int(0));
    int& current_catalog_page_num = *(new int(0));

    //Definition of static members
    int Page::capacity = PAGE_SIZE; //4KB

    //Definition of class Page
    Page::Page(const std::string& file_name, int pid_):
        p(new char[PAGE_SIZE]()), lock(0), dirty(0), last_use_time(-1)
    {
        std::ifstream ifs(file_name.c_str(), std::ios::in | std::ios::binary);
        ifs.seekg(pid_ * PAGE_SIZE);
        char *pw = p;
        for (int i=0; i<PAGE_SIZE; ++i) *pw++ = ifs.get();
        ifs.close();
    }

    //Definition of interfaces
    //Page::Page& pull(Table& tb, int pid, int*err_code) {

    //}

    Page& create(Table& tb, int pid, int *err_code) {
        if (current_table_page_num < BUFFER_PAGE_LIMIT) {
            Page& new_page = *(new Page(tb.get_name(), pid));
            new_page->set_lock();


            return *p;
        }
        else {
            //do some swap
        }
    }

}

//debug
int main() {
    namespace BM = BufferManager;
    BM::Page p1(std::string("a.txt"), 0);
    for (int i=0; i<BM::PAGE_SIZE; ++i) std::cout << p1.getc(i);
}
