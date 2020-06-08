#### Documentation

1. The first way is to use pthread_mutex_init to initialize a new mutex by passing the attribute, and the second way is to reinitialize the destroyed mutex object after pthread_mutex_destroy.
2. The variable destroyed by pthread_mutex_destroy is undefine, but you can reinitialize it to use it.
3. pthread_mutex_trylock intends to lock a mutex, but will return immediately if the mutex object referenced by mutex is currently locked.
4. It is useful when the mutex type is PTHREAD_MUTEX_RECURSIVE and the mutex is currently owned by the calling thread, the mutex lock count shall be incremented by one and the pthread_mutex_trylock shall immediately return success.

#### Experimentation

1. No change
* locking, locking and stuck. 
* Because the type of the mutex is NORMAL, and by locking it and relocking it will cause deadlock.
2. Change type
* We should use type PTHREAD_MUTEX_RECURSIVE to be able to use the mutex already being used by the thread.  
* We see the program successfully locking and unlocking 10 times as it should do.
* Application with loops to change a global variable independently. Application actively interacting with users might need this mutex type because they can have cetain mutex actively checking and receiving users' input.

#### Writing code

1. The output should be 1000.
2. After running it for 10 times, the result for each time is different. Outputs are 146, 148, 121, 153, 155, 129, 142, 113, 161 and 159.
3. After running it for 10 times, the result for each time is the same and agrees with the answer from question 1, 1000.
