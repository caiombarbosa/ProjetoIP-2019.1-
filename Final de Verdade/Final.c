// para compilar : gcc Final.c -o joguin -lallegro -lallegro_ttf -lallegro_image -lallegro_font -lallegro_dialog -lallegro_primitives -lallegro_acodec -lallegro_audio
// ta com o bug de andar pra baixo e pra cima

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define msgTAM 350 //MSG_MAX_SIZE
#define bufferTAM (msgTAM + 100)//BUFFER_SIZE
#define loginTAM 13 //LOGIN_MAX_SIZE
#define histTAM 30 //Tamanho do historico de msg
#define LARGURA_TELA 1240
#define ALTURA_TELA 800
#define fps 45
#define FADE_IN 2.5
#define FADE_OUT 4
#define FPS 60.0
#define tamFonte1 36
#define tamFonte2 18
#define largura_mapa 5000
#define altura_mapa 2500

//--* VARIAVES GLOBAIS --*//	

    // variaveis do server chat
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


// timer de 60 fps para o jogo e fila de eventos para o jogo
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_TIMER *timer = NULL;

// tudo isso eh para o menu 
ALLEGRO_EVENT_QUEUE *evento = NULL;
ALLEGRO_EVENT_QUEUE *evento_menu = NULL;
ALLEGRO_TIMER *tempo = NULL;
// variavel para o codigo do menu (n sei para que serve direito no momento)
double start_time = 0;

//////////////////*--  VARIAVEIS PARA SPRITES DOS PERSONAGENS --*////////////////////////////////////

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

//////////////////*-- VARIAVEIS PARA SPRITES DAS MAGIAS --*////////////////////////////////////////////

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

//======================================
//variáveis de audio

ALLEGRO_SAMPLE *sample=NULL;
ALLEGRO_AUDIO_STREAM *opening = NULL;


ALLEGRO_SAMPLE *timesup_sample = NULL;
ALLEGRO_SAMPLE *you_lose_sample = NULL;
ALLEGRO_SAMPLE *you_win_sample = NULL;
//======================================

//variável global da fonte
ALLEGRO_FONT *fonte = NULL;



//=====================================
//os bitmaps
ALLEGRO_BITMAP  *image = NULL;  
ALLEGRO_BITMAP *bar  = NULL;
ALLEGRO_BITMAP *def = NULL;
ALLEGRO_BITMAP *menu1 = NULL;
ALLEGRO_BITMAP *play = NULL;
ALLEGRO_BITMAP *help = NULL;
ALLEGRO_BITMAP *credits = NULL;
ALLEGRO_BITMAP *exit1 = NULL;
ALLEGRO_BITMAP *game_back = NULL;
ALLEGRO_BITMAP *game_mode= NULL;
ALLEGRO_BITMAP *you_won = NULL;
ALLEGRO_BITMAP *you_lost = NULL;
ALLEGRO_BITMAP *initial = NULL;
ALLEGRO_BITMAP *timesup =NULL;
ALLEGRO_BITMAP *credits1 = NULL;
ALLEGRO_BITMAP *help1 = NULL;
ALLEGRO_BITMAP *opening_1 = NULL;
ALLEGRO_BITMAP *opening_2 = NULL;
ALLEGRO_BITMAP *opening_3 = NULL;
ALLEGRO_BITMAP *opening_4 = NULL;
ALLEGRO_BITMAP *presents = NULL;
ALLEGRO_BITMAP *loading = NULL; //  comeco do jogo, tela de loading

//cabeçalho das funcoes.
bool inicializa();
bool loadBitmaps();
void error_msg(char *text);
void registerEvents();
void start_timer();
double get_time();
int menu();
void fadeout(int velocidade);
void fadein(ALLEGRO_BITMAP *imagem, int velocidade);
void Opening();
void destroy();

/////////////////////////////////////// --* FUNCOES DO PROGRAMA --* ////////////////////////////////////////////////////////////

// funcao para incializar antes do jogo
void inicializar1(){
    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a biblioteca Allegro.\n");
    }
 
    if (!al_install_keyboard()){
        fprintf(stderr, "Falha ao inicializar teclado.\n");
    }
 
    if (!al_init_image_addon()){
        fprintf(stderr, "Falha ao inicializar allegro_image.\n");
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

    al_set_window_title(janela, "WIZARD KING");

}

// funcao para desenhar os sprites do personagem
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


//RESPONSÁVEL POR VERIFICAR ERROS=====================================
void error_msg(char *text){
	al_show_native_message_box(janela,"ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}


//INICIALIZAÇÃO DAS BIBLIOTECAS============
bool inicializa(){

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

    if(!al_init_acodec_addon()){
      fprintf(stderr, "failed to initialize audio codecs!\n");
      return -1;
    }

    if(!al_init_primitives_addon()){
        error_msg("Falha ao iniciar A.P");
        return false;
    }

    evento = al_create_event_queue();

    if(evento == NULL){
         error_msg("Falha ao iniciar E");
        al_destroy_display(janela);
        return false;
    }

    evento_menu = al_create_event_queue();

    if(evento_menu == NULL){

         error_msg("Falha ao iniciar E.M");
        al_destroy_display(janela);
        return false;
    }

    tempo = al_create_timer(1.0);

     if(tempo == NULL){

        error_msg("Falha ao iniciar T");
        al_destroy_display(janela);
        return false;
    }

    loading = al_load_bitmap("opening1.jpg");
    
    if(loading == NULL){
         error_msg("Falha ao inicializar a Allegro");
        return -1;
    }

     if (!al_install_audio()){
       error_msg("Falha ao iniciar áldio");
        return false;
    }

    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        error_msg("Falha ao iniciar samples");
        return false;
    }

    return true;
}

/// --* CARREGA OS BITMAPS DOS PERSONAGENS, DO MAPA , DOS FUNDOS E AS FONTES DO CHAT
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

    if(sair){
        exit(-1);
    }
}

// funcao que termina o programa depois do jogo
void finalizar(){
    al_destroy_bitmap(fundoChat);
    al_destroy_font(fonte1);
    al_destroy_font(fonte2);
    al_destroy_bitmap(fundo);
    al_destroy_bitmap(folha_sprite_Caio_Black);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
}



// carrega os bitmaps que sao usados para o menu
bool loadBitmaps(){


    //THE BITMAPS WILL BE INITIALIZED HERE
    image = al_load_bitmap("top-3.jpg");

    //MENU
    menu1 = al_load_bitmap("start_0.jpg");
    play = al_load_bitmap("start_1.jpg");
    help = al_load_bitmap("start_2.jpg");
    credits = al_load_bitmap("start_3.jpg");
    exit1 = al_load_bitmap("start_4.jpg");

    game_mode = al_load_bitmap("guide.jpg"); //  onde o jogo começa
    game_back = al_load_bitmap("top-3.jpg");

    you_won = al_load_bitmap("top-3.jpg");
    you_lost = al_load_bitmap("top-3.jpg");

    initial = al_load_bitmap("top-3.jpg");
    timesup = al_load_bitmap("top-3.jpg"); 
    credits1 = al_load_bitmap("guide.jpg"); // guia do jogo
    help1 = al_load_bitmap("help.jpg"); // creditos do jogo


    opening_1 = al_load_bitmap("top-1.jpg");
    opening_2 = al_load_bitmap("top-2.jpg");
    opening_3 = al_load_bitmap("top-3.jpg");
    opening_4 = al_load_bitmap("top-1.jpg");


    presents = al_load_bitmap("fadin.jpg");
    
    sample = al_load_sample("Theme.wav");

    if(sample == NULL){
        error_msg("Falha ao carregar som menu");
        al_destroy_display(janela);
        return false;
    }


    if(menu1==NULL){
        error_msg("Falha na menu");    
        al_destroy_display(janela);
        return false;

    }
    if(play==NULL){
        error_msg("Falha na play");
        al_destroy_display(janela);
        return false;

    }

    if(exit1==NULL){
        error_msg("Falha na exit");
        al_destroy_display(janela);
        return false;

    }
    if(game_mode ==NULL){
        error_msg("Falha na game_mode");
        al_destroy_display(janela);
        return false;

    }
    if(game_back==NULL){

        error_msg("Falha na game_back");
        al_destroy_display(janela);
        return false;
    }

    if(initial == NULL){
        error_msg("Falha na play");
        al_destroy_display(janela);
        return false;

    }



    if(timesup == NULL){
        error_msg("Falha na timesup");
        al_destroy_display(janela);
        return false;

    }

    if(opening_1 == NULL){
        error_msg("Falha na opening1");
        al_destroy_display(janela);
        return false;

    }

    if(opening_2 == NULL){
        error_msg("Falha na opening2");
        al_destroy_display(janela);
        return false;

    }

    if(opening_3 == NULL){
        error_msg("Falha na opening3");
        al_destroy_display(janela);
        return false;

    }

    if(opening_4 == NULL){
        error_msg("Falha na opening4");
        al_destroy_display(janela);
        return false;

    }

    return true;

}


//FUNÇÃO QUE REGISTRA OS EVENTOS NA FILA DE EVENTOS
void registerEvents(){

    al_register_event_source(evento, al_get_display_event_source(janela));
    al_register_event_source(evento, al_get_keyboard_event_source());
    al_register_event_source(evento, al_get_timer_event_source(tempo));

    al_register_event_source(evento_menu, al_get_display_event_source(janela));
    al_register_event_source(evento_menu, al_get_keyboard_event_source());

}



//FUNÇAÕ AUXILIAR DE FPS
void start_timer(){
    start_time = al_get_time();
}


//FUNÇÃO AUXILIAR DE FPS
double get_time(){
    return al_get_time() - start_time;
}


    /*if(you_win_sample==NULL){

        error_msg("Falha ao iniciar you_win");
        return false;
    }

     if(you_lose_sample==NULL){

        error_msg("Falha ao iniciar you_lose");
        return false;
    }

    if(timesup_sample==NULL){

error_msg("Falha ao iniciar timesup");

    }
    
      
    return true;
}
*/



int menu(){

    ALLEGRO_EVENT event;
    ALLEGRO_TIMEOUT timeout;
    bool EXIT = false;
    bool BACK = false;
    int key = 0, imagem = 0, sub_menu =0, menu_play =0;
    bool begin = true;
    bool helpB = false;
    bool creditsB = false;
    bool start_menu_song = false;

    //É DEFINIDO UM TIME OUT PARA SER USADO NA FUNÇAO al_wait_for_event_until
    al_init_timeout(&timeout,1/fps);



    while(EXIT==false){

        al_play_sample(sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

        //FUNÇÃO QUE COMEÇARÁ UM CONTADOR QUE SERÁ USADO PARA CONTROLAR O FPS
        start_timer();


            if(imagem ==0 && menu_play == 0 && helpB == false && creditsB == false){
                al_flush_event_queue(evento_menu);
                al_draw_bitmap(menu1,0,0,0);
                al_rest(1.0);
                al_flip_display();
            }
           
            //ESPERA POR UM EVENTO DA ALLEGRO ATÉ QUE ACONTEÇA UM TIME OUT
            al_wait_for_event(evento_menu,&event);


            //VERIFICA QUAL TECLA FOI PRESSIONADA PELO JOGADOR
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){

                switch(event.keyboard.keycode){

                    case ALLEGRO_KEY_UP:
                        key =1;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        key = 2;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        key = 3;
                        break;
                    default:
                        break;
                }
            }

            //A PARTIR DA TECLA PRESSIONADA PELO JOGADOR 
            //FAZ CRESCER UM CONTADOR QUE DEFINE QUAL IMAGEM SERÁ COLOCADA NA TELA
            //E SE FOR PRESSIONADO ENTER EXECUTA UMA AÇÃO
            if(sub_menu ==0){
                if(key == 2 ){
                    if(imagem<=4){
                        imagem++;
                    }
                    if(imagem > 4){
                        imagem = 1;
                    }
                        key = 0;
                    }
                    else if(key == 1 ){
                        imagem --;

                        if(imagem <= 0){
                        imagem = 4;
                        }
                        key = 0;

                    }

                    //CASO A TECLA ENTER TENHA SIDO PRESIONADA DÁ UM VALOR A VARIÁVEL SUBMENU
                    //QUE É RESPONSÁVEL POR COLOCAR AS IMAGENS DOS SUBMENUS
                    //ALÉM DE AUXILIAR NA EXECUÇÃO DE ALGUMAS AÇÕES
                    if(key == 3){


                    if(imagem == 1){
                        sub_menu = 1;
                    }
                    else if(imagem ==2){
                        sub_menu =2;                    
                    }
                    else if(imagem == 3){
                        sub_menu =3;     
                    }
                    else if(imagem ==4){
                        EXIT = true;
                        exit(1); // termina o programa(botao exit)
                    }

                    }


                    if(imagem == 1){
                        al_draw_bitmap(play,0,0,0);
                        al_rest(0.1);
                        al_flip_display();

                    }
                    else if(imagem ==2){
                        al_draw_bitmap(help,0,0,0);
                        al_rest(0.1);
                        al_flip_display();

                    }
                    else if(imagem == 3){
                        al_draw_bitmap(credits,0,0,0);
                        al_rest(0.1);
                        al_flip_display();

                    }
                    else if(imagem ==4){
                        al_draw_bitmap(exit1,0,0,0);
                        al_rest(0.1);
                        al_flip_display();

                    }
            }


            //CASO A VARIÁVEL SUBMENU TENHA O VALOR IGUAL A 1
            //ISSO SIGNIFICA QUE O SUB MENU PLAY FOI ATIVADO
            //E É USADA UMA VARIÁVEL CHAMADA menu_play PARA AUXILIAR A NAVEGAR POR ESTE SUBMENU
            //USANDO A MESMA LÓGICA DA VARIÁVEL image USADA ANTERIORMENTE
            if(sub_menu == 1){

                if(key == 2 ){
                if(menu_play<=3){

                        menu_play ++;
                }
                if(menu_play > 3){
                        menu_play = 1;
                }
                    key = 0;
                }
                else if(key == 1 ){

                    menu_play --;

                    if(menu_play <= 0){
                        menu_play = 3;
                    }
                    key = 0;

                }

                /*//CASO A TECLA ENTER TENHA SIDO PRESSIONADA EXECUTA UMA AÇÃO
                else if(key == 3 && sub_menu == 1){
                    
                  break;
                */






                //ESTES IF STATEMENT APENAS DESENHARÁ AS IMAGENS 
                //DO SUBMENU PLAY NA TELA
                if(menu_play == 0 ){
                    // o jogo começa aqui
                    EXIT = true;
                    al_flip_display();


                }

                /*else if(menu_play == 1){
                    al_draw_bitmap(game_single,0,0,0);
                    al_rest(0.1);
                    al_flip_display();
                            
                }
                 
                else if(menu_play == 2){
                    al_draw_bitmap(game_multi,0,0,0);
                    al_rest(0.1);
                    al_flip_display();
                            
                            
                } */

                else if(menu_play == 3){

                    al_draw_bitmap(game_back,0,0,0);
                    al_rest(0.1);
                    al_flip_display();
                    if(key ==3){
                        imagem = 0;
                        sub_menu =0;
                        menu_play = 0;
                        key = 0;
                    }

                }

            }

            //MOSTRA A TELA DE AJUDA CASO ESSA OPÇÃO TENHA SIDO ESCOLHIDA
            else if(sub_menu == 2){

                    al_draw_bitmap(help1,0,0,0);
                    al_flip_display();
                    helpB= true;

                    if(key == 3||key == 1 || key==2){
                    imagem = 0;
                    sub_menu =0;
                    menu_play = 0;
                    key = 0;
                    helpB = true;

                    }
            }

            //MOSTRA A TELA DE CREDITOS DO JOGO
            else if(sub_menu == 3){

                al_draw_bitmap(credits1,0,0,0);
                al_flip_display();
                creditsB = true;

                if(key == 3||key == 2 || key ==1){

                    imagem = 0;
                    sub_menu =0;
                    menu_play = 0;
                    key = 0;
                    creditsB = false;

                }
                al_rest(4); // para dar tempo de ler
            }

            //FECHA O JOGO CASO O JOGADOR TENHA CLICADO PARA FECHAR
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                EXIT = true;
                return 0;
            }
            key = 0;
            al_rest(0.1);
            al_flip_display();

        //IF RESPONSÁVEL POR CONTROLE DO FPS, PARARÁ O LOOP POR ALGUNS INSTANTES CASO CHEGUE A QUANTIDADE DE FRAME 
        //POR SEGUNDOS DEFIINIDO start_time e get_time.
        if(get_time() < 1.0/fps){
            al_rest((1.0/fps) - get_time());
        }

        }

    return 0;

}

//FUNÇÃO RESPONSÁVEL PELO EFEITO DA FADEOUT
void fadeout(int velocidade){

    ALLEGRO_BITMAP *buffer = NULL;
    buffer = al_create_bitmap(LARGURA_TELA, ALTURA_TELA);

    al_set_target_bitmap(buffer);
    al_draw_bitmap(al_get_backbuffer(janela), 0, 0, 0);      ///ooooooooooooooolllllllllllhhhhhhhhhhaaaaaaaaaaaaaaaaaarrrrrrrrrrr
    al_set_target_bitmap(al_get_backbuffer(janela));

        if (velocidade <= 0){
        velocidade = 1;
        }
        else if (velocidade > 10){
        velocidade = 10;
        }

        int alfa;
        for (alfa = 0; alfa <= 255; alfa += velocidade){
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        al_draw_tinted_bitmap(buffer, al_map_rgba(255 - alfa, 255 - alfa, 255 - alfa, alfa), 0, 0, 0);
        al_flip_display();
        al_rest(0.005); // NÃ£o Ã© necessÃ¡rio caso haja controle de FPS
        }

        al_destroy_bitmap(buffer);
}

//FUNÇÃO RESPONSÁVEL PELO EFEITO FADE IN
void fadein(ALLEGRO_BITMAP *imagem, int velocidade){
    if (velocidade < 0){
        velocidade = 1;
    }
    else if (velocidade > 10){
        velocidade = 10;
    }

    int alfa;

    for (alfa = 0; alfa <= 255; alfa += velocidade){
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_tinted_bitmap(imagem, al_map_rgba(alfa, alfa, alfa, alfa), 0, 0, 0);
        al_flip_display();
        al_rest(0.005); 
    }

}
//AQUI SE ENCONTRA A OPENING DO JOGO
//QUE USA A FUNÇÕES FADE IN E FADE OUT
//ALÉM DE TOCAR A MÚSICA DE ABERTURA
void Opening(){
    /*  al_attach_audio_stream_to_mixer(opening, al_get_default_mixer());
            al_set_audio_stream_playing(opening, true); 
            */
            fadein(opening_1,FADE_IN);
            fadeout(FADE_OUT);
            fadein(opening_2,FADE_IN);
            fadeout(FADE_OUT);
            al_draw_bitmap(menu1,0,0,0);
            al_rest(2);          
            al_flip_display();

}

////acaba com todos os ponteiros

void destroy(){


    al_destroy_display(janela);
    al_destroy_event_queue(evento);
    al_destroy_event_queue(evento_menu);
    al_destroy_timer(tempo);


    //Bitmaps that will be destroyed
    al_destroy_bitmap(image);
    al_destroy_bitmap(menu1);
    al_destroy_bitmap(play);
    al_destroy_bitmap(credits);
    al_destroy_bitmap(exit1);
    al_destroy_bitmap(game_back);
    al_destroy_bitmap(game_mode);
    al_destroy_bitmap(you_won);
    al_destroy_bitmap(you_lost);
    al_destroy_bitmap(initial);
    al_destroy_bitmap(help1);
    al_destroy_bitmap(credits1);
    al_destroy_bitmap(timesup);
    al_destroy_bitmap(opening_1);
    al_destroy_bitmap(opening_2);
    al_destroy_bitmap(opening_3);
    al_destroy_bitmap(opening_4);
    al_destroy_bitmap(presents);

    //Fonts that will be destroyed
    al_destroy_font(fonte);


    //Audios that will be destroyed
    al_destroy_sample(sample);
    al_destroy_audio_stream(opening);
    al_destroy_sample(you_lose_sample);
    al_destroy_sample(you_win_sample);
    al_destroy_sample(timesup_sample);



}



int main(void)
{

    if(!inicializa()){ // inicializa tudo para o jogo
        fprintf(stderr, "ERROR, Program not initialized correct.\n");
        return -1;
    }


    al_draw_bitmap(loading,0,0,0); // inicia a tela de loading do jogo
    al_flip_display();
    al_rest(0.8);

    fonte = al_load_font("arial.ttf", 72, 0); // fonte do loading escrita, ela bugava quando eu colocava fora da main, entao ficou aqui
    if (fonte == NULL){
        error_msg("Falha ao carregar \"terror.ttf\"");
        al_destroy_display(janela);
        return 0;
    }

    al_draw_textf(fonte, al_map_rgb(255,255,255),LARGURA_TELA/2,ALTURA_TELA/2,ALLEGRO_ALIGN_CENTRE, "LOADING ...");
    al_flip_display();
    al_rest(2.0);
    
    if(!loadBitmaps()){
                error_msg("erro nos bitmaps");
                return -1;
    }

    registerEvents(); //  para o menu
    Opening(); // antes do menu 
    al_destroy_bitmap(loading); 
    menu(); // inicia o menu
    //escolhePersonagem(); // funcao que quero fazer para escolher os personagens
    destroy();
    inicializar1();
    carregar_arquivos();
/////////////////////////////////////////   VARIAVEIS DA MAIN PARA O JOGO   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
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

        // crio uma instancia da struct jogador, pois vou mandar ela para o servidor
        Jogador PlayerOne;


    ///---* Tamanho dos SPRITES DE MAGIA --*//////
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

        // VARIAVEL PARA MOSTRAR OS FRAMES DE MOVIMENTACAO E DE MAGIA DE FORMA ADEQUADA E FLUIDA

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

            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                // caso seja hora de finalizar o jogo
                sair = 1;

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

            else if(evento.type == ALLEGRO_EVENT_KEY_UP)
            {
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
                        sair = 1;
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

                al_flip_display();
                
                desenha = 0;
            }
        }
// finalizando o programa
    al_destroy_bitmap(fundo);
    switch (personagem){
        case 1:
            al_destroy_bitmap(folha_sprite_Caio_Black); break;
        case 2:
            al_destroy_bitmap(folha_sprite_Alisson); break;
        case 3:
            al_destroy_bitmap(folha_sprite_Bianca); break;
        case 4:
            al_destroy_bitmap(folha_sprite_Calabria); break;
        case 5:
            al_destroy_bitmap(folha_sprite_Eric); break;
        case 6:
            al_destroy_bitmap(folha_sprite_Juan); break;
        case 7:
            al_destroy_bitmap(folha_sprite_Lael); break;
        }
    al_destroy_bitmap(fundo);
    al_destroy_bitmap(magia_Fire);
    al_destroy_bitmap(magia_explosionPurple);
    al_destroy_bitmap(magia_teleport);
    al_destroy_bitmap(magia_Blue_Fire);
    al_destroy_bitmap(magia_Gelo);
    al_destroy_bitmap(magia_splash);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    return 0;
    destroy(); // destroi tudo da aplicacao
    return 0;
}