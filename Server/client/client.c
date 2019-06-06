#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define msgTAM 350 //MSG_MAX_SIZE
#define bufferTAM (msgTAM + 100)//BUFFER_SIZE
#define loginTAM 13 //LOGIN_MAX_SIZE
#define histTAM 200 //Tamanho do historico de msg

enum conn_ret_t tryConnect() {
  char server_ip[30];
  printf("Please enter the server IP: ");
  scanf(" %s", server_ip);
  getchar();
  return connectToServer(server_ip);
}

void printHello() {
  puts("Hello! Welcome to simple chat example.");
  puts("We need some infos to start up!");
}

void assertConnection() {
  printHello();
  enum conn_ret_t ans = tryConnect();
  while (ans != SERVER_UP) {
    if (ans == SERVER_DOWN) {
      puts("Server is down!");
    } else if (ans == SERVER_FULL) {
      puts("Server is full!");
    } else if (ans == SERVER_CLOSED) {
      puts("Server is closed for new connections!");
    } else {
      puts("Server didn't respond to connection!");
    }
    printf("Want to try again? [Y/n] ");
    int res;
    while (res = tolower(getchar()), res != 'n' && res != 'y' && res != '\n'){
      puts("anh???");
    }
    if (res == 'n') {
      exit(EXIT_SUCCESS);
    }
    ans = tryConnect();
  }
  char login[loginTAM + 4];
  printf("Please enter your login (limit = %d): ", loginTAM);
  scanf(" %[^\n]", login);
  getchar();
  int len = (int)strlen(login);
  sendMsgToServer(login, len + 1);
}

void runChat() {
  char str_buffer[bufferTAM], type_buffer[msgTAM] = {0};
  char msg_Hist[histTAM][msgTAM] = {{0}};
  int type_pointer = 0;
  
  while (1) {
    // LER UMA TECLA DIGITADA
    char ch = getch();
    if (ch == '\n') {
      type_buffer[type_pointer++] = '\0';
      int ret = sendMsgToServer((void *)type_buffer, type_pointer);
      if (ret == SERVER_DISCONNECTED) {
        return;
      }
      type_pointer = 0;
      type_buffer[type_pointer] = '\0';
    } else if (ch == 127 || ch == 8) { //127 = del; 8 = space
      if (type_pointer > 0) {
        --type_pointer;
        type_buffer[type_pointer] = '\0';
      }
    } else if (ch != NO_KEY_PRESSED && type_pointer + 1 < msgTAM) {
      type_buffer[type_pointer++] = ch;
      type_buffer[type_pointer] = '\0';
      
    }

    // LER UMA MENSAGEM DO SERVIDOR
    int ret = recvMsgFromServer(str_buffer, DONT_WAIT);
    if (ret == SERVER_DISCONNECTED) {
      return;
    } else if (ret != NO_MESSAGE) {
      int i;
      for (i = 0; i < histTAM - 1; ++i) {
        strcpy(msg_Hist[i], msg_Hist[i + 1]);
      }
      strcpy(msg_Hist[histTAM - 1], str_buffer);
    }

    // PRINTAR NOVO ESTADO DO CHAT
    system("clear");
    int i;
    for (i = 0; i < histTAM; ++i) {
      printf("%s\n", msg_Hist[i]);
    }
    printf("\nYour message: %s\n", type_buffer);
  }
}

int main() {
  assertConnection();

  puts("Welcome to the chat example");
  puts("Just type your messages e talk to your friends");
  puts("Press [Enter] to continue");
  getchar();
  while(1) {
    runChat();
    printf("Server disconnected! Want to try again? [Y/N] ");
    int res;
    while (res = tolower(getchar()), res != 'n' && res != 'y' && res != '\n'){
      puts("anh???");
    }
    if (res == 'y' || res == '\n') {
      assertConnection();
    } else {
      break;
    }
  }
  return 0;
}
