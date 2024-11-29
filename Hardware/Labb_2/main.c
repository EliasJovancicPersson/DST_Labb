#include "system_sam3x.h"
#include "at91sam3x8.h"
void Set_Led(int nLed);
void set_Address_Pointer();
void configure(){
  *AT91C_PMC_PCER = (1<<13);
  *AT91C_PMC_PCER = (1<<14);
  *AT91C_PIOC_PER = 0xFF3FC;
  *AT91C_PIOC_PPUDR = 0xFF3FC;
  *AT91C_PIOD_PER = 0x5;
  *AT91C_PIOD_PPUDR = 0x5;
}
void key_init(){
  //configure();
  *AT91C_PIOC_PER = 0x3BC;
  *AT91C_PIOD_OER = AT91C_PIO_PD2;
  *AT91C_PIOD_CODR = AT91C_PIO_PD2;
  *AT91C_PIOC_OER = 0x3BC;
  *AT91C_PIOC_SODR = 0x380;
  *AT91C_PIOC_ODR = 0x3C;
}
void delay(int Value){
  int i;
  for(i=0;i<Value;i++)
  asm("nop");
}
int key_pressed(){
  key_init();

  int value = 0;
  int col[] ={0x100, 0x200, 0x80}; //Kolumnenrna på keypaden
  int row[] ={0x20, 0x4, 0x8, 0x10}; //Raderna på keypaden
 
  for(int i=0; i<3 && value==0; i++){ //Loopar igenom och kollar vilken knapp som är itryckt
    *AT91C_PIOC_CODR = col[i];
  for(int j=0; j<4 && value ==0 ; j++){
    if((*AT91C_PIOC_PDSR & row[j]) == 0){
      delay(1600000);  
      value = j*3+i+1;
      //value = key_map[j][i];
      }
    }
  *AT91C_PIOC_SODR = col[i];
  }

  *AT91C_PIOC_ODR = 0x380;
  *AT91C_PIOD_SODR = (1<<2);
  return(value);
}
unsigned char Read_Status_Display(void){
  //configure();
  unsigned char temp = 0;
  *AT91C_PIOC_ODR = 0x3FC; //Databus som input
  *AT91C_PIOC_SODR = 1<<13; //dir som input (74chip, 1= input)
  *AT91C_PIOC_CODR = 1<<12; //Clear output (74chip, 0 = enable)
  *AT91C_PIOC_SODR = 1<<14; //Set C/D
  *AT91C_PIOC_CODR = 1<<15; //Clear chip select
  *AT91C_PIOC_CODR = 1<<16; //Clear read display
  delay(10); //Delay 150ns
  temp = ((*AT91C_PIOC_PDSR)>>2)&0x3; //Shiftar så vi får STA0 och STA1 till LSB
  *AT91C_PIOC_SODR = 1<<15; //set chip select
  *AT91C_PIOC_SODR = 1<<16; //set read display
  *AT91C_PIOC_SODR = 1<<12; //disable output (74chip)
  *AT91C_PIOC_CODR = 1<<13; //dir som output (74chip)
  return temp; //Retunerar värdet
}
void Write_Command_2_Display(unsigned char Command){
  //configure();
  while(Read_Status_Display() != 0x3){ }
  *AT91C_PIOC_CODR = 0x3FC; //Clearar databussen
  *AT91C_PIOC_SODR = (Command<<2); //Sätter command till databusen (shiftar till databussen)
  *AT91C_PIOC_CODR = 1<<13; //dir som output (74chip)
  *AT91C_PIOC_CODR = 1<<12; //enable output (74chip)
  *AT91C_PIOC_OER = 0x3FC; //Output enable på databussen
  *AT91C_PIOC_SODR = 1<<14; //C/D signal high (1 = command)
  *AT91C_PIOC_CODR = 1<<15; //Clear chip select display
  *AT91C_PIOC_CODR = 1<<17; //clear write display
  delay(10); //Delay
  *AT91C_PIOC_SODR = 1<<15; //Set chip enable display
  *AT91C_PIOC_SODR = 1<<17; //Set write display
  *AT91C_PIOC_SODR = 1<<12; //disable output (74chip)
  *AT91C_PIOC_ODR = 0x3FC; //Databus som input
}

void Write_Data_2_Display(unsigned char Data){
  //configure();
  while(Read_Status_Display() != 0x3){ }
  *AT91C_PIOC_CODR = 0x3FC; //Clearar databussen
  *AT91C_PIOC_SODR = (Data<<2); //Sätter data till bussen och shiftar till rätt plats.
  *AT91C_PIOC_CODR = 1<<13; //dir som output (74chip)
  *AT91C_PIOC_CODR = 1<<12; //enable output (74chip)
  *AT91C_PIOC_OER = 0x3FC; //databus som output
  *AT91C_PIOC_CODR = 1<<14; //clear C/D (0 = data)
  *AT91C_PIOC_CODR = 1<<15; //clear chip select display
  *AT91C_PIOC_CODR = 1<<17; //clear write display
  delay(10); //delay
  *AT91C_PIOC_SODR = 1<<15; //set chip enable display
  *AT91C_PIOC_SODR = 1<<17; //set write display
  *AT91C_PIOC_SODR = 1<<12; //disable output (74chip)
  *AT91C_PIOC_ODR = 0x3FC; //databus som input
}
void Init_Display(void){
  //configure();
  *AT91C_PIOC_OER = 0xFF3FC; //Sätter alla som output
  //*AT91C_PIOD_OER = 0x1; //Sätter reset som output
  *AT91C_PIOD_CODR = 0x1; //Clerar reseten
  delay(10);
  *AT91C_PIOD_SODR = 0x1; //Sätter reseten
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x40); //Set text home address
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x40);
  Write_Command_2_Display(0x42); //Set graphic home address
  Write_Data_2_Display(0x1e); // sätter rader för stor font (30 kolumner)
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x41); // Set text area
  *AT91C_PIOC_CODR = (1 << 18); // Sätter stor font
  Write_Data_2_Display(0x28);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x43); // Set graphic area
  Write_Command_2_Display(0x80); // text mode
  Write_Command_2_Display(0x94); // Text on graphic off
}

void clear_display(void){
  //set_Address_Pointer();
  for(int i = 0; i<480; i++){
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0xC0);
  }
  set_Address_Pointer();
}

void set_Address_Pointer(){  
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x24);
}

void test_print_character(){
  for(int i = 0; i < 479; i++){
    Write_Data_2_Display(0x30);
    Write_Command_2_Display(0xC0);
  }
}

void print_character(){
  Write_Data_2_Display(0x6e);
  Write_Command_2_Display(0xC0);
}

void writeChar(char str[]){
  char tempChar;
   for(int i = 0; str[i];i++){
      tempChar = str[i]-0x20;
      Write_Data_2_Display(tempChar);
      Write_Command_2_Display(0xC0);
  }
}

void writeInt(int a){
  if(a == 0) return;
  switch (a) {
    case 10:
      writeChar("*");
      return;
    case 11:
      writeChar("0");
      return;
    case 12:
      writeChar("#");
      return;
  }
  char ch[20];
  sprintf(ch, "%d", a);
  char tempchar;
  for(int i = 0; ch[i]; i++){
    tempchar = ch[i] - 0x20;
    Write_Data_2_Display(tempchar);
    Write_Command_2_Display(0xC0);
  }
}


void main(void){
  SystemInit();
  configure();
  Init_Display();
  set_Address_Pointer();
  clear_display();
  //test_print_character();
  //print_character();
  //write("* #");
  while(1){
    //checkKey(key_pressed());
    writeInt(key_pressed());
  }
}