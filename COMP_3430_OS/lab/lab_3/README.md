#### How to use my Makefile:
1. type *make*  it will compile two c program. One is sharedMem.c (the original code copyed from webpage), other one is sharedMenSig.c (modified to using signal as IPC).
2. type *./sharedMem* will run  sharedMem.c program.
3. type "./sharedMenSig" will run sharedMenSig.c program.

#### Documentation

1. man page describe mmap:

        mmap()  creates a new mapping in the virtual address space of the calling process. Memory mapped by mmap() is preserved across fork(2), with the same attributes.

2. Whar each argument must be if you intend to use mmap for IPC:

        1. void *addr: Dont necessary need to spicify. Because if addr is NULL, then the kernel chooses the (page-aligned) address at which to create the mapping;

        2. size_t len: some constant integer to use to specifies the length of the mapping when write and read information from shared memory.

        3. int prot: PROT_READ | PROT_WRITE;

        4. int flags: MAP_ANONYMOUS | MAP_SHARED

        5. int fd: in this case, we use MAP_ANONYMOUS, we need to set to -1. Or it is ignore this value

        6. off_t offset: start point
    
#### Using mmap

1. No other file created, just print out some address and number 30.

2. Answers for quesrtion:
    1. What happens when you send a signal to a process that’s already exited?
        
            The program will react the same as there is no one sending a signal to the process.
    2. What happens to a signal when you send it to a process that hasn’t yet registered a signal handler for that signal?

            The kernel wont be able to interrupt the process becuase that process havent get any chance to tell kernel to call some function for him when a signal send.

    3. What should the child do until the parent signals that the memory is ready to read?

            The child process should wait(while loop), or doing some other work, as long as the child did not exit before the signal happens, it should be ok.
    
#### Going beyond mmap
Question: Explain why we might want to use processes with shared memory instead of threads/pthreads.

    Threads under the same process are already shared the same memory, but in order to use the shared memory to inter-process communication, we need to use lock and unlock, to ensure that the only one thread is using the critical section and other threads need to either wait, or doing something else.

    On the other hand, when processes using shared memory, they can still concurrently working.