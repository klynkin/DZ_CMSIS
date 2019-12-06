#include "main.h"
#include <stdbool.h>
#include <string.h>

char RxBuffer[RX_BUFF_SIZE];
char TxBuffer[TX_BUFF_SIZE];
bool ReceivedCmd;
char str[256];
	uint32_t counter=0;
	uint32_t counter2=0;
	
void TIM2_IRQHandler(void)
{
		TIM2->SR=~TIM_SR_UIF;
		
		if(counter<10 && counter2==0)
		counter++;
		else if(counter==10 && counter2==0)
		{
		counter2=1;
		counter--;		
		}
		else if (counter>1 && counter2 == 1)
		{
		counter--;	
		}
		else if (counter==1 && counter2 == 1)
		{
		counter++;
		counter2=0;
		}
	//	TIM2->CR1 = 0;
}


void EXTI0_IRQHandler()
{
		EXTI->PR |= EXTI_PR_PR0;
		GPIOA->ODR |=0x003F;
		counter=0;
		counter2=0;
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



void initTim2()
{
  RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 24000-1; //2кГц
	TIM2->ARR = 1000-1;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;
	NVIC_DisableIRQ(TIM2_IRQn);
}

void initGPIO()
{
	
	RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= 0xF0000000;
	GPIOA->CRL |= 0x03333333;
}



void USART1_IRQHandler()
{
if ((USART1->SR & USART_SR_RXNE) != 0)
	{	
		uint8_t tmp = USART1->DR;
		if (tmp==0x0D)
		{
			ReceivedCmd=true;
			return;	
		}
		else
		{
			RxBuffer[strlen(RxBuffer)]=tmp;
		}
	}	

}

void tx_str(char *str)
{
	strcat(str, "\r");
	for (uint8_t i=0; i < strlen(str); i++)
	{
		USART1->DR=str[i];
		while((USART1->SR & USART_SR_TC)==0);
	}
}


void ExecuteCmd() {
	float F;
	
	memset(TxBuffer,0,TX_BUFF_SIZE);

	 if (strncmp(RxBuffer,"*IDN?",8)==0)
			{
				sprintf(str,"Klynkin,IU4-71");
				strcpy(TxBuffer,str);
			}
		
	else if(strncmp(RxBuffer,"START",6)==0)
	{
		TIM2->CR1=1;
		NVIC_EnableIRQ(TIM2_IRQn);
		TIM2->CNT=0;
		strcpy(TxBuffer, "OK");
	}

else if(strncmp(RxBuffer,"STOP",6)==0)
	{
		TIM2->CR1=0;
		NVIC_DisableIRQ(TIM2_IRQn);
		strcpy(TxBuffer, "OK");
	}

else if(strncmp(RxBuffer,"SET",3)==0)
	{
	memmove(RxBuffer,RxBuffer+3,strlen(RxBuffer));
		sscanf(RxBuffer,"%d",&counter);
		
			if ((0<=counter)&&(counter<=9))
			{
				counter++;
				strcpy(TxBuffer, "OK");
			}
			else
			{
				strcpy(TxBuffer, "Out of range");
			}
	}
	
	else if(strncmp(RxBuffer,"GET",3)==0)
	{
		memmove(RxBuffer,RxBuffer+3,strlen(RxBuffer));
		sprintf(str,"counter= %d", counter-1);
		strcpy(TxBuffer, str);

	}

	else
	{
				strcpy(TxBuffer, "Unknown command");
	}
	
	ReceivedCmd=false;
	tx_str(TxBuffer);
	memset(RxBuffer,0,RX_BUFF_SIZE);
	
}

void init_uart()
{
	RCC->APB2ENR|=RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	RCC->APB2ENR|=RCC_APB2ENR_USART1EN;
	
	//PA3-RX
	GPIOA->CRH &=~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
	GPIOA->CRH &=~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
	
	
	GPIOA->CRH |= (GPIO_CRH_CNF10_0);
	GPIOA->CRH |= (GPIO_CRH_CNF9_1|GPIO_CRH_MODE9_1);

	//AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;
	USART1->CR1=0;
	USART1->CR2=0;
	USART1->CR1 = USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_TE |USART_CR1_RE;
	
	USART1->BRR=0x5E; //0x13 - целая часть, 8 - это 0,5 от дробной части, умноженное на 16 (по формуле)
	
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn,0);
	
	
}

void indication()
{
	GPIOA->ODR &= 0xFF80;				//скидываем значение порта
							
						switch (counter)
						{
							case 1:
							GPIOA->ODR |=0x003F;
							break;
							case 2:
							GPIOA->ODR |=0x0006;
							break;
							case 3:
							GPIOA->ODR |=0x005B;
							break;
							case 4:
							GPIOA->ODR |=0x004F;
							break;
							case 5:
							GPIOA->ODR |=0x0066;
							break;
							case 6:
							GPIOA->ODR |=0x006D;
							break;
							case 7:
							GPIOA->ODR |=0x007D;
							break;
							case 8:
							GPIOA->ODR |=0x0007;
							break;
							case 9:
							GPIOA->ODR |=0x007F;
							break;
							case 10:
							GPIOA->ODR |=0x006F;
							break;
						}
}

int main(void)
{
	initTim2();	
	initGPIO();
	initBtn();
	init_uart();

	memset(RxBuffer,0,RX_BUFF_SIZE);
	ReceivedCmd=false;
	
	while(1)
{
	if(ReceivedCmd)
	{
		ExecuteCmd();
	}
	indication();
}
}
