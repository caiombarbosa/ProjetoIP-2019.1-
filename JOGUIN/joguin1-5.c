// para compilar   gcc joguin.c -o joguin -lallegro -lallegro_ttf -lallegro_image -lallegro_font -lallegro_dialog

    #include <stdio.h>
    #include <allegro5/allegro.h>
    #include <allegro5/allegro_font.h>
    #include <allegro5/allegro_ttf.h>
    #include <allegro5/allegro_native_dialog.h>
    #include <allegro5/allegro_image.h>
     
    #define FPS 60.0
    #define LARGURA_TELA 900
    #define ALTURA_TELA 900
    #define largura_mapa 5000
    #define altura_mapa 2500

    typedef struct {
        int vida;
        int posX, posY;
    } Jogador;

    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *folha_sprite = NULL;
    ALLEGRO_BITMAP *fundo = NULL;
    ALLEGRO_BITMAP *magia_Z = NULL;
    ALLEGRO_BITMAP *magia_X = NULL;
    ALLEGRO_BITMAP *magia_C = NULL;
    ALLEGRO_BITMAP *magia_V = NULL;
     
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

        folha_sprite = al_load_bitmap("zelda.png");
        if (!folha_sprite){
            error_msg("Falha ao carregar sprites");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magia_Z = al_load_bitmap("Burn.png");
        if (!magia_Z){
            error_msg("Falha ao carregar sprite da magia");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }
        magia_X = al_load_bitmap("iceBurst.png");
        if(!magia_X)
        {
            error_msg("Falha ao carregar sprite da magia");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }
        magia_C = al_load_bitmap("Thunder.png");
        if (!magia_C){
            error_msg("Falha ao carregar sprite da magia");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }
        magia_V = al_load_bitmap("Lazer.png");
        if (!magia_V){
            error_msg("Falha ao carregar sprite da magia");
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
            al_destroy_bitmap(folha_sprite);
            al_destroy_bitmap(magia_Z);
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
        int desenha = 1;
        int sair = 0;
        int frameAtual;
        int framesTotal;
        int frameReferencia = 0;
        int cameraX = 0;
        int cameraY = 0;
        char direcaoMagia;
        //essa variavel vai determinar para qual direcao a magia sera lancada
        int magiaPos = 0;
        //vou usar para receber os comandos do teclado
        char tecla;
     
//////////////////////////////////////////////////////////////////////////////// *-- DEFININDO OS TAMANHOS DE CADA SPRITE --*//////////////////////////////////////////////////////////////////////
        int i;

    ///*--SPRITES DE PERSONAGENS--*///
        //largura e altura de cada sprite dentro da folha
        //104 x 100
        int altura_sprite=104, largura_sprite=100;
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
        int altura_magia = 128, largura_magia = 128;
        // posicao da magia
        int pos_x_magiaZ = 0, pos_y_magiaZ = 0;
        int regiao_x_magia_Z = 0, regiao_y_magia_Z = 0;

        int pos_x_magiaX = 0, pos_y_magiaX = 0;
        int regiao_x_magia_X = 0, regiao_y_magia_X = 0;

        int pos_x_magiaC = 0, pos_y_magiaC = 0;
        int regiao_x_magia_C = 0, regiao_y_magia_C = 0;

        int pos_x_magiaV = 0, pos_y_magiaV = 0;
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
                                direcaoMagia = magiaPos;
                                frameReferencia = framesTotal;
                                tecla = 5;
                            }
                            break;

                    // Botão X magia de gelo
                        case ALLEGRO_KEY_X:
                            if(framesTotal - frameReferencia >= 60){
                                direcaoMagia = magiaPos;
                                frameReferencia = framesTotal;  
                                tecla = 6;
                            }
                            break;

                    // Botão C magia de raio
                        case ALLEGRO_KEY_C:
                            if(framesTotal - frameReferencia >= 60){
                                direcaoMagia = magiaPos;
                                frameReferencia = framesTotal;
                                tecla = 7;
                            }
                            break;

                    //Botão V magia de lazer
                        case ALLEGRO_KEY_V:
                            if(framesTotal - frameReferencia >= 60){ // cooldownzinho
                                direcaoMagia = magiaPos;
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
                        frameAtual = 0;
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
            
     
            ////////////////////////////////////////////////////////////* -- ATUALIZA TELA -- *///////////////////////////////////////////////////////////////////////////////
            if(desenha && al_is_event_queue_empty(fila_eventos)) {
     
                //desenha o fundo na tela
               // al_draw_bitmap_region(fundo,0,0,LARGURA_TELA,ALTURA_TELA,0,0,0);
                
                cameraX = pos_x_sprite - LARGURA_TELA/2;
                cameraY = pos_y_sprite - ALTURA_TELA/2;
                al_draw_bitmap(fundo,(cameraX*-1)-400,(cameraY*-1)-400,0);
            
                ////////////////////////////////////////////////* -- DESENHANDO AS MAGIAS QUE FORAM USADA --*///////////////////////////////////////////////////////////////////////////

                // vao ter 10 frames
                // tenho que desenhar elas frame por frame
                

                ////// --* MAGIA DE FOGO //////////////////////////////////////////////////////////////////
                // PARA BAIXO
                if(magiaPos == 'B' && tecla == 5 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                        printf("printando magia aqui!\n");
                        int cooldown = framesTotal - frameReferencia;
                        printf("%d", cooldown);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite + 250, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite + 200, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite + 150, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite + 100, 0);
                }
                // PARA CIMA
                if(magiaPos == 'C' && tecla == 5 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                        printf("printando magia aqui!\n");
                        int cooldown = framesTotal - frameReferencia;
                        printf("%d", cooldown);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite - 250, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite - 200, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite - 150, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite - 100, 0);
                }
                // PARA DIREITA
                if(magiaPos == 'D' && tecla == 5 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                        printf("printando magia aqui!\n");
                        int cooldown = framesTotal - frameReferencia;
                        printf("%d", cooldown);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite + 250,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite + 200,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite + 150,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite + 100,pos_y_sprite, 0);
                }
                // PARA ESQUERDA
                if(magiaPos == 'E' && tecla == 5 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                        printf("printando magia aqui!\n");
                        int cooldown = framesTotal - frameReferencia;
                        printf("%d", cooldown);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite - 250,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite - 200,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite - 150,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_Z,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite - 100,pos_y_sprite, 0);
                }

                ////////// *-- MAGIA DE GELO /////////////////////////////////////////////////
                if(magiaPos == 'B' && tecla == 6 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                        printf("printando magia aqui!\n"); 
                        int cooldown = framesTotal - frameReferencia;
                        printf("%d", cooldown);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite + 250, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite + 200, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite + 150, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite + 100, 0);
                }
                // PARA CIMA
                if(magiaPos == 'C' && tecla == 6 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                        printf("printando magia aqui!\n");
                        int cooldown = framesTotal - frameReferencia;
                        printf("%d", cooldown);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_Z,regiao_y_magia_Z+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite - 250, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite - 200, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite - 150, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite,pos_y_sprite - 100, 0);
                }
                // PARA DIREITA
                if(magiaPos == 'D' && tecla == 6 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                        printf("printando magia aqui!\n");
                        int cooldown = framesTotal - frameReferencia;
                        printf("%d", cooldown);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite + 250,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite + 200,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite + 150,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite + 100,pos_y_sprite, 0);
                }
                // PARA ESQUERDA
                if(magiaPos == 'E' && tecla == 6 && framesTotal - frameReferencia < 12 && frameReferencia != 0){
                        printf("printando magia aqui!\n");
                        int cooldown = framesTotal - frameReferencia;
                        printf("%d", cooldown);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite - 250,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite - 200,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite - 150,pos_y_sprite, 0);
                            al_draw_bitmap_region(magia_X,
                            regiao_x_magia_X,regiao_y_magia_X+(128*cooldown),
                            largura_magia,altura_magia,
                            pos_x_sprite - 100,pos_y_sprite, 0);
                }
                
                

                
                
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                
                //velocidade nula (parado)
                if(vel_y_sprite == 0 && vel_x_sprite == 0){
                    //desenha sprite na posicao X Y da janela, a partir da regiao X Y da folha
                    al_draw_bitmap_region(folha_sprite,
                        regiao_x_folha,regiao_y_folha,
                        largura_sprite,altura_sprite,
                        pos_x_sprite,pos_y_sprite,0);
                }
                // o sprite esta subindo
                else if(vel_y_sprite > 0){
                    pos_y_sprite += vel_y_sprite;
                    //desenha sprite na posicao X Y da janela, a partir da regiao X Y da folha
                    al_draw_bitmap_region(folha_sprite,
                        regiao_x_folha,regiao_y_folha,
                        largura_sprite,altura_sprite,
                        pos_x_sprite,pos_y_sprite,0);
                }
                // o sprite esta descendo
                else if(vel_y_sprite < 0){
                    pos_y_sprite += vel_y_sprite;
                    //desenha sprite na posicao X Y da janela, a partir da regiao X Y da folha
                    al_draw_bitmap_region(folha_sprite,
                        regiao_x_folha,regiao_y_folha,
                        largura_sprite,altura_sprite,
                        pos_x_sprite,pos_y_sprite,0);                
                }
                
                else if (vel_x_sprite > 0){
                    pos_x_sprite += vel_x_sprite;
                    //desenha sprite na posicao X Y da janela, a partir da regiao X Y da folha
                    al_draw_bitmap_region(folha_sprite,
                        regiao_x_folha,regiao_y_folha,
                        largura_sprite,altura_sprite,
                        pos_x_sprite,pos_y_sprite,0);
                }
                else if(vel_x_sprite < 0){
                    pos_x_sprite += vel_x_sprite;
                    //desenha sprite, igual acima, com a excecao que desenha a largura negativa, ou seja, espelhado horizontalmente
                    //desenha sprite na posicao X Y da janela, a partir da regiao X Y da folha
                    al_draw_bitmap_region(folha_sprite,
                        regiao_x_folha,regiao_y_folha,
                        largura_sprite,altura_sprite,
                        pos_x_sprite,pos_y_sprite,0);
                }

                al_flip_display();
                
                desenha = 0;
            }
        }
        al_destroy_bitmap(magia_Z);
        al_destroy_bitmap(folha_sprite);
        al_destroy_bitmap(fundo);
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return 0;
}