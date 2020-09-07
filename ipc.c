#include "ipc.h"

int connect_socket(int *socket_fd)
{
  int len;
  struct sockaddr_un sock_addr;
  int enable = 1;

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    error("setsockopt(SO_REUSEADDR) failed");

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

char *make_json(
  float active,
  float reactive,
  float powerl1,
  float current,
  float voltage,
  float phaseanglecurrentvoltagel1
  )
{
  char *string = NULL;
  cJSON *root  = cJSON_CreateObject();

  cJSON_AddItemToObject(
    root,
    "active",
    cJSON_CreateNumber(active)
    );
  cJSON_AddItemToObject(
    root,
    "reactive",
    cJSON_CreateNumber(active)
    );
  cJSON_AddItemToObject(
    root,
    "powerl1",
    cJSON_CreateNumber(powerl1)
    );
  cJSON_AddItemToObject(
    root,
    "current",
    cJSON_CreateNumber(current)
    );
  cJSON_AddItemToObject(
    root,
    "voltage",
    cJSON_CreateNumber(voltage)
    );
  cJSON_AddItemToObject(
    root,
    "phaseanglecurrentvoltagel1",
    cJSON_CreateNumber(phaseanglecurrentvoltagel1)
    );

  string = cJSON_Print(root);
  if (string == NULL) {
    fprintf(stderr, "Failed to print monitor.\n");
  }
   /* free all objects under root and root itself */
   cJSON_Delete(root);
   return string;
}
