# Task2 - Producer–Consumer with Semaphores
## Build and run
### Build:
> `cmake . && make`
### Run:
>First terminal: `./producer`
>
>Second terminal: `./consumer`
### Interrupt:
>`ctrl + c`
## Idea
The producer-consumer pattern is implemented. Synchronization is implemented using two semaphores: `empty_semapthore` and `full_semaphore`.
### Schema:
Init: 
```
empty_semapthore = 1 full_semapthore = 0
```

Producer:
```
wait(empty_semapthore)
insert_items
signal(full_semapthore)
```

Consumer:
```
wait(full_semapthore)
remove_items
signal(empty_semapthore)
```
## Usefull command
View All Exist Shared Resources:
>`ls -li /dev/shm/`

Remove Unnecessary Resources:
>`sudo rm /dev/shm/default_memory`

Show page content:
>`man /dev/shm/default_memory`

View All Exist Semaphores:
>`ls -li /dev/shm/sem.*`