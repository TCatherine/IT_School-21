#include "consumer.h"

Consumer::Consumer() {
	get_shared_memory();
	init_semaphore(full_semaphore, sem_full_fn, 0);
	init_semaphore(empty_semaphore, sem_empty_fn, 1);

 while (true) {
		processing();
  }
}

Consumer::~Consumer() {
	deinit_semaphore(full_semaphore, sem_full_fn);
	deinit_semaphore(empty_semaphore, sem_empty_fn);
  close(file_description);
}

void Consumer::get_shared_memory(){
	try {
		file_description = shm_open(shm_name.c_str(), O_CREAT | O_RDWR, S_MODE);

		if (file_description < 0)
			throw "shm_open failed!";
					
		ftruncate(file_description, MMAP_DATA_SIZE);
		address = (char*)mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, file_description, 0);
		if (!address) {
			close(file_description);
			throw "mmap failed!";    
		}
	}

	catch(const char* error_msg){
		std::cout << error_msg << std::endl;
		exit(0);
	}
}

void Consumer::init_semaphore(sem_t*& sem, std::string& sem_name, int value) {
	try {
		sem = sem_open(sem_name.c_str(), O_CREAT, S_MODE, value);
		if (sem == SEM_FAILED)
			throw "sem open failed!";
	}

	catch(const char* error_msg) {
		close_shared_memory();
		deinit_semaphore(full_semaphore, sem_full_fn);
		deinit_semaphore(empty_semaphore, sem_empty_fn);
		std::cout << error_msg << std::endl;
		exit(0);
	}
}

void Consumer::processing(){
	try {
		if(!sem_wait(full_semaphore)){
			mlock(address, MMAP_DATA_SIZE);
			std::cout << address << std::endl;
			memset(address, 0, MMAP_DATA_SIZE);
			munlock(address, MMAP_DATA_SIZE);
			sem_post(empty_semaphore);
		}
	}
		catch(const char* error_msg) {
		close_shared_memory();
		deinit_semaphore(full_semaphore, sem_full_fn);
		deinit_semaphore(empty_semaphore, sem_empty_fn);
		exit(0);
	}
}

void Consumer::deinit_semaphore(sem_t*& sem, std::string& name){
	if (sem!= nullptr){
		sem_close(sem);
		sem_unlink(name.c_str());
	}
}

void Consumer::close_shared_memory(){
	munmap(address, MMAP_DATA_SIZE);
	close(file_description);
	unlink(shm_name.c_str());
}

int main(int argc,char * argv[])
{
  Consumer my_consumer;
}
