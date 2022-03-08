#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

char* readHostname();
char* readCpuName();
double GetCPULoad();

int main (int argc, char** argv){
     if(argc != 2){
        printf("Input a correct number of a port.\n");
        return 0;
    }
    int listenSock, connSock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int port = atoi(argv[1]);

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    memset(address.sin_zero, '\0', sizeof(address.sin_zero));

    if ((listenSock = socket(AF_INET, SOCK_STREAM,  IPPROTO_TCP)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    if (bind(listenSock, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(listenSock, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    char* httpHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";
    
    //hlavni prijmaci smycka
    while(1){
       // int connSock = accept(listenSock,NULL, NULL);
        if (( connSock = accept(listenSock, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        char buffer[256] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";
        char clientMessage[512];
        strcat(buffer,httpHeader);
        read(connSock,clientMessage,512);
        
       if(strncmp("GET /hostname",clientMessage,13)==0){
           strcat(buffer,readHostname());
           send(connSock,buffer,strlen(buffer),0);  
           free(readHostname());
       }
       else if(strncmp("GET /cpu-name",clientMessage,13)==0){
           strcat(buffer,readCpuName());
           write(connSock,buffer,strlen(buffer));
           free(readCpuName());
       }
       else if(strncmp("GET /load",clientMessage,9)==0){ 
          char* buf = malloc(sizeof(char)*10);
          gcvt (GetCPULoad(), 10, buf);
          strcat(buffer,buf);
          write(connSock,buffer,strlen(buffer));
          free(buf);
       }
       memset(buffer,0,strlen(buffer));
       close(connSock);
    }
    close(listenSock);
    return 0;
}
char* readHostname(){
    char* hostname = malloc(128*sizeof(char));
    FILE* f = fopen("/proc/sys/kernel/hostname", "r");
    fgets(hostname,128,f);
    return hostname;
}
char* readCpuName(){
    char* cpuName = malloc(128*sizeof(char));
    FILE* f;
    f = popen("cat /proc/cpuinfo | grep \"model name\"", "r");
    fgets(cpuName,128,f);
    return cpuName;
}
double GetCPULoad() {
    char prevInfo[1024];
    char currInfo[1024];
    double prevValues[11];
    double currValues[11];

    FILE* info = popen("cat /proc/stat","r");
	fgets(prevInfo,1024,info);

    char* token = strtok(prevInfo, " ");
    for(int i=0; token != NULL; i++ ) {
        prevValues[i] = atof(token);  

        token = strtok(NULL, " ");
    }
    
    sleep(1);

    info = popen("cat /proc/stat","r");
    fgets(currInfo,1024,info);
   // printf("%s\n",currInfo);

    char* token2 = strtok(currInfo, " ");
    for(int i=0; token2 != NULL; i++ ) {
        currValues[i] = atof(token2);  
        token2 = strtok(NULL, " ");
    }

    //this code is equivalent to https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    double prevIdle = prevValues[4]+prevValues[5];
    double idle = currValues[4]+currValues[5];
    double prevNonIdle = prevValues[1]+prevValues[2]+prevValues[3]+prevValues[6]+prevValues[7]+prevValues[8];
    double nonIdle = currValues[1]+currValues[2]+currValues[3]+currValues[6]+currValues[7]+currValues[8];

    double prevTotal=prevIdle+prevNonIdle;
    double total = idle+nonIdle;

    double totaf = total - prevTotal;
    double idled = idle - prevIdle;

     double CPU_Percentage = (totaf - idled)/totaf;
     return CPU_Percentage*100;
}
// int getCpuUsage(){
// 	clock();
// }