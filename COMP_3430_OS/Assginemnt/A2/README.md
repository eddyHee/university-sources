# Assignment 2
# Student Name: Jialiang He
# Student number: 7806975


## **Run program**:
    I have 2 separate c file. one named 'q1.c' and other named 'q2.c'.
    
### To compile those files, please use the ***Makefile***:
~~~~
make q1                     // this will compile q1.c
make q2                     // this will compile q2.c
./q1 vim <text file>        // this will run thread.c
./q2                        // this will run process.c
~~~~
### ***Note***: when you run q1.c, it requires command line argument. Please pass: vim ***_<xxx.txt>_***

### Question 2
### What happens if a signal handler gets invoked when it’s already handling a signal with that same handler?
    In q2.c I implemented a handle function that just print "%d oh hi mark" for 10 times with 1 second sleep between each print. 
    In the program calling fork() function call to create a child.
    In side child process, it register 2 signals that both direct to the same handle function.
    In side parent process, it send 5 signals to the child process, with 3 seconds sleep between signal. The signal it send is in this order: SIGUSR1, SIGUSR1, SIGUSR2, SIGUSR2, SIGUSR2.

    The output from this program is shows below:
```
    ./q2 
    In the child, waiting: /.
    Sending 1st signal SIGUSR1.
    0 oh hi mark
    1 oh hi mark
    2 oh hi mark
    3 oh hi mark

    Sending 2nd signal SIGUSR1.
    4 oh hi mark
    5 oh hi mark
    6 oh hi mark

    Sending 3rd signal SIGUSR2.
    0 oh hi mark
    1 oh hi mark
    2 oh hi mark
    3 oh hi mark

    Sending 4th signal SIGUSR2.
    4 oh hi mark
    5 oh hi mark
    6 oh hi mark

    Sending 5th signal SIGUSR2.
    7 oh hi mark
    8 oh hi mark
    9 oh hi mark

    ---->  FINISH SIGNAL31
    0 oh hi mark
    1 oh hi mark
    2 oh hi mark
    3 oh hi mark
    4 oh hi mark
    5 oh hi mark
    6 oh hi mark
    7 oh hi mark
    8 oh hi mark
    9 oh hi mark

    ---->  FINISH SIGNAL31
    7 oh hi mark
    8 oh hi mark
    9 oh hi mark

    ---->  FINISH SIGNAL30
    0 oh hi mark
    1 oh hi mark
    2 oh hi mark
    3 oh hi mark
    4 oh hi mark
    5 oh hi mark
    6 oh hi mark
    7 oh hi mark
    8 oh hi mark
    9 oh hi mark

    ---->  FINISH SIGNAL30
    In the child, waiting: \.
    FINISH ALL TASKS!
```
    From the man page, it saids: Multiple instances of real-time signals can be queued.  By contrast, if multiple instances of a standard signal are delivered while that signal is currently blocked, then only one instance is queued. 
    So in out program, we regiested one handler for SIGUSR1, when the signal first time send to the child process, it immediately run the function we want it to run. And when the signal SIGUSR1 send for the second time from parent process, this time the handler queue the signal up, it keep working on the first signal. So the second signal is in the queque right now. For the signal SIGUSR2, parent process is sending three SIGUSR2 one after another. but we can see from the result, it seems like the handler ignore one of the SIGUSR2, because while it is handling the first comming SIGUSR2, it execute handle function, and when the second SIGUSR2 comming, it queued up, and when third SIGUSR2 comming, the second is blocked, then as the man page said, the handler only queue one signal instance. so the last signal is ignored.
    Therefore we can see that for the signal handler, it uses a queue for the same signal, and if multiple instance of a standard signal comming, while this signal is blocked for some reason it will ignal the rest. only keep one.

    The data structure for signal handler handle different signal instances is stack.
    When the handler is doing one signal while another different signal is comming, it will stop the current signal, and executing the new comming signal. Like the output from out program, when parent process is sending a new signal (SIGUSR2), while the child process is still working on the SIGUSR1, it simply stop the child process from executing SIGUSR1, and let the child process working on SIGUSR2. Later when more SIGUSR2 occurs, signal handler queue them up. 

    Therefore, when the parent process sending the signal to child as following order: SIGUSR1, SIGUSR1, SIGUSR2, SIGUSR2, SIGUSR2. The child process is first working on SIGUSR1, and when the first SIGUSR2 is given, the handler stack the signal up, so the child process bascally stop executing SIGUSR1, and working on SIGUSR2, and while he is working on it, 2 more SIGUSR2 is sending from parents, the handler queue the second SIGUSR2 up and ignore the third comming SIGUSR2 signal, and when the child process finish executing the first SIGUSR2, signal handler moving on the queque, so the child is start executing the second SIGUSR2, when it finish executing the second SIGUSR2, there is nothing on the current queue, so signal handler pop the stack, therefore the child process keep executing first SIGUSR1 from where he left over last time. Finally when he finish executing the first SIGUSR1, the handler move to the second SIGUSR1 that currently store in the queue, And at the end of executing this signal, signal handler finish his job, since there is no more signal left over.