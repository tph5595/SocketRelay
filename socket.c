#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    //printf("Please enter the message: ");
    //bzero(buffer,256);
    //fgets(buffer,255,stdin);
    printf("hello!\n");
    int i = 0;
    while(1){
	++i;
    i %= 100;
    char *buffer = malloc(sizeof(char));
    buffer[0] = (i/10)+'0';
    buffer[1] = (i%10)+'0';
    buffer[2] = '\0';
	printf("%d\n", i);
	n = write(sockfd,buffer,strlen(buffer));
    	printf("hello2\n");
	if (n < 0) 
             error("ERROR writing to socket");
        bzero(buffer,256);
    	n = read(sockfd,buffer,255);
    	if (n < 0) 
             error("ERROR reading from socket");
    	printf("%s\n",buffer);
        free(buffer);
	//wait
	unsigned int secs = 1;
	unsigned int retTime = time(0) + secs;   // Get finishing time.
        while (time(0) < retTime);               // Loop until it arrives.

    }
    return 0;
}
