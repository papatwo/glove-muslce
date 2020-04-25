
int sensor0 = A12;
int sensor1 = A13;
int sensor2 = A11;
int sensor3 = A10;
int sensor4 = A9;
int sensor5 = A8;
int sensor6 = A7;
int sensor7 = A6;
int sensor8 = A5;
int sensor9 = A4;
int sensor10 = A3;
int sensor11 = A2;
int sensor12 = A1;
int sensor13 = A0;
char data[500];



void setup() {
//    Serial.begin(115200);
  Serial.begin(250000);
//  Serial.begin(1200);
//  Serial.println("Ready"); // print "Ready" once

}

void loop() {
  
    int value0 = analogRead(sensor0);
    int value1 = analogRead(sensor1);
    int value2 = analogRead(sensor2);
    int value3 = analogRead(sensor3);
    int value4 = analogRead(sensor4);
    int value5 = analogRead(sensor5);
    int value6 = analogRead(sensor6);
    int value7 = analogRead(sensor7);
    int value8 = analogRead(sensor8);
    int value9 = analogRead(sensor9);
    int value10 = analogRead(sensor10);
    int value11 = analogRead(sensor11);
    int value12 = analogRead(sensor12);
    int value13 = analogRead(sensor13);


    sprintf(data,"%i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i", 
            value12, value13, // thumb
            value9, value10, value11, // index
            value6, value7, value8, // middle
            value3, value4, value5, // ring
            value0, value1, value2); // little
    Serial.println(data);
//    Serial.flush();

//    delay(500);
    
}
