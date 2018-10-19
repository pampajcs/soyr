/*----Programa TP1----
//Descripcion: Simulacion de funcionamiento
//             de un router de cuatro bocas,
//             cada una de ellas conectadas
//             a una subred distinta
//Materia: Sistemas operativos y redes
//Profesor: Gerardo Sager
//Autores: Scatuerchio Juan Cruz
//         Aparicio Facundo*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/*----VARIABLES----*/

struct boca{
    char ip[16];
    char subnet[16];
    char mask[16];
};

struct boca bocaA;
struct boca bocaB;
struct boca bocaC;
struct boca bocaD;
char def_gateway[16];
char ip_privada[]="192.168.0.255";

struct pack{
    char ip_origen[16];
    char ip_destino[16];
    int fin_archivo;
};

struct pack packA;
struct pack packB;
struct pack packC;
struct pack packD;
struct in_addr *addr;

FILE *config,*pack_bocaA,*pack_bocaB,*pack_bocaC,*pack_bocaD,*rechazados,*decisiones_qd,*decisiones_hexa;

    unsigned long dir;
    char default_gateway[]="no";

/*----FUNCIONES----*/

int get_config(FILE * archivo);
int verificacion(struct boca boca_n);
struct pack read_pack(FILE * pack_boca);
void out(struct pack paquetes);
unsigned long identif_boca(unsigned long ip_origen_);

/*----PROGRAMA----*/

int main(){
    config = fopen("routerconfig.txt","r");
    pack_bocaA = fopen("bocaA.txt","r");
    pack_bocaB = fopen("bocaB.txt","r");
    pack_bocaC = fopen("bocaC.txt","r");
    pack_bocaD = fopen("bocaD.txt","r");
    rechazados = fopen("rechazados.txt","w");
    decisiones_qd = fopen("decisiones_qd.txt","w");
    decisiones_hexa = fopen("decisiones_hexa.txt","w");
    if( config == NULL ) {
        printf("No se encontro el archivo de configuracion");
    }
    get_config(config);
    fprintf(decisiones_qd,"IP origen        IP destino      Boca IN     Boca OUT        DG\n");
    printf("boca A\n");
    while(packA.fin_archivo==0){
        packA = read_pack(pack_bocaA);
        out(packA);
    }
    printf("boca B\n");
    while(packB.fin_archivo==0){
        packB = read_pack(pack_bocaB);
        out(packB);
    }
    printf("boca C\n");
    while(packC.fin_archivo==0){
        packC = read_pack(pack_bocaC);
        out(packC);
    }
    printf("boca D\n");
    while(packD.fin_archivo==0){
        packD = read_pack(pack_bocaD);
        out(packD);
    }
    return 0;
}

int get_config(FILE * file){
    int error=0;
    fscanf(file,"%s",bocaA.ip);
    fscanf(file,"%s",bocaA.subnet);
    fscanf(file,"%s",bocaA.mask);
    fscanf(file,"%s",bocaB.ip);
    fscanf(file,"%s",bocaB.subnet);
    fscanf(file,"%s",bocaB.mask);
    fscanf(file,"%s",bocaC.ip);
    fscanf(file,"%s",bocaC.subnet);
    fscanf(file,"%s",bocaC.mask);
    fscanf(file,"%s",bocaD.ip);
    fscanf(file,"%s",bocaD.subnet);
    fscanf(file,"%s",bocaD.mask);
    fscanf(file,"%s",def_gateway);
    return error;
}

int verificacion(struct boca boca_n){
    /*hacer comparacion de la salida de la and entre ip y mask con subred*/



    /*Lo hace Facu??*/
    return 0;
}

struct pack read_pack(FILE * pack_boca ){           /*funcion que lee el archivo de paquetes*/
    struct pack salida;
    if(!feof(pack_boca)){
        fscanf(pack_boca,"%s",salida.ip_origen);
        fscanf(pack_boca,"%s",salida.ip_destino);
        salida.fin_archivo=0;
    }
    else{
        salida.fin_archivo=1;
    }
    return salida;
}

void out(struct pack paquetes){             /*funcion que escribe en el archivo de decisiones*/
    if(paquetes.fin_archivo==0){
        unsigned long ip_or=ntohl(inet_addr(paquetes.ip_origen));
        unsigned long ip_des=ntohl(inet_addr(paquetes.ip_destino));
        unsigned long boca_en=identif_boca(ip_or);
        unsigned long boca_sal=identif_boca(ip_des);
        if(boca_en == 0 || boca_sal==0){
            printf("rechazado\n");
            fprintf(rechazados,"%X %X %s %s\n",ip_or,ip_des,paquetes.ip_origen,paquetes.ip_destino);
        }
        else{
            printf("%X  %X  %X  %X  %s\n", ip_or,ip_des,boca_en, boca_sal,default_gateway);
            fprintf(decisiones_hexa,"%X %X  %X      %X      %s\n", ip_or,ip_des,boca_en, boca_sal,default_gateway);
            fprintf(decisiones_qd,"%s   %s    %X    %X     %s\n", paquetes.ip_origen,paquetes.ip_destino,boca_en, boca_sal,default_gateway);
        }
    }
}

unsigned long identif_boca(unsigned long ip_origen_dest){       /*funcion que identifica la boca de ip de origen o destino*/
    unsigned long identif_boca_sal;
    if(ip_origen_dest==ntohl(inet_addr(ip_privada))){
        identif_boca_sal=0;
        printf("salida = 0");
    }
    else{
        if((ip_origen_dest & ntohl(inet_addr(bocaA.mask))) == ntohl(inet_addr(bocaA.subnet))){
            identif_boca_sal=ntohl(inet_addr(bocaA.ip));
        }
        else{
            if((ip_origen_dest & ntohl(inet_addr(bocaB.mask))) == ntohl(inet_addr(bocaB.subnet))){
                identif_boca_sal=ntohl(inet_addr(bocaB.ip));
            }
            else{
                if((ip_origen_dest & ntohl(inet_addr(bocaC.mask))) == ntohl(inet_addr(bocaC.subnet))){
                    identif_boca_sal=ntohl(inet_addr(bocaC.ip));
                }
                else{
                    if((ip_origen_dest & ntohl(inet_addr(bocaD.mask))) == ntohl(inet_addr(bocaD.subnet))){
                        identif_boca_sal=ntohl(inet_addr(bocaD.ip));
                    }
                    else{
                        if(ip_origen_dest==ntohl(inet_addr(ip_privada))){
                            identif_boca_sal=0;
                        }
                        else{
                            /*identif_boca_sal = boca del default gateway
                            default_gateway[0]='s';
                            default_gateway[1]='i';*/
                        }
                    }
                }
            }
        }
    }
    if(ip_origen_dest == ntohl(inet_addr(def_gateway))){
        default_gateway[0]='s';
        default_gateway[1]='i';
    }
    else{
        default_gateway[0]='n';
        default_gateway[1]='o';
    }
    return identif_boca_sal;
}

