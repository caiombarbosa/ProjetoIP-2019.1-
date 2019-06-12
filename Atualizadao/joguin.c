// para compilar   gcc joguin.c -o joguin -lallegro -lallegro_ttf -lallegro_image -lallegro_font -lallegro_dialog

    #include <stdio.h>
    #include <allegro5/allegro.h>
    #include <allegro5/allegro_font.h>
    #include <allegro5/allegro_ttf.h>
    #include <allegro5/allegro_native_dialog.h>
    #include <allegro5/allegro_image.h>
     
    #define FPS 60.0
    #define LARGURA_TELA 1240
    #define ALTURA_TELA 900
    #define largura_mapa 5000
    #define altura_mapa 2500
					
					//--* VARIAVES GLOBAIS DE VERDADE --*//	

	// variaveis que vou usar para ciclar entre os sprites de movimentacao dos personagens
	int andandoDireita[] = {};
    int andandoEsquerda[] = {};
    int andandoCima[] = {};
    int andandoBaixo[] = {};


	// variavel global do tamanho dos sprites do personagens
    int altura_sprite=100, largura_sprite=80;

    typedef struct {
        int vida;
        int posX, posY;
    } Jogador;


    ALLEGRO_DISPLAY *janela = NULL;
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

/////////////////////////////////////////////////////*-- FUNCOES --*////////////////////////////////////////////

void desenhaSpritePersonagem(char magiaPos, int pos_x_sprite, int pos_y_sprite){
    	if(magiaPos == 'C'){
            if(andandoCima[0] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
                    0,altura_sprite*3, // posicao para tirar da folha dos sprites
                    largura_sprite,altura_sprite, // tamanho para tirar
                    pos_x_sprite,pos_y_sprite,0); //  onde colocar a imagem, flags
            }
            if(andandoCima[1] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
                  	largura_sprite*1,altura_sprite*3,
                   	largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
            }
          	if(andandoCima[2] == 1){
               	al_draw_bitmap_region(folha_sprite_Caio_Black,
          		   	largura_sprite*2,altura_sprite*3,
                    largura_sprite,altura_sprite,
                   	pos_x_sprite,pos_y_sprite,0);
            }
        }
        if(magiaPos == 'B'){
            if(andandoBaixo[0] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
                    0,0,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
            }
        	if(andandoBaixo[1] == 1){
             	al_draw_bitmap_region(folha_sprite_Caio_Black,
                    largura_sprite*1,altura_sprite*0,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
           	}
            if(andandoBaixo[2] == 1){
                al_draw_bitmap_region(folha_sprite_Caio_Black,
             	    largura_sprite*2,altura_sprite*0,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
            }
        }
        if(magiaPos == 'D'){
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
        if(magiaPos == 'E'){
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void error_msg(char *text){
    	al_show_native_message_box(janela,"ERRO",
    		"Ocorreu o seguinte erro e o programa sera finalizado:",
    		text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
    }
     
    int inicializar(){
        if (!al_init()){
            error_msg("Falha ao inicializar a Allegro");
            return 0;
        }

        if (!al_init_image_addon()){
            error_msg("Falha ao inicializar o addon de imagens");
            return 0;
        }
     
        timer = al_create_timer(1.0 / FPS);
        if(!timer) {
            error_msg("Falha ao criar temporizador");
            return 0;
        }

        janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
        if(!janela) {
            error_msg("Falha ao criar janela");
            al_destroy_timer(timer);
            return 0;
        }
        al_set_window_title(janela, "Wizard King");
     
     
        fila_eventos = al_create_event_queue();
        if(!fila_eventos) {
            error_msg("Falha ao criar fila de eventos");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            return 0;
        }

//////////////////////////////////////// *-- CARREGANDO SPRITES --*////////////////////////////////////////////////////

///////////////////////////////// *-- SṔRITES DE PERSONAGENS --* /////////////////////////////////////////////////////


        folha_sprite_Caio_White = al_load_bitmap("caioandandoWhite.png");
        if (!folha_sprite_Caio_White){
            error_msg("Falha ao carregar sprites 1");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }
        folha_sprite_Caio_White_Batalha = al_load_bitmap("caioemposedebatalhaWhite.png");
        if (!folha_sprite_Caio_White_Batalha){
            error_msg("Falha ao carregar sprites 2");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Caio_Black = al_load_bitmap("caioandandoBlack.png");
        if (!folha_sprite_Caio_Black){
            error_msg("Falha ao carregar sprites 3");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Caio_Black_Batalha = al_load_bitmap("caioemposedebatalhaBlack.png");
        if (!folha_sprite_Caio_Black_Batalha){
            error_msg("Falha ao carregar sprites 4");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Juan = al_load_bitmap("juanandando.png");
        if (!folha_sprite_Juan){
            error_msg("Falha ao carregar sprites 5");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Juan_Batalha = al_load_bitmap("juanemposedebatalha.png");
        if (!folha_sprite_Juan_Batalha){
            error_msg("Falha ao carregar sprites 6");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Bianca = al_load_bitmap("biancaandando.png");
        if (!folha_sprite_Bianca){
            error_msg("Falha ao carregar sprites 7");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Bianca_Batalha = al_load_bitmap("biancaemposedebatalha.png");
        if (!folha_sprite_Bianca_Batalha){
            error_msg("Falha ao carregar sprites 8");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Alisson = al_load_bitmap("alissonandando.png");
        if (!folha_sprite_Alisson){
            error_msg("Falha ao carregar sprites 9");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Alisson_Batalha = al_load_bitmap("alissonemposedebatalha.png");
        if (!folha_sprite_Alisson_Batalha){
            error_msg("Falha ao carregar sprites 10");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Calabria = al_load_bitmap("calabriaandando.png");
        if (!folha_sprite_Calabria){
            error_msg("Falha ao carregar sprites 11");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Calabria_Batalha = al_load_bitmap("calabriaemposedebatalha.png");
        if (!folha_sprite_Calabria_Batalha){
            error_msg("Falha ao carregar sprites 12");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Eric = al_load_bitmap("ericandando.png");
        if (!folha_sprite_Eric){
            error_msg("Falha ao carregar sprites 13");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Eric_Batalha = al_load_bitmap("ericemposedebatalha.png");
        if (!folha_sprite_Eric_Batalha){
            error_msg("Falha ao carregar sprites 14");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Lael = al_load_bitmap("laelandando.png");
        if (!folha_sprite_Lael){
            error_msg("Falha ao carregar sprites 15");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        folha_sprite_Lael_Batalha = al_load_bitmap("laelemposedebatalha.png");
        if (!folha_sprite_Lael_Batalha){
            error_msg("Falha ao carregar sprites 16");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        //////////////// *-- SPRITES DE MAGIA --*/////////////////////////////////////////////////////////////////

        magia_Fire = al_load_bitmap("Burn.png");
        if (!magia_Fire){
            error_msg("Falha ao carregar sprite da magia 1");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }
        magia_Gelo = al_load_bitmap("iceBurst.png");
        if(!magia_Gelo)
        {
            error_msg("Falha ao carregar sprite da magia 2");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }
        magia_Thunder = al_load_bitmap("Thunder.png");
        if (!magia_Thunder){
            error_msg("Falha ao carregar sprite da magia 3");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }
        magia_Lazer = al_load_bitmap("Lazer.png");
        if (!magia_Lazer){
            error_msg("Falha ao carregar sprite da magia 4");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magia_Blue_Energy_Ball = al_load_bitmap("blueEnergyBall.png");
        if (!magia_Blue_Energy_Ball){
            error_msg("Falha ao carregar sprite da magia 5");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magia_Blue_Fire = al_load_bitmap("blueFlame.png");
        if (!magia_Blue_Fire){
            error_msg("Falha ao carregar sprite da magia 6");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magia_explosionPurple = al_load_bitmap("explosionPurple.png");
        if (!magia_explosionPurple){
            error_msg("Falha ao carregar sprite da magia 7");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magia_green_Energy_Ball = al_load_bitmap("greenEnergyBall.png");
        if (!magia_green_Energy_Ball){
            error_msg("Falha ao carregar sprite da magia 8");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magia_ice_splash = al_load_bitmap("iceSplash.png");
        if (!magia_ice_splash){
            error_msg("Falha ao carregar sprite da magia 9");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magia_lightningRadial = al_load_bitmap("lightning_radial.png");
        if (!magia_lightningRadial){
            error_msg("Falha ao carregar sprite da magia 10");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magia_red_Energy_Ball = al_load_bitmap("redEnergyBall.png");
        if (!magia_red_Energy_Ball){
            error_msg("Falha ao carregar sprite da magia 11");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magia_splash = al_load_bitmap("Splash.png");
        if (!magia_splash){
            error_msg("Falha ao carregar sprite da magia 12");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magia_teleport = al_load_bitmap("teleportLightning.png");
        if (!magia_teleport){
            error_msg("Falha ao carregar sprite da magia 13");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magia_yellow_Energy_Ball = al_load_bitmap("yellowEnergyBall.png");
        if (!magia_yellow_Energy_Ball){
            error_msg("Falha ao carregar sprite da magia 14");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

//////////////////////////////////////////////////////////////////////-- * CARREGANDO AS OUTRAS PARADAS --*////////////////////////////////////////////////////

        if(!al_init_font_addon()){
            error_msg("Falha ao incializar o font addon!");
            return 0;
        }
        
        if (!al_init_ttf_addon()){
            error_msg("Falha ao inicializar add-on allegro_ttf");
            return 0;
        }
 
        //inicializa addon do teclado
        if (!al_install_keyboard()){
            error_msg("Falha ao inicializar o teclado");
            return 0;
        }

        //carrega o fundo
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
            return 0;
        }
     
        al_register_event_source(fila_eventos, al_get_display_event_source(janela));
        al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
        al_register_event_source(fila_eventos, al_get_keyboard_event_source());
        al_start_timer(timer);
     
        return 1;
    }

/////////////////////////////////////////   VARIAVEIS 'GLOBAIS'   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int main(void){
        andandoDireita[0] = 1;
        andandoBaixo[0] = 1;
        andandoEsquerda[0] = 1;
        andandoEsquerda[0] = 1;
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
        char magiaPos = 0;
        //vou usar para receber os comandos do teclado
        char tecla;
//////////////////////////////////////////////////////////////////////////////// *-- DEFININDO OS TAMANHOS DE CADA SPRITE --*//////////////////////////////////////////////////////////////////////

    ///*--SPRITES DE PERSONAGENS--*///
        //largura e altura de cada sprite dentro da folha de personagens
        //104 x 100
        int altura_sprite=100, largura_sprite=60;
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
    
    ///*-- POSICOES DAS MAGIAS NA FOLHA PARA DESENHA-LAS --*///////////

        int regiao_x_magia_Z = 0, regiao_y_magia_Z = 0;
        
        int regiao_x_magia_X = 0, regiao_y_magia_X = 0;
        
        int regiao_x_magia_C = 0, regiao_y_magia_C = 0;

        int regiao_x_magia_V = 0, regiao_y_magia_V = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!inicializar()){
            return -1;
        }
     
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
                            break;
                    //seta para baixo
                        case ALLEGRO_KEY_DOWN:
                            tecla = 2;
                            magiaPos = 'B';
                            vel_y_sprite = 2;
                            linha_atual = 0;
                            break;
                    //seta para esquerda
                        case ALLEGRO_KEY_LEFT:
                            tecla = 3;
                            magiaPos = 'E';
                            vel_x_sprite = -2;
                            linha_atual = 1;
                            break;
                    //seta para direita.
                        case ALLEGRO_KEY_RIGHT:
                            tecla = 4;
                            magiaPos = 'D';
                            vel_x_sprite = 2;
                            linha_atual = 3;
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
                            if(framesTotal - frameReferencia >= 60){
                                frameReferencia = framesTotal;  
                                tecla = 6;
                            }
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
                    //esc. sair=1 faz com que o programa saia do loop principal
                    case ALLEGRO_KEY_ESCAPE:
                        sair = 1;
                        break;
                    }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            }

            else if(evento.type == ALLEGRO_EVENT_TIMER){
                //a cada disparo do timer, incrementa cont_frames
                cont_frames++;
                framesTotal += 1;
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
     
                //atualiza as posicoes X Y do sprite de acordo com a velocidade, positiva ou negativa
                pos_x_sprite += vel_x_sprite;
                pos_y_sprite += vel_y_sprite;
     
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
                    // PARA BAIXO
                    if(magiaPos == 'B' && tecla == 5 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                            printf("printando magia aqui!\n");
                            int cooldown = framesTotal - frameReferencia;
                            printf("%d", cooldown);
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
                                pos_x_sprite,pos_y_sprite - 100, 0);
                    }
                    // PARA DIREITA
                    if(magiaPos == 'D' && tecla == 5 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                            printf("printando magia aqui!\n");
                            int cooldown = framesTotal - frameReferencia;
                            printf("%d", cooldown);
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

                    if(tecla == 6 && framesTotal - frameReferencia < 34 && frameReferencia != 0){
                            printf("printando magia aqui!\n");
                            int cooldown = framesTotal - frameReferencia;
                            if(cooldown/8 > 1){
                                regiao_x_magia_Z = 1;
                                cooldown -= 8;
                                if(cooldown/16 > 1){
                                    regiao_x_magia_Z = 2;
                                    cooldown -= 16;
                                    if(cooldown/24 > 1){
                                        cooldown -=  24;
                                        regiao_x_magia_Z = 3;
                                    }
                                }
                            }
                            printf("%d", cooldown);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite,pos_y_sprite + 100, 0);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite,pos_y_sprite - 100, 0);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite - 100,pos_y_sprite, 0);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite + 100,pos_y_sprite, 0);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite + 100,pos_y_sprite + 100, 0);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite - 100,pos_y_sprite -100, 0);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite - 100,pos_y_sprite +100, 0);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite + 100,pos_y_sprite -100, 0);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite + 200,pos_y_sprite -200, 0);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite - 200,pos_y_sprite +200, 0);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite - 200,pos_y_sprite -200, 0);
                                al_draw_bitmap_region(magia_Fire,
                                regiao_x_magia_Z,regiao_y_magia_Z+(largura_magia_roxa*(cooldown-1)),
                                largura_magia,altura_magia,
                                pos_x_sprite + 200,pos_y_sprite + 200, 0);
                    }

                    if(tecla == 7 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                        int cooldown = framesTotal - frameReferencia;
                    }
                }

//--------------------------------------------//////////// *-- MAGIA DE GELO (X) ////////////////--------------------------------------------/////
                
                //PARA BAIXO
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
                        if(magiaPos == 'D'){
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

                    // o sprite esta indo para direita ou esquerda
                    else if(vel_x_sprite > 0 || vel_x_sprite < 0){
                        // adequando as posicoes de acordo com a velocidade
                        pos_x_sprite += vel_x_sprite;
                    	int linhas = 0;

                        // tenho 3 sprites para cada direcao
                        if(magiaPos == 'C'){
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
                        if(magiaPos == 'D'){
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
                            break;
                        case 'B':
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
                            break;
                        case 'D':
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
                            break;
                        case 'E':
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
                            break;
                }
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                al_flip_display();
                
                desenha = 0;
            }
        }
        al_destroy_bitmap(fundo);
        al_destroy_bitmap(folha_sprite_Caio_Black);
        al_destroy_bitmap(fundo);
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return 0;
}
