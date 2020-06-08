output: os_info.o broken.o
        gcc os_info.o -o os_info
        gcc broken.o -o broken

os_info.o: os_info.c
        gcc -c os_info.c

broken.o: broken.c
        gcc -c broken.c

clean:
        rm *.o os_info broken

