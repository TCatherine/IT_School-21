#include "producer.h"

bool stop_flag = false;

void handler( int ){ stop_flag = true;};

Producer::Producer() {
	signal(SIGINT, handler);

	get_shared_memory();
	init_semaphore(full_semaphore, sem_full_fn, 0);
	init_semaphore(empty_semaphore, sem_empty_fn, 1);

 while (stop_flag != true) {
		processing();
  }
}

Producer::~Producer(){
	deinit_semaphore(full_semaphore, sem_full_fn);
	deinit_semaphore(empty_semaphore, sem_empty_fn);
  close_shared_memory();
}

void Producer::processing() {
	try {
		if (!sem_wait(empty_semaphore)) {
			get_data();
			mlock(address, MMAP_DATA_SIZE);
			memcpy(address, data.c_str(), data.length()+1);
			munlock(address, MMAP_DATA_SIZE);
			sem_post(full_semaphore);
		}
	}

	catch(const char* error_msg){
		deinit_semaphore(full_semaphore, sem_full_fn);
		deinit_semaphore(empty_semaphore, sem_empty_fn);
  	close_shared_memory();
		exit(0);
	}
}

void Producer::get_shared_memory(){
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

void Producer::init_semaphore(sem_t*& sem, std::string& sem_name, int value) {
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

void Producer::deinit_semaphore(sem_t*& sem, std::string& name){
	if (sem!= nullptr){
		sem_close(sem);
		sem_unlink(name.c_str());
	}
}

void Producer::close_shared_memory(){
	munmap(address,MMAP_DATA_SIZE);
	close(file_description);
	unlink(shm_name.c_str());
}

void Producer::get_data() {
	do {
		std::cout << "> ";
		std::getline(std::cin, data);
		if (data.length() >= MMAP_DATA_SIZE)
			std::cout << "Input data size is bigger than shared memory. Try again!" << std::endl;
	} while (data.length() >= MMAP_DATA_SIZE);
}

int main(int argc,char * argv[])
{
	Producer current_producer;
}
