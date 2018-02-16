#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>


// error messages: print message and terminate program
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char const *argv[]) {
    int menu, pacotes, Inteiro, UltimoInteiro, PacotesRecebidos;
    char host[100];
    int socketTCP, socketUDP, portno, n, length;
    struct sockaddr_in serv_addr, server2;
    struct hostent *server, *hp;
    int invalido= 1, CorrerPrograma =1;


    //CONFIGURACOES POR DEFEITO
    portno= 50000;
    pacotes= 5;
    UltimoInteiro= 1337;
    Inteiro= 1143;
    strcpy(host, "localhost");


    system("clear");
    while(CorrerPrograma) {
        system("clear");
        printf("TestConnection 2016\nby [Joao Ferreira & Jose Castanheira] (DEEC-UC)\nCurrent ports: %d[TCP] and %d[UDP]\nCurrent number of test packets: %d\n\nMenu:\n0. Exit\n1. Set destination host\n2. Test connection to destination\n3. Set number of test packets\n4. Set ports (49152–65535)\n",portno,portno+1,pacotes);
        scanf("%d", &menu);

        switch(menu) {



            {
            case 0 :
                system("clear");
                printf("De certeza que quer sair?\n1.Sim\n2.Nao\n");
                int sair;
                scanf("%d",&sair);
                if (sair==1) {
                    system("clear");
                    printf("Adeus!\n");
                    sleep(2);
                    return 0;
                }
                else break;
            }


            {
            case 1 :
                system("clear");
                invalido=1;
                struct hostent *aux;
                aux=0;
                while(aux==0) {
                    printf("Introduza um novo destination host válido (IP ou string equivalente).\n");
                    scanf("%s",host);

                    //verifica se foi valido
                    aux = gethostbyname(host); //retorna um NULL POINTER se aconteceu erro


                }
                printf("Sucesso! O novo host será: %s\n ",host);
                sleep(2);
                break;
            }

            {
            case 2 :
                system("clear");
                {   //##############################################################################################criacao das sockets (aula de TCP e UDP)
                    //TCP
                    socketTCP = socket(AF_INET, SOCK_STREAM, 0);
                    if (socketTCP < 0) error("ERRO a abrir socket\n");
                    server = gethostbyname(host);  //uses DNS to know IP address
                    if (server == NULL) {
                        error("Unknown host");
                    }
                    bzero((char *) &serv_addr, sizeof(serv_addr)); //clears serv_addr
                    serv_addr.sin_family = AF_INET;
                    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
                    serv_addr.sin_port = htons(portno); //portno must be in network format

                    //---connect (to server)
                    if (connect(socketTCP,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
                        error("ERRO a conectar. Host não está ligado?\n");
                    //UDP
                    socketUDP= socket(AF_INET, SOCK_DGRAM, 0);
                    if (socketUDP < 0) error("socket");
                    server2.sin_family = AF_INET;
                    hp = gethostbyname(host);
                    if (hp==0) error("Unknown host");
                    bcopy((char *)hp->h_addr,(char *)&server2.sin_addr, hp->h_length);
                    server2.sin_port = htons(portno+1);
                    length=sizeof(struct sockaddr_in);
                }//#######################################################################################
                //Envia pacotes por UDP
                for (int i = 0; i < pacotes; i++) {

                    if(i==pacotes-1) {
                        printf("Enviando pacote contendo: %d\n",UltimoInteiro);
                        UltimoInteiro=htonl(UltimoInteiro);
                        n=sendto(socketUDP,&UltimoInteiro,sizeof(UltimoInteiro),0,(struct sockaddr *)&server2,length);
                        UltimoInteiro=ntohl(UltimoInteiro);
                        if (n < 0) error("Error in sendto UDP");
                    }
                    else {
                        printf("Enviando pacote contendo: %d\n",Inteiro);
                        Inteiro=htonl(Inteiro);
                        n=sendto(socketUDP,&Inteiro,sizeof(Inteiro),0,(struct sockaddr *)&server2,length);
                        Inteiro=ntohl(Inteiro);
                        if (n < 0) error("Error in sendto UDP");
                    }
                }

                //Recebe o numero de pacotes que o servidor recebeu
                n = read(socketTCP,&PacotesRecebidos,sizeof(PacotesRecebidos));
                if (n < 0) error("ERROR reading from socket");
                float percentagem= (PacotesRecebidos/pacotes)*100;
                printf("\n%d Pacotes enviados e %d pacotes recebidos no outro host,\nA taxa de sucesso é de %f%%\n",pacotes,PacotesRecebidos,percentagem);
                close(socketUDP);
				close(socketTCP);
                sleep(10);
                break;
            }

            {
            case 3 :
                system("clear");
                invalido=1;
                while(invalido)
                {
                    printf("Por favor Introduza o Número de Pacotes que deseja enviar para o outro host:\n");
                    scanf("%d",&pacotes);
                    if (pacotes<=0)
                        printf("Número de pacotes inválido\n");
                    else
                    {
                        printf("Sucesso! O numero de pacotes que será enviado é: %d\n",pacotes);
                        sleep(2);
                        invalido=0;
                    }
                }
                break;
            }
        case 4 :
            system("clear");
            int auxiliar= 99999;
            while(auxiliar< 49152 || auxiliar>65535) {
                printf("Introduza a porta em que deseja fazer a conexao, entre 49152 e 65535.\n");
                scanf("%d",&auxiliar);
                portno=auxiliar;

                if(auxiliar< 49152 || auxiliar>65535)
                {
                    printf("A Porta inserida não é válida, introduza de novo\n");
                }
                printf("Sucesso! A porta TCP foi atualizada e é agora %d!\nA Porta UDP é então %d\n",portno,portno+1);
                sleep(2);
            }
            break;

        default :
            system("clear");
            printf("Por Favor Introduza uma escolha do menu válida!\n");
            sleep(2);
            break;

        }
    }

    
    return 0;
}

