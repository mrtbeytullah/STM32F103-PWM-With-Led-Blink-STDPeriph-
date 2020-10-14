/*
 *  Bu uygulamada PWM kullanarak 4 adet ledin Duty cycle larýný degistirerek yakacagýz
 * tabi led yerine bir servo motorda koyup pulse ayarlamalýrýyla oynayarak servo motor
 * kontrol edebiliriz ve bunun gibi pek çok þeyi Ledin yerine kullanabiliriz
 *
 * PWM ile Led Blink uygulamasý yaparken gerek duyacagýmýz period ,prescaler ,pulse degerleri icin bazý formuller;
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

GPIO_InitTypeDef GPIO_InitStruct;        // Bu satýrda GPIO ayarlamalarýný yapmak icin Struct tanýmlýyoruz
TIM_TimeBaseInitTypeDef TIM_InitStruct; // Bu satýrda TIMER ayarlamalarýný yapmak icin Struct tanýmlýyoruz
TIM_OCInitTypeDef TIM_OCInitStruct;    // Bu satýrda PWM ayarlamalarýný yapmak icin Struct tanýmlýyoruz

  /*  Normalde GPIO,Timer...v.b ayarlamalarý int main fonksiyonu içinde de ayarlayabiliriz
   * ancak kodlarýmýzýn daha toplu olmasý için ayrý fonksiyonlar oluþturup onlarýn icine yazmanýzý tavsiye ederim
   */

void GPIO_Config()  // Bu satýrda GPIO ayarlamalarý icin Fonksiyon olusturup ayarlamalarý yapýyoruz
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_DeInit(GPIOA);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; // Bu Projede 4 Led kullanacagýmdan A portunun ilk dort pinini ayarladým
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&GPIO_InitStruct);
}
void TIM_Config()  // Bu satýrda Timer ayarlamalarý icin Fonksiyon olusturup ayarlamalarý yapýyoruz
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	TIM_DeInit(TIM2);

	TIM_InitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period=9999; //TIM2 kullandým APB1 Hattý Max. 36 MHz de calýsacagýndan period ve prescaleri ona gore ayarlayacagýz
	TIM_InitStruct.TIM_Prescaler=3599;
	TIM_InitStruct.TIM_RepetitionCounter=0;

	TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
	TIM_Cmd(TIM2,ENABLE);
}

void TIMOC_config()  // Bu satýrda PWM ayarlamalarý icin Fonksiyon olusturup ayarlamalarý yapýyoruz
{
	    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
		TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_High;
		TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
}

int main(void)
{
	GPIO_Config();    // Burada yaptýgýmýz GPIO ayarlamalarýnýn ayarlanmasý için olusturdugumuz ilgili fonksiyonu cagýrýyoruz.
	TIM_Config();    // Burada yaptýgýmýz Timer ayarlamalarýnýn ayarlanmasý için olusturdugumuz ilgili fonksiyonu cagýrýyoruz.
	TIMOC_config(); // Burada yaptýgýmýz PWM ayarlamalarýnýn ayarlanmasý için olusturdugumuz ilgili fonksiyonu cagýrýyoruz.


  while (1)
  {

 /*  simdi ise ledlerimizin devamlý yanmasý icn While icinde yukarýda vermis oldugum formulleri kullanarak
  * istedigimiz duty cycle ile pulse degerini ayarlayacagýz ve pulse degerlerine gore ledleri yakabilecegiz.
  */

	  TIM_OCInitStruct.TIM_Pulse=9999; // Burada 1.Ledimin tam yanmasý için duty cycle degerini %100 olarak aldým
	  TIM_OC1Init(TIM2,&TIM_OCInitStruct);
	  TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);

	  TIM_OCInitStruct.TIM_Pulse=7499;// Burada 2.Ledimin duty cycle degerini %75 olarak aldým
	  TIM_OC2Init(TIM2,&TIM_OCInitStruct);
	  TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);

	  TIM_OCInitStruct.TIM_Pulse=4999; // Burada 3.Ledimin duty cycle degerini %50 olarak aldým
	  TIM_OC3Init(TIM2,&TIM_OCInitStruct);
	  TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);

	  TIM_OCInitStruct.TIM_Pulse=2499; // Burada 4.Ledimin duty cycle degerini %25 olarak aldým
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
