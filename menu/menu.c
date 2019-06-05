#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;

int main(void)
{
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_BITMAP *sair = NULL, *continuar = 0;
    ALLEGRO_FONT *fonte = NULL;
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    fonte = al_load_font("TOONISH.ttf", 40, 0);
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    al_set_window_title(janela, "Joguin");
    al_install_mouse();
    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    continuar = al_create_bitmap(200, 50);
    sair = al_create_bitmap(200, 50);
    fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    int saida = 0, saiu = 0, continuou = 0;
    while(!saida){
        while(!al_is_event_queue_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            // Se o evento foi de movimentação do mouse
            if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(continuar) / 2 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(continuar) / 2 &&
                    evento.mouse.y >= ALTURA_TELA / 4 - al_get_bitmap_height(continuar) / 2 &&
                    evento.mouse.y <= ALTURA_TELA / 4 + al_get_bitmap_height(continuar) / 2)
                {
                    continuou = 1;
                }
                else
                {
                    continuou = 0;
                }
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(sair) / 2 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(sair) / 2 &&
                    evento.mouse.y >= (3 * ALTURA_TELA) / 4 - al_get_bitmap_height(sair) / 2 &&
                    evento.mouse.y <= (3 * ALTURA_TELA) / 4 + al_get_bitmap_height(sair) / 2)
                {
                    saiu = 1;
                }
                else
                {
                    saiu = 0;
                }
            }
            // Ou se o evento foi um clique do mouse
            else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(sair) / 2 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(sair) / 2 &&
                    evento.mouse.y <= (3 * ALTURA_TELA) / 4 + al_get_bitmap_height(sair) / 2 &&
                    evento.mouse.y >= (3 * ALTURA_TELA) / 4 - al_get_bitmap_height(sair) / 2)
                {
                    saida = 1;
                }
            }
        }
        al_clear_to_color(al_map_rgb(0, 0, 0));
        // Continuar
        al_set_target_bitmap(continuar);
        if(continuou){
            al_clear_to_color(al_map_rgb(0, 255, 0));
        }
        else{
            al_clear_to_color(al_map_rgb(0, 0, 255));
        }
        // Sair

        al_set_target_bitmap(sair);

        if(saiu){
            al_clear_to_color(al_map_rgb(255, 0, 0));
        }
        else{
            al_clear_to_color(al_map_rgb(0, 0, 255));
        }

        al_draw_bitmap(continuar, LARGURA_TELA / 2 - al_get_bitmap_width(continuar) / 2,
                            ALTURA_TELA / 4 - al_get_bitmap_height(continuar) / 2, 0);
        al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 4 - 14, ALLEGRO_ALIGN_CENTRE, "Continuar");

        al_draw_bitmap(sair, LARGURA_TELA / 2- al_get_bitmap_width(sair) / 2,
                           (3 * ALTURA_TELA) / 4 - al_get_bitmap_height(sair) / 2, 0);
        al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, (3 * ALTURA_TELA) / 4 - 14, ALLEGRO_ALIGN_CENTRE, "Sair");

        al_flip_display();
    }
    al_destroy_font(fonte);
    al_destroy_bitmap(sair);
    al_destroy_bitmap(continuar);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    return 0;
}
