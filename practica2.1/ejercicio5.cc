//compilar usando g++

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
 
#include <time.h>
#include <string.h>
#include <iostream>
 
 
int main(int argc, char **argv)
{
   if (argc < 3){
       if (argc == 1)   
           printf("Address not specified \n");          
       if (argc == 2)   
           printf("Port not specified \n");
  
   printf("try with : ./program address serv\n");
   return 1;
   }
 
   struct addrinfo hints;
   struct addrinfo *info;
 
   memset(&hints, 0, sizeof(struct addrinfo));
 
   // hints.ai_flags    = AI_PASSIVE; //Devolver 0.0.0.0
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
 
   int rc = getaddrinfo(argv[1], argv[2], &hints, &info);
 
  if (rc != 0)
   {
       fprintf(stderr, "Error getaddrinfo: %s\n", gai_strerror(rc));
       return -1;
   }
 
  
  
   int sd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
 
   if (sd == -1)
   {
       fprintf(stderr, "Error opening the socket %s\n", gai_strerror(sd));
       return -1;
   }
  
   int c = connect(sd, info->ai_addr, info->ai_addrlen);
  
   if(c ==-1){
       fprintf(stderr, "Couldn't establish connection %s\n",gai_strerror(c) );
       return -1;
   }
   freeaddrinfo(info);
 
 
   int bufferLen = 120;
   char buffer[bufferLen];
   memset(&buffer, '\0', sizeof(char) * bufferLen);
 
   char recvBuffer[bufferLen];
   memset(&recvBuffer, '\0', sizeof(char) *bufferLen);
 
  
   ssize_t bytes = 0;
   while (1)
   {
       std::cin >> buffer;
 
       if ((buffer[0] == 'Q' || buffer[0] == 'q') && buffer[1] == '\0')
       {
           break;
       }
 
       send(sd, buffer, strlen(buffer) + 1, 0);
     
       bytes = recv(sd, recvBuffer, (bufferLen - 1) * sizeof(char), 0);
     
       recvBuffer[bytes] = '\0';
       printf("%s\n", recvBuffer);
   }
  
   close(sd);
   return 0;
}
 
 

