//====================================================== file = server1.c ====
//=  A message "server" program to demonstrate sockets programming           =
//=   - UDP/IP client/server model is implemented                            =
//============================================================================
//=  Notes:                                                                  =
//=    1) This program conditionally compiles for Winsock and BSD sockets.   =
//=       Set the initial #define to WIN or BSD as appropriate.              =
//=    2) There is *no* error checking in this program.  Error checking was  =
//=       omitted to simplify the program.                                   =
//=    3) This program serves a message to program client1 running on        =
//=       another host.                                                      =
//=    4) This program assumes that the client1 has the IP address hardcoded =
//=       into "#define IP_ADDR"                                             =
//=    5) The steps #'s correspond to lecture topics.                        =
//=--------------------------------------------------------------------------=
//=  Build: bcc32 server1.c or cl server1.c wsock32.lib for Winsock          =
//=         gcc server1.c -lsocket -lnsl for BSD                             =
//=--------------------------------------------------------------------------=
//=  History:  KJC (03/02/99) - Genesis (from server.c)                      =
//============================================================================
#define  WIN                // WIN for Winsock and BSD for BSD sockets

//----- Include files -------------------------------------------------------
#include <stdio.h>          // Needed for printf()
#include <string.h>         // Needed for memcpy() and strcpy()
#ifdef WIN
  #include <windows.h>      // Needed for all Winsock stuff
#endif
#ifdef BSD
  #include <sys/types.h>    // Needed for system defined identifiers.
  #include <netinet/in.h>   // Needed for internet address structure.
  #include <sys/socket.h>   // Needed for socket(), bind(), etc...
  #include <arpa/inet.h>    // Needed for inet_ntoa()
  #include <fcntl.h>        // Needed for sockets stuff
  #include <netdb.h>        // Needed for sockets stuff
#include <unistd.h>
#endif

//----- Defines -------------------------------------------------------------
#define  PORT_NUM           2018  // Port number used
#define  IP_ADDR "127.0.0.1" // IP address of server1 (*** HARDWIRED ***)

int estado,e;
int c=0;
char pedido[20];
char out_buf[100];    // 100-byte buffer for output data
char in_buf[100];     // 100-byte buffer for input data
unsigned int         server_s;        // Server socket descriptor
struct sockaddr_in   server_addr;     // Server1 Internet address
struct sockaddr_in   client_addr;     // Client1 Internet address
int                  addr_len;        // Internet address length
int q=0;

int comparacion (char *cadena);
void hora(void);
void chargen(void);
/*void echo(char cadena);*/
/*void quit(void);*/

//===== Main program ========================================================
void main(void)
{
#ifdef WIN
  WORD wVersionRequested = MAKEWORD(1,1);       // Stuff for WSA functions
  WSADATA wsaData;                              // Stuff for WSA functions
#endif
  long int             i;               // Loop counter

#ifdef WIN
  // This stuff initializes winsock
  WSAStartup(wVersionRequested, &wsaData);
#endif

  // >>> Step #1 <<<
  // Create a socket
  //   - AF_INET is Address Family Internet and SOCK_DGRAM is datagram
  server_s = socket(AF_INET, SOCK_DGRAM, 0);

  // >>> Step #2 <<<
  // Fill-in my socket's address information
  server_addr.sin_family      = AF_INET;            // Address family to use
  server_addr.sin_port        = htons(PORT_NUM);    // Port number to use
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Listen on any IP address
  bind(server_s, (struct sockaddr *)&server_addr, sizeof(server_addr));

  // >>> Step #3 <<<
  // Fill-in client1 socket's address information
  client_addr.sin_family      = AF_INET;            // Address family to use
  client_addr.sin_port        = htons(PORT_NUM);    // Port num to use
  client_addr.sin_addr.s_addr = inet_addr(IP_ADDR); // IP address to use

    addr_len = sizeof(client_addr);
  // >>> Step #4 <<<

    estado=1;
    while(q==0){
    switch (estado){
        case 1:/*escucha*/
            // Wait to receive a message from client1
            printf("Escuchando\n");
            recvfrom(server_s, in_buf, sizeof(in_buf), 0,(struct sockaddr *)&client_addr, &addr_len);
            // Output the received message
            printf("Comando: %s\n", in_buf);
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
                strcpy(out_buf, "No entiendo su pedido. Envíe un comando válido: echo, fecha, chargen o quit");
                sendto(server_s, out_buf, (strlen(out_buf) + 1), 0,(struct sockaddr *)&client_addr, sizeof(client_addr));

            }
            if (comparacion(in_buf)==1 && c==0){ /*echo*/
                estado=2;
                strcpy(out_buf, "echo");
                sendto(server_s, out_buf, (strlen(out_buf) + 1), 0,(struct sockaddr *)&client_addr, sizeof(client_addr));
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
            // Output the received message
            /*printf("%s \n", in_buf);*/
            while(estado==2){
                recvfrom(server_s, in_buf, sizeof(in_buf), 0,(struct sockaddr *)&client_addr, &addr_len);
                if (comparacion(in_buf)==1){
                    estado=1;
                    strcpy(out_buf, "echo");
                    sendto(server_s, out_buf, (strlen(out_buf) + 1), 0,(struct sockaddr *)&client_addr, sizeof(client_addr));
                }
                else{
                    strcpy(out_buf, in_buf);
                    sendto(server_s, out_buf, (strlen(out_buf) + 1), 0,(struct sockaddr *)&client_addr, sizeof(client_addr));
                    /*printf("Envio: %s\n",out_buf);*/
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
            sendto(server_s, out_buf, (strlen(out_buf) + 1), 0,(struct sockaddr *)&client_addr, sizeof(client_addr));
            break;

    }
}
  // >>> Step #6 <<<
  // Close all open sockets
#ifdef WIN
  closesocket(server_s);
#endif
#ifdef BSD
  close(server_s);
#endif
#ifdef WIN
  // This stuff cleans-up winsock
  WSACleanup();
#endif
}

int comparacion (char *cadena){ /*compara la cadena de caracteres ingresada por el cliente con los estados posibles, depende el estado manda un flag con un valor */
    char estadoe[]="echo";
    char estadoh[]="hora";
    char estadoc[]="chargen";
    char estadoq[]="quit";
    int flag;

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
    }
    return flag;
}

void hora(void){
        time_t t;
        struct tm *tm;
        char horayfecha[100];
        t=time(NULL);
        tm=localtime(&t);
        strftime(horayfecha, 100, "%H:%M:%S %d/%m/%Y", tm);
        /*printf ("%s\n", horayfecha);*/
        strcpy(out_buf, horayfecha);
        sendto(server_s, out_buf, (strlen(out_buf) + 1), 0,(struct sockaddr *)&client_addr, sizeof(client_addr));
        return;
}

void chargen(void){
        int cnt=0;
        char cadena[100];
        for (cnt=32;cnt<=126;cnt++){
            cadena[cnt-32]=cnt;
        }
        cadena[95]='\0';
        strcpy(out_buf,cadena);
        sendto(server_s, out_buf, (strlen(out_buf) + 1), 0,(struct sockaddr *)&client_addr, sizeof(client_addr));
        /*printf("%s\n",out_buf);*/
        return;
}


