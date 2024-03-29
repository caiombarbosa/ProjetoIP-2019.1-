#include "server.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h> 

#define msgTAM 350 //MSG_MAX_SIZE
#define bufferTAM (msgTAM + 100) //BUFFER_SIZE
#define loginTAM 13 //LOGIN_MAX_SIZE
#define maxPessoasNoChat 3 //MAX_CHAT_CLIENTS //O id é um num entre 0 e maxPessoas, entao havera 4 pessoas
#define lTela 480 //Largura da tela
#define aTela 300 //Altura da tela


typedef struct{
  char login[loginTAM];
  int id;
}Player;

typedef struct {
    int vida; // o personagem podera ser atacado 2 vezes, na 3a ele morre
    int posX, posY;
    int velX, velY;
    int magia; // para saber se ele lancou uma magia
    char virado; //  variavel para ver para qual lado o personagem esta virado
    int character; // variavel para saber qual personagem tenho q desenhar
    int posxInicial, posyInicial;
} Jogador;



int main() {
  Player jogador[4];
  char str_buffer[bufferTAM], aux_buffer[bufferTAM];
  ALLEGRO_DISPLAY *janela = NULL;
  ALLEGRO_FONT *fonte = NULL;
  ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
  int connected = 0;
  bool jogar = false;
  int auxX = 0, auxY = 0;
  bool sair = false;
  bool naoMsg = false;
  char saia = 'p';

  //Inicia o Servidor
  serverInit(maxPessoasNoChat);//tem que ser chamada uma unica vez sempre para iniciar o server
  puts("Server is running!!");

  //Inicia o allegro
  al_init();
  
  fila_eventos = al_create_event_queue();
  janela = al_create_display(lTela, aTela);
  
  //Iniciando a fonte
  al_init_font_addon();
  al_init_ttf_addon();
  fonte = al_load_font("pressStart.ttf", 40, 0);


  //=-=-=-=-=-=-=-=-=-=-  LOOP PARA IDENTIFICAR PLAYERS =-=-=-=-=-=-=-=-=-=-=-==-//

  while (!sair){
    bool sairChat = false;
    while (!jogar && !sair) {
      int i;
      ALLEGRO_EVENT evento;
      ALLEGRO_TIMEOUT timeout;
      al_init_timeout(&timeout, 0.05);

      //Essa parte serve para configurar a janela a fechar quando apertar no x vermelho
      int tem_eventos = al_wait_for_event_until(fila_eventos, &evento, &timeout);

      if (tem_eventos && evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        sair = true;
        break;
      }//Se apertar o x vermelho, a janela fecha

      int id = acceptConnection();//Busca por uma conexão nova, caso tenha alguma pendente.

      if (id == 0){
        al_clear_to_color(al_map_rgb(0, 0, 0)); //limpa a mensagem inicial
      }

      if (id != NO_CONNECTION) {
        connected++;
        recvMsgFromClient(jogador[id].login, id, WAIT_FOR_IT);
        strcpy(str_buffer, jogador[id].login);
        strcat(str_buffer, " connected to chat");
        broadcast(str_buffer, (int)strlen(str_buffer) + 1);
        printf("%s connected id = %d\n", jogador[id].login, id);
        jogador[id].id = id;
        //Atualiza a mensagem na janela:
        al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela / 2, 40 + id*65, ALLEGRO_ALIGN_CENTRE, jogador[id].login);
      }
      else if(connected == 0){
        // Preenchemos a tela com a cor preta
        al_clear_to_color(al_map_rgb(0, 0, 0)); 
      
        // Texto centralizado
        al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela / 2, 70, ALLEGRO_ALIGN_CENTRE, "Esperando");
        al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela / 2, 130, ALLEGRO_ALIGN_CENTRE, "jogadores");
        al_register_event_source(fila_eventos, al_get_display_event_source(janela));
        al_flip_display();
      }

      //Essa parte serve para enviar no chat
      struct msg_ret_t msg_ret = recvMsg(aux_buffer);
      if (msg_ret.status == MESSAGE_OK) {
        if(!strcmp(aux_buffer, "65561")){
          jogar = true;
          for(i=0; i<= maxPessoasNoChat; i++){
            if(i != msg_ret.client_id){
              sendMsgToClient(aux_buffer, (int)strlen(aux_buffer) + 1, i);
            }
          }
          naoMsg = true;
        }
        else{
          sprintf(str_buffer, "%s-%d: %s", jogador[msg_ret.client_id].login, msg_ret.client_id, aux_buffer);
          broadcast(str_buffer, (int)strlen(str_buffer) + 1);//Envia mensagem para todos os clientes
        }
      } 
      else if (msg_ret.status == DISCONNECT_MSG) {
        sprintf(str_buffer, "%s disconnected", jogador[msg_ret.client_id].login);
        printf("%s disconnected, id = %d is free\n", jogador[msg_ret.client_id].login, msg_ret.client_id);
        connected--;
        //Atualizando a janela
        al_clear_to_color(al_map_rgb(0, 0, 0));
        int i, aux=0;
        for(i=0; i <= maxPessoasNoChat; i++){
          if(jogador[i].id == msg_ret.client_id){
            i++;
          }
          else if(i < 4){
            al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela / 2, 40 + aux*65, ALLEGRO_ALIGN_CENTRE, jogador[i].login);
            aux++;
          }
        }
        if(!naoMsg){
          broadcast(str_buffer, (int)strlen(str_buffer) + 1);
        }
      }
      al_flip_display();
    }
    naoMsg = false;
    int i;

    Jogador pessoa[maxPessoasNoChat + 1];
    printf("connected %d\n\n", connected);
    broadcast(&connected, sizeof(int));
    
    if(jogar){
      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela / 2, 70, ALLEGRO_ALIGN_CENTRE, "Em jogo");
      al_flip_display();


      for (i = 0; i<4; i++){
        if(i == 0){
            auxX = 350;
            auxY = 350;
          }
          if(i == 1){
            auxX = 500;
            auxY = 500;
          }
          if(i == 2){
            auxX = 890;
            auxY = 350;
          }
          if(i == 3){
            auxX = 890;
            auxY = 500;
          }
          pessoa[i].posxInicial = auxX;
          pessoa[i].posyInicial = auxY;
          pessoa[i].character = 1;
          pessoa[i].posX = auxX;
          pessoa[i].posY = auxY;
          pessoa[i].velX = 0;
          pessoa[i].velY = 0;
          pessoa[i].magia = 0;
          pessoa[i].virado = 'D';
          pessoa[i].vida = 3;
      }
    }
    for(i = 0; i < connected; i++){
      printf("mandei id %d\n\n", i);
      sendMsgToClient(&i, sizeof(int), i);
    }
    broadcast(pessoa, 4*sizeof(Jogador));

    //=-=-=-=-=-=-=-=-=-=-=-=-=-  LOOP PARA ATUALIZAR STATUS DOS PLAYERS =-=-=-=-=-=-=-=-=-=-=-=-=-//
    int j, k=0;
    int morreram[4];
    int rangeMagia;
    int m;
    Jogador aux;
    while (jogar && !sair){
      ALLEGRO_EVENT evento;
      ALLEGRO_TIMEOUT timeout;
      al_init_timeout(&timeout, 0.05);

      //Essa parte serve para configurar a janela a fechar quando apertar no x vermelho
      int tem_eventos = al_wait_for_event_until(fila_eventos, &evento, &timeout);

      if (tem_eventos && evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        sair = true;
        break;
      }//Se apertar o x vermelho, a janela fecha

      //Recebendo e enviando o status dos jogadores
      //O dano é levado a 528 pixels
      rangeMagia = 0;
      
      for(i=0; i<connected; i++){
        struct msg_ret_t msg_ret = recvMsgFromClient(&pessoa[i], i, WAIT_FOR_IT);
        if(msg_ret.status != DISCONNECT_MSG){
          for(j = 0; j < connected; j++){
            if(j != msg_ret.client_id){
              if(pessoa[msg_ret.client_id].magia == 5){
                if(pessoa[msg_ret.client_id].virado = 'C'){
                  rangeMagia = pessoa[msg_ret.client_id].posY + 580;
                  if(pessoa[j].posX <= pessoa[msg_ret.client_id].posX + 128 && pessoa[j].posX >= pessoa[msg_ret.client_id].posX){
                    if(pessoa[j].posY <= rangeMagia && pessoa[j].posY >= pessoa[msg_ret.client_id].posY){
                      pessoa[j].vida--;
                    }
                  }
                }
                if(pessoa[msg_ret.client_id].virado = 'B'){
                  rangeMagia = pessoa[i].posY - 580;
                  if(pessoa[j].posX <= pessoa[msg_ret.client_id].posX + 128 && pessoa[j].posX >= pessoa[msg_ret.client_id].posX){
                    if(pessoa[j].posY <= rangeMagia && pessoa[j].posY >= pessoa[msg_ret.client_id].posY){
                      pessoa[j].vida--;
                    }
                  }
                }
                if(pessoa[msg_ret.client_id].virado = 'D'){
                  rangeMagia = pessoa[msg_ret.client_id].posX + 580;
                  if(pessoa[j].posY <= pessoa[msg_ret.client_id].posY + 128 && pessoa[j].posY >= pessoa[msg_ret.client_id].posY){
                    if(pessoa[j].posX <= rangeMagia && pessoa[j].posX >= pessoa[msg_ret.client_id].posX){
                      pessoa[j].vida--;
                    }
                  }
                }
                if(pessoa[msg_ret.client_id].virado = 'E'){
                  rangeMagia = pessoa[msg_ret.client_id].posX - 580;
                  if(pessoa[j].posY <= pessoa[msg_ret.client_id].posY + 128 && pessoa[j].posY >= pessoa[msg_ret.client_id].posY){
                    if(pessoa[j].posX <= rangeMagia && pessoa[j].posX >= pessoa[msg_ret.client_id].posX){
                      pessoa[j].vida--;
                    }
                  }
                }
              }
            }
          }
          if(pessoa[j].vida == 0){
            disconnectClient(j);
          }
        }
        else if (msg_ret.status == DISCONNECT_MSG){
          jogar = false;
          connected=0;
        }
      }
      broadcast(pessoa, 4*sizeof(Jogador));
      for(i=0; i<connected; i++){
        printf("Mandei posX = %d e posY = %d para %d\n\n", pessoa[i].posX, pessoa[i].posY, i);
      }
    }
    naoMsg = false;
  }
  al_destroy_font(fonte);
  al_destroy_display(janela);
  al_destroy_event_queue(fila_eventos);
  return 0;
}