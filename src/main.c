/*
 *  Bu uygulamada PWM kullanarak 4 adet ledin Duty cycle lar�n� degistirerek yakacag�z
 * tabi led yerine bir servo motorda koyup pulse ayarlamal�r�yla oynayarak servo motor
 * kontrol edebiliriz ve bunun gibi pek �ok �eyi Ledin yerine kullanabiliriz
 *
 * PWM ile Led Blink uygulamas� yaparken gerek duyacag�m�z period ,prescaler ,pulse degerleri icin baz� formuller;
 *
 *    **************************************************
 *    *  Period= (Timer_Tick_Freq / PWM_Freq) -1       *
 *    *                                                *
 *    * PWM_Freq= Timer_Tick_Freq / (Period+1)         *
 *    *                                                *
 *    * Timer_Tick_Freq = Timer_Clk / (Prescaler +1)   *
 *    *                                                *
 *    * Pulse = [(Period+1) * (Duty Cycle)] -1         *
 *    **************************************************
 *    **************************
 *    * LinkedIn: mrtbeytullah *
 *    **************************
 */

#include <stddef.h>
#include "stm32f10x.h"

GPIO_InitTypeDef GPIO_InitStruct;        // Bu sat�rda GPIO ayarlamalar�n� yapmak icin Struct tan�ml�yoruz
TIM_TimeBaseInitTypeDef TIM_InitStruct; // Bu sat�rda TIMER ayarlamalar�n� yapmak icin Struct tan�ml�yoruz
TIM_OCInitTypeDef TIM_OCInitStruct;    // Bu sat�rda PWM ayarlamalar�n� yapmak icin Struct tan�ml�yoruz

  /*  Normalde GPIO,Timer...v.b ayarlamalar� int main fonksiyonu i�inde de ayarlayabiliriz
   * ancak kodlar�m�z�n daha toplu olmas� i�in ayr� fonksiyonlar olu�turup onlar�n icine yazman�z� tavsiye ederim
   */

void GPIO_Config()  // Bu sat�rda GPIO ayarlamalar� icin Fonksiyon olusturup ayarlamalar� yap�yoruz
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_DeInit(GPIOA);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; // Bu Projede 4 Led kullanacag�mdan A portunun ilk dort pinini ayarlad�m
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&GPIO_InitStruct);
}
void TIM_Config()  // Bu sat�rda Timer ayarlamalar� icin Fonksiyon olusturup ayarlamalar� yap�yoruz
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	TIM_DeInit(TIM2);

	TIM_InitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period=9999; //TIM2 kulland�m APB1 Hatt� Max. 36 MHz de cal�sacag�ndan period ve prescaleri ona gore ayarlayacag�z
	TIM_InitStruct.TIM_Prescaler=3599;
	TIM_InitStruct.TIM_RepetitionCounter=0;

	TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
	TIM_Cmd(TIM2,ENABLE);
}

void TIMOC_config()  // Bu sat�rda PWM ayarlamalar� icin Fonksiyon olusturup ayarlamalar� yap�yoruz
{
	    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
		TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_High;
		TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
}

int main(void)
{
	GPIO_Config();    // Burada yapt�g�m�z GPIO ayarlamalar�n�n ayarlanmas� i�in olusturdugumuz ilgili fonksiyonu cag�r�yoruz.
	TIM_Config();    // Burada yapt�g�m�z Timer ayarlamalar�n�n ayarlanmas� i�in olusturdugumuz ilgili fonksiyonu cag�r�yoruz.
	TIMOC_config(); // Burada yapt�g�m�z PWM ayarlamalar�n�n ayarlanmas� i�in olusturdugumuz ilgili fonksiyonu cag�r�yoruz.


  while (1)
  {

 /*  simdi ise ledlerimizin devaml� yanmas� icn While icinde yukar�da vermis oldugum formulleri kullanarak
  * istedigimiz duty cycle ile pulse degerini ayarlayacag�z ve pulse degerlerine gore ledleri yakabilecegiz.
  */

	  TIM_OCInitStruct.TIM_Pulse=9999; // Burada 1.Ledimin tam yanmas� i�in duty cycle degerini %100 olarak ald�m
	  TIM_OC1Init(TIM2,&TIM_OCInitStruct);
	  TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);

	  TIM_OCInitStruct.TIM_Pulse=7499;// Burada 2.Ledimin duty cycle degerini %75 olarak ald�m
	  TIM_OC2Init(TIM2,&TIM_OCInitStruct);
	  TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);

	  TIM_OCInitStruct.TIM_Pulse=4999; // Burada 3.Ledimin duty cycle degerini %50 olarak ald�m
	  TIM_OC3Init(TIM2,&TIM_OCInitStruct);
	  TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);

	  TIM_OCInitStruct.TIM_Pulse=2499; // Burada 4.Ledimin duty cycle degerini %25 olarak ald�m
	  TIM_OC4Init(TIM2,&TIM_OCInitStruct);
	  TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);
  }


}

void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}


void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
