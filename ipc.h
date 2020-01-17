#ifndef IPC_H
#define IPC_H

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "ipc.h"

#define SOCKET_FILE "/tmp/cs5463.sock"
#define HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
#define PORT = 65432        # Port to listen on (non-privileged ports are > 1023)

int connect_socket(int *socket_fd);
int socket_send_data(int *socket_fd, char *buff);

#endif
