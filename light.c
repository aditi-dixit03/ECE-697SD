#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	FILE *io, *iodir, *ioval;	
	//io = fopen("/sys/class/gpio/export", "w");
   	//fseek(io,0,SEEK_SET);
   	//fprintf(io,"%d",60);
   	//fflush(io);
	iodir = fopen("/sys/class/gpio/gpio60/direction", "w");
	fseek(iodir,0,SEEK_SET);
   	fprintf(iodir,"in");
   	fflush(iodir);

  	int num=0;
    	for(int i=0; i<10; i++){
		ioval = fopen("/sys/class/gpio/gpio60/value", "r");
		fseek(ioval, 0, SEEK_SET);
        	fscanf(ioval, "%d", &num);
        	if(num == 1){
            		printf("HIGH\n");
        	}
        	else{
            		printf("LOW\n");
        	}
		//fseek(ioval, 0, SEEK_SET);
        	sleep(1);
    	}
	
	FILE* ioclose;
	//ioclose = fopen("/sys/class/gpio/unexport/", "w");
	//fseek(ioclose, 0, SEEK_SET);
	//fprintf(ioclose, "%d", 60);
	return 0;

}
