#ifndef PRODUCER_H
#define PRODUCER_H

#include <iostream>
#include <functional>
#include <memory>

#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>


#define MMAP_DATA_SIZE 1024
#define S_MODE S_IRWXU | S_IRWXG |S_IRWXO

class Producer {
    public:

    Producer();
		~Producer();
    void get_data();

    private:
		char * address = nullptr; 
    std::string data;
    int file_description ;

    std::string shm_name = "default_memory";
    std::string sem_empty_fn = "semaphore_empty";
    std::string sem_full_fn = "semaphore_full";

		sem_t* full_semaphore = nullptr;
		sem_t* empty_semaphore = nullptr;

		void get_shared_memory();
		void processing();
		void init_semaphore(sem_t* &sem, std::string& sem_name, int value);
		void deinit_semaphore(sem_t*& sem, std::string& name);
		void close_shared_memory();
};

#endif //PRODUCER_H