//CREADO POR EL ING J HERNANDEZ
//CODIGO QUE REALIZA UNA CERRADURA ELECTRONICA CON SENSORES
//ESTE CODIGO SE CORRE EN PIC C COMPILER
//CREADO EL DIA 09/04/2016
//SIMULACION EN PROTEUS LLAMADO cerradura casa domotica
#include <16f877a.h>
#FUSES XT
#use delay(clock=4M)
#define LCD_DB4   PIN_D4
#define LCD_DB5   PIN_D5
#define LCD_DB6   PIN_D6
#define LCD_DB7   PIN_D7
#define LCD_RS    PIN_D2
#define LCD_E     PIN_D3
#byte port_a = 0x05
#byte port_c = 0x07
#byte tris_a = 0x85
#byte tris_c = 0x87
#include <lcd1.c> //Libreria para manejar el lcd 
#include <KBD1.C>//Libreria para manejar el keypad
#rom 0x2100={'1','1','1','1'}   //SE GUARDA EN LA MEMORIA EEPROM CONTRASEÑA
int i,j=0;   //SE USARAN COMO CONTADORES
char k;    //lectura del teclado
char dato[4],clave[4];     //dato almacena los digitos, clave almacena la contraseña
int l;
int p;

void main()
{
   //tris_a = 0xFF;
   tris_a = 0b11000011;
   tris_c = 0x00;
   output_low(PIN_C0);  //Led puerta abierta
   output_low(PIN_C1);  //Led puerta cerrada
   output_low(PIN_C2);  //Buzzer
   output_low(PIN_C3);  //Servo
   //Modificaciones
   output_low(PIN_A2);  //Salida Esp Puerta Cerrada
   output_low(PIN_A3);  //Salida Esp Puerta Cerrada  
   
   lcd_init();   //iniciliza la lcd
   kbd_init();   //inicializa el teclado  
   port_b_pullups(true);  //con esta delaracion activa las resistencias internas del pic
   
   while(1)
    {
      lcd_putc("\f  Casa Domotica");
      lcd_putc("\n      IOT");
      delay_ms(500);
       if(input(PIN_A0)==1)
      {
         output_low(PIN_C0);  //APAGA EL INDICADOR DE PUERTA ABIERTA
         output_low(PIN_A2); //Apaga el indicador de la puerta
         lcd_putc("\f La puerta se ");
         lcd_putc("\n cerrara");
         delay_ms(1000);
          for(i=0;i<60;i++)
             {
               //MOVER 0 GRADOS  
               output_high(PIN_C3);
               delay_us(2500);
               output_low(PIN_C3);
               delay_ms(20);
             }
         output_high(PIN_C1);          //INDICA QUE LA PUERTA ESTA CERRADA
         //Modif
         
      }  
       if(input(PIN_A1)==1)
       {
         
         lcd_putc("\fIngrese su clave");
         delay_ms(1000);
         output_low(PIN_C0);   //LED INDICADOR DE PUERTA ABIERTA
         i=0;
         while(i<4)
         {
         k=kbd_getc(); // leer la tecla presiono
            if(k!=0)
            {
               dato[i]=k;
               i++;
               lcd_gotoxy(i,2); // posicion i de la segunda linea
               lcd_putc('*');
            }
         }
          for(i=0;i<4;i++)
   clave[i]=read_eeprom(i); //guarda la constraseña  en el vector clave
   
   if( dato[0]==clave[0] &&  dato[1]==clave[1] && dato[2]==clave[2] && dato[3]==clave[3])
   {  
      output_low(PIN_C1);  //APAGA EL LED DE PUERTA CERRADA
       for(i=0;i<60;i++)
          {
            //MOVER 180 GRADOS  
            output_high(PIN_C3);
            delay_us(750);
            output_low(PIN_C3);
            delay_ms(20);
          }
      output_high(PIN_C0); //PRENDE EL LED DE PUERTA ABIERTA
      output_high(PIN_A2); //Prende indicador de esp8266
      lcd_putc("\fClave Correcta! ");
      lcd_putc("\nAdelante!!  ");
      delay_ms(2000);
      lcd_putc("\fBIENVENIDOS");
      delay_ms(2000);
     
   }
   else
   {
      lcd_putc("\fClave incorecta ");
      lcd_putc("\nIntente de nuevo  ");
      delay_ms(1200);            
      j++;
      if(j==3)
      {
      lcd_putc("\fTecladoBloqueado ");
      lcd_putc("\nActivando Alarma  ");
      for(p=0;p<10;p++)
      {
      output_high(PIN_C2);       //ACTIVA BUZZER
      output_high(PIN_C1);       //ACTIVA LED DE PUERTA CERRADA
      delay_ms(120);
      output_low(PIN_C2);       //ACTIVA BUZZER
      output_low(PIN_C1);       //ACTIVA LED DE PUERTA CERRADA
      delay_ms(120);
      }
      output_low(PIN_C2);
      output_high(PIN_C1);
      delay_ms(5000);
      j=0;
     
      }
         
   }
       } 