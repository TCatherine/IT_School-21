#include "producer.h"

bool stop_flag = false;

void handler( int ){ stop_flag = true;};

Producer::Producer() {
	signal(SIGINT, handler);

	get_shared_memory();
	init_semaphore(full_semaphore, m_sem_full_fn, 0);
	init_semaphore(empty_semaphore, m_sem_empty_fn, 1);
}

void Producer::run(){
	while (stop_flag != true) {
		process();
  }
}

Producer::~Producer(){
	deinit_semaphore(full_semaphore, m_sem_full_fn);
	deinit_semaphore(empty_semaphore, m_sem_empty_fn);
  	close_shared_memory();
}

void Producer::process() {
	if (!sem_wait(empty_semaphore)) {
		get_data();
		mlock(m_address, MMAP_DATA_SIZE);
		memcpy(m_address, m_data.c_str(), m_data.length()+1);
		munlock(m_address, MMAP_DATA_SIZE);
		
		sem_post(full_semaphore);
	}
}

void Producer::get_shared_memory(){
	m_file_description = shm_open(m_shm_name.c_str(), O_CREAT | O_RDWR, S_MODE);

	if (m_file_description < 0)
		throw "shm_open failed!";
				
	ftruncate(m_file_description, MMAP_DATA_SIZE);
	m_address = (char*)mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, m_file_description, 0);
	if (!m_address) {
		close(m_file_description);
		throw "mmap failed!";    
	}
}

void Producer::init_semaphore(sem_t*& sem, std::string& sem_name, int value) {
	sem = sem_open(sem_name.c_str(), O_CREAT, S_MODE, value);
	if (sem == SEM_FAILED)
		throw "sem open failed!";
}

void Producer::deinit_semaphore(sem_t*& sem, std::string& name){
	if (sem!= nullptr){
		sem_close(sem);
		sem_unlink(name.c_str());
	}
}

void Producer::close_shared_memory(){
	munmap(m_address,MMAP_DATA_SIZE);
	close(m_file_description);
	unlink(m_shm_name.c_str());
}

void Producer::get_data() {
	do {
		std::cout << "> ";
		std::getline(std::cin, m_data);
		if (m_data.length() >= MMAP_DATA_SIZE)
			std::cout << "Input data size is bigger than shared memory. Try again!" << std::endl;
	} while (m_data.length() >= MMAP_DATA_SIZE);
}

int main(int argc,char * argv[])
{
	try {
		Producer current_producer;
		current_producer.run();
	}
	catch(const std::string& ex){
		std::cout << ex<< std::endl;
	}
	catch(...){

	}

	return 0;
}
