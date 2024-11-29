#include "system_sam3x.h"
#include "at91sam3x8.h"

int counter  = 1000; //count++ each interupt
int isCount = 0; //bool to handle if we should count
int bPressed = 0;
void init(void){
  SysTick_Config((SystemCoreClock/1000)-1);
  
  (*AT91C_PIOD_IFER) = (1<<1);
  *((volatile uint32_t *)0x400E1484) = (1<<1);
  *((volatile uint32_t *)0x400E148C) = 0xff;
  *((volatile uint32_t *)0x400E14B0) = (1<<1);
  (*AT91C_PIOD_IER) = (1<<1);
  (*AT91C_PIOD_IFER) = (1<<1);
   NVIC_ClearPendingIRQ(PIOD_IRQn);
   NVIC_SetPriority(PIOD_IRQn,2);
   NVIC_EnableIRQ(PIOD_IRQn);
}

void PIOD_Handler(void){
   if((*AT91C_PIOD_ISR) & (1<<1))
     if(isCount == 1)
       isCount = 0;
   else 
     isCount = 1;
}

void Set_Led(unsigned int nLed){
  (*AT91C_PMC_PCER) = (1<<14);
  
  (*AT91C_PIOD_PER) = (1<<3);
  
  (*AT91C_PIOD_OER) = (1<<3);
  
  if(nLed == 1)
    (*AT91C_PIOD_SODR) = AT91C_PIO_PD3;
  else
    (*AT91C_PIOD_CODR) = AT91C_PIO_PD3;
}

void SysTick_Handler(void){
  if(isCount) counter++;
  if((counter % 2000) > 1000){
   Set_Led(1);
  }
  else{
    Set_Led(0);
  }
}

void ReadButton(unsigned int *nButton){
  (*AT91C_PMC_PCER) = (1<<14);
  (*AT91C_PIOD_PER) = (1<<1);
  (*AT91C_PIOD_PPUDR) = (1<<1);
  (*AT91C_PIOD_ODR) = (1<<1);
  
  if(((*AT91C_PIOD_PDSR) & (1<<1)) == 0)(*nButton) = 1;
  else (*nButton) = 0;
}

int main(){
  SystemInit();
  init();

  while(1){
   /* ReadButton(&bPressed);
    Set_Led(bPressed);*/
  }
  return 0;
}
