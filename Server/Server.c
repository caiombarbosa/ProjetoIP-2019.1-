#include "server.h"
#include <stdio.h>
#include <string.h>

#define msgTAM 350 //MSG_MAX_SIZE
#define bufferTAM(msgTAM + 100) //BUFFER_SIZE
#define loginTAM 13 //LOGIN_MAX_SIZE
#define maxPessoasNoChat 3 //MAX_CHAT_CLIENTS //O id é um num entre 0 e maxPessoas, entao havera 4 pessoas

int main() {
  char nomesClientes[maxPessoasNoChat][loginTAM];
  char str_buffer[bufferTAM], aux_buffer[bufferTAM];

  serverInit(maxPessoasNoChat);//tem que ser chamada uma unica vez sempre para iniciar o server
  puts("Server is running!!");
  while (1) {
    int id = acceptConnection();//Busca por uma conexão nova, caso tenha alguma pendente.
    if (id != NO_CONNECTION) {
      recvMsgFromClient(nomesClientes[id], id, WAIT_FOR_IT);
      strcpy(str_buffer, nomesClientes[id]);
      strcat(str_buffer, " connected to chat");
      broadcast(str_buffer, (int)strlen(str_buffer) + 1);
      printf("%s connected id = %d\n", nomesClientes[id], id);
    }

    struct msg_ret_t msg_ret = recvMsg(aux_buffer);
    if (msg_ret.status == MESSAGE_OK) {
      sprintf(str_buffer, "%s-%d: %s", nomesClientes[msg_ret.client_id],
              msg_ret.client_id, aux_buffer);
      broadcast(str_buffer, (int)strlen(str_buffer) + 1);
    } else if (msg_ret.status == DISCONNECT_MSG) {
      sprintf(str_buffer, "%s disconnected", nomesClientes[msg_ret.client_id]);
      printf("%s disconnected, id = %d is free\n",
             nomesClientes[msg_ret.client_id], msg_ret.client_id);
      broadcast(str_buffer, (int)strlen(str_buffer) + 1);
    }
  }
}