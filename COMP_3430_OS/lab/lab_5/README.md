#### How to use my Makefile:
1. type *make*  it will compile the c program. 
2. type *./mmap* will run  mmap.c program.

## Reading the docs
1. Why do you suppose this is generally not used?

        When we choose a address, we need to check that it is a free space that we can use, if it is not a free space, then we still need to ask OS to give us some address in the memory that is free to use. So in generally we let OS to decide the address to use.

2. How do we ask the OS to pick the address?
          
        We just leave the first parameter Null, then OS will automatically pick the address for us.

## Running some code
3. Which commands is being executed in mmap.c?

        pmap -x <process id>

4. Take a look at the manual page for this command and briefly describe what is does.

        The pmap command reports the memory map of a process or processes. Flag '-x' show the extened format. 

5. What is the size of the mapping?

        4 kbytes

6. What is different about these listings?

        The mode for the thrree mapping are different.

7. Can you explain this difference?

        The three different mode represent 3 different elements in process image.
        r-x--: read and executable represent Program code
        r----: read only represent the constant program data
        rw---: read and write represent global and static variable in program data

8. Does the return value of mmap and pmap match?

        The return address is  0x7f2661e3e000, it match to the address of mmap.txt