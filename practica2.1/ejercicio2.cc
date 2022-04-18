#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
 
#include <time.h>
#include <string.h>
#include <unistd.h>
 
 
 
int main(int argc, char *argv[])
{
 
   if (argc < 3){
       if (argc == 1)   
           printf("Address not specified \n");          
       if (argc == 2)   
           printf("Port not specified \n");
  
   printf("try with : ./program address serv\n");
   return 1;
   }
 
   struct addrinfo *info;
   struct addrinfo hints;
   memset(&hints, 0, sizeof(struct addrinfo));
 
   // hints.ai_flags = AI_PASSIVE;
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
 
   //asigna direccion al socket, y ya puede recibir mensajes
   bind(sd, info->ai_addr, info->ai_addrlen);
 
   freeaddrinfo(info);
 
   const int bufferLen = 80;
 
   while (true)
   {
       char buffer[bufferLen] = {};
 
       struct sockaddr client;
       socklen_t client_len = sizeof(sockaddr);
 
       //comunicación con el cliente
       int bytes = recvfrom(sd, buffer, bufferLen, 0, &client, &client_len);
       buffer[bytes] = '\0';
 
       char serv[NI_MAXSERV];
       char host[NI_MAXHOST];
      
       int check = getnameinfo(&client, client_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
 
       if (check != 0)
       {
           fprintf(stderr, "Error getnameinfo: %s\n", gai_strerror(check));
           continue;
       }
 
       printf("%d bytes de %s:%s\n", bytes, host, serv);
       time_t rawTime;
       struct tm *timeInfo;
 
       //Para obtener tiempo y fecha
       time(&rawTime);
 
       timeInfo = localtime(&rawTime);
 
       //variable que permite a strftime identificar que se le pide
       char *format;
      
       char send[80];
       int timeBytes;
       
       switch (buffer[0])
       {
       case 't':
         
           timeBytes = strftime(send, bufferLen, "%r", timeInfo);
            send[timeBytes] = '\n';
            sendto(sd, send, timeBytes + 1, 0, &client, client_len);
           break;
       case 'd':
           
            timeBytes = strftime(send, bufferLen, "%F", timeInfo);
            send[timeBytes] = '\n';
            sendto(sd, send, timeBytes + 1, 0, &client, client_len);
           break;
       case 'q':
           printf("Saliendo...\n");           
           return 0;
       default:
           printf("Comando no soportado %s", buffer);
          
           break;
       }
 
      
   }
 
   close(sd);
 
   return 0;
}

