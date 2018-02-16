#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// error messages: print message and terminate program
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char const *argv[])
{
	int socketUDP, socketTCP, socketResults, portno, length, n, inteiro,PacotesRecebidos=0;
	socklen_t fromlen, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    struct sockaddr_in server, from;
   
	
	if (argc != 2) { printf("Deve ser usado da forma: ./[Programa] [porta]\n"); //SE nao for começado da maneira que se deve, encerra.
                    exit(1);
	}
	portno = atoi(argv[1]); //a porta do servidor vai ter de ser passada quando se inicia o programa (exemplo: ./servidor 55555)

    {//###########################################################################parte da socket tcp##############################
    //cria a socket tcp
	socketTCP= socket(AF_INET, SOCK_STREAM, 0);
	if (socketTCP < 0) error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(socketTCP, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	          error("ERROR on binding");
  	//---waits for clients
     listen(socketTCP,5); 
     clilen = sizeof(cli_addr);
	//conexao estabelecida na socket tcp
  	socketResults = accept(socketTCP, (struct sockaddr *) &cli_addr, &clilen);
  	}//##############################################################################################################################
  	
  	
  	{//##################################################################################parte da socket udp####################################
    //criar a socket UDP
    socketUDP=socket(AF_INET, SOCK_DGRAM, 0);
	if (socketUDP < 0) error("Error in opening socket");
	length = sizeof(server);
	bzero(&server,length);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(portno+1);
	if (bind(socketUDP,(struct sockaddr *)&server,length)<0) 
	   error("Error in binding");
	fromlen = sizeof(struct sockaddr_in);
	//###########################################################################################################################################
}
	
	
	while(1){
		//recebe os packets (inteiros a 1)
		n = recvfrom(socketUDP,&inteiro,sizeof(int),0,(struct sockaddr *)&from,&fromlen);
		inteiro=ntohl(inteiro); 
		printf("Recebido pacote que contém o inteiro: %d\n", inteiro);
		if (n < 0) error("Error a receber os pacotes do cliente (UDP)");
		PacotesRecebidos++;
		if(inteiro==1337) break; //o ultimo pacote enviado é 1337 e marca o fim
	}

	
	//envia numero de pacotes recebidos ao cliente
	printf("Recebi %d pacotes, e vou enviar esse numero para o host em que está fazer o teste\n",PacotesRecebidos);
    n = write(socketResults,&PacotesRecebidos,sizeof(int));
    if (n < 0) error("ERRO a enviar numero de pacotes recebidos (TCP)");
    

    close(socketResults);
    close(socketUDP);
    close(socketTCP);
	
	return 0;
}
