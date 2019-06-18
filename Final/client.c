#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

#define msgTAM 350 //MSG_MAX_SIZE
#define bufferTAM (msgTAM + 100)//BUFFER_SIZE
#define loginTAM 13 //LOGIN_MAX_SIZE
#define histTAM 30 //Tamanho do historico de msg
#define LARGURA_TELA 1240 //Largura da tela
#define ALTURA_TELA 800 //Altura da tela
#define lTela 1240
#define aTela 800
#define largura_mapa 5000
#define altura_mapa 2500
#define tamFonte1 36
#define tamFonte2 18
#define FPS 60.0




				//--* VARIAVES GLOBAIS --*//	

bool sair = false;//fechar janela
ALLEGRO_BITMAP *fundoChat = NULL;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_FONT *fonte1 = NULL;
ALLEGRO_FONT *fonte2 = NULL;

// variaveis que vou usar para ciclar entre os sprites de movimentacao dos personagens
int andandoDireita[] = {};
int andandoEsquerda[] = {};
int andandoCima[] = {};
int andandoBaixo[] = {};

//largura e altura de cada sprite dentro da folha de personagens
int altura_sprite = 98, largura_sprite = 90;

typedef struct {
    int vida; // o personagem podera ser atacado 2 vezes, na 3a ele morre
    int posX, posY;
    int magia; // para saber se ele lancou uma magia
    char virado; //  variavel para ver para qual lado o personagem esta virado    
    int character; // variavel para saber qual personagem tenho q desenhar
} Jogador;


ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_TIMER *timer = NULL;

//////////////////*-- PERSONAGENS --*////////////////////////////////////

ALLEGRO_BITMAP *folha_sprite_Caio_White = NULL;
ALLEGRO_BITMAP *folha_sprite_Caio_White_Batalha = NULL;
ALLEGRO_BITMAP *folha_sprite_Caio_Black = NULL;
ALLEGRO_BITMAP *folha_sprite_Caio_Black_Batalha = NULL;
ALLEGRO_BITMAP *folha_sprite_Eric = NULL;
ALLEGRO_BITMAP *folha_sprite_Eric_Batalha = NULL;
ALLEGRO_BITMAP *folha_sprite_Bianca = NULL;
ALLEGRO_BITMAP *folha_sprite_Bianca_Batalha = NULL;
ALLEGRO_BITMAP *folha_sprite_Juan = NULL;
ALLEGRO_BITMAP *folha_sprite_Juan_Batalha = NULL;
ALLEGRO_BITMAP *folha_sprite_Alisson = NULL;
ALLEGRO_BITMAP *folha_sprite_Alisson_Batalha = NULL;
ALLEGRO_BITMAP *folha_sprite_Lael = NULL;
ALLEGRO_BITMAP *folha_sprite_Lael_Batalha = NULL;
ALLEGRO_BITMAP *folha_sprite_Calabria = NULL;
ALLEGRO_BITMAP *folha_sprite_Calabria_Batalha = NULL;
ALLEGRO_BITMAP *fundo = NULL;

//////////////////*-- MAGIAS --*////////////////////////////////////////////

ALLEGRO_BITMAP *magia_Fire = NULL;
ALLEGRO_BITMAP *magia_Gelo = NULL;
ALLEGRO_BITMAP *magia_Thunder = NULL;
ALLEGRO_BITMAP *magia_Lazer = NULL;
ALLEGRO_BITMAP *magia_Blue_Fire = NULL;
ALLEGRO_BITMAP *magia_Blue_Energy_Ball = NULL;
ALLEGRO_BITMAP *magia_explosionPurple = NULL;
ALLEGRO_BITMAP *magia_green_Energy_Ball = NULL;
ALLEGRO_BITMAP *magia_lightningRadial = NULL;
ALLEGRO_BITMAP *magia_red_Energy_Ball = NULL;
ALLEGRO_BITMAP *magia_yellow_Energy_Ball = NULL;
ALLEGRO_BITMAP *magia_ice_splash = NULL;
ALLEGRO_BITMAP *magia_splash = NULL;
ALLEGRO_BITMAP *magia_teleport = NULL;





void error_msg(char *text){
    	al_show_native_message_box(janela,"ERRO",
    		"Ocorreu o seguinte erro e o programa sera finalizado:",
    		text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}


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

    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        error_msg("Falha ao criar temporizador");
    }

    if(!al_init_font_addon()){
        error_msg("Falha ao incializar o font addon!");
    }
 
    if (!al_init_ttf_addon()){
        fprintf(stderr, "Falha ao inicializar allegro_ttf.\n");
    }
 
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        fprintf(stderr, "Falha ao criar a janela.\n");
    }

    fila_eventos = al_create_event_queue();
        if(!fila_eventos) {
            error_msg("Falha ao criar fila de eventos");
            al_destroy_timer(timer);
            al_destroy_display(janela);
        }
 
    al_set_window_title(janela, "WIZARD KING");
 
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_start_timer(timer);
}



void desenhaSpritePersonagem(char magiaPos, int pos_x_sprite, int pos_y_sprite){
    	if(magiaPos == 'C'){
            if(andandoCima[0] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
                    largura_sprite*0,  (altura_sprite)*3, // posicao para tirar da folha dos sprites
                    largura_sprite,(altura_sprite)*3, // tamanho para tirar
                    pos_x_sprite,pos_y_sprite,0); //  onde colocar a imagem, flags
            }
            if(andandoCima[1] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
                  	largura_sprite*1,(altura_sprite)*3,
                   	largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
            }
          	if(andandoCima[2] == 1){
               	al_draw_bitmap_region(folha_sprite_Caio_Black,
          		   	largura_sprite*2,(altura_sprite)*3,
                    largura_sprite,altura_sprite,
                   	pos_x_sprite,pos_y_sprite,0);
            }
        }
        else if(magiaPos == 'B'){
            if(andandoBaixo[0] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
                    largura_sprite*0,altura_sprite*0,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
            }
        	if(andandoBaixo[1] == 1){
             	al_draw_bitmap_region(folha_sprite_Caio_Black,
                    largura_sprite*1,0,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
           	}
            if(andandoBaixo[2] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
             	    largura_sprite*2,0,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
            }
        }
        else if(magiaPos == 'D'){
          	if(andandoDireita[0] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
                    0,altura_sprite*2,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
        	}
        	if(andandoDireita[1] == 1){
               	al_draw_bitmap_region(folha_sprite_Caio_Black,
                    largura_sprite*1,altura_sprite*2,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
       		}
            if(andandoDireita[2] == 1){
               	al_draw_bitmap_region(folha_sprite_Caio_Black,
                    largura_sprite*2,altura_sprite*2,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
            }
       	}
        else if(magiaPos == 'E'){
            if(andandoEsquerda[0] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
                    0 ,altura_sprite*1,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
        	}
            if(andandoEsquerda[1] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
                   	largura_sprite*1 ,altura_sprite*1,
                    largura_sprite,altura_sprite,
                	pos_x_sprite,pos_y_sprite,0);
           	}
            if(andandoEsquerda[2] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
                    largura_sprite*2 ,altura_sprite*1,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
            }
        }
    }





//-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=- LOAD ARQUIVOS -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-




void carregar_arquivos(){
    fundoChat = al_load_bitmap("fundoChat.jpg");
    if (!fundoChat){
        fprintf(stderr, "Falha ao carregar \"fundoChat.jpg\".\n");
        sair = true;
    }
 
    fonte1 = al_load_font("pressStart.ttf", tamFonte1, 0);
    if (!fonte1){
        fprintf(stderr, "Falha ao carregar \"pressStart.ttf\".\n");
        sair = true;
    }
    fonte2 = al_load_font("pressStart.ttf", tamFonte2, 0);
    if (!fonte2){
        fprintf(stderr, "Falha ao carregar \"pressStart.ttf\".\n");
        sair = true;
    }

    fundo = al_load_bitmap("mapa1-1.png");
    if (!fundo){
        error_msg("Falha ao carregar fundo");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_bitmap(folha_sprite_Alisson);
        al_destroy_bitmap(folha_sprite_Alisson_Batalha);
        al_destroy_bitmap(folha_sprite_Bianca);
        al_destroy_bitmap(folha_sprite_Bianca_Batalha);
        al_destroy_bitmap(folha_sprite_Caio_Black);
        al_destroy_bitmap(folha_sprite_Caio_Black_Batalha);
        al_destroy_bitmap(folha_sprite_Caio_White);
        al_destroy_bitmap(folha_sprite_Caio_White_Batalha);
        al_destroy_bitmap(folha_sprite_Calabria);
        al_destroy_bitmap(folha_sprite_Calabria_Batalha);
        al_destroy_bitmap(folha_sprite_Eric);
        al_destroy_bitmap(folha_sprite_Eric_Batalha);
        al_destroy_bitmap(magia_Fire);
        sair = true;
    }
    ///////////////////////////////// *-- SṔRITES DE PERSONAGENS --* /////////////////////////////////////////////////////


    folha_sprite_Caio_White = al_load_bitmap("caioandandoWhite.png");
    if (!folha_sprite_Caio_White){
        error_msg("Falha ao carregar sprites 1");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }
    folha_sprite_Caio_White_Batalha = al_load_bitmap("caioemposedebatalhaWhite.png");
    if (!folha_sprite_Caio_White_Batalha){
        error_msg("Falha ao carregar sprites 2");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Caio_Black = al_load_bitmap("caioandandoBlack.png"); // personagem = 1
    if (!folha_sprite_Caio_Black){
        error_msg("Falha ao carregar sprites 3");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Caio_Black_Batalha = al_load_bitmap("caioemposedebatalhaBlack.png"); 
    if (!folha_sprite_Caio_Black_Batalha){
        error_msg("Falha ao carregar sprites 4");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Juan = al_load_bitmap("juanandando.png"); // pesonagem = 2
    if (!folha_sprite_Juan){
        error_msg("Falha ao carregar sprites 5");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Juan_Batalha = al_load_bitmap("juanemposedebatalha.png");
    if (!folha_sprite_Juan_Batalha){
        error_msg("Falha ao carregar sprites 6");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Bianca = al_load_bitmap("biancaandando.png"); // personagem = 3
    if (!folha_sprite_Bianca){
        error_msg("Falha ao carregar sprites 7");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Bianca_Batalha = al_load_bitmap("biancaemposedebatalha.png");
    if (!folha_sprite_Bianca_Batalha){
        error_msg("Falha ao carregar sprites 8");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Alisson = al_load_bitmap("alissonandando.png"); // personagem = 4
    if (!folha_sprite_Alisson){
        error_msg("Falha ao carregar sprites 9");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Alisson_Batalha = al_load_bitmap("alissonemposedebatalha.png");
    if (!folha_sprite_Alisson_Batalha){
        error_msg("Falha ao carregar sprites 10");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Calabria = al_load_bitmap("calabriaandando.png"); // personagem = 5
    if (!folha_sprite_Calabria){
        error_msg("Falha ao carregar sprites 11");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Calabria_Batalha = al_load_bitmap("calabriaemposedebatalha.png");
    if (!folha_sprite_Calabria_Batalha){
        error_msg("Falha ao carregar sprites 12");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Eric = al_load_bitmap("ericandando.png"); // personagem  = 6
    if (!folha_sprite_Eric){
        error_msg("Falha ao carregar sprites 13");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Eric_Batalha = al_load_bitmap("ericemposedebatalha.png");
    if (!folha_sprite_Eric_Batalha){
        error_msg("Falha ao carregar sprites 14");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Lael = al_load_bitmap("laelandando.png"); // personagem = 7
    if (!folha_sprite_Lael){
        error_msg("Falha ao carregar sprites 15");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    folha_sprite_Lael_Batalha = al_load_bitmap("laelemposedebatalha.png");
    if (!folha_sprite_Lael_Batalha){
        error_msg("Falha ao carregar sprites 16");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }


    //////////////// *-- SPRITES DE MAGIA --*/////////////////////////////////////////////////////////////////


    magia_Fire = al_load_bitmap("Burn.png");
    if (!magia_Fire){
        error_msg("Falha ao carregar sprite da magia 1");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }
    magia_Gelo = al_load_bitmap("iceBurst.png");
    if(!magia_Gelo)
    {
        error_msg("Falha ao carregar sprite da magia 2");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }
    magia_Thunder = al_load_bitmap("Thunder.png");
    if (!magia_Thunder){
        error_msg("Falha ao carregar sprite da magia 3");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }
    magia_Lazer = al_load_bitmap("Lazer.png");
    if (!magia_Lazer){
        error_msg("Falha ao carregar sprite da magia 4");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    magia_Blue_Energy_Ball = al_load_bitmap("blueEnergyBall.png");
    if (!magia_Blue_Energy_Ball){
        error_msg("Falha ao carregar sprite da magia 5");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    magia_Blue_Fire = al_load_bitmap("blueFlame.png");
    if (!magia_Blue_Fire){
        error_msg("Falha ao carregar sprite da magia 6");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    magia_explosionPurple = al_load_bitmap("explosionPurple.png");
    if (!magia_explosionPurple){
        error_msg("Falha ao carregar sprite da magia 7");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    magia_green_Energy_Ball = al_load_bitmap("greenEnergyBall.png");
    if (!magia_green_Energy_Ball){
        error_msg("Falha ao carregar sprite da magia 8");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    magia_ice_splash = al_load_bitmap("iceSplash.png");
    if (!magia_ice_splash){
        error_msg("Falha ao carregar sprite da magia 9");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    magia_lightningRadial = al_load_bitmap("lightning_radial.png");
    if (!magia_lightningRadial){
        error_msg("Falha ao carregar sprite da magia 10");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    magia_red_Energy_Ball = al_load_bitmap("redEnergyBall.png");
    if (!magia_red_Energy_Ball){
        error_msg("Falha ao carregar sprite da magia 11");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    magia_splash = al_load_bitmap("Splash.png");
    if (!magia_splash){
        error_msg("Falha ao carregar sprite da magia 12");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    magia_teleport = al_load_bitmap("teleportLightning.png");
    if (!magia_teleport){
        error_msg("Falha ao carregar sprite da magia 13");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }

    magia_yellow_Energy_Ball = al_load_bitmap("yellowEnergyBall.png");
    if (!magia_yellow_Energy_Ball){
        error_msg("Falha ao carregar sprite da magia 14");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        sair = true;
    }
}





//-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=- FUNCAO FINALIZA -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-
 
void finalizar(){
    al_destroy_bitmap(fundoChat);
    al_destroy_font(fonte1);
    al_destroy_font(fonte2);
    al_destroy_display(janela);
    al_destroy_bitmap(fundo);
    al_destroy_bitmap(folha_sprite_Caio_Black);
    al_destroy_bitmap(fundo);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
}








//=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=- MANIPULA ENTRADA =-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=- 

void manipular_entrada(ALLEGRO_EVENT evento, char *str, int count){
  if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
    if (strlen(str) <= count){
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
        al_draw_text(fonte1, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, (ALTURA_TELA - tamFonte1) / 2, ALLEGRO_ALIGN_CENTRE, str);
    }
}






//=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=- FUNCAO CONECTAR -=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-

enum conn_ret_t tryConnect() {
  bool concluido = false;
  char server_ip[30];

  ALLEGRO_EVENT_QUEUE *fila_eventos1 = NULL;
  fila_eventos1 = al_create_event_queue();

  al_register_event_source(fila_eventos1, al_get_display_event_source(janela));
  al_register_event_source(fila_eventos1, al_get_keyboard_event_source());

  strcpy(server_ip, "");

  while (!sair && !concluido){
    while (!al_is_event_queue_empty(fila_eventos1)){
      ALLEGRO_EVENT evento;
      al_wait_for_event(fila_eventos1, &evento);

      if (!concluido){
          manipular_entrada(evento, server_ip, 30);
          
          //Vai salvar o que esta escrito até apertar enter
          if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
              concluido = true;
          }
      }

      if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){//Se fechar janela
          sair = true; concluido = true;
      }
    }

    al_draw_bitmap(fundoChat, 0, 0, 0);

    if (!concluido){
        al_draw_text(fonte1, al_map_rgb(255, 255, 255), LARGURA_TELA / 2,(ALTURA_TELA / 2) / 2, ALLEGRO_ALIGN_CENTRE, "Please enter the server IP: ");
    }
    
    exibir_texto_centralizado(server_ip);

    al_flip_display();
    }
    al_flip_display();
    al_destroy_event_queue(fila_eventos1);

    return connectToServer(server_ip);
}






//=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-= FUNCAO CONEXAO -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



void assertConnection() {
  enum conn_ret_t ans = tryConnect();
  bool concluido1 = false;
  bool concluido = false;
  char res[2];
  char login[loginTAM + 4];
  
  strcpy(res, "");
  strcpy(login, "");

  ALLEGRO_EVENT_QUEUE *fila_eventos1 = NULL;
  fila_eventos1 = al_create_event_queue();
  ALLEGRO_EVENT_QUEUE *fila_eventos2 = NULL;
  fila_eventos2 = al_create_event_queue();

  al_register_event_source(fila_eventos1, al_get_display_event_source(janela));
  al_register_event_source(fila_eventos1, al_get_keyboard_event_source());
  al_register_event_source(fila_eventos2, al_get_display_event_source(janela));
  al_register_event_source(fila_eventos2, al_get_keyboard_event_source());

  

  while (ans != SERVER_UP && !sair){
        ans = tryConnect();
  }

    //login
  while(!sair && !concluido){
    while (!al_is_event_queue_empty(fila_eventos1)){
          ALLEGRO_EVENT evento;
          al_wait_for_event(fila_eventos1, &evento);

          if (!concluido){
              manipular_entrada(evento, login, loginTAM);
              
              //Vai salvar o que esta escrito até apertar enter
              if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                  concluido = true;
              }
          }

          if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){//Se fechar janela
              sair = true; concluido = true;
          }
      }
    al_draw_bitmap(fundoChat, 0, 0, 0);
    
    if (!concluido){
      al_draw_text(fonte1, al_map_rgb(255, 255, 255), LARGURA_TELA / 2 + 15, ALTURA_TELA/4, ALLEGRO_ALIGN_CENTRE, "Conectado, digite o login(max 13): ");
    }
    exibir_texto_centralizado(login);
    
    al_flip_display();         
  }

  int len = (int)strlen(login);
  sendMsgToServer(login, len + 1);    
  
  al_flip_display();
  al_destroy_event_queue(fila_eventos1);
  al_destroy_event_queue(fila_eventos2);
}




//-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=- FUNCAO PARA PRINTAR HISTORICO DE MENSAGENS-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-

void printHistorico(char *hist, int posicao){
    posicao++;
    al_draw_text(fonte2, al_map_rgb(255, 255, 255), 5, posicao*(tamFonte2+5), ALLEGRO_ALIGN_LEFT, hist);        
}





//-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=- FUNCAO DA MENSAGEM -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-



//Parte digitada
void runChat() {
  char msgServer[bufferTAM], msg[msgTAM] = {0};
  char msgHist[histTAM][msgTAM] = {{0}};//Historico de mensagens
  bool concluido = false;
  bool sairChat = false;
  bool teveMsg = false;
  int count = 0;

  ALLEGRO_EVENT_QUEUE *fila_eventos1 = NULL;
  fila_eventos1 = al_create_event_queue();

  al_register_event_source(fila_eventos1, al_get_display_event_source(janela));
  al_register_event_source(fila_eventos1, al_get_keyboard_event_source());

  strcpy(msg, "");


  //printf("Entrei runchat\n");
  while (!sair && !sairChat) {
    
    // LER UMA TECLA DIGITADA
    while(!sair && !concluido && !sairChat){
      //printf("Entrei digitar mensagem\n");
      while(!al_is_event_queue_empty(fila_eventos1)){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos1, &evento);

        if (!concluido){
            manipular_entrada(evento, msg, msgTAM);
            
            //Vai salvar o que esta escrito até apertar enter
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                if(strlen(msg) > 0){
                concluido = true;
                }
            }
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN && (evento.keyboard.keycode == ALLEGRO_KEY_LCTRL || evento.keyboard.keycode == ALLEGRO_KEY_RCTRL)){
                sairChat = true;
            }
        }

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){//Se fechar janela
          sair = true; concluido = true;
        }
      }

      if (concluido){
        count = strlen(msg) + 1;
        int ret = sendMsgToServer((void *)msg, count);
        if (ret == SERVER_DISCONNECTED) {
          return;}
        strcpy(msg, "");
      }

      // LER UMA MENSAGEM DO SERVIDOR
      int ret = recvMsgFromServer(msgServer, DONT_WAIT);
      if (ret == SERVER_DISCONNECTED) {
        return;} 
      else if (ret != NO_MESSAGE) {
          int i;
          for (i = 0; i < histTAM - 1; i++) {
            strcpy(msgHist[i], msgHist[i + 1]);
          }
        strcpy(msgHist[histTAM - 1], msgServer);
        teveMsg = true;
      }


      // PRINTAR NOVO ESTADO DO CHAT
      
      al_draw_bitmap(fundoChat, 0, 0, 0);

      al_draw_text(fonte2, al_map_rgb(255, 255, 255), 5, 7, ALLEGRO_ALIGN_LEFT, "Pressione control para sair do chat.");
      int i;

      for (i = 0; i < histTAM; i++) {
        printHistorico(msgHist[i], i);
      }
      
      al_draw_text(fonte2, al_map_rgb(255, 255, 255), 5, aTela-tamFonte1, ALLEGRO_ALIGN_LEFT, msg);//Printa a mensagem na tela

      al_flip_display();

    }
    

    //printf("mensagem digitada com sucesso.\n");

    concluido = false;
  }
  al_destroy_event_queue(fila_eventos1);
}




//=-=-=-=-=-=-=-=-=-=-=-=-=- -=-=-=-=-=-=-=-=-=-=-=-=-=-FUNCAO MAIN -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-


int main(){
  bool concluido = false;

  inicializar();
  carregar_arquivos();

  assertConnection();
  runChat();

  
  andandoDireita[0] = 1;
  andandoEsquerda[0] = 1;
  andandoCima[0] = 1;
  andandoBaixo[0] = 1;
  int entrei;
  int spriteAtual = 0; // para ciclar entre os sprites na movimentacao
  int i;
  int personagem = 1;
  int desenha = 1;
  int sair = 0;
  int framesTotal;
  int frameReferencia = 0;
  int cameraX = 0;
  int cameraY = 0;

  

  int ret; //  vai mandar as msg pro server

  //essa variavel vai determinar para qual direcao a magia sera lancada
  char magiaPos = 'D'; //  começo o personagem virado pra direita
  //vou usar para receber os comandos do teclado
  char tecla;
  //////////////////////////////////////////////////////////////////////////////// *-- DEFININDO OS TAMANHOS DE CADA SPRITE --*//////////////////////////////////////////////////////////////////////

  ///*--SPRITES DE PERSONAGENS--*/// variaveis para manipular ele na hora de printar
  //quantos sprites tem em cada linha da folha, e a atualmente mostrada
  int coluna_atual=0;
  //quantos sprites tem em cada coluna da folha, e a atualmente mostrada
  int linha_atual=0;
  //posicoes X e Y da folha de sprites que serao mostradas na tela
  int regiao_x_folha=0, regiao_y_folha=0;
  //quantos frames devem se passar para atualizar para o proximo sprite
  int frames_sprite = 1, cont_frames=0;
  //posicao X Y da janela em que sera mostrado o sprite
  int pos_x_sprite=150, pos_y_sprite=150;
  //velocidade X Y que o sprite ira se mover pela janela
  int vel_x_sprite=0, vel_y_sprite=0;

  // startando a struct do personagem desse cliente
  Jogador PlayerOne;
  // vetor que vou usar para receber os personagens do servidor, sao 3 pois o a struct desse proprio client sao sera entrege à ele
  Jogador* jogadores =  (Jogador *) malloc (sizeof(Jogador*3));


  ///---* SPRITES DE MAGIA --*//////
  // tamanho do sprite de magia de fogo e gelin
  int altura_magia = 128, largura_magia = 128;
  // tamanho do sprite da magia de raio
  int altura_magia_Thunder = 256, largura_magia_Thunder = 64; 
  // tamanho dos sprites da magia de lazer
  int altura_magia_Lazer = 64 , largura_magia_Lazer = 256;
  // tamanho do sprite das bolas de energia
  int altura_magia_energy = 125, largura_magia_energy = 100;
  // tamanho do sprite da explosao roxa
  int altura_magia_roxa = 256, largura_magia_roxa = 256;
  // tamanho do sprite do splash de gelo
  int altura_magia_Ice_Splash = 192, largura_magia_Ice_Splash = 192; 
  // tamando do sprite do splash de agua
  int altura_magia_Splash = 200, largura_magia_Splash = 200;
  // tamanho do sprite da chama azul
  int altura_magia_Blue_Fire = 300, largura_magia_Blue_Fire = 256;
  // tamanho do sprite lightning radial
  int altura_magia_lightning_radial = 450, largura_magia_lightning_radial = 450;
  // tamanho do sprite de teleporte
  int altura_magia_teleport = 100, largura_magia_teleport = 600;

  // VARIAVEL PARA MOSTRAR OS FRAMES DA MOVIMENTACAO DE FORMA ADEQUADA E FLUIDA

  int cooldown_magia_Roxa = 0;
  int contador_andarCima;
  int contador_andarBaixo;
  int contador_andarDireita;
  int contador_andarEsquerda;

  int cooldown_magia_Fogo = 0;

  ///*-- POSICOES DAS MAGIAS NA FOLHA PARA DESENHA-LAS --*///////////

  int regiao_x_magia_Z = 0, regiao_y_magia_Z = 0;
  
  int regiao_x_magia_X = 0, regiao_y_magia_X = 0;
  
  int regiao_x_magia_C = 0, regiao_y_magia_C = 0;

  int regiao_x_magia_V = 0, regiao_y_magia_V = 0;


  

  while(!sair){
    /* -- EVENTOS -- */

    PlayerOne.magia = 0; // se ele lancar alguma magia nesse frame, sera mudado para um, para sinalizar isso

    ALLEGRO_EVENT evento;
    al_wait_for_event(fila_eventos, &evento);

    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        // caso seja hora de finalizar o jogo
        sair = true;

    }


  ////////////////////////////////////////////* -- EVENTOS DO TECLADO -- *////////////////////////////////////////////////////////////////////////////////////////////



  //----------------------------------------------------//  SECÃO DE MOVIMENTACAO E MAGIAS //----------------------------------------------//
  else if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
    //verifica qual tecla foi pressionada
    //crio um switch para adequar as velocidades ao movimento desejado
    switch(evento.keyboard.keycode){
    //seta para cima
        case ALLEGRO_KEY_UP:
            tecla = 1;
            magiaPos = 'C';
            vel_y_sprite = -2;
            linha_atual = 2;
            contador_andarCima = 0;
            break;
    //seta para baixo
        case ALLEGRO_KEY_DOWN:
            tecla = 2;
            magiaPos = 'B';
            vel_y_sprite = 2;
            linha_atual = 0;
            contador_andarBaixo = 0;
            break;
    //seta para esquerda
        case ALLEGRO_KEY_LEFT:
            tecla = 3;
            magiaPos = 'E';
            vel_x_sprite = -2;
            linha_atual = 1;
            contador_andarEsquerda = 0;
            break;
    //seta para direita.
        case ALLEGRO_KEY_RIGHT:
            tecla = 4;
            magiaPos = 'D';
            vel_x_sprite = 2;
            linha_atual = 3;
            contador_andarDireita = 0;
            break;
    //Botão Z magia de fogo
        case ALLEGRO_KEY_Z:
        // COOLDOWN DE 3 SEGUNDOS
            if(framesTotal - frameReferencia >= 60){
                printf("Reconheci o Z!\n");
                frameReferencia = framesTotal;
                tecla = 5;
            }
            break;

    // Botão X magia de gelo
        case ALLEGRO_KEY_X:
            frameReferencia = framesTotal;  
            tecla = 6;   
            break;

    // Botão C magia de raio
        case ALLEGRO_KEY_C:
            if(framesTotal - frameReferencia >= 60){
                frameReferencia = framesTotal;
                tecla = 7;
            }
            break;

    //Botão V magia de lazer
        case ALLEGRO_KEY_V:
            if(framesTotal - frameReferencia >= 60){ // cooldownzinho
                frameReferencia = framesTotal;
                tecla = 8;
            }
            break;

    //esc. sair=1 faz com que o programa saia do loop principal
        case ALLEGRO_KEY_ESCAPE:
            sair = 1;
            break;
        }
      }

    else if(evento.type == ALLEGRO_EVENT_KEY_UP){
        //verifica qual tecla foi despressionada
            switch(evento.keyboard.keycode){
            //seta para cima
            case ALLEGRO_KEY_UP:
                tecla = 0;
                vel_y_sprite = 0;
                linha_atual = 2;
                break;
            //seta para baixo
            case ALLEGRO_KEY_DOWN:
                tecla = 0;
                vel_y_sprite = 0;
                linha_atual = 0;
                break;
            //seta para esquerda
            case ALLEGRO_KEY_LEFT:
                tecla = 0;
                vel_x_sprite = 0;
                linha_atual = 1;
                break;
            //seta para direita.
            case ALLEGRO_KEY_RIGHT:
                tecla = 0;
                vel_x_sprite = 0;
                linha_atual = 3;
                break;
            
            // tecla z da maiga
            case ALLEGRO_KEY_Z:
                tecla = 0;
                break;
            
            // tecla x da magia
            case ALLEGRO_KEY_X:
                tecla = 0;
                break;
            // tecla c da magia
            case ALLEGRO_KEY_C:
                tecla = 0;
                break;
            //esc. sair=1 faz com que o programa saia do loop principal
            case ALLEGRO_KEY_ESCAPE:
                sair = true;
                break;
            }
    }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  else if(evento.type == ALLEGRO_EVENT_TIMER){
      //a cada disparo do timer, incrementa cont_frames
      cont_frames++;
      framesTotal += 1;
      if(framesTotal > 300){
          framesTotal -= 300;
          frameReferencia -= 300;
      }
      //se alcancou a quantidade de frames que precisa passar para  mudar para o proximo sprite
      if (cont_frames >= frames_sprite){
          //reseta cont_frames
          cont_frames = 0;
          //calcula a posicao Y da folha que sera mostrada
          regiao_y_folha = linha_atual * altura_sprite;
          //calcula a regiao X da folha que sera mostrada
          regiao_x_folha = coluna_atual * largura_sprite;
      }

      //se o sprite estiver perto dos limites verticais
      if ( pos_x_sprite + largura_sprite > largura_mapa - 20 || pos_x_sprite < 20 ){
      //inverte o sentido da velocidade X, para andar no outro sentido
      vel_x_sprite = -vel_x_sprite;
      }
      // se o sprite chegar perto dos limites horizontais
      if( pos_y_sprite + altura_sprite > altura_mapa - 20 || pos_y_sprite < 20 ) {
      //inverte o sentido da velocidade Y, para andar no outro sentido
      vel_y_sprite = -vel_y_sprite;
      }

      desenha=1;
  }
  ////////////////////////////////////////////////* -- DESENHANDO AS MAGIAS QUE FORAM USADA --*///////////////////////////////////////////////////////////////////////////
    
  //////////////////////////////////////////////////////////////////////////////////////////////////////* -- ATUALIZA TELA -- *///////////////////////////////////////////////////////////////////////////////
    if(desenha && al_is_event_queue_empty(fila_eventos)) {

        //desenha o fundo na tela
        // al_draw_bitmap_region(fundo,0,0,LARGURA_TELA,ALTURA_TELA,0,0,0);
        
        cameraX = pos_x_sprite - LARGURA_TELA/2;
        cameraY = pos_y_sprite - ALTURA_TELA/2;
        al_draw_bitmap(fundo,(cameraX*-1)-400,(cameraY*-1)-400,0);
    
        ////////////////////////////////////////////////* -- DESENHANDO AS MAGIAS QUE FORAM USADA --*///////////////////////////////////////////////////////////////////////////

        if(personagem == 1){ //  esse sera o personagem Caio Black
  //--------------------------------------------//////////// --* MAGIA DE FOGO (Z) //////////////------------------------------------------------------------------------------//
            //a intencao eh fazer uma linha de fogo em frente ao personagem(300 pixels em frente do personagem)
            // PARA BAIXO
            if(magiaPos == 'B' && tecla == 5 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                printf("printando magia aqui!\n");
                int cooldown = framesTotal - frameReferencia;
                printf("%d", cooldown);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite + 400, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite + 350, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite + 300, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite + 250, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite + 200, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite + 150, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite + 100, 0);
            }
            // PARA CIMA
            if(magiaPos == 'C' && tecla == 5 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                printf("printando magia aqui!\n");
                int cooldown = framesTotal - frameReferencia;
                printf("%d", cooldown);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite - 400, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite - 350, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite - 300, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite - 250, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite - 200, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite,pos_y_sprite - 150, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite, pos_y_sprite - 100, 0);
            }
            // PARA DIREITA
            if(magiaPos == 'D' && tecla == 5 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                printf("printando magia aqui!\n");
                int cooldown = framesTotal - frameReferencia;
                printf("%d", cooldown);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite + 400,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite + 350,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite + 300,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite + 250,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite + 200,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite + 150,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite + 100,pos_y_sprite, 0);
            }
            // PARA ESQUERDA
            if(magiaPos == 'E' && tecla == 5 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                printf("printando magia aqui!\n");
                int cooldown = framesTotal - frameReferencia;
                printf("%d", cooldown);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite - 400,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite - 350,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite - 300,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite - 250,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite - 200,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite - 150,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_Fire,
                    regiao_x_magia_Z,regiao_y_magia_Z+(128*(cooldown-1)),
                    largura_magia,altura_magia,
                    pos_x_sprite - 100,pos_y_sprite, 0);
            }
  //--------------------------------------------////////////--* MAGIA DE EXPLOSAO ROXA(X) --*//////////////////--------------------------------------------////
            // a intencao eh criar uma magia com uma area grande em volta do personagem
            if(tecla == 6 && frameReferencia != 0){
                char printar = 0; // uso char porque essa variavel sera apenas um booleano
                printf("frames total - ref = %d", frameReferencia);
                if((framesTotal-frameReferencia)%4 == 0){
                    printar = 1;
                }
                if(cooldown_magia_Roxa == 32) cooldown_magia_Roxa = 0;
                printf("%d\n", printar);
                if(printar){
                    cooldown_magia_Roxa += 1;
                    printf("Aqui\n");
                    // para printar um sprite a cada quatro frames 
                    if(cooldown_magia_Roxa/8 > 1){
                        regiao_y_magia_X = 1*largura_magia_roxa;
                        cooldown_magia_Roxa -= 8;
                        if(cooldown_magia_Roxa/16 > 1){
                            regiao_y_magia_X = 2*largura_magia_roxa;
                            cooldown_magia_Roxa -= 16;
                            if(cooldown_magia_Roxa/24 > 1){
                                cooldown_magia_Roxa -=  24;
                                regiao_y_magia_X = 3*largura_magia_roxa;
                            }
                        }
                    }
                
                    printf("%d", cooldown_magia_Roxa);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite,pos_y_sprite + 100, 0);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite,pos_y_sprite - 100, 0);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite - 100,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite + 100,pos_y_sprite, 0);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite + 100,pos_y_sprite + 100, 0);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite - 100,pos_y_sprite -100, 0);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite - 100,pos_y_sprite +100, 0);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite + 100,pos_y_sprite -100, 0);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite + 200,pos_y_sprite -200, 0);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite - 200,pos_y_sprite +200, 0);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite - 200,pos_y_sprite -200, 0);
                    al_draw_bitmap_region(magia_explosionPurple,
                        regiao_x_magia_X + (largura_magia_roxa*(cooldown_magia_Roxa-1)),regiao_y_magia_X,
                        largura_magia,altura_magia,
                        pos_x_sprite + 200,pos_y_sprite + 200, 0);
                }
            }

            if(tecla == 7 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                int cooldown = framesTotal - frameReferencia;
            }
        }

  //--------------------------------------------//////////// *-- MAGIA DE GELO (X) ////////////////--------------------------------------------/////
            
            else if(personagem == 2){//Personagem 2
                    if(magiaPos == 'B' && tecla == 6 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                            printf("printando magia aqui!\n"); 
                            int cooldown = framesTotal - frameReferencia;
                            printf("%d", cooldown);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite,pos_y_sprite + 250, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite,pos_y_sprite + 200, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite,pos_y_sprite + 150, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite,pos_y_sprite + 100, 0);
                    }
                    // PARA CIMA
                    if(magiaPos == 'C' && tecla == 6 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                            printf("printando magia aqui!\n");
                            int cooldown = framesTotal - frameReferencia;
                            printf("%d", cooldown);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_Z+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite,pos_y_sprite - 250, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite,pos_y_sprite - 200, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite,pos_y_sprite - 150, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite,pos_y_sprite - 100, 0);
                    }
                    // PARA DIREITA
                    if(magiaPos == 'D' && tecla == 6 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                            printf("printando magia aqui!\n");
                            int cooldown = framesTotal - frameReferencia;
                            printf("%d", cooldown);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite + 250,pos_y_sprite, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite + 200,pos_y_sprite, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite + 150,pos_y_sprite, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite + 100,pos_y_sprite, 0);
                    }
                    // PARA ESQUERDA
                    if(magiaPos == 'E' && tecla == 6 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                            printf("printando magia aqui!\n");
                            int cooldown = framesTotal - frameReferencia;
                            printf("%d", cooldown);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite - 250,pos_y_sprite, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite - 200,pos_y_sprite, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite - 150,pos_y_sprite, 0);
                                al_draw_bitmap_region(magia_Gelo,
                                regiao_x_magia_X,regiao_y_magia_X+(128*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite - 100,pos_y_sprite, 0);
                    }
                    
    //--------------------------------------------///////// --* MAGIA DE RAIO (C) ///////////////--------------------------------------------/////
                    // ESSA VAI SER UMA PAREDE EM VOLTA DO USUARIO
                    

                    // PRINTANDO EM VOLTA DO USUARIO
                if(tecla == 7 && framesTotal - frameReferencia < 10 && frameReferencia != 0){
                    printf("printando magia aqui!\n"); 
                    int cooldown = framesTotal - frameReferencia;
                    printf("%d", cooldown);
                        // em baixo
                        al_draw_bitmap_region(magia_Thunder,
                        regiao_x_magia_C+((cooldown-1)*64),regiao_x_magia_C,
                        largura_magia_Thunder,altura_magia_Thunder,
                        pos_x_sprite, pos_y_sprite - 100, 0);
                        // em cime
                        al_draw_bitmap_region(magia_Thunder,
                        regiao_x_magia_C+((cooldown-1)*64),regiao_x_magia_C,
                        largura_magia_Thunder,altura_magia_Thunder,
                        pos_x_sprite, pos_y_sprite + 100, 0);
                        // direita
                        al_draw_bitmap_region(magia_Thunder,
                        regiao_x_magia_C+((cooldown-1)*64),regiao_x_magia_C,
                        largura_magia_Thunder,altura_magia_Thunder,
                        pos_x_sprite + 100, pos_y_sprite, 0);
                        // esquerda
                        al_draw_bitmap_region(magia_Thunder,
                        regiao_x_magia_C+((cooldown-1)*64),regiao_x_magia_C,
                        largura_magia_Thunder,altura_magia_Thunder,
                        pos_x_sprite - 100, pos_y_sprite, 0);
                        // diagonal direita superior
                        al_draw_bitmap_region(magia_Thunder,
                        regiao_x_magia_C+((cooldown-1)*64),regiao_x_magia_C,
                        largura_magia_Thunder,altura_magia_Thunder,
                        pos_x_sprite + 100, pos_y_sprite + 100, 0);
                        // diagonal esquerda superior
                        al_draw_bitmap_region(magia_Thunder,
                        regiao_x_magia_C+((cooldown-1)*64),regiao_x_magia_C,
                        largura_magia_Thunder,altura_magia_Thunder,
                        pos_x_sprite - 100, pos_y_sprite + 100, 0);
                        // diagonal esquerda inferior
                        al_draw_bitmap_region(magia_Thunder,
                        regiao_x_magia_C+((cooldown-1)*64),regiao_x_magia_C,
                        largura_magia_Thunder,altura_magia_Thunder,
                        pos_x_sprite - 100, pos_y_sprite - 100, 0);
                        //  diagonal direita inferior
                        al_draw_bitmap_region(magia_Thunder,
                        regiao_x_magia_C+((cooldown-1)*64),regiao_x_magia_C,
                        largura_magia_Thunder,altura_magia_Thunder,
                        pos_x_sprite + 100, pos_y_sprite - 100, 0);
                    }


    //--------------------------------------------////////////////////////*-- MAGIA DE LAZER --*/////////////////////////////--------------------------------------------////
                    // VAI SER UM LASER COM O RANGE BEM ALTO MAS COOLDOWN ALTO TBM
                        // PARA BAIXO
                    if(magiaPos == 'B' && tecla == 8 && framesTotal - frameReferencia < 9 && frameReferencia != 0){
                        printf("printando magia aqui!\n");
                        int cooldown = framesTotal - frameReferencia;
                        printf("%d", cooldown);
                            al_draw_bitmap_region(magia_Lazer,
                            regiao_x_magia_V,regiao_y_magia_V+(altura_magia_Lazer*(cooldown-1)),
                            largura_magia_Lazer,altura_magia_Lazer,
                            pos_x_sprite,pos_y_sprite + 300, 0);
                            al_draw_rotated_bitmap(magia_Lazer,
                            pos_x_sprite, pos_y_sprite + 300,
                            pos_x_sprite, pos_y_sprite + 300,
                            90, 0);

                    }
                    // PARA CIMA
                    if(magiaPos == 'C' && tecla == 8 && framesTotal - frameReferencia < 9 && frameReferencia != 0){
                        printf("printando magia aqui!\n");
                        int cooldown = framesTotal - frameReferencia;
                        printf("%d", cooldown);
                            al_draw_bitmap_region(magia_Lazer,
                            regiao_x_magia_V,regiao_y_magia_V+(altura_magia_Lazer*(cooldown-1)),
                            largura_magia_Lazer,altura_magia_Lazer,
                            pos_x_sprite,pos_y_sprite - 300, 0);
                            al_draw_rotated_bitmap(magia_Lazer,
                            pos_x_sprite, pos_y_sprite + 300,
                            pos_x_sprite, pos_y_sprite + 300,
                            90, 0);
                    }
                    // PARA DIREITA
                    if(magiaPos == 'D' && tecla == 8 && framesTotal - frameReferencia < 9 && frameReferencia != 0){
                            printf("printando magia aqui!\n");
                            int cooldown = framesTotal - frameReferencia;
                            printf("%d", cooldown);
                                al_draw_bitmap_region(magia_Lazer,
                                regiao_x_magia_V,regiao_y_magia_V + (altura_magia_Lazer*(cooldown-1)),
                                largura_magia_Lazer,altura_magia_Lazer,
                                pos_x_sprite + 300,pos_y_sprite, 0);
                    }
                    // PARA ESQUERDA
                    if(magiaPos == 'E' && tecla == 8 && framesTotal - frameReferencia < 9 && frameReferencia != 0){
                        printf("printando magia aqui!\n");
                        int cooldown = framesTotal - frameReferencia;
                        printf("%d", cooldown);
                            al_draw_bitmap_region(magia_Lazer,
                            regiao_x_magia_V,regiao_y_magia_V+(altura_magia_Lazer*(cooldown-1)),
                            largura_magia_Lazer,altura_magia_Lazer,
                            pos_x_sprite - 500,pos_y_sprite, 0);
                    }
                }
        
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /////////////////////// *-- DESENHANDO CAIO BLACK --* ///////////////////////////////////////////////////////////////////////////////////////////////////// 
        if(personagem == 1){
          // o sprite esta descendo ou subindo
            if(vel_y_sprite > 0 || vel_y_sprite < 0){
                // adequando as posicoes de acordo com a velocidade
                pos_y_sprite += vel_y_sprite;
                
              int linhas = 0;
                int referencia;
                // tenho 3 sprites para cada direcao
                    if(magiaPos == 'C'){
                        contador_andarCima += 1; //  variavel para alternar entre os sprites
                        if(andandoCima[0] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                            entrei = 0;
                        }
                        if(andandoCima[1] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                            entrei = 1;
                        }
                        if(andandoCima[2] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                            entrei = 2;
                        }
                    }
                    if(magiaPos == 'B'){
                        contador_andarBaixo += 1;
                        if(andandoBaixo[0] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                                entrei = 0;
                        }
                        if(andandoBaixo[1] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                            entrei = 1;
                        }
                        if(andandoBaixo[2] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                            entrei = 2;
                        }
                    }
            }

            // o sprite esta indo para direita ou esquerda
            else if(vel_x_sprite > 0 || vel_x_sprite < 0){
                // adequando as posicoes de acordo com a velocidade
                pos_x_sprite += vel_x_sprite;
              int linhas = 0;

                // tenho 3 sprites para cada direcao
                    if(magiaPos == 'D'){
                        contador_andarDireita += 1;
                        if(andandoDireita[0] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                            entrei = 0;
                        }
                        if(andandoDireita[1] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                            entrei = 1;
                        }
                        if(andandoDireita[2] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                            entrei = 2;
                        }
                    }
                    if(magiaPos == 'E'){
                        contador_andarEsquerda += 1;
                        if(andandoEsquerda[0] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                            entrei = 0;
                        }
                        if(andandoEsquerda[1] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                            entrei = 1;
                        }
                        if(andandoEsquerda[2] == 1){
                            desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                            entrei = 2;
                        }
                    }
            }

            else if(vel_x_sprite == 0 && vel_y_sprite == 0){
                switch (magiaPos)
                {
                    case 'B':
                        desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                        break;
                    case 'C':
                        desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                        break;
                    case 'D':
                        desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                        break;
                    case 'E':
                        desenhaSpritePersonagem(magiaPos, pos_x_sprite, pos_y_sprite);
                        break;
                }
            }      
        }

        ////---*--- SWITCH PARA CICLAR ENTRE OS SPRITES ----*----////
                switch (magiaPos)
            {
                case 'C':
                    if(contador_andarCima == 10){
                        if(entrei == 0){
                            andandoCima[0] = 0;
                            andandoCima[1] = 1;
                        }
                        if(entrei == 1){
                            andandoCima[1] = 0;
                            andandoCima[2] = 1;
                        }
                        if(entrei == 2){
                            andandoCima[2] = 0;
                            andandoCima[0] = 1;
                        }
                        contador_andarCima = 0;
                    }
                    break;
                case 'B':
                    if (contador_andarBaixo == 10){
                        if(entrei == 0){
                            andandoBaixo[0] = 0;
                            andandoBaixo[1] = 1;
                        }
                        if(entrei == 1){
                            andandoBaixo[1] = 0;
                            andandoBaixo[2] = 1;
                        }
                        if(entrei == 2){
                            andandoBaixo[2] = 0;
                            andandoBaixo[0] = 1;
                        }
                        contador_andarBaixo = 0;
                    }
                    break;
                case 'D':
                    if (contador_andarDireita == 10){
                        if(entrei == 0){
                            andandoDireita[0] = 0;
                            andandoDireita[1] = 1;
                        }
                        if(entrei == 1){
                            andandoDireita[1] = 0;
                            andandoDireita[2] = 1;
                        }
                        if(entrei == 2){
                            andandoDireita[2] = 0;
                            andandoDireita[0] = 1;
                        }
                        contador_andarDireita = 0;
                    }
                    break;
                case 'E':
                    if(contador_andarEsquerda == 10){
                        if(entrei == 0){
                            andandoEsquerda[0] = 0;
                            andandoEsquerda[1] = 1;
                        }
                        if(entrei == 1){
                            andandoEsquerda[1] = 0;
                            andandoEsquerda[2] = 1;
                        }
                        if(entrei == 2){
                            andandoEsquerda[2] = 0;
                            andandoEsquerda[0] = 1;
                        }
                        contador_andarEsquerda = 0;
                    }
                    break;
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // no fim do loop atualiza as posicoes do personagem na struct, e se ele lancou magia ou nao

        PlayerOne.posX = pos_x_sprite;
        PlayerOne.posY = pos_y_sprite;
        PlayerOne.virado = magiaPos;
        
        if(tecla == 5 || tecla == 6 || tecla == 7){
            // ele lancou magia
            PlayerOne.magia = tecla;
        }


        ///////////////////////////////////////////////////////// --* RECEBE E MANDA MENSAGENS PRO SERVER --* //////////////////////////////////////////////////////////////////////


        ret = sendMsgToServer((void *)PlayerOne, sizeof(Jogador));
                if (ret == SERVER_DISCONNECTED) {
                    return;}

        jogadores = recvMsgFromServer(msgServer, DONT_WAIT);
                if (ret == SERVER_DISCONNECTED){
                    return;}

        for(i = 0; i < 2; i++){
            if(jogadores[i].personagem != 0){
                // quer dizer que existe o jogador e ele deve ser printado

            }
        }

        al_flip_display();
        
        desenha = 0;
    }
  
  }
  free(jogadores); // liberando a memoria do meu vetor de jogadores
  finalizar();
  return 0;
}