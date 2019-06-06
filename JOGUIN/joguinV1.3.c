// para compilar   gcc joguin.c -o joguin -lallegro -lallegro_ttf -lallegro_image -lallegro_font -lallegro_dialog

    #include <allegro5/allegro.h>
    #include <allegro5/allegro_font.h>
    #include <allegro5/allegro_ttf.h>
    #include <allegro5/allegro_native_dialog.h>
    #include <allegro5/allegro_image.h>
     
    #define FPS 60.0
    #define LARGURA_TELA 400
    #define ALTURA_TELA 400
    
    typedef struct {
        int vida;
        int posX, posY;
    } Jogador;

    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *folha_sprite = NULL;
    ALLEGRO_BITMAP *fundo = NULL;
    ALLEGRO_BITMAP *magiaFogo = NULL;
     
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
        al_set_window_title(janela, "Joguinho");
     
     
        fila_eventos = al_create_event_queue();
        if(!fila_eventos) {
            error_msg("Falha ao criar fila de eventos");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            return 0;
        }

        //carrega a folha de sprites na variavel
        folha_sprite = al_load_bitmap("sprite_teste.png");
        if (!folha_sprite){
            error_msg("Falha ao carregar sprites");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

        magiaFogo = al_load_bitmap("/home/CIN/cmb2/Documents/Animation Pack/Fire.png")
        if(!magiaFogo){
            error_msg("Falha ao carregar sprites de fogo");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }

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
        
        //usa a cor rosa como transparencia
        al_convert_mask_to_alpha(folha_sprite,al_map_rgb(255,0,255));

        //carrega o fundo
        fundo = al_load_bitmap("mapinhanovo.png");
        if (!fundo){
            error_msg("Falha ao carregar fundo");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            al_destroy_bitmap(folha_sprite);
            return 0;
        }
     
        al_register_event_source(fila_eventos, al_get_display_event_source(janela));
        al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
        al_register_event_source(fila_eventos, al_get_keyboard_event_source());
        al_start_timer(timer);
     
        return 1;
    }
     
    int main(void){
        int desenha = 1;
        int sair = 0;
        // 16 x 32

        //vou usar para receber os comandos do teclado
        int tecla;

        //////////////////////// *-- DIMENSOES DO SPRITE DE MAGIA --*///////////////////////////////////////////

        // tamanho do sprite do fogo
        int largura_fogo = 100, altura_fogo = 128;
        // variaveis para selecionar qual sprite eu quero
        int coluna_fogo = 0, linha_fogo = 0;
        //////////////////////////////////////////////// KEYCODES //////////////////////////////////////////////////////

        ALLEGRO_KEY_X = 2



        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
     
        //largura e altura de cada sprite dentro da folha
        int altura_sprite = 32, largura_sprite = 16;
        //quantos sprites tem em cada linha da folha, e a atualmente mostrada
        int coluna_atual=0; //colunas_folha = 0;
        //quantos sprites tem em cada coluna da folha, e a atualmente mostrada
        int linha_atual=0; // linhas_folha=3;
        //posicoes X e Y da folha de sprites que serao mostradas na tela
        int regiao_x_folha=0, regiao_y_folha=0;
        //quantos frames devem se passar para atualizar para o proximo sprite
        int frames_sprite=2, cont_frames=0;
        //posicao X Y da janela em que sera mostrado o sprite
        int pos_x_sprite=150, pos_y_sprite=150;
        //velocidade X Y que o sprite ira se mover pela janela
        int vel_x_sprite=0, vel_y_sprite=0;
     
        if (!inicializar()){
            return -1;
        }

        int primeiro = 1;
        int segundo = 0;
     
        while(!sair){

            /* -- EVENTOS -- */

            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                // caso seja hora de finalizar o jogo
                sair = 1;

            }

            else if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
//////////////////* -- CONDICOES PARA OS SPRITES -- *//////////////////////////////////////////////
                    //verifica qual tecla foi pressionada
                    switch(evento.keyboard.keycode){
                    //seta para cima
                        case ALLEGRO_KEY_UP:
                            tecla = 1;
                            vel_y_sprite = -1;
                            linha_atual = 2;
                                if(primeiro){
                                coluna_atual = 0;
                                primeiro = 0;
                                segundo = 1;
                                break;
                            }
                            else if(segundo){
                                coluna_atual = 1;
                                segundo = 0;
                                primeiro = 1;
                                break;
                            }
                            break;
                    //seta para baixo
                        case ALLEGRO_KEY_DOWN:
                            tecla = 2;
                            vel_y_sprite = 1;
                            linha_atual = 0;
                            if(primeiro){
                                coluna_atual = 0;
                                segundo = 1;
                                primeiro = 0;
                                break;
                            }
                            else if(segundo){
                                coluna_atual = 1;
                                segundo = 0;
                                primeiro = 1;
                                break;
                            }
                            break;
                    //seta para esquerda
                        case ALLEGRO_KEY_LEFT:
                            tecla = 3;
                            vel_x_sprite = -1;
                            linha_atual = 1;
                            if(primeiro){
                                coluna_atual = 1;
                                segundo = 1;
                                primeiro = 0;
                                break;
                            }
                            else if(segundo){
                                coluna_atual = 2;
                                segundo = 0;
                                primeiro = 1;
                                break;
                            }
                            break;
                    //seta para direita.
                        case ALLEGRO_KEY_RIGHT:
                            tecla = 4;
                            vel_x_sprite = 1;
                            if(primeiro){
                                linha_atual = 0;
                                coluna_atual = 2;
                                segundo = 1;
                                primeiro = 0;
                                break;
                            }
                            else if(segundo){
                                linha_atual = 1;
                                coluna_atual = 0;
                                segundo = 0;
                                primeiro = 1;
                                break;
                            }
                            break;
                    //esc. sair=1 faz com que o programa saia do loop principal
                        case ALLEGRO_KEY_ESCAPE:
                            sair = 1;
                            break;
                        }
                    //crio um switch para adequar as velocidades ao movimento desejado
                }

            else if(evento.type == ALLEGRO_EVENT_KEY_UP)
            {
                //verifica qual tecla foi despressionada
                    switch(evento.keyboard.keycode){
                    //seta para cima
                    case ALLEGRO_KEY_UP:
                        tecla = 0;
                        vel_y_sprite = 0;
                        break;
                    //seta para baixo
                    case ALLEGRO_KEY_DOWN:
                        tecla = 0;
                        vel_y_sprite = 0;
                        break;
                    //seta para esquerda
                    case ALLEGRO_KEY_LEFT:
                        tecla = 0;
                        vel_x_sprite = 0;
                        break;
                    //seta para direita.
                    case ALLEGRO_KEY_RIGHT:
                        tecla = 0;
                        vel_x_sprite = 0;
                        break;
                    //esc. sair=1 faz com que o programa saia do loop principal
                    case ALLEGRO_KEY_ESCAPE:
                        sair = 1;
                        break;
                    }
            }

            else if(evento.type == ALLEGRO_EVENT_TIMER){
                //a cada disparo do timer, incrementa cont_frames
                cont_frames++;
                //se alcancou a quantidade de frames que precisa passar para  mudar para o proximo sprite
                if (cont_frames >= frames_sprite){
                    //reseta cont_frames
                    cont_frames=0;
                    //calcula a posicao Y da folha que sera mostrada
                    regiao_y_folha = linha_atual * altura_sprite;
                    //calcula a regiao X da folha que sera mostrada
                    regiao_x_folha = coluna_atual * largura_sprite;
                }
                //se o sprite estiver perto da borda direita ou esquerda da tela
                if ( pos_x_sprite + largura_sprite > LARGURA_TELA - 20 || pos_x_sprite < 20 ){
                    //inverte o sentido da velocidade X, para andar no outro sentido
                    vel_x_sprite = -vel_x_sprite;
                }
                if( pos_y_sprite + altura_sprite > ALTURA_TELA - 20 || pos_y_sprite < 20 ) {
                    //inverte o sentido da velocidade Y, para andar no outro sentido
                    vel_y_sprite = -vel_y_sprite;
                }
     
                //atualiza as posicoes X Y do sprite de acordo com a velocidade, positiva ou negativa
                pos_x_sprite += vel_x_sprite;
                pos_y_sprite += vel_y_sprite;
     
                desenha=1;
            
            ///////////////////////// * -- MAGIAS -- */////////////////////////////////////////////////////////////////////





            ////////////////////////////////////////////////////////////////////////////////////////////

            }
            
     
            /* -- ATUALIZA TELA -- */
            if(desenha && al_is_event_queue_empty(fila_eventos)) {
     
                //desenha o fundo na tela
                al_draw_bitmap_region(fundo,0,0,LARGURA_TELA,ALTURA_TELA,0,0,0);
     
                
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
     
        al_destroy_bitmap(folha_sprite);
        al_destroy_bitmap(fundo);
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
     
        return 0;
}