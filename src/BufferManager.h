#include <string>
#include <iostream>
#include <ctime>

//declarations
namespace BufferManager {
	/*
	   Some magic numbers
	 */

	const int PAGE_SIZE = 4096 * 1024;
	const int BUFFER_PAGE_LIMIT = 10;

	//error code
	const int FETCH_SUCCESS = 4;
	const int UNABLE_TO_SWAP_OUT_PAGE = 6;
	const int COMMIT_FAIL = 13;

	//lock
	const int UNLOCKED = 9;
	const int LOCKED = 10;

	/*
	   Class Page
	 */

	class Page {
		private:

			/*
			 *private member objects
			 * */
			std::string file_name;
			char * const p;
			int pid, lock, last_use_time;

			/*
			 *private member functions
			 * */

		public:
			//ctors
			Page(const char* _file_name, int _pid): 
				p(new char[PAGE_SIZE]()), file_name(_file_name), lock(LOCKED), pid(_pid), last_use_time(-1) 
			{
				std::cout << "New born one!" << _file_name << "   " << _pid << std::endl;
			};

			//interfaces
			void set_lock(int mode) {
				this->lock = mode;
			}

			int get_lock() const {
				return this->lock;
			}

			void set_pid(int _pid) {
				this->pid = _pid;
			}

			int get_pid() const{
				return this->pid;
			}

			void set_file_name(const char* _file_name) {
				this->file_name = std::string(_file_name);
			}

			const char* get_file_name() const{
				return this->file_name.c_str();
			}

			void set_last_use_time() {
				last_use_time = time(NULL);
			}

			int get_last_use_time() const{
				return last_use_time;
			}

			int fetch_from_disk(const char* _file_name, int _pid) const;
			void write_back() const;
			void paint_white() const {
				memset(this->p, 0, PAGE_SIZE);
			}
			void swap_out_with(const char* _file_name, int _pid);
	};

	/*
	   Interfaces for Users
	 */

	const Page* pull(const char* _file_name, int _pid);
	int commit(const char* _file_name, int _pid);

	/*
	   Internal utilities
	 */
	class BMExcep {
		private:
			int err_code;
		public:
			BMExcep(int ec): err_code(ec) {};
	};

	inline std::string convert_to_uuid(const char* a, int b) {
		return std::string(a).append(std::to_string(b));
	}

	inline std::pair<std::string, Page*> wrapper_to_pair(const char* _file_name, int _pid, Page* p) {
		return std::pair<std::string, Page*>(convert_to_uuid(_file_name, _pid), p);
	}

	inline void debug(const char* msg) {
		std::cout << msg << std::endl;
	}
}
