# Server in C
* Simple and lightweight server written in C language.
---
## Basic functions
* `cpu-name` - Return the **CPU name** from the server
* `hostname` - Return the **server name**
* `load` - Return the **CPU load**
* `./hinfosvc 12345` - **Adjustable port**
---
## Usage
1. Compile the server using Makefile
    `make`
2. Start the server and chose the port
    `./hinfosvc 12345`
3. Use curl or browser to enter the server and chose response you want to get\
    `curl http://localhost:12345/hostname`\
    `curl http://localhost:12345/cpu-name`\
    `curl http://localhost:12345/load`
4. Close the server by pressing **CTRL + C**
---
## Examples
* Getting the CPU-name from the server on port 8080:\
`$ ./hinfosvc 8080`\
`$ curl http://localhost:8080/cpu-name`\
`$ Intel(R) Core(TM) i3-7130U CPU @ 2.70GHz`
* Getting the CPU load from the server on port 456:\
`$ ./hinfosvc 456`\
`$ curl http://localhost:456/load`\
`$ 12.45687%`