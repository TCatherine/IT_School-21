#ifndef CONSUMER_H
#define CONSUMER_H

#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdexcept>
#include <semaphore.h>

 
#define MMAP_DATA_SIZE 1024
#define S_MODE S_IRWXU | S_IRWXG | S_IRWXO

class Consumer {
  public:
    Consumer();
	~Consumer();
    void show_data();
	void run();

  private:
	char* m_address = nullptr; 
	std::string m_data;
	int m_file_description = -1;

	std::string m_shm_name = "default_memory";
	std::string m_sem_empty_fn = "semaphore_empty";
	std::string m_sem_full_fn = "semaphore_full";

	sem_t* m_full_semaphore = nullptr;
	sem_t* m_empty_semaphore = nullptr;

	void get_shared_memory();
	void init_semaphore(sem_t*& sem, std::string& sem_name, int value);
	void deinit_semaphore(sem_t*& sem, std::string& name);
	void close_shared_memory();
	void process();
};

#endif //CONSUMER_H