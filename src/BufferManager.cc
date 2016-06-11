#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <ctime>

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
		Some meta-informations 
	 */
	int currentPageNum = 0;


	/*
	   Class Page
	*/

	class Page {
	private:
		char * const p;
		int lock, dirty, type;
		time_t lastUseTime;
	public:
		static int capacity;
		Page(const std::string& fileName, int pid_);
		char getc(int offset) const {
			return *(p+offset);
		}
	};

	/*
	   Interfaces for Users
	*/

	//for table users
	//Page& pull(Table& tb, int pid, int* errCode); 
	//Page& create(Table& tb, int user, int* errCode); 
	//int commit(Table& tb, int user, int* errcode);

	//for index users
	//Page& pull(Index& idx, int pid, int* errCode); 
	//Page& create(Index& idx, int user, int* errCode); 
	//int commit(Index& idx, int user, int* errcode);

	//for catalog users
	//Page& pull(int pid, int* errCode); 
	//Page& create(int user, int* errCode); 
	//int commit(int user, int* errcode);

	/*
		Internal ultilities
	 */
}



namespace BufferManager {


	//Definition of static members
	int Page::capacity = PAGE_SIZE; //4KB

	//Definition of class Page
	Page::Page(const std::string& fileName, int pid_):
		p(new char[PAGE_SIZE]())
	{
		std::ifstream ifs(fileName.c_str(), std::ios::in | std::ios::binary);
		ifs.seekg(pid_ * PAGE_SIZE);
		char *pw = p;
		for (int i=0; i<PAGE_SIZE; ++i)	*pw++ = ifs.get();
		ifs.close();
	}

	//Definition of interfaces
	//Page::Page& pull(Table& tb, int pid, int*errCode) {

	//}

	Page::Page& create(Table& tb, int pid, int*errCode) {
		if (currentPageNum < BUFFER_PAGE_LIMIT) {
			Page newPage(tb.getname(), pid+1); //fist page use for bookkeeping
		}
		else {
			//do some swap
		}
	}

	//Definition of internal ultilities
}

int main() {
		
	namespace BM = BufferManager;
	BM::Page p1(std::string("a.txt"), 0);
	for (int i=0; i<BM::PAGE_SIZE; ++i) std::cout << p1.getc(i);
	std::cout << std::endl;
}
