#### How to use my Makefile:
1. type *make*  it will compile two c program. One is checkFile.c, other one is mklarge.c
2. type *./checkFile \<filename\>* will run  checkFile.c program.
3. type "./mklarge" will run mklarge.c program.

## Investigating the file system

#### stat (the program)
1. What are the two different types of file system?
```
goose.cs.umanitoba.ca 114% stat -f /
  File: "/"
    ID: a7ea40d1fea1ae4 Namelen: 255     Type: ext2/ext3
Block size: 4096       Fundamental block size: 4096
Blocks: Total: 10288440   Free: 9151737    Available: 8623353
Inodes: Total: 2621440    Free: 2541292
goose.cs.umanitoba.ca 115% stat -f ~
  File: "/home/student/hej3456"
    ID: 0        Namelen: 255     Type: nfs
Block size: 1048576    Fundamental block size: 1048576
Blocks: Total: 3096240    Free: 2813383    Available: 2656097
Inodes: Total: 201326592  Free: 194147321
```
        The root directory of the system's type is ext2/ext3
        The home folder's type is nfs

2. Briefly describe each file system type by referring to the manual page for the file system (e.g., man \<fstype\>).

       ext2/ext3 are Linux file systems that have historically been the default file system for many Linux distributions. They are general purpose file  systems that have been designed for extensibility  and backwards compatibility.
       NFS is an Internet Standard protocol, was developed to allow file sharing between systems residing on a local area network.

3. What are the block sizes of these devices?
        
        The block size of root directory is: 4096
        The block size of home folder is: 1048576

        The root directory is on my local machine, therefore we can keep the block size smaller, since we can directly access with no transfer between internet. So we can allocate it more right away when we need it.
        But for the home foler, we are access it remotely, therefore if we make it too small, we need frequenly read and write to the block.
        therefore for an optimal choice of block size for NFS file system, is larger then ext2/ext3 file system's block size. So that when we request a file, it can give us more space, so that we wont frequently write and read.

#### stat(the system call)

1. What did you notice? what soes this tell you about hard and symbolic links?

    - file tmp1 has inode #117703956
    - file tmp1-hardlink has inode #117703956
    - file tmp1-softlink has inode #117704076

          Hard links are esssentially different names to the same file. Hard links share the same inode number with their original file. They point to the same inode of a hard drive. Hard links are great for backup purposes, but they lack the ability to link directories or across file systems.
          Soft links(or symbolic links), points to the name of another file and does not contain any data of their own.

2. Whar did you find? Explain this result.
    
    - file tmp1 has inode #117703956
      
      file tmp1 has the number of hard links to the inode #2
    - file tmp1-hardlink has inode #117703956

      file tmp1-hardlink has the number of hard links to the inode #2
    - file tmp1-softlink has inode #117704076

      file tmp1-softlink has the number of hard links to the inode #1
    
          Since tmp1 had a inode that point to the its data, and we create a hardlink that also point to the inode of tmp1, therefore for both tmp1 and tmp1-hardlink, the both point to the same inode, that why it shows number of hard links to the inode is 2
          But for the tmp1-softlink it has a inode that point to tmp1, not tmp1 inode. therefore for tmp1-softlink, it only has one hardlink to point to itself.


#### That file takes how many blocks???

1. What did you find?

  - file 512b has inode #393226

    file 512b has the number of hard links to the inode #1
    
    file 512b has the number of blocks that is using on disk #8
  - file 2k has inode #393227
    
    file 2k has the number of hard links to the inode #1
    
    file 2k has the number of blocks that is using on disk #8
  - file 4k has inode #393228
    
    file 4k has the number of hard links to the inode #1
    
    file 4k has the number of blocks that is using on disk #8
  - file 4k1 has inode #393229
    
    file 4k1 has the number of hard links to the inode #1
    
    file 4k1 has the number of blocks that is using on disk #16
  - file tmp1 has inode #393221
    
    file tmp1 has the number of hard links to the inode #1
    
    file tmp1 has the number of blocks that is using on disk #0

        for file 512b, 2k, 4k, they have the same number of blocks 8
        But file 4k1 use 16 blocks on disk.
        And tmp1 use 0 block on disk.

2. Can you explain those findings?

    - Since all of those files are the ext2/ext3 type. Therefore they all have the same block size 4096 bytes. As long as we create a file with less 4096 bytes, the file system always make entire 8 blocks for us, but if we exceed the block size by only 1 bit, the file system will make another 8 blocks for that file we created.
    Therefore, we can see that for file 512b, 2k, 4k, they all have 8 blocks that is using on disk, while 4k1 has 16 blocks that is using on disk
    - For tmp1, since we create it as a empty file, there is no block size needed for it.

3. How can this data explain the block size of the file system?
    
    - for the ext2/ext3 file system, it always allocate 8 block size at a time, if the file is too big for that, file system will create another 8 block size.

4. How large should a file be before it requires 24-512B blocks?

        when the file is just have 23 blocks of 512B, which is 11776B. because if the file try to write 1 more bit, the file system will give another 512B for it.

    - on this system how large will a file actually be before it requires 24 blocks? Why is this?

          when we using OS for allocate blocks, it will give us 8 blocks a time. therefore, if we write all 8-512B blocks(4096B) and wants to write more, the OS will give me another 8, therefore, I will have 16-512B(8192B) blocks now.
          If I finish those 16-512B blocks, and want more, OS will give another 8, therefore we will have 24-412blocks.
          So we will have write all of the 16-512B blocks, before OS give 24 blocks.

              -rw-------.  1 hej3456 csunderg  8192 Jun 27 16:00 text
              -rw-------.  1 hej3456 csunderg  8193 Jun 27 16:02 text1  

              file text has inode #393224
              file text has the number of hard links to the inode #1
              file text has the number of blocks: #16

              file text1 has inode #393227
              file text1 has the number of hard links to the inode #1
              file text1 has the number of blocks: #24




5. It is waste on the HD, since the file itself is not crrently using it, and other one can not use it. Then the worst case for each file will waste is (4K -1) bits.

6. What did you find?
    
    - file large.dat has inode #34

      file large.dat has the number of hard links to the inode #1
      
      file large.dat has the number of blocks: #16

          the number of blocks allocate for large.dat is only 16. Because the code only write a byte at the start of the file, and it just jump another 100mb, but not write anything to it, therefore the file system still think the memory space we jump is free to others. So even we jump alot of space, but we are not actually take those memory. Then when we write another byte to the memory, the file system will allocate another 8 blocks for us, just because we skip the entire first 8 blocks the file system give us initially.
          So it end up with 16 blocks total.