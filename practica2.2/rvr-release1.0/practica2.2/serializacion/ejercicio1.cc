#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, NAME_SIZE);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
        int32_t jugador_size = NAME_SIZE * sizeof(char) + 2 * sizeof (int16_t) ;
        alloc_data(jugador_size);

        //memcpy(void *restrict dest, const void *restrict src, size_t n);
        char *tmp = _data;

        memcpy(tmp, name, NAME_SIZE*sizeof(char));
        tmp += NAME_SIZE *sizeof(char);

        memcpy(tmp, &x, sizeof(int16_t));
        tmp += sizeof(int16_t);

        memcpy(tmp, &y, sizeof(int16_t));
    }
    

    int from_bin(char * data)
    {  //CREO QUE ESTA BIEN
        char *tmp = data;
        memcpy(name, tmp, NAME_SIZE*sizeof(char)); 
        tmp += NAME_SIZE *sizeof(char);
        memcpy(&x, tmp, sizeof(int16_t));
        tmp += sizeof(int16_t);
        memcpy(&y, tmp, sizeof(int16_t)); 
        tmp += sizeof(int16_t); 
        
        return 0;
    }


public:
    static const size_t NAME_SIZE = 80;
    char name[NAME_SIZE];    
    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
    char* fileName = "./jugador.bin";

    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);

    // 1. Serializar el objeto one_w
    one_w.to_bin();

    // 2. Escribir la serialización en un fichero
    int fd = open (fileName, O_CREAT | O_TRUNC |O_RDWR, 0666); 
    write(fd, one_w.data(), one_w.size());  
    close(fd);

    // 3. Leer el fichero
    char buffer[one_w.size()];
    fd = open (fileName, O_RDONLY,0666);
    //write(fd, one_w_data(), one_w_size());
    read(fd, buffer, one_w.size());
    close(fd);

    // 4. "Deserializar" en one_r
    one_r.from_bin(buffer);
    
    // 5. Mostrar el contenido de one_r
    std::cout<<"NAME: "<<one_r.name << std::endl;
    std::cout<<"X: "<<one_r.x << std::endl;
    std::cout<<"Y: "<<one_r.y << std::endl;

    return 0;
}

