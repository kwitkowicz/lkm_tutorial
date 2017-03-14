#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define MAX_BUFFER_LEN 255
static char receive[MAX_BUFFER_LEN];

int main(){
	int ret, fd;
	char buffer[MAX_BUFFER_LEN];
	printf("Opening character device\n");
	fd = open("/dev/mychar", O_RDWR);
	if (fd < 0){
		perror("Failed to open the device.");
		return errno;
	}
	printf("Type text to send to kernel:\n");
	scanf("%[^\n]%*c", buffer);
	printf("Writing message to the device [%s].\n", buffer);
	ret = write(fd, buffer, strlen(buffer)); 
	if (ret < 0){
		perror("Failed to write the message to the device.");
		return errno;
	}
	sleep(5);
	printf("Wait 5 seconds for reading back from the device.\n");
	printf("Reading from the device...\n");
	ret = read(fd, receive, MAX_BUFFER_LEN);
	if (ret < 0){
		perror("Failed to read the message from the device.");
		return errno;
	}
	printf("The read message is: [%s]\n", receive);
	return 0;
}
