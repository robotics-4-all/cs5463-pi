#include "ipc.h"

int connect_socket(int *socket_fd)
{
  int len;
  struct sockaddr_un sock_addr;

  if ((*socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
    perror("socket");
  }

  memset(&sock_addr, 0, sizeof(sock_addr));
  sock_addr.sun_family = AF_UNIX;
  strcpy(sock_addr.sun_path, SOCKET_FILE);

  if (connect(*socket_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
  {
    perror("connect");
    return 0;
  }
  else
  {
    printf("Connected to socket!\n");
    return 1;
  }
}

int socket_send_data(int *socket_fd, char *buff)
{
  if (send(*socket_fd, buff, strlen(buff)+1, 0) == -1) {
    perror("send");
    return 0;
  }
  else
  {
    printf ("Sent Data\n");
    return 1;
  }
}
