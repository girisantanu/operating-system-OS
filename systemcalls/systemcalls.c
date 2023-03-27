#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main (void)
{
    char buffer[100];

    // open file read.txt in the readonly mode. the file descriptor is stored in fdr.
    int fdr = open("read.txt", O_RDONLY);
    // open file write.txt in readwrite mode, create if does not exist, or truncate (delete existing
    // contents) if exists, give permission to new file such that owner can read/write and others can only read
    // store the file descriptor in fdw
    int fdw = open("write.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);

    // print the file descriptors to see what integer values do that have
    printf("fdr is %d\n", fdr);
    printf("fdw is %d\n", fdw);

    // read from fdr, store data in buffer, tell read not to put more than 100 bytes in buffer as that is its size
    // there is no guarantee that the buffer will be filled up a file can have fewer data. so, store the actual amount
    // of data stored in readcount
    int readcount = read(fdr, buffer, 100);

    printf("Actually read %d bytes from the file\n", readcount);

    // write the contents of buffer to fdw. the entire buffer need not be written. that is what we tell write that
    // the buffer has only readcount amount of meaningfully content
    write(fdw, buffer, readcount);

    // write something to the screen (prints to whatever is connected to fd 1)
    printf("To screen\n");
    // same effect but with explicit mention of file descriptor 1 which is the standard output connected to the terminal
    // by default.
    dprintf(1, "To screen\n");
    // this time we are writing to the file pointed to by fdw. this shows the power of file descriptors and how standard
    // input, output, and disk files can be put on a common platform by file descriptors.
    dprintf(fdw, "To file\n");

    // close the files
    close(fdr);
    close(fdw);

    // there are, by default, three fds that any program is given by the OS
    // they are 0 - standard input (connected to your keyboard), 1 - standard output (connected to your monitor)
    // and 2 - standard error (connected to your monitor, used for printing error information in programs)
    // printf always outputs to fd 1. if fd is connected to the monitor, it will print there. We will see below
    // how we can connect fd 1 to a file and then printf will print to the file instead of the screen.
    // also scanf always reads from fd 0 which is connected to the keyboard by default. we will see below how we can
    // connect fd 1 to a file and make scanf read from that.

    close(0); // close the keyboard-based standard input file descriptor
    close(1); // close the monitor-based standard output file descriptor

    // open a file for reading. This will be given fd 0 as it is the lowest one available.
    // 0 is available because we have closed it above.
    fdr = open("read1.txt", O_RDONLY);
    // open a file for writing. This will be given fd 1 as it is the lowest one available.
    // 1 is available because we have closed it above.
    fdw = open("write1.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);

    // printing fdr and fdw for proof. using dprintf with fd 2 as fd 2 is for standard error and that is
    // still connected to the screen. 1 is not connected to the screen anymore.
    dprintf(2, "fdr is %d and fdw is %d\n", fdr, fdw);

    // printf always prints to fd 1 but fd 1 is now given to a file. so, printf will print to that file
    // instead of the screen.
    printf("printf output going to file\n");

    int fromscanf;

    // scanf always accepts from fd 0 but fd 0 is now given to a file. so, scanf will accept from that file
    // instead of the keyboard.
    scanf("%d", &fromscanf);

    // use dprintf to 2 to print the value to screen as 2 is still connected to the screen. normal printf
    // cannot be used to print to screen as fd 1 has been given to a file.
    dprintf(2, "%d\n", fromscanf);

    // close the file descriptors before exiting
    close(fdr);
    close(fdw);

    exit(0);

}
