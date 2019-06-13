#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define msgTAM 350 //MSG_MAX_SIZE
#define bufferTAM (msgTAM + 100)//BUFFER_SIZE
#define loginTAM 13 //LOGIN_MAX_SIZE
#define histTAM 200 //Tamanho do historico de msg
#define lTela 1400 //Largura da tela
#define aTela 800 //Altura da tela
#define tamFonte 36

bool sair = false;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_FONT *fonte = NULL;

 

//-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=- INICIA LIB -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-

void inicializar(){
    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a biblioteca Allegro.\n");
    }
 
    if (!al_install_keyboard()){
        fprintf(stderr, "Falha ao inicializar teclado.\n");
    }
 
    if (!al_init_image_addon()){
        fprintf(stderr, "Falha ao inicializar allegro_image.\n");
    }
 
    al_init_font_addon();
 
    if (!al_init_ttf_addon()){
        fprintf(stderr, "Falha ao inicializar allegro_ttf.\n");
    }
 
    janela = al_create_display(lTela, aTela);
    if (!janela){
        fprintf(stderr, "Falha ao criar a janela.\n");
    }
 
    al_set_window_title(janela, "GAME");//Futuramente sera o nome do jogo
 
    
}






//-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=- LOAD ARQUIVOS -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-




void carregar_arquivos(){
    fundo = al_load_bitmap("fundoChat.jpg");
    if (!fundo){
        fprintf(stderr, "Falha ao carregar \"fundoChat.jpg\".\n");
    }
 
    fonte = al_load_font("pressStart.ttf", tamFonte, 0);
    if (!fonte){
        fprintf(stderr, "Falha ao carregar \"pressStart.ttf\".\n");
    }
}





//-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=- FUNCAO FINALIZA -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-
 
void finalizar(){
    al_destroy_bitmap(fundo);
    al_destroy_font(fonte);
    al_destroy_display(janela);
}









//-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=- FUNCAO DA MENSAGEM -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-



//Parte digitada
void runChat(ALLEGRO_EVENT evento, char *type_buffer) {
  char str_buffer[bufferTAM];
  char msg_Hist[histTAM][msgTAM] = {{0}};
  int type_pointer = 0;
  bool sair = false;
  bool concluido = false;

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






//=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=- MANIPULA ENTRADA =-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=- 

void manipular_entrada(ALLEGRO_EVENT evento, char *str){
  if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
    if (strlen(str) <= 16){
      char temp[] = {evento.keyboard.unichar, '\0'};
        
      if (evento.keyboard.unichar == ' '){
        strcat(str, temp);
      }
      else if (evento.keyboard.unichar >= '0' && evento.keyboard.unichar <= '9'){
        strcat(str, temp);
      }
      else if (evento.keyboard.unichar >= 'A' && evento.keyboard.unichar <= 'Z'){
        strcat(str, temp);
      }
      else if (evento.keyboard.unichar >= 'a' && evento.keyboard.unichar <= 'z'){
        strcat(str, temp);
      }
      else if(evento.keyboard.unichar == '.'){
        strcat(str, temp);
      }
    }
    if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0){//Apaga letra
      str[strlen(str) - 1] = '\0';
    }
  }
}






//=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=- EXIBIR TEXTO -=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-
void exibir_texto_centralizado(char str[30]){
    if (strlen(str) > 0){
        al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela / 2, (aTela - tamFonte) / 2, ALLEGRO_ALIGN_CENTRE, str);
    }
}






//=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=- FUNCAO CONECTAR -=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-

enum conn_ret_t tryConnect() {
  bool concluido = false;
  char server_ip[30];
  ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
  fila_eventos = al_create_event_queue();

  al_register_event_source(fila_eventos, al_get_display_event_source(janela));
  al_register_event_source(fila_eventos, al_get_keyboard_event_source());

  strcpy(server_ip, "");

  while (!sair && !concluido){
        while (!al_is_event_queue_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
 
            if (!concluido){
                manipular_entrada(evento, server_ip);
                
                //Vai salvar o que esta escrito até apertar enter
                if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    concluido = true;
                }
            }
 
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){//Se fechar janela
                sair = true; concluido = true;
            }
        }
 
        al_draw_bitmap(fundo, 0, 0, 0);
 
        if (!concluido){
            al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela / 2,(aTela / 2) / 2, ALLEGRO_ALIGN_CENTRE, "Please enter the server IP: ");
        }
        exibir_texto_centralizado(server_ip);
 
        al_flip_display();
    }
    al_flip_display();
    al_destroy_event_queue(fila_eventos);

    return connectToServer(server_ip);
}






//=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-= FUNCAO CONEXAO -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



void assertConnection() {
  enum conn_ret_t ans = tryConnect();
  bool concluido1 = false;
  bool concluido = false;
  char res[2];
  strcpy(res, "");

  ALLEGRO_EVENT_QUEUE *fila_eventos1 = NULL;
  fila_eventos1 = al_create_event_queue();
  ALLEGRO_EVENT_QUEUE *fila_eventos2 = NULL;
  fila_eventos2 = al_create_event_queue();
  ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
  fila_eventos = al_create_event_queue();

  al_register_event_source(fila_eventos1, al_get_display_event_source(janela));
  al_register_event_source(fila_eventos1, al_get_keyboard_event_source());
  al_register_event_source(fila_eventos2, al_get_display_event_source(janela));
  al_register_event_source(fila_eventos2, al_get_keyboard_event_source());
  al_register_event_source(fila_eventos, al_get_display_event_source(janela));
  al_register_event_source(fila_eventos, al_get_keyboard_event_source());

  

  while(!sair && !concluido){
      while (ans != SERVER_UP && !sair){
        /*while(!concluido && !sair){
          al_draw_bitmap(fundo, 0, 0, 0);
          
          while(!al_is_event_queue_empty(fila_eventos1)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos1, &evento);
    
            if (!concluido1){
              manipular_entrada(evento, res);
                
              //Vai salvar o que esta escrito até apertar enter
              if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                concluido1 = true;
              }
            }

            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){//Se fechar janela
                sair = true; concluido = true;
            }
          }

          if (ans == SERVER_DOWN) {
              al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela/2, aTela/4, ALLEGRO_ALIGN_CENTRE, "Server is down!");
            } 
            
          else if (ans == SERVER_FULL) {
              al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela/2, aTela/4, ALLEGRO_ALIGN_CENTRE, "Server is full!");
            } 
            
          else if (ans == SERVER_CLOSED) {
              al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela/2, aTela/4, ALLEGRO_ALIGN_CENTRE, "Server is closed for new connections!");
            } 
            
          else {
              al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela/2, aTela/4, ALLEGRO_ALIGN_CENTRE, "Server didn't respond to connection!");
            }

          al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela/2, aTela/6, ALLEGRO_ALIGN_CENTRE, "Want to try again? [Y/n] ");            

          exibir_texto_centralizado(res);
  
          al_flip_display();
        

          //Ser der erro
          while (!strcmp(res,"n") && !strcmp(res, "y") && !sair){
            char res[2];
            concluido1 = false;
            printf("entrei 1\n");
            while(!al_is_event_queue_empty(fila_eventos2)){
              ALLEGRO_EVENT evento;
              al_wait_for_event(fila_eventos2, &evento);
              printf("entrei 2\n");
              if (!concluido1){
                manipular_entrada(evento, res);
                      
                //Vai salvar o que esta escrito até apertar enter
                if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                  concluido1 = true;
                }
              }
              if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){//Se fechar janela
                sair = true; concluido = true;
              }
            }
            if (!strcmp(res,"n") && !strcmp(res, "y")){
              al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela / 2,(aTela / 2) / 2, ALLEGRO_ALIGN_CENTRE, "anh???");
            }
            exibir_texto_centralizado(res);
            
            al_flip_display();
            
            if (!strcmp(res,"n")) {
              exit(EXIT_SUCCESS);
              sair = true;
              concluido = true; 
            }
          }
        }*/
        ans = tryConnect();
      }


    //login
    while(!sair && !concluido){
      char login[loginTAM + 4];

      strcpy(login, "");
      printf("Please enter your login (limit = %d): ", loginTAM);

      while (!al_is_event_queue_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
 
            if (!concluido){
                manipular_entrada(evento, login);
                
                //Vai salvar o que esta escrito até apertar enter
                if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    concluido = true;
                }
            }
 
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){//Se fechar janela
                sair = true; concluido = true;
            }
        }
      al_draw_bitmap(fundo, 0, 0, 0);
      
      if (!concluido){
        al_draw_text(fonte, al_map_rgb(255, 255, 255), lTela / 2,(aTela / 2) / 2, ALLEGRO_ALIGN_CENTRE, "Conectado, digite o login(max 13): ");
      }
      exibir_texto_centralizado(login);
      
      al_flip_display();

      int len = (int)strlen(login);
      sendMsgToServer(login, len + 1);    
    }   
  }
  al_flip_display();
  al_destroy_event_queue(fila_eventos1);
  al_destroy_event_queue(fila_eventos2);
  al_destroy_event_queue(fila_eventos);
}


//=-=-=-=-=-=-=-=-=-=-=-=-=- -=-=-=-=-=-=-=-=-=-=-=-=-=-FUNCAO MAIN -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-


int main(){
    bool concluido = false;
    char frase[msgTAM] = " ";
    char str[msgTAM];

    inicializar();
    strcpy(str, "");
    carregar_arquivos();
    assertConnection();
    finalizar();
    return 0;
}