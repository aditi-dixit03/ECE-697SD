#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void main (void)
{
int file;
char tx_buf[10];
char rx_buf[10]={0};
size_t nbytes;
ssize_t bytes_written;
ssize_t bytes_read;
struct termios oldtio, newtio;

if ((file = open("/dev/ttyS2", O_RDWR | O_NOCTTY ))<0)
{
		printf("UART: Failed to open the file.\n");
		return;
}

//save the current serial settings
tcgetattr(file, &oldtio);
bzero(&newtio, sizeof(newtio));

newtio.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
newtio.c_iflag = IGNPAR | ICRNL;

tcflush(file, TCIFLUSH);
tcsetattr(file, TCSANOW, &newtio);
strcpy(tx_buf, "Hello \n");
nbytes = strlen(tx_buf);

while (1)
{
	bytes_written = write(file, tx_buf, nbytes);
	sleep(1);
	bytes_read = read(file, rx_buf, nbytes);
	printf("%s" , rx_buf);
	sleep(1);
}
	close(file);

}
