#include "header.h"

bool ring_created = false; 

int new(ring_s *ring, int tcp_c_fd, struct sockaddr_in mynodeaddr_tcp_s){ //cria o anel com um nó com s=self 
    char buf[MAX_CHAR];
    bzero(buf, sizeof(buf));
    if(ring->me.ID==ring->pred.ID){
        printf("Ring already created\n");
        return 0;
    }else{
        
        connect(tcp_c_fd, (struct sockaddr*)&mynodeaddr_tcp_s,
							sizeof(mynodeaddr_tcp_s));
        strcpy(buf, "connecting to self\n");
        write(tcp_c_fd, buf, sizeof(buf));
        printf("Executa a função new()\n");
        ring->pred.ID=ring->me.ID;
        ring->pred.PORT=ring->me.PORT;
        ring->pred.IP=ring->me.IP;
        printf("ID %d\n", ring->me.ID);
        printf("IP %s\n", ring->me.IP);
        printf("PORT %s\n", ring->me.PORT);

        ring->suc.ID=ring->me.ID;
        ring->suc.PORT=ring->me.PORT;
        ring->suc.IP=ring->me.IP;

        

    }
    
    return 1;

}

int pentry(mensagem){

}

char* verify_ip(char* ip_string){

	char delim[] = ".";
    char *ip_string_copy;
    ip_string_copy = (char*) malloc(strlen(ip_string+1)*sizeof(char));
    strcpy(ip_string_copy, ip_string);
    char *ptr = strtok(ip_string, delim);
    int point_counter=0;
    
    while(ptr != NULL)
	{
        if(atoi(ptr)<0 || atoi(ptr)>255 || point_counter > 4){
            printf("IP not valid\n");
            return "error";
        }
        point_counter++;
		ptr = strtok(NULL, delim);
	}
    return ip_string_copy;
}

int split_string(char* command_string){
    	char delim[] = " ";
    printf("command_string= %s", command_string);
    char *ptr = strtok(command_string, delim);
    char *ip_copy;
    int arg_counter = 0;
    char *args[4];
    char *ret;
    while(ptr != NULL)
	{
		printf("'%s'\n", ptr);
        args[arg_counter] = (char*) malloc(strlen(ptr+1)*sizeof(char));
        strcpy(args[arg_counter],ptr);
        arg_counter++;
		ptr = strtok(NULL, delim);
	}
    for(int i = 0; i<arg_counter; i++){
        printf("arg[%d]=%s\n",i, args[i] );
    }
     
    ip_copy = (char*) malloc(strlen(args[2]+1)*sizeof(char));
    strcpy(ip_copy, args[2]);
    ret = verify_ip(ip_copy);
    if(strcmp(ret,"error")==0){
        printf("The IP entered in the command was not valid.\n");
        return 1; 
    }

    for(int i = 0; i<arg_counter; i++){
        printf("arg[%d]=%s\n",i, args[i] );
    }
    //printf("Valid IP!\n");
    return 0;
}

int valid_arguments(int argc, char *argv[], ring_s *ring){
    //return 1 if failure, return 0 if valid
    
    
    //argv[0]= i
    //argv[1]= ip
    //argv[2]=port
    char *ret;
    ret = (char*) malloc(strlen(argv[1]+1)*sizeof(char)); //return do verify ip;
    ret = verify_ip(argv[1]);
    if(argc<3){
        printf("You've Entered Too Few Arguments. Usage: ring i i.IP i.PORT\n");
        return(1);
    }else if(strcmp(ret,"error")==0){                                       //validar o IP

        printf("Each Octave of the IP must be between 0 and 255 for it to be valid!\n");
        return(1);
    }else if(atoi(argv[0]) < 0 || atoi(argv[0]) > 31){                  //validar i
        printf("O i tem de ser um numero entre 0 e 31.\n");
        return(1);
    }else 
    //printf("ret :           %s\n", ret);
    ring->me.ID=atoi(argv[0]);
    strcpy(ring->me.PORT, argv[2]);
    strcpy(ring->me.IP, ret);    
    //validar o IP
    

    return(0);
}

void show(ring_s *ring){
    printf("My ID:%d\nMy IP:%s\nMy PORT:%s\n",ring->me.ID,ring->me.IP,ring->me.PORT);
    if(ring->pred.IP!=NULL || ring->pred.PORT!=NULL){
        printf("\n----------PRED-----------\n");
        printf("ID:%d\n", ring->pred.ID);
        printf("IP:%s\n", ring->pred.IP);
        printf("PORT:%s\n", ring->pred.PORT);
    }
    if(ring->suc.IP!=NULL || ring->suc.PORT!=NULL){
        printf("\n----------SUC-----------\n");
        printf("ID:%d\n", ring->suc.ID);
        printf("IP:%s\n", ring->suc.IP);
        printf("PORT:%s\n", ring->suc.PORT);
    }
}

void initialize_ring_memory(ring_s *ring,int argc, char *argv[]){
    for(int i=0; i<argc; i++){
        printf("arg[%d]=%s\n", i, argv[i]);
    }
    ring->me.ID= atoi(argv[0]);
    ring->me.IP= (char*) malloc(strlen(argv[1]+1)*sizeof(char));
    ring->me.PORT= (char*) malloc(strlen(argv[2]+1)*sizeof(char));
    
    ring->pred.ID=-1;
    ring->suc.ID=-1;
   
    ring->pred.IP=      NULL;
    ring->pred.PORT=    NULL;
    ring->suc .IP=      NULL;
    ring->suc .PORT=    NULL;
 
}



void* handle_connection(int client_socket){
    char message[BUFSIZE]; 
    size_t message_bytes;  // size_t: unsigned integer
    
  

    //read the client's message -- the name of the file to read 
    
    message_bytes=read(client_socket, message, sizeof(message)-1);
   
    message[strlen(message)-1] = 0; //null the terminate message and remove the \n

    printf("REQUEST: %s\n", message);
    write(client_socket, message, message_bytes);
    close(client_socket);

   
    return NULL; 

}


int accept_new_connection(int server_socket){
    int addr_size = sizeof(SA_IN);
    int client_socket; 
    SA_IN client_addr; 
    client_socket=accept(server_socket,(SA*)&client_addr,(socklen_t*)&addr_size);
    return client_socket; 

}

int check(int exp, const char *msg){
    if (exp == SOCKETERROR){
        perror(msg);
        exit(1);
    }
    return(exp); 
}

int max(int x, int y)
{
	if (x > y)
		return x;
	else
		return y;
}

int main(int argc, char *argv[]){

                      //
                                   
    char buf[MAX_CHAR];                     // buffer que vai guardar os caracteres
    int sret /*return do select*/;     //                              //
    
    ring_s ring;
    struct sockaddr_in clinodeaddr, mynodeaddr_tcp_s, mynodeaddr_tcp_c;
    socklen_t len;
    ssize_t n; 
    char* message = "Hello Client";

    int listenfd;/*socket listen do tcp server*/
    int connfd, connfd_cpy; /*socket tcp returned from connect*/
    int udpfd; /*socket udp */
    int tcp_c_fd;

    int maxfdp1; /*max file descriptors inside table*/
    fd_set rset, rset_cpy;

    void sig_chld(int);

    initialize_ring_memory(&ring, argc-1, argv+1); //sets pred and suc memory to NULL (para n ter lixo) and mallocs memory for self node.

    if(valid_arguments(argc - 1, argv + 1, &ring))exit(-1); //se a porta ou ip não estiverem válidos então o programa fecha

    /*create TCP listening socket and UDP SERVERS */
                    /*TCP SERVER*/
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&mynodeaddr_tcp_s, sizeof(mynodeaddr_tcp_s));
	mynodeaddr_tcp_s.sin_family = AF_INET;
	mynodeaddr_tcp_s.sin_addr.s_addr = htonl(INADDR_ANY);
	mynodeaddr_tcp_s.sin_port = htons(atoi(ring.me.PORT));
                    /*UDP SERVER*/
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    pid_t childpid;
    /*bind das sockets UDP e TCP ao "meu" ip e porta*/
    bind(listenfd, (struct sockaddr*)&mynodeaddr_tcp_s, sizeof(mynodeaddr_tcp_s));
	listen(listenfd, 10);
    bind(udpfd, (struct sockaddr*)&mynodeaddr_tcp_s, sizeof(mynodeaddr_tcp_s));

    /*CREATE CLIENT UDP AND TCP CLIENTS*/
    mynodeaddr_tcp_c.sin_family=AF_INET;
	mynodeaddr_tcp_c.sin_port = htons(atoi(ring.me.PORT));
	mynodeaddr_tcp_c.sin_addr.s_addr = inet_addr(ring.me.IP);

    tcp_c_fd = socket(AF_INET, SOCK_STREAM, 0);




    /*clear file descriptor table/set */
    FD_ZERO(&rset);
    
    
    
    // get maxfd
	maxfdp1 = max(listenfd, udpfd) + 1;
    
    while(1){

        /*insert sockets in the fd set*/
        FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);
        FD_SET(STDIN_FILENO, &rset);
        FD_SET(tcp_c_fd, &rset);
        
        rset_cpy = rset;

        sret = select(FD_SETSIZE, &rset_cpy, NULL, NULL, NULL);
        
        if(sret==-1){
            printf("erro no select\n");
            exit(0);
        }

        /*if tcp socket is readable then handle it by accepting the connection*/
        if (FD_ISSET(listenfd, &rset_cpy)) {
           
                len = sizeof(clinodeaddr);

            
                connfd = accept(listenfd, (struct sockaddr*)&clinodeaddr, &len);
                printf("listen: %d\nconn:%d\n", listenfd, connfd);
                //close(listenfd);
                //listen(listenfd, 10);
            
            /*o nó q ta a entrar mete o seu pred */
                
                
            if ((childpid = fork()) == 0) {
                    if(ring_created){
                    close(listenfd);
                    bzero(buf, sizeof(buf));
                
                    read(connfd, buf, sizeof(buf));
                    printf("Message From TCP client (pred): %s\n", buf);
                    
                    write(connfd, (const char*)message, sizeof(buf));
                    close(connfd);
                    exit(0);
                    FD_SET(connfd, &rset);
                }else printf("Can't receive data from TCP. I'm not inside a ring yet.\n");
                
                //close(connfd);
                
                /*NO PENTRY QUERO FECHAR AS SOCKETS E ADMITIR OUTRAS*/
                /*VERIFICAR SE "EU" TENHO ANEL CRIADO*/
                /*TESTAR SE EXISTE ALGUM NÓ NA CHAVE*/
                /*SE NAO EXISTIR ENTAO */
            }
                    
                    
            
        }

        /*if(FD_ISSET(connfd, &rset)){
            printf("a responder ao connfd: %d\n", connfd);
            read(connfd, buf, sizeof(buf));
                //printf("Message From TCP client (pred): %s\n", buf);

            write(connfd, (const char*)message, sizeof(buf));
        }*/

        // if udp socket is readable receive the message.
		if (FD_ISSET(udpfd, &rset_cpy)) {
			len = sizeof(clinodeaddr);
			bzero(buf, sizeof(buf));
			n = recvfrom(udpfd, buf, sizeof(buf), 0,
						(struct sockaddr*)&clinodeaddr, &len);
			printf("Message from UDP client (shortcut): %s\n",buf);
            //puts(buf);
            /*   MESSAGE HANDLER PARA O SHORCUT */

            /*a mensaguem recebida por udp é o buf*/
			sendto(udpfd, (const char*)message, sizeof(buf), 0,
				(struct sockaddr*)&clinodeaddr, sizeof(clinodeaddr));
		}
        
        if(FD_ISSET(STDIN_FILENO, &rset_cpy)){
            bzero(buf, sizeof(buf));
            read(STDIN_FILENO,buf, sizeof(buf));
            printf("User input %s\n", buf);
            /*HANDLE MESSAGES FROM USER INPUT*/
            if(strcmp(buf, "n\n")==0){
                ring_created = true;
                new(&ring, tcp_c_fd, mynodeaddr_tcp_s);
            }else if(strcmp(buf, "e\n")==0  || strcmp(buf, "l\n")==0 || strcmp(buf, "ex\n")==0){
                printf("e,s,l pressed\n");
            }else if(strcmp(buf, "s\n")==0){ //show
                show(&ring);
             
            }else if(strcmp(buf, "\n")==0){
                continue;
            }
        }
      
            //separar string 
            //split_string(buf); //isto vai ser recebido por sockets de clientes 
            // pentry: 
            // verificar se existe o nó "pred", e se não tiver 
            

        
        
      


        
        
    }/*while*/
}/*main*/