#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <ctime>

namespace BufferManager {
	/*
	   Some constants
	 */ 
	const PAGE_SIZE = 4096 * 1024;

	/*
	   Class Page
	*/

	class Page {
	private:
		char *p;
		int lock, dirty;
		time_t lastUseTime;
	public:
		static int capacity;
		Page();
	};

	/*
	   Interfaces for Users
	*/

	//for table users
	Page& pull(Table& tb, int pid, int* errCode); 
	Page& create(Table& tb, int user, int* errCode); 
	int commit(Table& tb, int user, int* errcode);

	//for index users
	Page& pull(Index& idx, int pid, int* errCode); 
	Page& create(Index& idx, int user, int* errCode); 
	int commit(Index& idx, int user, int* errcode);

	//for catalog users
	Page& pull(int pid, int* errCode); 
	Page& create(int user, int* errCode); 
	int commit(int user, int* errcode);

	/*
		Internal ultilities
	 */
	void initialize();
}



namespace BufferManager {


	//Definition of static members
	int Page::capacity = PAGE_SIZE; //4KB

	//Definition of class Page
	Page::Page(): p(new char[PAGE_SIZE]) {
		
	}

	//Definition of interfaces

	//Definition of internal ultilities
	void initialize() {

	}
}

int main() {
	std::cout << BufferManager::Page::capacity << std::endl;
}
