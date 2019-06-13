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
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define largura 800
#define altura 500
#define fps 45
#define FADE_IN 2.5
#define FADE_OUT 4

//THIS IS THE INITIALIZATION OF THE MAIN VARIABLES
ALLEGRO_DISPLAY  *janela = NULL;
ALLEGRO_EVENT_QUEUE *evento = NULL;
ALLEGRO_EVENT_QUEUE *evento_menu = NULL;
ALLEGRO_TIMER *tempo = NULL;


double start_time = 0;
//======================================
//variáveis de aldio

ALLEGRO_AUDIO_STREAM *somMenu =NULL;
ALLEGRO_AUDIO_STREAM *opening = NULL;


ALLEGRO_SAMPLE *cheater_sample = NULL;
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
ALLEGRO_BITMAP *dicas = NULL;
ALLEGRO_BITMAP *cheater = NULL;
ALLEGRO_BITMAP *opening_1 = NULL;
ALLEGRO_BITMAP *opening_2 = NULL;
ALLEGRO_BITMAP *opening_3 = NULL;
ALLEGRO_BITMAP *opening_4 = NULL;
ALLEGRO_BITMAP *start_0 = NULL;
ALLEGRO_BITMAP *start_1 = NULL;
ALLEGRO_BITMAP *start_2 = NULL;
ALLEGRO_BITMAP *start_3 = NULL;
ALLEGRO_BITMAP *start_4 = NULL;
ALLEGRO_BITMAP *presents = NULL;
ALLEGRO_BITMAP *dicass = NULL;

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


//INICIALIZAÇÃO DAS BIBLIOTECAS============
bool inicializa(){

    if(!al_init()){
      error_msg("Falha ao inicializar a Allegro");
        return false;
    }


    if(!al_init_image_addon()){
        error_msg("Falha ao iniciar A.I");
        return false;
    }


    if(!al_init_font_addon()){
     error_msg("Falha ao iniciar A.F");
        return false;
    }
    
    if(!al_init_ttf_addon()){
     error_msg("Falha ao iniciar A.F");
        return false;
    

    }
    if(!al_init_primitives_addon()){
        error_msg("Falha ao iniciar A.P");
        return false;
    }


    janela = al_create_display(largura,altura);

    if(janela==NULL){
         error_msg("Falha ao iniciar J");
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
    if(!al_install_keyboard()){
         error_msg("Falha ao iniciar K");
        al_destroy_display(janela);
        return false;
    }
    if(!al_install_mouse()){
         error_msg("Falha ao iniciar M");
        al_destroy_display(janela);
        return false;
    }

     if (!al_install_audio()){
        fprintf(stderr, "Falha ao inicializar Ãudio.\n");
        return false;
    }

    if (!al_init_acodec_addon()){
        fprintf(stderr, "Falha ao inicializar codecs de Ãudio.\n");
        return false;
    }
    

    if (!al_reserve_samples(1)){
        fprintf(stderr, "Falha ao alocar canais de Ãudio.\n");
        return false;
    }
    
 

    return true;
}







bool loadBitmaps(){

    
    //THE BITMAPS WILL BE INITIALIZED HERE
    image = al_load_bitmap("top-3.jpg");

    //MENU
    menu1 = al_load_bitmap("menu.jpg");
    play = al_load_bitmap("top-3.jpg");
    help = al_load_bitmap("top-3.jpg");
    credits = al_load_bitmap("top-3.jpg");
    exit1 = al_load_bitmap("top-3.jpg");
    
    game_mode = al_load_bitmap("top-3.jpg");
    game_back = al_load_bitmap("top-3.jpg");

    you_won = al_load_bitmap("top-3.jpg");
    you_lost = al_load_bitmap("top-3.jpg");

    initial = al_load_bitmap("top-3.jpg");
    timesup = al_load_bitmap("top-3.jpg");
    credits1 = al_load_bitmap("top-3.jpg");
    help1 = al_load_bitmap("top-3.jpg");
    dicas = al_load_bitmap("top-3.jpg");
    cheater = al_load_bitmap("top-3.jpg");


    opening_1 = al_load_bitmap("start_1.png");
    opening_2 = al_load_bitmap("start_2new.jpg");
    opening_3 = al_load_bitmap("start_3.jpeg");
    opening_4 = al_load_bitmap("Night of Terror_banner.jpg");


    start_0 = al_load_bitmap("top-3.jpg");
    start_1 = al_load_bitmap("top-3.jpg");
    start_2 = al_load_bitmap("top-3.jpg");
    start_3 = al_load_bitmap("top-3.jpg");
    start_4 = al_load_bitmap("top-3.jpg");
    presents = al_load_bitmap("top-3.jpg");


    dicass = al_load_bitmap("top-3.jpg");

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

    if(dicas == NULL){
        error_msg("Falha na dicas");
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


    if(start_0 == NULL){
        error_msg("Falha na start0");
        al_destroy_display(janela);
        return false;

    }

    if(start_1 == NULL){
        error_msg("Falha na start1");
        al_destroy_display(janela);
        return false;

    }

    if(start_2 == NULL){
        error_msg("Falha na start1");
        al_destroy_display(janela);
        return false;

    }

    
    return true;

    }
    //RESPONSÁVEL POR VERIFICAR ERROS=====================================
void error_msg(char *text){
	al_show_native_message_box(janela,"ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
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
                        return 0;
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

                    al_draw_bitmap(game_mode,0,0,0);
                    al_rest(0.1);
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
    buffer = al_create_bitmap(largura, altura);
    
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
            fadein(opening_1,FADE_IN);
            fadeout(FADE_OUT);
            fadein(opening_2,FADE_IN);
            fadeout(FADE_OUT);
            fadein(opening_3,FADE_IN);
            fadeout(FADE_OUT);
            fadein(opening_4,FADE_IN);
            fadeout(FADE_OUT);
            fadein(presents,FADE_IN);
            fadeout(FADE_OUT);
            fadein(opening_2,FADE_IN);
            fadeout(FADE_OUT);       
            fadein(opening_3,FADE_IN);
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
    al_destroy_bitmap(cheater);
    al_destroy_bitmap(dicas);
    al_destroy_bitmap(opening_1);
    al_destroy_bitmap(opening_2);
    al_destroy_bitmap(opening_3);
    al_destroy_bitmap(opening_4);
    al_destroy_bitmap(start_0);
    al_destroy_bitmap(start_1);
    al_destroy_bitmap(start_2);
    al_destroy_bitmap(start_3);
    al_destroy_bitmap(presents);

    //Fonts that will be destroyed
    al_destroy_font(fonte);
    

    //Audios that will be destroyed
    al_destroy_audio_stream(somMenu);
    al_destroy_audio_stream(opening);
    al_destroy_sample(you_lose_sample);
    al_destroy_sample(you_win_sample);
    al_destroy_sample(cheater_sample);
    al_destroy_sample(timesup_sample);
    


}



int main(void)
{
    ALLEGRO_BITMAP *loading = NULL;
    
    if(!inicializa()){
        fprintf(stderr, "ERROR, Program not initialized correct.\n");
        return -1;
    }

    if(!loadBitmaps()){
        error_msg("erro nos bitmaps");
        return -1;
    }
    loading = al_load_bitmap("opening1.jpg");
    if(loading == NULL){
         error_msg("Falha ao inicializar a Allegro");
        return -1;
    }
    al_draw_bitmap(loading,0,0,0);
    al_flip_display();
    al_rest(0.8);
 
    fonte = al_load_font("terror.ttf", 72, 0);
    if (fonte == NULL){
        error_msg("Falha ao carregar \"terror.ttf\"");
        al_destroy_display(janela);
        return 0;
    }
    al_draw_textf(fonte, al_map_rgb(255,255,255),largura/2,altura/2,ALLEGRO_ALIGN_CENTRE, "LOADING ...");
    al_flip_display();
    al_rest(2.0);

    registerEvents();
    Opening();
    al_destroy_bitmap(loading);
    menu();

    destroy();   
    return 0;
}