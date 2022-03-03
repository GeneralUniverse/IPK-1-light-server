#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

char* readHostname();
char* readCpuName();
float GetCPULoad();
struct sockaddr_in servad;

int main (int argc, char** argv){
// set up the port
    if(argc != 2){
        printf("Input a correct number of a port.\n");
        return 0;
    }

    int port = atoi(argv[1]);

    servad.sin_family = AF_INET;
    servad.sin_addr.s_addr = htonl(INADDR_ANY);
    servad.sin_port = htons(port);

    int listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    bind(listen, servad , sizeof(servad));

    printf("Name of the hostname: %s\n",readHostname());
    free(readHostname());
    printf("CPU - %s\n",readCpuName());
    free(readCpuName());
   // printf("CPU usage: %d",GetCPULoad());
     printf("\n\n%f %%\n",GetCPULoad());
    //while(1){

   // }
    return 0;
}
char* readHostname(){
    char* hostname = malloc(1024*sizeof(char));
    FILE* f = fopen("/proc/sys/kernel/hostname", "r");
    fscanf(f,"%s",hostname);
    return hostname;
}
char* readCpuName(){
    char* cpuName = malloc(1024*sizeof(char));
    FILE* f;
    f = popen("cat /proc/cpuinfo | grep \"model name\"", "r");
    // fscanf(f,"%s",cpuName);
    fgets(cpuName,1024,f);
    return cpuName;
}
float GetCPULoad() {
    char prevInfo[1024];
    char currInfo[1024];
    float prevValues[10];
    float currValues[10];

    FILE* info = popen("cat /proc/stat","r");
	fgets(prevInfo,1024,info);

    char* token = strtok(prevInfo, " ");
    for(int i=0; token != NULL; i++ ) {
        prevValues[i] = atoi(token);  
        //printf( "%f - ", prevValues[i] );
        token = strtok(NULL, " ");
    }

    sleep(1);
    printf("\n");

    info = popen("cat /proc/stat","r");
    fgets(currInfo,1024,info);

    token = strtok(currInfo, " ");
    for(int i=0; token != NULL; i++ ) {
        currValues[i] = atoi(token);  
        //printf( "%f - ", currValues[i] );
        token = strtok(NULL, " ");
    }
    //this code is equivalent to https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    float prevIdle = prevValues[3]+prevValues[4];
    float idle = currValues[3]+currValues[4];
    float prevNonIdle = prevValues[0]+prevValues[1]+prevValues[2]+prevValues[5]+prevValues[6]+prevValues[7];
    float nonIdle = currValues[0]+currValues[1]+currValues[2]+currValues[5]+currValues[6]+currValues[7];

    float prevTotal=prevIdle+prevNonIdle;
    float total = idle+nonIdle;

    float totaf = total - prevTotal;
    float idled = idle - prevIdle;

    float CPU_Percentage = (totaf - idled)/totaf;

    return CPU_Percentage*100;
}
// int getCpuUsage(){
// 	clock();
// }