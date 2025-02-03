#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"

#define MATRIZ_LEDS 7
#define GREEN 11
#define BLUE 12
#define RED 13
#define BUTTON_A 5
#define BUTTON_B 6

//Garante que não haja possibilidade de iniciar múltiplos callbacks
bool alarmes_ativados=0;

void gpio_init_all(void){
gpio_init(RED);
    gpio_set_dir(RED, GPIO_OUT);

    gpio_init(GREEN);
    gpio_set_dir(GREEN, GPIO_OUT);

    gpio_init(BLUE);
    gpio_set_dir(BLUE, GPIO_OUT);
    
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
}

int64_t desligar_vermelho(){gpio_put(RED,0); return 0;}
int64_t desligar_azul(){gpio_put(BLUE,0); return 0;}
int64_t desligar_verde(){gpio_put(GREEN,0); alarmes_ativados=0; return 0;}


//Rotina de interrupção
void interrupt(uint gpio, uint32_t events)
{
 if(!alarmes_ativados){
  alarmes_ativados=1;
  gpio_put(RED,1);
  gpio_put(BLUE,1);
  gpio_put(GREEN,1);
  add_alarm_in_ms(3000, desligar_azul, NULL, false);
  add_alarm_in_ms(6000, desligar_vermelho, NULL, false);
  add_alarm_in_ms(9000, desligar_verde, NULL, false);
 }
}

int main()
{

    stdio_init_all();
    gpio_init_all();
    //A borda de descida do botão A ativa a função
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &interrupt);

    //Faz o LED vermelho piscar 5 vezes por segundo
    while (1)
    {   
     sleep_ms(5000);
     printf("Passou-se 5 segundos\n");
    }

    return 0; //Teoricamente, o programa nunca chegará aqui
}
