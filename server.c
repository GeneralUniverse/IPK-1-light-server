#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>

#define DEFAULT_SERVER_PORT 420
#define SA struct sockadrr
char* readHostname();
char* readCpuName();
int main (int argc, char** argv){
// set up the port
    if(argc != 2){
        printf("Input a correct number of a port.\n");
        return 0;
    }
    else{
        int port = atoi(argv[1]);
    }

    printf("Name of the hostname: %s\n",readHostname());
    free(readHostname());
    printf("CPU - %s\n",readCpuName());
    free(readCpuName());
    while(1){
        socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 

    }
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
// int getCpuUsage(){
// 	clock();
// }