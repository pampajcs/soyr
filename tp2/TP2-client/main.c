//====================================================== file = client1.c =====
//=  A message "client" program to demonstrate sockets programming            =
//=   - UDP/IP client/server model is implemented                             =
//=============================================================================
//=  Notes:                                                                   =
//=    1) This program conditionally compiles for Winsock and BSD sockets.    =
//=       Set the initial #define to WIN or BSD as appropriate.               =
//=    2) There is *no* error checking in this program.  Error checking was   =
//=       omitted to simplify the program.                                    =
//=    3) This program needs server1 to be running on another host.  Program  =
//=       server1 should be started first.                                    =
//=    4) This program assumes that the server1 has the IP address hardcoded  =
//=       into "#define IP_ADDR"                                              =
//=    5) The steps #'s correspond to lecture topics.                         =
//=---------------------------------------------------------------------------=
//=  Build: bcc32 client1.c or cl client1.c wsock32.lib for Winsock           =
//=         gcc client1.c -lsocket -lnsl for BSD                              =
//=---------------------------------------------------------------------------=
//=  History:  KJC (03/02/99) - Genesis (from client.c)                       =
//=            KJC (05/20/03) - Updated socket naming to match Kurose et al.  =
//=============================================================================
#define  WIN                // WIN for Winsock and BSD for BSD sockets

//----- Include files ---------------------------------------------------------
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

//----- Defines ---------------------------------------------------------------
#define  PORT_NUM           2018  // Port number used
#define  IP_ADDR "127.0.0.1" // IP address of server1 (*** HARDWIRED ***)

int q=0;
int e=0;

//===== Main program ==========================================================
void main(void)
{
#ifdef WIN
  WORD wVersionRequested = MAKEWORD(1,1);       // Stuff for WSA functions
  WSADATA wsaData;                              // Stuff for WSA functions
#endif

  unsigned int         client_s;        // Client socket descriptor
  struct sockaddr_in   server_addr;     // Server Internet address
  int                  addr_len;        // Internet address length
  char                 out_buf[100];    // 100-byte buffer for output data
  char                 in_buf[100];     // 100-byte buffer for input data

#ifdef WIN
  // This stuff initializes winsock
  WSAStartup(wVersionRequested, &wsaData);
#endif

  client_s = socket(AF_INET, SOCK_DGRAM, 0);

  server_addr.sin_family      = AF_INET;            // Address family to use
  server_addr.sin_port        = htons(PORT_NUM);    // Port num to use
  server_addr.sin_addr.s_addr = inet_addr(IP_ADDR); // IP address to use
  printf("Servidor conectado\n\n");
  printf("Comandos validos: 'echo', 'hora', 'chargen'(nnn) o 'quit'\n");
  addr_len = sizeof(server_addr);
  while(q==0){
    printf("Ingrese comando:\n");
    gets(out_buf);
    sendto(client_s, out_buf, (strlen(out_buf) + 1), 0,
    (struct sockaddr *)&server_addr, sizeof(server_addr));
    recvfrom(client_s, in_buf, sizeof(in_buf), 0,
    (struct sockaddr *)&server_addr, &addr_len);
    printf("%s \n", in_buf);
    if(strcmp(in_buf,"echo")==0){
        e=1;
        while(e==1){
            gets(out_buf);
            sendto(client_s, out_buf, (strlen(out_buf) + 1), 0,
            (struct sockaddr *)&server_addr, sizeof(server_addr));
            recvfrom(client_s, in_buf, sizeof(in_buf), 0,
            (struct sockaddr *)&server_addr, &addr_len);
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
#ifdef WIN
  closesocket(client_s);
#endif
#ifdef BSD
  close(client_s);
#endif

#ifdef WIN
  WSACleanup();
#endif
}
