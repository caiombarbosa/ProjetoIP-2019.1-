#include "server.h"
#include <stdio.h>
#include <string.h>
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


int main() {
  Player jogador[4];
  char str_buffer[bufferTAM], aux_buffer[bufferTAM];
  ALLEGRO_DISPLAY *janela = NULL;
  ALLEGRO_FONT *fonte = NULL;
  ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
  int connected = 0;
  
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


  while (1) {
    ALLEGRO_EVENT evento;
    ALLEGRO_TIMEOUT timeout;
    al_init_timeout(&timeout, 0.05);

    //Essa parte serve para configurar a janela a fechar quando apertar no x vermelho
    int tem_eventos = al_wait_for_event_until(fila_eventos, &evento, &timeout);

    if (tem_eventos && evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
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
      sprintf(str_buffer, "%s-%d: %s", jogador[msg_ret.client_id].login, msg_ret.client_id, aux_buffer);
      broadcast(str_buffer, (int)strlen(str_buffer) + 1);//Envia mensagem para todos os clientes
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

      broadcast(str_buffer, (int)strlen(str_buffer) + 1);
    }
    al_flip_display();
  }
  al_destroy_font(fonte);
  al_destroy_display(janela);
  al_destroy_event_queue(fila_eventos);
  return 0;
}