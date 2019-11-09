#include "main.h"

	uint16_t counter=0;
	uint16_t counter2=0;
void TIM2_IRQHandler(void)
{
	
		TIM2->SR=~TIM_SR_UIF;
		if(counter<999)
		counter++;
		else	
		counter=0;
}
void EXTI0_IRQHandler()
{
		
				EXTI->PR |= EXTI_PR_PR0;
				NVIC_DisableIRQ(TIM2_IRQn);

		
}
void EXTI1_IRQHandler()
{
		
			EXTI->PR |= EXTI_PR_PR0;
			NVIC_EnableIRQ(TIM2_IRQn);

		
}

void initBtn()
{
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPBEN;
	GPIOB->CRL &= ~GPIO_CRL_CNF0_0;
	GPIOB->CRL |= GPIO_CRL_CNF0_1;
	GPIOB->CRL	&= ~GPIO_CRL_MODE0 ;
        //подтягиваем к питанию
	GPIOB->BSRR |= GPIO_BSRR_BR0;
	
         //выбор порта и пина для внешнего прерывания
	 AFIO->EXTICR [0] |= AFIO_EXTICR1_EXTI0_PB;
         //по спадающему фронту
	 EXTI->FTSR |= EXTI_FTSR_TR0;
         //устанавливаем маску
   EXTI->IMR |= EXTI_IMR_MR0;
         //разрешаем прерывания  EXTI0
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn,1);
	
		
}



void initBtn2()
{
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPBEN;
	GPIOB->CRL &= ~GPIO_CRL_CNF1_0;
	GPIOB->CRL |= GPIO_CRL_CNF1_1;
	GPIOB->CRL	&= ~GPIO_CRL_MODE1 ;
        //подтягиваем к питанию
	GPIOB->BSRR |= GPIO_BSRR_BR1;
	
         //выбор порта и пина для внешнего прерывания
	 AFIO->EXTICR [0] |= AFIO_EXTICR1_EXTI1_PB;
         //по спадающему фронту
	 EXTI->FTSR |= EXTI_FTSR_TR1;
         //устанавливаем маску
   EXTI->IMR |= EXTI_IMR_MR1;
         //разрешаем прерывания  EXTI1
	
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_SetPriority(EXTI1_IRQn,1);
	
		
}

int main(void)
{
//	RCC->CR=RCC_CR_HSEON;
//	while((RCC->CR=RCC_CR_HSERDY == RESET));
  RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;
//	RCC->CR&=~RCC_CR_PLLON;
	TIM2->PSC = 36000-1; //2кГц
	TIM2->ARR = 2000-1;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM2_IRQn);
	
	RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= 0xF0000000;
	GPIOA->CRL |= 0x03333333;

	GPIOA->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_MODE8);	
	GPIOA->CRH |= (GPIO_CRH_MODE8_1);
	GPIOA->BSRR=GPIO_BSRR_BR8;
	GPIOA->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10);	
	GPIOA->CRH |= (GPIO_CRH_MODE10_1);
	GPIOA->BSRR=GPIO_BSRR_BR10;
	GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);	
	GPIOA->CRH |= (GPIO_CRH_MODE9_1);
	GPIOA->BSRR=GPIO_BSRR_BR9;
	initBtn();
	initBtn2();
	
	while(1)
	{
		
//		GPIOA->ODR &= 0xFF80;
//		GPIOA->BSRR=GPIO_BSRR_BS9;
//		GPIOA->BSRR=GPIO_BSRR_BR10;
//		GPIOA->ODR |=0x003F;
//		delay(10);
//		GPIOA->ODR &= 0xFF80;
//		GPIOA->BSRR=GPIO_BSRR_BS10;
//		GPIOA->BSRR=GPIO_BSRR_BR9;
//		GPIOA->ODR |=0x005B;
//		delay(10);
		

   // delay(10000000);
							
				
				
							GPIOA->ODR &= 0xFF80;
							counter2=counter%10;
							GPIOA->BSRR=GPIO_BSRR_BS9;
							GPIOA->BSRR=GPIO_BSRR_BS10;
							GPIOA->BSRR=GPIO_BSRR_BR8;
								
						switch (counter2)
						{
							case 0:
							GPIOA->ODR |=0x003F;
							break;
							
							case 1:
							GPIOA->ODR |=0x0006;
							break;
							
							case 2:
							GPIOA->ODR |=0x005B;
							break;
							
							case 3:
							GPIOA->ODR |=0x004F;
							break;
							
							case 4:
							GPIOA->ODR |=0x0066;
							break;
							
							case 5:
							GPIOA->ODR |=0x006D;
							break;
							
							case 6:
							GPIOA->ODR |=0x007D;
							break;
							
							case 7:
							GPIOA->ODR |=0x0007;
							break;
							
							case 8:
							GPIOA->ODR |=0x007F;
							break;
							
							case 9:
							GPIOA->ODR |=0x006F;
							break;
						
				}

				counter2=counter/10;
				if (counter2>0)
				counter2=counter2%10;	
				else continue;
							
							GPIOA->ODR &= 0xFF80;
							GPIOA->BSRR=GPIO_BSRR_BS10;
							GPIOA->BSRR=GPIO_BSRR_BS8;			
							GPIOA->BSRR=GPIO_BSRR_BR9;

				switch (counter2)
						{
							case 0:
							GPIOA->ODR |=0x003F;
							break;
							
							case 1:
							GPIOA->ODR |=0x0006;
							break;
							
							case 2:
							GPIOA->ODR |=0x005B;
							break;
							
							case 3:
							GPIOA->ODR |=0x004F;
							break;
							
							case 4:
							GPIOA->ODR |=0x0066;
							break;
							
							case 5:
							GPIOA->ODR |=0x006D;
							break;
							
							case 6:
							GPIOA->ODR |=0x007D;
							break;
							
							case 7:
							GPIOA->ODR |=0x0007;
							break;
							
							case 8:
							GPIOA->ODR |=0x007F;
							break;
							
							case 9:
							GPIOA->ODR |=0x006F;
							break;
						
				}
						
				
				counter2=counter/100;
				if (counter2>0)
				counter2=counter2%10;	
				else continue;
							
							GPIOA->ODR &= 0xFF80;
							GPIOA->BSRR=GPIO_BSRR_BS8;			
							GPIOA->BSRR=GPIO_BSRR_BS9;
							GPIOA->BSRR=GPIO_BSRR_BR10;

				switch (counter2)
						{
							case 0:
							GPIOA->ODR |=0x003F;
							break;
							
							case 1:
							GPIOA->ODR |=0x0006;
							break;
							
							case 2:
							GPIOA->ODR |=0x005B;
							break;
							
							case 3:
							GPIOA->ODR |=0x004F;
							break;
							
							case 4:
							GPIOA->ODR |=0x0066;
							break;
							
							case 5:
							GPIOA->ODR |=0x006D;
							break;
							
							case 6:
							GPIOA->ODR |=0x007D;
							break;
							
							case 7:
							GPIOA->ODR |=0x0007;
							break;
							
							case 8:
							GPIOA->ODR |=0x007F;
							break;
							
							case 9:
							GPIOA->ODR |=0x006F;
							break;
					
				}
				
				
														

				
	}
}

void delay(uint32_t t)
{
	for( uint32_t i=0; i<=t; i++);
}