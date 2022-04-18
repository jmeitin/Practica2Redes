//compilar con g++ -pthread

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
 
#include <time.h>
#include <string.h>
#include <unistd.h>
 
#include <thread>
#include <iostream>
 
 
class TCPThread
{
private:
   int sd;
 
public:
 
   void message()
   {
         const int bufferLen = 80;
       int l = 0;
        char buffer[bufferLen] = {};
       while (true)
       {
       l = recv(sd, buffer, (bufferLen - 1) * sizeof(char), 0);
      
       if(l == 0){
           printf("Conexión terminada\n");
           close(sd);
           break;
           }
       else if(l == -1){
           fprintf(stderr, "client_sd not redeable %s", gai_strerror(l));
           }
       else {      
           send(sd, buffer, bufferLen, 0);
           }
       }       
       close(sd);
   }
 
   TCPThread(int sc) {sd = sc;}
};
 
 
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
 
 
   hints.ai_flags    = AI_PASSIVE; //Devolver 0.0.0.0
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
 
   int rc = getaddrinfo(argv[1], argv[2], &hints, &info);
 
   if (rc != 0)
   {
       printf("getaddrinfo: %s\n", gai_strerror(rc));
       return -1;
   }
 
   int sd = socket(info->ai_family, info->ai_socktype, 0);
   if (sd == -1)
   {
       fprintf(stderr, "Error opening the socket %s\n", gai_strerror(sd));
       return -1;
   }
   bind(sd, info->ai_addr, info->ai_addrlen);
  
 
   freeaddrinfo(info);
 
   //numero de conexiones en espera posibles
   listen(sd, 5);  
 
   struct sockaddr cliente_addr;
   socklen_t cliente_len = sizeof(struct sockaddr);
   char host[NI_MAXHOST];
   char serv[NI_MAXSERV];
   const int bufferLen = 80;
   
    char buffer[bufferLen];
       memset(&buffer, '\0', sizeof(char) * bufferLen);
while(1){
 
   int cliente_sd = accept(sd, (struct sockaddr *) &cliente_addr, &cliente_len);  
   getnameinfo((struct sockaddr *) &cliente_addr, cliente_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);
   printf("Conexión desde Host:%s Puerto:%s\n",host, serv);
   if(cliente_sd < 0){
      fprintf(stderr, "Connection not accepted %s" , gai_strerror(cliente_sd));
       return -1;
   }
   else{
   TCPThread* TCPth = new TCPThread(cliente_sd);
 
       std::thread([TCPth]() {
           TCPth->message(); delete TCPth;}).detach();
           char* msg = "Conection accepted\n";
           send(cliente_sd, msg, strlen(msg), 0);
     
   }
}
 
   return 0;
 
}

