/*----- Include files -------------------------------------------------------*/
#include <stdio.h>          /*Needed for printf()*/
#include <string.h>         /* Needed for memcpy() and strcpy()*/
#include <windows.h>      /*Needed for all Winsock stuff*/
#include <time.h>
/*----- Defines -------------------------------------------------------------*/
#define  PORT_NUM           2018  /* Port number used*/
#define  IP_ADDR "127.0.0.1" /*IP address of server1 (*** HARDWIRED ***)*/

int estado,e;
int c=0;
char pedido[20];
char out_buf[100];    /* 100-byte buffer for output data*/
char in_buf[100];     /* 100-byte buffer for input data*/
char cadena[100];
char lect_com[100];
unsigned int         server_s;        /*Server socket descriptor*/
unsigned int         client_s;        /*client socket descriptor*/
struct sockaddr_in   server_addr;     /*Server1 Internet address*/
struct sockaddr_in   client_addr;     /*Client1 Internet address*/
int                  addr_len;        /*Internet address length*/
int q=0;
int sin_size;

int comparacion (char *cadena);
void hora(void);
void chargen(void);
void recibir(char *buffer_in);

/*//===== Main program ========================================================*/
int main(void)
{
    WORD wVersionRequested = MAKEWORD(1,1);      /* Stuff for WSA functions*/
    WSADATA wsaData;                             /* Stuff for WSA functions*/
    long int             i;               /*Loop counter*/

    /* This stuff initializes winsock*/
    WSAStartup(wVersionRequested, &wsaData);

    /*>>> Step #1 <<<
    Create a socket
    - AF_INET is Address Family Internet and SOCK_DGRAM is datagram*/
    server_s = socket(AF_INET, SOCK_STREAM, 0);

    /* >>> Step #2 <<<
    Fill-in my socket's address information*/
    server_addr.sin_family      = AF_INET;            /*Address family to use*/
    server_addr.sin_port        = htons(PORT_NUM);    /*Port number to use*/
    server_addr.sin_addr.s_addr = inet_addr(IP_ADDR);/*(inet_addr(IP_ADDR));  /*Listen on any IP address*/
    memset(&(server_addr.sin_zero), '\0', 8); /* Poner a cero el resto de la estructura*/
    bind(server_s, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_s, 10);
    sin_size = sizeof(struct sockaddr_in);
    client_s = accept(server_s, (struct sockaddr *)&client_addr, &sin_size);
    printf("acept: %d",client_s);
    estado=1;
    while(q==0 && client_s>=0){
    switch (estado){    /*Maquina de estados de los comandos echo chargen hora quit*/
        case 1:/*escucha*/
            /* Wait to receive a message from client1*/
            /*printf("Escuchando\n");*/
            i=recv(client_s, in_buf, sizeof(in_buf), 0);
            recibir(in_buf);
            if(i == -1){
                printf("errno: %d\n", errno);
            }
            else{
                /*printf("%s\n",in_buf);*/
                /*strcpy(out_buf,"recibi");*/
                send(client_s, out_buf, sizeof(out_buf), 0);
            }
            /* Output the received message*/
            /*printf("Comando: %s\n", in_buf);*/
            if (c==1){
                    if(comparacion(in_buf)==0 || comparacion(in_buf)==3){
                        estado=4;
                    }
                    if (comparacion(in_buf)==1){ /*echo*/
                        estado=2;
                        c=0;
                    }
                    if (comparacion(in_buf)==2){/*hora*/
                        estado=3;
                        c=0;
                    }
                    if (comparacion(in_buf)==4){/*quit*/
                        estado=5;
                        c=0;
                    }

            }
            if(comparacion(in_buf)==0 && c==0){
                strcpy(out_buf, "No entiendo su pedido. Envie un comando valido: echo, hora, chargen o quit");
                send(server_s, out_buf, sizeof(out_buf), 0);

            }
            if (comparacion(in_buf)==1 && c==0){ /*echo*/
                estado=2;
                strcpy(out_buf, "echo");
                send(server_s, out_buf, sizeof(out_buf), 0);
            }
            if (comparacion(in_buf)==2 && c==0){/*hora*/
                estado=3;
            }
            if (comparacion(in_buf)==3 && c==0){/*chargen*/
                estado=4;

            }
            if (comparacion(in_buf)==4 && c==0){/*quit*/
                estado=5;
            }
            break;

        case 2:/*echo*/
            while(estado==2){
                recv(server_s, in_buf, sizeof(in_buf), 0);
                if (comparacion(in_buf)==1){
                    estado=1;
                    strcpy(out_buf, "echo");
                    send(server_s, out_buf, sizeof(out_buf), 0);
                }
                else{
                    strcpy(out_buf, in_buf);
                    send(server_s, out_buf, sizeof(out_buf), 0);
                }
            }
            break;

        case 3:/*hora*/
            hora();
            estado=1;
            break;

        case 4:/*chargen*/
            c=1;
            chargen();
            estado=1;
            break;
        case 5:
            q=1;
            strcpy(out_buf, "Servidor cerrando conexion. Que tenga un buen dia");
            send(server_s, out_buf, sizeof(out_buf), 0);
            break;

    }
}
 /* >>> Step #6 <<<
  Close all open sockets*/
  closesocket(server_s);
  /* This stuff cleans-up winsock*/
  WSACleanup();
}

int comparacion (char *cadena){ /*compara la cadena de caracteres ingresada por el cliente con los estados posibles, depende el estado manda un flag con un valor */

    int flag=10;
   /* char estadoe[]="echo";
    char estadoh[]="hora";
    char estadoc[]="chargen";
    char estadoq[]="quit";

    flag=0;

    if (strcmp (estadoe,cadena) == 0){
        flag=1;
    }

    if (strcmp (estadoh,cadena) == 0){
        flag=2;
    }

    if (strcmp (estadoc,cadena) == 0){
        flag=3;
    }

    if (strcmp (estadoq,cadena) == 0){
        flag=4;
    }*/
    return flag;
}

void hora(void){ /*Entrega la hora*/
        time_t t;
        struct tm *tm;
        char horayfecha[100];
        t=time(NULL);
        tm=localtime(&t);
        strftime(horayfecha, 100, "%H:%M:%S %d/%m/%Y", tm);
        strcpy(out_buf, horayfecha);
        send(server_s, out_buf, sizeof(out_buf), 0);
        return;
}

void chargen(void){ /*Entrega la cadena ascii*/
        int cnt=0;
        char cadena[100];
        for (cnt=32;cnt<=126;cnt++){
            cadena[cnt-32]=cnt;
        }
        cadena[95]='\0';
        strcpy(out_buf,cadena);
        send(server_s, out_buf, sizeof(out_buf), 0);
        return;
}
void recibir(char *buffer){
    char cad_aux[100];
    int tam=0;
    int k=0;
    int c=0;
    cad_aux[0]='\n';
        strcat(cadena,buffer);
        for(k=0;k<=100;k++){
            printf("%c-",cadena[k]);
            if(cadena[k] == '\n'){
                *lect_com='\0';
                strcpy(lect_com,cadena);
                /*printf("lect_com: %s\n",lect_com);*/
                for(c=0;c<100;c++){
                    cadena[c]=0;
                }
            }
        }
        printf("\n");
        k=0;
    /*}*/
}

