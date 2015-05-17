/******************************************
Filename : irobot.c
Function : control irobot by keyboard
Author   : Will Xiao
Date     : 2014-12-7
******************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);
int open_port(int fd, int comport);
int kbhit(void);
#define cmd_T 500*1000

int main(void)
{
    int fd;
    int nwrite;
    int i,stop=0;
    char buff[]="Hello";
    char key ;
    char sendBuf[7];
    sendBuf[0]=0x80;
    sendBuf[1]=0x83;
    sendBuf[2]=0x89;
    sendBuf[3]=0x00;
    sendBuf[4]=0x32; //50mm/s  ,40mm/s=0x1e
    sendBuf[5]=0x80; //Ç°½ø
    sendBuf[6]=0x00;
    
    
    if((fd = open_port(fd,1)) < 0)
    {
        perror("open_port error");
        return 0;
    }

    if((i = set_opt(fd,57600,8,'N',1)) < 0)
    {
        perror("set_opt error");
        return 0;
    }
    
    printf("fd=%d\n",fd);
    while(!stop)
    {
		while(!kbhit()) ;
  
		key=getchar() ;
		
		switch(key)
		{
		   case 'w' : //前进
		            {
						 sendBuf[3]=0x00;
						 sendBuf[4]=0x80;
						 sendBuf[5]=0x80;  //80
						 sendBuf[6]=0x00;
				
					     nwrite = write(fd,sendBuf,7);
					 /*    usleep(cmd_T);
					     
					     sendBuf[3]=0x00;
						 sendBuf[4]=0x00;
						 sendBuf[5]=0x00;
						 sendBuf[6]=0x00;
						
						 nwrite = write(fd,sendBuf,7); */
					} break ;	
		   case 's' : //停止
		            {
						 sendBuf[3]=0x00;
						 sendBuf[4]=0x00;
						 sendBuf[5]=0x00;
						 sendBuf[6]=0x00;
						
						 nwrite = write(fd,sendBuf,7);
					} break ;
		   case 'a' : //左转
		            {
						 sendBuf[3]=0x00;
						 sendBuf[4]=0x32;
						 sendBuf[5]=0x01;
						 sendBuf[6]=0x2c;
						
						 nwrite = write(fd,sendBuf,7);
						/* usleep(cmd_T);
					     
					     sendBuf[3]=0x00;
						 sendBuf[4]=0x00;
						 sendBuf[5]=0x00;
						 sendBuf[6]=0x00;
						
						 nwrite = write(fd,sendBuf,7);   */
					} break ;
		   case 'd' : //右转
		            {
						 sendBuf[3]=0x00; 
						 sendBuf[4]=0x32;
						 sendBuf[5]=0xfe; 
						 sendBuf[6]=0xd3;
						
						 nwrite = write(fd,sendBuf,7);
					 /*	 usleep(cmd_T);
					     
					     sendBuf[3]=0x00;
						 sendBuf[4]=0x00;
						 sendBuf[5]=0x00;
						 sendBuf[6]=0x00;
						
						 nwrite = write(fd,sendBuf,7);  */
					} break ;
		   
		   case 'q' : //停止
		            {
						 sendBuf[3]=0x00;
						 sendBuf[4]=0x00;
						 sendBuf[5]=0x00;
						 sendBuf[6]=0x00;
						
						 nwrite = write(fd,sendBuf,7);
						 stop=1 ;
					} break ;		
					
		   default : {printf("you pressed a wrong key....\n");}
	    }
	    
		printf("nwrite=%d\n",nwrite);
	}
    
    	
    close(fd);
    return 0;
}


int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio;
    struct termios oldtio;
    
    if(tcgetattr(fd,&oldtio) != 0)
    {
        perror("SetupSerial 1");
        return -1;
    }
    
    bzero(&newtio,sizeof(newtio));
    newtio.c_cflag |= CLOCAL |CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch(nBits)
    {
        case 7:
            newtio.c_cflag |= CS7;
 	    break;
        case 8:
	    newtio.c_cflag |= CS8;
   	    break;    
    }

    switch(nEvent)
    {
        case 'O':
            newtio.c_cflag |= PARENB;
   	    newtio.c_cflag |= PARODD;
   	    newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
 	    newtio.c_iflag |= (INPCK |ISTRIP);
   	    newtio.c_cflag |= PARENB;
   	    newtio.c_cflag &= ~PARODD;
	    break;
 	case 'N':
 	    newtio.c_cflag &= ~PARENB;
	    break;
    }

    switch(nSpeed)
    {
        case 57600:
	    cfsetispeed(&newtio,B57600);
	    cfsetospeed(&newtio,B57600);
            break;
	case 4800:
   	    cfsetispeed(&newtio,B4800);
	    cfsetospeed(&newtio,B4800);
	    break;
	case 9600:
   	    cfsetispeed(&newtio,B9600);
	    cfsetospeed(&newtio,B9600);
	    break;
	case 115200:
   	    cfsetispeed(&newtio,B115200);
	    cfsetospeed(&newtio,B115200);
	    break;
	case 460800:
   	    cfsetispeed(&newtio,B460800);
	    cfsetospeed(&newtio,B460800);
   	    break;		    
	default:
            cfsetispeed(&newtio,B9600);
            cfsetospeed(&newtio,B9600);
            break;
    }

    if(nStop == 1){
    	newtio.c_cflag &= ~CSTOPB;
    }
    else if(nStop ==2){
        newtio.c_cflag |= CSTOPB;
    }
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;

    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio)) != 0)
    {
        perror("com set error");
        return -1;
    }
    printf("set done!\n");
    return 0;
}

int open_port(int fd,int comport)
{
    char *dev[]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2"};
    long vdisable;
    if(comport == 1)
    {
        fd = open("/dev/ttyUSB0",O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd == -1){
	    perror("Can't Open Serial Port");
	    return -1;
        }
    }

    else if(comport == 2)
    {
        fd = open("/dev/ttyS1",O_RDWR|O_NOCTTY|O_NDELAY);
        if(fd == -1){
            perror("Can't Open Serial Port");
            return -1;
        }
    }

    else if(comport == 3)
    {
        fd = open("/dev/ttyS2",O_RDWR|O_NOCTTY|O_NDELAY);
        if(fd == -1){
            perror("Can't Open Serial Port");
            return -1;
        }
    }

    if(fcntl(fd,F_SETFL,0) < 0){
  	printf("fcntl failed\n");
    }
    else{
	printf("fcntl=%d\n",fcntl(fd,F_SETFL,0));
    }

    if(isatty(STDIN_FILENO) == 0){
	printf("standard input is not a terminal device\n");
    }
    else{
     	printf("isatty sucess!\n");
    }

    printf("fd-open=%d\n",fd);
    return fd;
}

int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF)
	{
	ungetc(ch, stdin);
	return 1;
	}
	
  return 0;
}


