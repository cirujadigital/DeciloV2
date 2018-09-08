//Decilo! V2- ISD1760+Arduino Pro Mini Version

#include <ISD1700.h> //Librería para usar el ISD1700 de forma más sencilla. Disponible en: https://github.com/georgepatterson/ISD17xx/blob/master/sketch.pde

//IMPORTANTE: MODIFICAR EN LA LIBRERIA LA LINEA -tanto en el archivo .h como en el .cpp #include Wprogram.h por #include Arduino.h 

ISD1700 chip(10); //Pines SPI: 10 SS; 11 MOSI, 12 MISO, 13 SCK - Arduino UNO/NANO/PRO MINI 
uint16_t playback_ptr[9]; //Modificar según la cantidad de botones
uint16_t rec_ptr[9]; //Modificar según la cantidad de botones
byte num_msj= 9; //numero de mensajes máximos posibles. Modificar según la cantidad de botones
byte pos = 0;
uint16_t pos0 = 0x010;

int boton;
int boton_previo=0;
int boton_apretado;


boolean primer_ciclo_grabado;

int lectura_analog;

boolean boton_presionado=false;;
void setup()
{

  primer_ciclo_grabado= true;
  playback_ptr[0]= pos0;
  chip.pu();
  chip.reset();
  delay(100);
  Serial.begin(9600);
  Serial.println("Decilo! v2. ISD1760 Version. 2018 CirujaDigital");


}

void loop()
{
  
if (primer_ciclo_grabado)
{
chip.pu();
leer_playbackpointer();
delay(200);
primer_ciclo_grabado=false;
}


  lectura_analog= analogRead(A0);

  
  if(lectura_analog==0) {boton=1;}
  if(lectura_analog>=88 && lectura_analog<=93) {boton=2; }
  if(lectura_analog>=168 && lectura_analog<=171) {boton=3;}
  if(lectura_analog>=336 && lectura_analog<=342) {boton=4;}
  if(lectura_analog>=288 && lectura_analog<=292) {boton=5;}
  if(lectura_analog>=230 && lectura_analog<=236) {boton=6;}
  if(lectura_analog>=378 && lectura_analog<=385) {boton=7;}
  if(lectura_analog>=415 && lectura_analog<=425) {boton=8;}
  if(lectura_analog>=445 && lectura_analog<=460) {boton=9;}

  if(lectura_analog>=1020)  {boton=0; boton_previo = 0;} 
  if (boton_previo ==boton) {                                           
    boton_previo= boton;                                              
    boton_apretado= 0;                                               
  }  

  else {                                                                
    boton_previo = boton;                                             
    boton_apretado = boton;                                        
  }
  delay(20);
  
 switch (boton_apretado){

  case 1:
  chip.pu(); chip.set_play( playback_ptr[0],playback_ptr[1]);
  break;

  case 2:
  chip.pu(); chip.set_play( playback_ptr[1],playback_ptr[2]);Serial.println("mensaje1");
  break;
  
  case 3:
  chip.pu(); chip.set_play( playback_ptr[2],playback_ptr[3]);
  break;
  
  case 4:
  chip.pu(); chip.set_play( playback_ptr[3],playback_ptr[4]);
  break;
  
  case 5:
  chip.pu(); chip.set_play( playback_ptr[4],playback_ptr[5]);
  break;
  
  case 6:
  chip.pu(); chip.set_play( playback_ptr[5],playback_ptr[6]);
  break;
  
  case 7:
  chip.pu(); chip.set_play( playback_ptr[6],playback_ptr[7]);
  break;
  
  case 8:
  chip.pu(); chip.set_play( playback_ptr[7],playback_ptr[8]);
  break;

  case 9:
  chip.pu(); chip.set_play( playback_ptr[8],rec_ptr[8]);Serial.println("mensaje9");
  break;
 
 }

  
}

void leer_playbackpointer(){ 
  
chip.fwd(); // Esto es porque reseteado o no siempre arranca en el ultimo mensaje grabado con el playback pointer (entonces al avanzarlo vuelve a cero)

  delay(20);
  rec_ptr[0]= chip.rd_rec_ptr(); 
  pos =0;
  //while(playback_ptr[0] != playback_ptr[pos]){ 
   while(pos<num_msj) {
    
    playback_ptr[pos]=chip.rd_play_ptr(); 
    rec_ptr[pos]= chip.rd_rec_ptr(); 
    chip.fwd(); 
    pos++; 
    if(playback_ptr[pos]=0x010){playback_ptr[pos]=rec_ptr[pos];}
    delay(50);
    Serial.println(String(pos));
    }
    }

  

