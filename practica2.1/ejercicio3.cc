#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
 
#include <time.h>
#include <string.h>
#include <unistd.h>
 
 
int main(int argc, char *argv[])
{
 
   if (argc < 4){
       if (argc == 1)   
           printf("Address not specified \n");          
       if (argc == 2)   
           printf("Port not specified \n");
       if (argc == 3)   
           printf("Enter a valid command \n");
  
   printf("try with : ./program address port command\n");
   return 1;
   }
 
   struct addrinfo hints;
   struct addrinfo *info; 
  
   memset(&hints, 0, sizeof(struct addrinfo));
 
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_DGRAM;
 
   int rc = getaddrinfo(argv[1], argv[2], &hints, &info);
 
   if (rc != 0)
   {
       fprintf(stderr, "Error getaddrinfo: %s\n", gai_strerror(rc));
       return -1;
   }
 
   int sd = socket(info->ai_family, info->ai_socktype, 0);
   if (sd == -1)
   {
       fprintf(stderr, "Error opening the socket %s\n", gai_strerror(sd));
       return -1;
   }
  
   freeaddrinfo(info);
 
   
   int bufferLen = 80;
   char buffer[bufferLen];
   memset(&buffer, '\0', bufferLen);
   char host[NI_MAXHOST];
   char service[NI_MAXSERV];
 
   socklen_t client_len = sizeof(struct sockaddr);
 
   ssize_t bytes;
 
   //Envio de comando
   sendto(sd, argv[3], strlen(argv[3]) + 1, 0, info->ai_addr, info->ai_addrlen);   
   bytes = recvfrom(sd, buffer, bufferLen * sizeof(char), 0, info->ai_addr, &info->ai_addrlen); 

   printf("%s\n", buffer);
 
  
   close(sd);
   return 0;
}

