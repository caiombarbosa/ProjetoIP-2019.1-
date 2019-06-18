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
    int magia; // para saber se ele lancou uma magia
    char virado; //  variavel para ver para qual lado o personagem esta virado
    int character; // variavel para saber qual personagem tenho q desenhar
    int posxInicial, posyInicial;
} Jogador;

void igualarStruct(){

}


int main() {
  Player jogador[4];
  char str_buffer[bufferTAM], aux_buffer[bufferTAM];
  ALLEGRO_DISPLAY *janela = NULL;
  ALLEGRO_FONT *fonte = NULL;
  ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
  int connected = 0;
  bool jogar = false;
  bool sair = false;
  int id_player[4];
  int p;


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
    while (!jogar && !sair) {
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
          break;
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

        broadcast(str_buffer, (int)strlen(str_buffer) + 1);
      }
      al_flip_display();
    }

    Jogador pessoa[maxPessoasNoChat + 1];

    if(jogar){
      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela / 2, 70, ALLEGRO_ALIGN_CENTRE, "Em jogo");
      al_flip_display();

      pessoa[0].posxInicial = 350;
      pessoa[0].posyInicial = 350;

      pessoa[1].posxInicial = 350;
      pessoa[1].posyInicial = 700;
      
      pessoa[2].posxInicial = 890;
      pessoa[2].posyInicial = 350;
      
      pessoa[3].posxInicial = 890;
      pessoa[3].posyInicial = 700;
    }

    broadcast(pessoa, sizeof(Jogador));

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

        struct msg_ret_t msg_ret = recvMsg(aux);
        pessoa[msg_ret.client_id] = aux;
        for(j = 0; j <= maxPessoasNoChat; j++){
          if(j != msg_ret.client_id){
            if(pessoa[i].magia == 5){
              if(pessoa[i].virado = 'C'){
                rangeMagia = pessoa[i].posY + 580;
                if(pessoa[j].posX <= pessoa[i].posX + 128 && pessoa[j].posX >= pessoa[i].posX){
                  if(pessoa[j].posY <= rangeMagia && pessoa[j].posY >= pessoa[i].posY){
                    pessoa[j].vida--;
                  }
                }
              }
              if(pessoa[i].virado = 'B'){
                rangeMagia = pessoa[i].posY - 580;
                if(pessoa[j].posX <= pessoa[i].posX + 128 && pessoa[j].posX >= pessoa[i].posX){
                  if(pessoa[j].posY <= rangeMagia && pessoa[j].posY >= pessoa[i].posY){
                    pessoa[j].vida--;
                  }
                }
              }
              if(pessoa[i].virado = 'D'){
                rangeMagia = pessoa[i].posX + 580;
                if(pessoa[j].posY <= pessoa[i].posY + 128 && pessoa[j].posY >= pessoa[i].posY){
                  if(pessoa[j].posX <= rangeMagia && pessoa[j].posX >= pessoa[i].posX){
                    pessoa[j].vida--;
                  }
                }
              }
              if(pessoa[i].virado = 'E'){
                rangeMagia = pessoa[i].posX - 580;
                if(pessoa[j].posY <= pessoa[i].posY + 128 && pessoa[j].posY >= pessoa[i].posY){
                  if(pessoa[j].posX <= rangeMagia && pessoa[j].posX >= pessoa[i].posX){
                    pessoa[j].vida--;
                  }
                }
              }
            }
            sendMsgToClient(&pessoa[msg_ret.client_id], msg_ret.size, i);
          }
        }
        if(pessoa[i].vida == 0){
          disconnectClient(i);
        }
      }
      else if (msg_ret.status == DISCONNECT_MSG){
        jogar = false;
        connected=0;
      }
    }
  }
  al_destroy_font(fonte);
  al_destroy_display(janela);
  al_destroy_event_queue(fila_eventos);
  return 0;
}