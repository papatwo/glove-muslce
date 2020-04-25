#include <stdio.h>
//#include <SPI.h>

#define MAXDAC 48 //MAX DAC 48ch
#define EMV 48 //電空レギュレータ数

//LTC1660 CS
int DACSpin0= 40; //DAC0 connected to LTC1660 CS/LD
int DACSpin1= 41; //DAC1 connected to LTC1660 CS/LD
int DACSpin2= 42; //DAC2 connected to LTC1660 CS/LD
int DACSpin3= 43; //DAC2 connected to LTC1660 CS/LD
int DACSpin4= 44; //DAC2 connected to LTC1660 CS/LD
int DACSpin5= 45; //DAC2 connected to LTC1660 CS/LD


//SPI connections for Arduino Mega 1280 or 2560
//https://www.arduino.cc/en/Reference/SPI
//MOSI      : 51 or ICSP-4
//MISO      : 50 or ICSP-1
//SCK       : 52 or ICSP-3
//SS(SLAVE) : 53
//SS(MASTER): -
//Level     : 5V
int MOSIpin = 51; //LTC1660 Din pin
//int MISOpin = 50; // not use. LTC1660 Dout pin
int SCKpin  = 52; //LTC1660 CLK pin
//int CLRpin =54; // not use. connected to Arduino VCC(5V).

int vvalue[MAXDAC] = {}; //10bit DA値
bool isSame[MAXDAC] = {};
int m;

String inData; // for incoming serial data

void setup() {
//  SPI.begin();
//  SPI.setBitOrder(MSBFIRST);
//  SPI.setDataMode(SPI_MODE0);
//  SPI.setClockDivider(SPI_CLOCK_DIV128);
//  
//  //
//  pinMode(DACSpin0, OUTPUT);      
//  pinMode(DACSpin1, OUTPUT);     
//  pinMode(DACSpin2, OUTPUT); 
//  pinMode(DACSpin3, OUTPUT);      
//  pinMode(DACSpin4, OUTPUT);     
//  pinMode(DACSpin5, OUTPUT); 
//  
//  pinMode(MOSIpin, OUTPUT);
////  pinMode(MISOpin, INPUT);
//  pinMode(SCKpin, OUTPUT);
////  pinMode(CLRpin, OUTPUT);
//
//  digitalWrite(DACSpin0, HIGH);
//  digitalWrite(DACSpin1, HIGH);
//  digitalWrite(DACSpin2, HIGH);
//  digitalWrite(DACSpin3, HIGH);
//  digitalWrite(DACSpin4, HIGH);
//  digitalWrite(DACSpin5, HIGH);
//  
//  digitalWrite(SCKpin, LOW);
  
  Serial.begin(250000);
  
  vvalue[0] = 1023*0/20;
  vvalue[1] = 1023*0/20;
  vvalue[2] = 1023*0/20;
  vvalue[3] = 1023*0/20;
  vvalue[4] = 1023*0/20;
  vvalue[5] = 1023*0/20;
  vvalue[6] = 1023*0/20;
  vvalue[7] = 1023*0/20;
  vvalue[8] = 1023*0/20;
  vvalue[9] = 1023*0/20;
  vvalue[10] = 1023*0/20;
  vvalue[11] = 1023*0/20;
  vvalue[12] = 1023*0/20;
  vvalue[13] = 1023*0/20;
  vvalue[14] = 1023*0/20;
  vvalue[15] = 1023*0/20;
  vvalue[16] = 1023*0/20;
  vvalue[17] = 1023*0/20;
  vvalue[18] = 1023*0/20;
  vvalue[19] = 1023*0/20;
  vvalue[20] = 1023*0/20;
  vvalue[21] = 1023*0/20;
  vvalue[22] = 1023*0/20;
  vvalue[23] = 1023*0/20;
  vvalue[24] = 1023*0/20;
  vvalue[25] = 1023*0/20;
  vvalue[26] = 1023*0/20;
  vvalue[27] = 1023*0/20;
  vvalue[28] = 1023*0/20;
  vvalue[29] = 1023*0/20;
  vvalue[30] = 1023*0/20;
  vvalue[31] = 1023*0/20;
  vvalue[32] = 1023*0/20;
  vvalue[33] = 1023*0/20;
  vvalue[34] = 1023*0/20;
  vvalue[35] = 1023*0/20;
  vvalue[36] = 1023*0/20;
  vvalue[37] = 1023*0/20;
  vvalue[38] = 1023*0/20;
  vvalue[39] = 1023*0/20;
  vvalue[40] = 1023*0/20;
  vvalue[41] = 1023*0/20;
  vvalue[42] = 1023*0/20;
  vvalue[43] = 1023*0/20;
  vvalue[44] = 1023*0/20;
  vvalue[45] = 1023*0/20;
  vvalue[46] = 1023*0/20;
  vvalue[47] = 1023*0/20;

  int i;
  for(i=0; i < MAXDAC; i++){
    isSame[i] = false;
  }

//  valveout();
}


//int valveout(){
//  //---------------LTC1660-0-------------------
//  //LTC1660-0-0ch
//  if(!isSame[0]){
//    digitalWrite(DACSpin0, LOW);
//    //----Send ADDRESS/CONTROL(4bits) and INPUT CODE(10bits)-------------
//    //LTC1660 input words(SPI)
//    //A3-A2-A1-A0 D9-D8-D7-D6 D5-D4-D3-D2 D1-D0-X1-X2
//    //A3-A2-A1-A0: ADDRESS/CONTROL
//    //D9-D8-D7-D6 D5-D4-D3-D2 D1-D0: INPUT CODE
//    //X1-X2: DON'T CARE
//    //LOAD DAC A: 0001, ~ , LOAD DAC H: 1000 (8ch)
//    //SPI.transferは1byte(8bit)送信
//    SPI.transfer(0b00010000 + (vvalue[0] >> 6)); //upper bit
//    //0b00010000: 上4桁がLOAD DAC．ch指定．
//    //vvalue[]には10bitの数値が格納される．LOAD DACに続きvvalue[]上4桁を送信するために下6桁をマスク．
//    SPI.transfer((vvalue[0] << 4) & 0b11111100 ); //lower bit
//    //vvalue[]の下6桁を送信するために上4桁をマスクする．
//    //最後の下2桁はDON'T CARE．
//    //---------------------------------------------------------
//    digitalWrite(DACSpin0, HIGH);
//  }
//  //LTC1660-0-1ch
//  if(!isSame[1]){
//    digitalWrite(DACSpin0, LOW);
//    SPI.transfer(0b00100000 + (vvalue[1] >> 6)); //upper bit
//    SPI.transfer((vvalue[1] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin0, HIGH);
//  }
//
//  //LTC1660-0-2ch
//  if(!isSame[2]){
//    digitalWrite(DACSpin0, LOW);
//    SPI.transfer(0b00110000 + (vvalue[2] >> 6)); //upper bit
//    SPI.transfer((vvalue[2] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin0, HIGH);
//  }
//
//  //LTC1660-0-3ch
//  if(!isSame[3]){
//    digitalWrite(DACSpin0, LOW);
//    SPI.transfer(0b01000000 + (vvalue[3] >> 6)); //upper bit
//    SPI.transfer((vvalue[3] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin0, HIGH);
//  }
//
//  //LTC1660-0-4ch
//  if(!isSame[4]){
//    digitalWrite(DACSpin0, LOW);
//    SPI.transfer(0b01010000 + (vvalue[4] >> 6)); //upper bit
//    SPI.transfer((vvalue[4] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin0, HIGH);
//  }
//
//  //LTC1660-0-5ch
//  if(!isSame[5]){
//    digitalWrite(DACSpin0, LOW);
//    SPI.transfer(0b01100000 + (vvalue[5] >> 6)); //upper bit
//    SPI.transfer((vvalue[5] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin0, HIGH);
//  }
//
//  //LTC1660-0-6ch
//  if(!isSame[6]){
//    digitalWrite(DACSpin0, LOW);
//    SPI.transfer(0b01110000 + (vvalue[6] >> 6)); //upper bit
//    SPI.transfer((vvalue[6] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin0, HIGH);
//  }
//
//  //LTC1660-0-7ch
//  if(!isSame[7]){
//    digitalWrite(DACSpin0, LOW);
//    SPI.transfer(0b10000000 + (vvalue[7] >> 6)); //upper bit
//    SPI.transfer((vvalue[7] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin0, HIGH);
//  }
//
//  //---------------LTC1660-1-------------------
//  //LTC1660-1-0ch
//  if(!isSame[8]){
//    digitalWrite(DACSpin1, LOW);
//    SPI.transfer(0b00010000 + (vvalue[8] >> 6)); //upper bit
//    SPI.transfer((vvalue[8] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin1, HIGH);
//  }
//
//  //LTC1660-1-1ch
//  if(!isSame[9]){
//    digitalWrite(DACSpin1, LOW);
//    SPI.transfer(0b00100000 + (vvalue[9] >> 6)); //upper bit
//    SPI.transfer((vvalue[9] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin1, HIGH);
//  }
//
//  //LTC1660-1-2ch
//  if(!isSame[10]){
//    digitalWrite(DACSpin1, LOW);
//    SPI.transfer(0b00110000 + (vvalue[10] >> 6)); //upper bit
//    SPI.transfer((vvalue[10] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin1, HIGH);
//  }
//
//  //LTC1660-1-3ch
//  if(!isSame[11]){
//    digitalWrite(DACSpin1, LOW);
//    SPI.transfer(0b01000000 + (vvalue[11] >> 6)); //upper bit
//    SPI.transfer((vvalue[11] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin1, HIGH);
//  }
//
//  //LTC1660-1-4ch
//  if(!isSame[12]){
//    digitalWrite(DACSpin1, LOW);
//    SPI.transfer(0b01010000 + (vvalue[12] >> 6)); //upper bit
//    SPI.transfer((vvalue[12] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin1, HIGH);
//  }
//
//  //LTC1660-1-5ch
//  if(!isSame[13]){
//    digitalWrite(DACSpin1, LOW);
//    SPI.transfer(0b01100000 + (vvalue[13] >> 6)); //upper bit
//    SPI.transfer((vvalue[13] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin1, HIGH);
//  }
//
//  //LTC1660-1-6ch
//  if(!isSame[14]){
//    digitalWrite(DACSpin1, LOW);
//    SPI.transfer(0b01110000 + (vvalue[14] >> 6)); //upper bit
//    SPI.transfer((vvalue[14] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin1, HIGH);
//  }
//
//  //LTC1660-1-7ch
//  if(!isSame[15]){
//    digitalWrite(DACSpin1, LOW);
//    SPI.transfer(0b10000000 + (vvalue[15] >> 6)); //upper bit
//    SPI.transfer((vvalue[15] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin1, HIGH);
//  }
//
//  //---------------LTC1660-2-------------------
//  //LTC1660-2-0ch
//  if(!isSame[16]){
//    digitalWrite(DACSpin2, LOW);
//    SPI.transfer(0b00010000 + (vvalue[16] >> 6)); //upper bit
//    SPI.transfer((vvalue[16] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin2, HIGH);
//  }
//
//  //LTC1660-2-1ch
//  if(!isSame[17]){
//    digitalWrite(DACSpin2, LOW);
//    SPI.transfer(0b00100000 + (vvalue[17] >> 6)); //upper bit
//    SPI.transfer((vvalue[17] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin2, HIGH);
//  }
//
//  //LTC1660-2-2ch
//  if(!isSame[18]){
//    digitalWrite(DACSpin2, LOW);
//    SPI.transfer(0b00110000 + (vvalue[18] >> 6)); //upper bit
//    SPI.transfer((vvalue[18] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin2, HIGH);
//  }
//
//  //LTC1660-2-3ch
//  if(!isSame[19]){
//    digitalWrite(DACSpin2, LOW);
//    SPI.transfer(0b01000000 + (vvalue[19] >> 6)); //upper bit
//    SPI.transfer((vvalue[19] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin2, HIGH);
//  }
//
//    //LTC1660-2-4ch
//  if(!isSame[20]){
//    digitalWrite(DACSpin2, LOW);
//    SPI.transfer(0b01010000 + (vvalue[20] >> 6)); //upper bit
//    SPI.transfer((vvalue[20] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin2, HIGH);
//  }
//
//    //LTC1660-2-5ch
//  if(!isSame[21]){
//    digitalWrite(DACSpin2, LOW);
//    SPI.transfer(0b01100000 + (vvalue[21] >> 6)); //upper bit
//    SPI.transfer((vvalue[21] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin2, HIGH);
//  }
//
//    //LTC1660-2-6ch
//  if(!isSame[22]){
//    digitalWrite(DACSpin2, LOW);
//    SPI.transfer(0b01110000 + (vvalue[22] >> 6)); //upper bit
//    SPI.transfer((vvalue[22] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin2, HIGH);
//  }
//
//    //LTC1660-2-3ch
//  if(!isSame[23]){
//    digitalWrite(DACSpin2, LOW);
//    SPI.transfer(0b10000000 + (vvalue[23] >> 6)); //upper bit
//    SPI.transfer((vvalue[23] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin2, HIGH);
//  }
//
//  //---------------LTC1660-3-------------------
//  //LTC1660-3-0ch
//  if(!isSame[24]){
//    digitalWrite(DACSpin3, LOW);
//    SPI.transfer(0b00010000 + (vvalue[24] >> 6)); //upper bit
//    SPI.transfer((vvalue[24] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin3, HIGH);
//  }
//
//  //LTC1660-3-1ch
//  if(!isSame[25]){
//    digitalWrite(DACSpin3, LOW);
//    SPI.transfer(0b00100000 + (vvalue[25] >> 6)); //upper bit
//    SPI.transfer((vvalue[25] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin3, HIGH);
//  }
//
//  //LTC1660-3-2ch
//  if(!isSame[26]){
//    digitalWrite(DACSpin3, LOW);
//    SPI.transfer(0b00110000 + (vvalue[26] >> 6)); //upper bit
//    SPI.transfer((vvalue[26] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin3, HIGH);
//  }
//
//  //LTC1660-3-3ch
//  if(!isSame[27]){
//    digitalWrite(DACSpin3, LOW);
//    SPI.transfer(0b01000000 + (vvalue[27] >> 6)); //upper bit
//    SPI.transfer((vvalue[27] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin3, HIGH);
//  }
//
//    //LTC1660-3-4ch
//  if(!isSame[28]){
//    digitalWrite(DACSpin3, LOW);
//    SPI.transfer(0b01010000 + (vvalue[28] >> 6)); //upper bit
//    SPI.transfer((vvalue[28] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin3, HIGH);
//  }
//
//    //LTC1660-3-5ch
//  if(!isSame[29]){
//    digitalWrite(DACSpin3, LOW);
//    SPI.transfer(0b01100000 + (vvalue[29] >> 6)); //upper bit
//    SPI.transfer((vvalue[29] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin3, HIGH);
//  }
//
//    //LTC1660-3-6ch
//  if(!isSame[30]){
//    digitalWrite(DACSpin3, LOW);
//    SPI.transfer(0b01110000 + (vvalue[30] >> 6)); //upper bit
//    SPI.transfer((vvalue[30] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin3, HIGH);
//  }
//
//    //LTC1660-3-7ch
//  if(!isSame[31]){
//    digitalWrite(DACSpin3, LOW);
//    SPI.transfer(0b10000000 + (vvalue[31] >> 6)); //upper bit
//    SPI.transfer((vvalue[31] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin3, HIGH);
//  }
//
//    //---------------LTC1660-4-------------------
//  //LTC1660-4-0ch
//  if(!isSame[32]){
//    digitalWrite(DACSpin4, LOW);
//    SPI.transfer(0b00010000 + (vvalue[32] >> 6)); //upper bit
//    SPI.transfer((vvalue[32] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin4, HIGH);
//  }
//
//  //LTC1660-4-1ch
//  if(!isSame[33]){
//    digitalWrite(DACSpin4, LOW);
//    SPI.transfer(0b00100000 + (vvalue[33] >> 6)); //upper bit
//    SPI.transfer((vvalue[33] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin4, HIGH);
//  }
//
//  //LTC1660-4-2ch
//  if(!isSame[34]){
//    digitalWrite(DACSpin4, LOW);
//    SPI.transfer(0b00110000 + (vvalue[34] >> 6)); //upper bit
//    SPI.transfer((vvalue[34] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin4, HIGH);
//  }
//
//  //LTC1660-4-3ch
//  if(!isSame[35]){
//    digitalWrite(DACSpin4, LOW);
//    SPI.transfer(0b01000000 + (vvalue[35] >> 6)); //upper bit
//    SPI.transfer((vvalue[35] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin4, HIGH);
//  }
//
//    //LTC1660-4-4ch
//  if(!isSame[36]){
//    digitalWrite(DACSpin4, LOW);
//    SPI.transfer(0b01010000 + (vvalue[36] >> 6)); //upper bit
//    SPI.transfer((vvalue[36] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin4, HIGH);
//  }
//
//    //LTC1660-4-5ch
//  if(!isSame[37]){
//    digitalWrite(DACSpin4, LOW);
//    SPI.transfer(0b01100000 + (vvalue[37] >> 6)); //upper bit
//    SPI.transfer((vvalue[37] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin4, HIGH);
//  }
//
//    //LTC1660-4-6ch
//  if(!isSame[38]){
//    digitalWrite(DACSpin4, LOW);
//    SPI.transfer(0b01110000 + (vvalue[38] >> 6)); //upper bit
//    SPI.transfer((vvalue[38] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin4, HIGH);
//  }
//
//    //LTC1660-4-7ch
//  if(!isSame[39]){
//    digitalWrite(DACSpin4, LOW);
//    SPI.transfer(0b10000000 + (vvalue[39] >> 6)); //upper bit
//    SPI.transfer((vvalue[39] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin4, HIGH);
//  }
//
//      //---------------LTC1660-5-------------------
//  //LTC1660-5-0ch
//  if(!isSame[40]){
//    digitalWrite(DACSpin5, LOW);
//    SPI.transfer(0b00010000 + (vvalue[40] >> 6)); //upper bit
//    SPI.transfer((vvalue[40] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin5, HIGH);
//  }
//
//  //LTC1660-5-1ch
//  if(!isSame[41]){
//    digitalWrite(DACSpin5, LOW);
//    SPI.transfer(0b00100000 + (vvalue[41] >> 6)); //upper bit
//    SPI.transfer((vvalue[41] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin5, HIGH);
//  }
//
//  //LTC1660-5-2ch
//  if(!isSame[42]){
//    digitalWrite(DACSpin5, LOW);
//    SPI.transfer(0b00110000 + (vvalue[42] >> 6)); //upper bit
//    SPI.transfer((vvalue[42] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin5, HIGH);
//  }
//
//  //LTC1660-5-3ch
//  if(!isSame[43]){
//    digitalWrite(DACSpin5, LOW);
//    SPI.transfer(0b01000000 + (vvalue[43] >> 6)); //upper bit
//    SPI.transfer((vvalue[43] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin5, HIGH);
//  }
//
//    //LTC1660-5-4ch
//  if(!isSame[44]){
//    digitalWrite(DACSpin5, LOW);
//    SPI.transfer(0b01010000 + (vvalue[44] >> 6)); //upper bit
//    SPI.transfer((vvalue[44] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin5, HIGH);
//  }
//
//    //LTC1660-5-5ch
//  if(!isSame[45]){
//    digitalWrite(DACSpin5, LOW);
//    SPI.transfer(0b01100000 + (vvalue[45] >> 6)); //upper bit
//    SPI.transfer((vvalue[45] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin5, HIGH);
//  }
//
//    //LTC1660-5-6ch
//  if(!isSame[46]){
//    digitalWrite(DACSpin5, LOW);
//    SPI.transfer(0b01110000 + (vvalue[46] >> 6)); //upper bit
//    SPI.transfer((vvalue[46] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin5, HIGH);
//  }
//
//    //LTC1660-5-7ch
//  if(!isSame[47]){
//    digitalWrite(DACSpin5, LOW);
//    SPI.transfer(0b10000000 + (vvalue[47] >> 6)); //upper bit
//    SPI.transfer((vvalue[47] << 4) & 0b11111100 ); //lower bit
//    digitalWrite(DACSpin5, HIGH);
//  }
//
//  return 1;
//}



//int rcv_serial(){
//  int i;
//  int tmp;
//  if (Serial.available() > 0) {
//    // read the incoming byte:
//    char c = Serial.read();
//    inData += c;
//
//    // say what you got:
//    if (c == '\0') 
//    {
//      vvalue[0]=atoi(strtok(inData.c_str(),","));
//      for (i = 1; i < EMV; i++) {
//        tmp = atoi(strtok(NULL, ","));
//        if(tmp == -1){
//          isSame[i] = true;
//        } else if(tmp ==vvalue[i]){
//          isSame[i] = true;
//        } else{
//          isSame[i] = false;
//          vvalue[i] = tmp;
//        }
//      }
//      Serial.print("I received: ");
//      Serial.println(inData);
//      inData = "";
//    }  
//  }
////  return 1;
//}


void loop() 
{
  // send data only when you receive data:
//  rcv_serial();
  int i;
  int tmp;
  if (Serial.available() > 0) {
    // read the incoming byte:
    char c = Serial.read();
    inData += c;

    // say what you got:
    if (c == '\0') 
    {
      vvalue[0]=atoi(strtok(inData.c_str(),","));
      for (i = 1; i < EMV; i++) {
        tmp = atoi(strtok(NULL, ","));
        if(tmp == -1){
          isSame[i] = true;
        } else if(tmp ==vvalue[i]){
          isSame[i] = true;
        } else{
          isSame[i] = false;
          vvalue[i] = tmp;
        }
      }
      
      // for testing received content
      Serial.print("I received: ");
      Serial.println(inData);
      
      inData = "";
    }  
  }
}
