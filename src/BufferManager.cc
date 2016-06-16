#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>

#include <cstring>

#include "BufferManager.h"

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

//definitions
namespace BufferManager {

	/*
	   Definitions of some  maintainers
	 */

	std::map<std::string, Page*> page_cache_pool;

	/*
	 *Definitions of class Page
	 * */

	int Page::fetch_from_disk(const char* _file_name, int _pid) const {
		std::fstream fs(_file_name, std::ios::in | std::ios::binary);

		fs.seekg(0, std::ios::end);
		if (_pid * PAGE_SIZE + PAGE_SIZE - 1 > fs.tellg()) { //extend to pid PAGE
			fs.close();	
			fs.open(_file_name, std::ios::out | std::ios::binary);
			fs.seekp(_pid * PAGE_SIZE + PAGE_SIZE - 1);
			fs.put('\0');
			this->paint_white();
		}
		else {
			fs.seekg(_pid * PAGE_SIZE);
			fs.read(this->p, PAGE_SIZE);
		}

		fs.close();
	}

	void Page::write_back() const {
		std::fstream fs(this->get_file_name(), std::ios::out | std::ios::binary);
		fs.write(this->p, PAGE_SIZE);
		fs.close();
	}

	void Page::swap_out_with(const char* _file_name, int _pid) {
		std::cout << "kicked! and pid = " << this->pid << std::endl;
		this->write_back();
		this->fetch_from_disk(_file_name, _pid);
		this->pid = _pid;
		this->file_name = std::string(_file_name);
	}

	/*
	 * Definitions of interface
	 * */
	
	const Page* pull(const char* _file_name, int _pid) {
		Page* p;
		int err_code;
		std::string uuid_to_pull = convert_to_uuid(_file_name, _pid);

		//check if this page has been cached in memory
		std::map<std::string, Page*>::iterator it;
		it = page_cache_pool.find(uuid_to_pull);
		if (it != page_cache_pool.end()) { //already cached
			debug("hit");
			p = it->second;
			p->set_lock(LOCKED);
		}
		else { //not cached
			if (page_cache_pool.size() < BUFFER_PAGE_LIMIT) { //create a page directly
				p = new Page(_file_name, _pid);
				p->fetch_from_disk(_file_name, _pid);
				page_cache_pool.insert(wrapper_to_pair(p->get_file_name(), p->get_pid(), p));
			}
			else { //swap out the LRU page
				std::map<std::string, Page*>::iterator LRU_it = page_cache_pool.begin();
				for (LRU_it; LRU_it != page_cache_pool.end(); ++LRU_it)
					if (LRU_it->second->get_lock() == UNLOCKED) {
						break;
					}

				it = LRU_it;
				it++;
				for (it; it != page_cache_pool.end(); ++it)
					if (it->second->get_lock() != LOCKED && 
							it->second->get_last_use_time() < LRU_it->second->get_last_use_time()) {
						LRU_it = it;
					}

				p = LRU_it->second;
				page_cache_pool.erase(LRU_it);
				p->swap_out_with(_file_name, _pid);
				p->set_lock(LOCKED);
				page_cache_pool.insert(wrapper_to_pair(_file_name, _pid, p));
			}
		}
		return p;
	}

	int commit(const char* _file_name, int _pid) {
		std::map<std::string, Page*>::iterator it;
		it = page_cache_pool.find(convert_to_uuid(_file_name, _pid));
		if (it != page_cache_pool.end()) {
			it->second->set_lock(UNLOCKED);
			it->second->set_last_use_time();
		}
		else {
			throw BMExcep(COMMIT_FAIL);
		}
	}

}


//debug
int main() {
	namespace BM = BufferManager;
	const BM::Page* p[10];
	for (int i=0; i<10; ++i) {
		p[i] = BM::pull("test.txt", i);
		std::cout << "pid ==> " << p[i]->get_pid() << std::endl;
		BM::commit("test.txt", i);
		std::cout << p[i]->get_lock() << std::endl;
	}

	for (int i=10; i<20; ++i) {
		p[i] = BM::pull("test.txt", i);
		std::cout << "pid ==> " << p[i]->get_pid() << std::endl;
		//BM::commit(p[i]->get_file_name(), p[i]->get_pid());
	}
}
