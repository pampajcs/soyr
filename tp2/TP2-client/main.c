
/*//----- Include files ---------------------------------------------------------*/
#include <stdio.h>        /* Needed for printf()*/
#include <string.h>        /*Needed for memcpy() and strcpy()*/
#include <windows.h>      /* Needed for all Winsock stuff*/


/*//----- Defines ---------------------------------------------------------------*/
#define  PORT_NUM           2018  /* Port number used*/
#define  IP_ADDR "127.0.0.1" /* IP address of server1 (*** HARDWIRED ***)*/

int q=0;
int e=0;

/*//===== Main program ==========================================================*/
int main(void)
{

  WORD wVersionRequested = MAKEWORD(1,1);       /*// Stuff for WSA functions*/
  WSADATA wsaData;                              /*// Stuff for WSA functions*/


  unsigned int         client_s;        /*// Client socket descriptor*/
  unsigned int         server_s;
  struct sockaddr_in   server_addr;     /*// Server Internet address*/
  int                  addr_len;        /*// Internet address length*/
  char                 out_buf[100];    /*// 100-byte buffer for output data*/
  char                 in_buf[100];    /* // 100-byte buffer for input data*/
  /*// This stuff initializes winsock*/
  WSAStartup(wVersionRequested, &wsaData);


  client_s = socket(AF_INET, SOCK_STREAM, 0);

  server_addr.sin_family      = AF_INET;            /*// Address family to use*/
  server_addr.sin_port        = htons(PORT_NUM);    /*// Port num to use*/
  server_addr.sin_addr.s_addr = inet_addr(IP_ADDR); /*// IP address to use*/

  connect(client_s, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));

  printf("Servidor conectado\n\n");
  printf("Comandos validos: 'echo', 'hora', 'chargen'(nnn) o 'quit'\n");
  addr_len = sizeof(server_addr);
  while(q==0){
    printf("Ingrese comando:\n");
    gets(out_buf);
    send(server_s, out_buf, sizeof(out_buf), 0);
    recv(server_s, in_buf, sizeof(in_buf), 0);
    printf("%s \n", in_buf);
    if(strcmp(in_buf,"echo")==0){
        e=1;
        while(e==1){
            gets(out_buf);
            send(server_s, out_buf, sizeof(out_buf), 0);
            recv(server_s, in_buf, sizeof(in_buf), 0);
            if(!strcmp(in_buf,"echo")){
                e=0;
            }
            printf("%s \n", in_buf);
        }
    }
    if(!strcmp(in_buf,"Servidor cerrando conexion. Que tenga un buen dia")){
        q=1;
    }
  }

  closesocket(client_s);
  WSACleanup();
}
