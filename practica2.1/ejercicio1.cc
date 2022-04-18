#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>


int main (int argc, char* argv[]){
    
    //Si no se ha escrito ninguna dirección
    if(argc<2)
    {
        printf("Direccion no introducida, prueba con ./program www.pagina.com \n");
        return 1;
    }

    struct addrinfo* info;
    //devuelve 0 en caso de erros y un numero identificando al error, el cual se saca de gai_strerror
    int rc = getaddrinfo(argv[1],argv[2],NULL,&info);

    if(rc != 0){
        printf("Error getaddrinfo: %s\n" ,gai_strerror(rc));
        return -1;
    }
     

    for(struct addrinfo* it = info; it != NULL; it = it->ai_next){

        char hostname[NI_MAXHOST];
        getnameinfo(it->ai_addr, it->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
       
        if(hostname != "\0"){
            printf("%s  |   %d  |  %d\n", hostname, it->ai_family, it->ai_socktype);
        }


    }

    freeaddrinfo(info);

    return 0;
}