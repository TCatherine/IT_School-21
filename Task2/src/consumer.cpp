#include "consumer.h"

Consumer::Consumer() {
	get_shared_memory();
	init_semaphore(m_full_semaphore, m_sem_full_fn, 0);
	init_semaphore(m_empty_semaphore, m_sem_empty_fn, 1);
}

Consumer::~Consumer() {
	deinit_semaphore(m_full_semaphore, m_sem_full_fn);
	deinit_semaphore(m_empty_semaphore, m_sem_empty_fn);
	close(m_file_description);
}

void Consumer::get_shared_memory(){
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

void Consumer::init_semaphore(sem_t*& sem, std::string& sem_name, int value) {
	sem = sem_open(sem_name.c_str(), O_CREAT, S_MODE, value);
	if (sem == SEM_FAILED)
		throw "sem open failed!";
}

void Consumer::process(){
	if(!sem_wait(m_full_semaphore)){
		mlock(m_address, MMAP_DATA_SIZE);

		std::cout << m_address << std::endl;
		memset(m_address, 0, MMAP_DATA_SIZE);

		munlock(m_address, MMAP_DATA_SIZE);

		sem_post(m_empty_semaphore);
	}
}

void Consumer::deinit_semaphore(sem_t*& sem, std::string& name){
	if (sem!= nullptr){
		sem_close(sem);
		sem_unlink(name.c_str());
  }
}

void Consumer::close_shared_memory(){
	munmap(m_address, MMAP_DATA_SIZE);
	close(m_file_description);
	unlink(m_shm_name.c_str());
}

void Consumer::run(){
	while (true)
		process();
}

int main(int argc,char * argv[])
{
	try {
	Consumer my_consumer;
	my_consumer.run();
	}
	catch(const std::string& ex){
		std::cout << ex<< std::endl;
	}
	catch(...){

	}

	return 0;
}
