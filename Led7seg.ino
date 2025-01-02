#include <ShiftRegister74HC595.h>
#define ShiftRegister74HC595_h
ShiftRegister74HC595<4> sr(6, 5, 4);

int number; // <--- Change it from 0 to 99

int digit1, digit2, digit3, digit4, a, b, h, dk;
int but2 = digitalRead(2);  // Giá trị chân button chân D2
int but3 = digitalRead(3);  // Giá trị chân button chân D3
// Chân led 1
int ledr_1 = 12, ledy_1 = 11, ledg_1 = 10;
// Chân led 2
int ledr_2 = 9, ledy_2 = 8, ledg_2 = 7;
uint8_t numberB[] = {
    B11000000, //0
    B11111001, //1
    B10100100, //2
    B10110000, //3
    B10011001, //4
    B10010010, //5
    B10000010, //6
    B11111000, //7
    B10000000, //8
    B10010000  //9
};

void setup()
{
  Serial.begin(9600);
  // Led 1 - OUTPUT
  pinMode(ledr_1,OUTPUT);
  pinMode(ledy_1,OUTPUT);
  pinMode(ledg_1,OUTPUT);
  // Led 2 - OUTPUT
  pinMode(ledr_2,OUTPUT);
  pinMode(ledy_2,OUTPUT);
  pinMode(ledg_2,OUTPUT);
  // Pin Mode - INPUT
  pinMode(2, INPUT); //INPUT_PULLUP chân button 1
  pinMode(3, INPUT); //INPUT_PULLUP chân button 2
  // Led 1 - 0
  digitalWrite(ledr_1,0);
  digitalWrite(ledy_1,0);
  digitalWrite(ledg_1,0);
  // Led 2 - 0
  digitalWrite(ledr_2,0);
  digitalWrite(ledy_2,0);
  digitalWrite(ledg_2,0);
  //Pin - 0
  digitalWrite(2,0);
  digitalWrite(3,0);
  // Interrupts 
  attachInterrupt(digitalPinToInterrupt(2),led1,1);
}

void loop()
{
  number = 1510;
  mode();
}

void red1()
{
  digitalWrite(ledy_1,0);
  digitalWrite(ledg_1,0);
  digitalWrite(ledr_1,1);
  green2();
}

void green1()
{
  digitalWrite(ledr_1,0);
  digitalWrite(ledy_1,1);
  digitalWrite(ledg_1,0);
  digitalWrite(ledr_2,0);
  digitalWrite(ledy_2,1);
  digitalWrite(ledg_2,0);
  delay(1500);
  digitalWrite(ledr_1,0);
  digitalWrite(ledy_1,0);
  digitalWrite(ledg_1,1);
  red2();
}

void red2()
{
  digitalWrite(ledy_2,0);
  digitalWrite(ledg_2,0);
  digitalWrite(ledr_2,1);
  green1();
}

void green2()
{
  digitalWrite(ledr_2,0);
  digitalWrite(ledy_2,1);
  digitalWrite(ledg_2,0);
  digitalWrite(ledr_1,0);
  digitalWrite(ledy_1,1);
  digitalWrite(ledg_1,0);
  delay(1500);
  digitalWrite(ledr_2,0);
  digitalWrite(ledy_2,0);
  digitalWrite(ledg_2,1);
  red1();
}

void led1()
{
  int k=10;
  Serial.println("ngat");
  for (int i=0;i<k;i++)
  {
    delay(1500);
    Serial.println(but3);
    i=1;
  }

}

void led2()
{
  a = 0;
  sr.setAllHigh();
  delay(50);
  if (but3 == 1)
  {  
    red2();
    for (a; a == 0; a)
    {
    if (but3 == 1)
      for (b; b == 0; b)
      {
        green2();
        if (but3 == 1) 
        {
          b = 1;
          if (but3 == 1) a = 1;
        }
      }
    }
  }
}

void countDown()
{
  for (number; number > h; number)
  {
    //Split number to digits:
    digit1 = number % 10;
    digit2 = (number / 10) % 10;
    digit3 = (number / 100) % 10;
    digit4 = (number / 1000) % 10;
    //Send them to 7 segment displays
    uint8_t numberToPrint[] = {numberB[digit4], numberB[digit3], numberB[digit2], numberB[digit1]};
    sr.setAll(numberToPrint);
    //Reset them for next time
    digit4 = 0;
    digit3 = 0;
    digit2 = 0;
    digit1 = 0;
    delay(1000); // Repeat every 1 sec  
    number = number - 101;
  }
}

void blink()
{
  sr.setAllHigh(); // set all pins Low (off)
  delay(50);
}

void mode()
{
  switch (number)
  {
  case 1510: // Red_1 - Green_2
    digitalWrite (ledg_1,0);            // Green_1 OFF - Red_1 ON
    digitalWrite (ledr_1,1); 
    digitalWrite (ledr_2,0);            // Red_2 OFF - Green_2 ON
    digitalWrite (ledg_2,1);
    h = 500;
    countDown();
    blink();

  case 500:
    number = 505;
    dk = 1;

  case 505: 
    if (dk == 1)      // Red_1 - Yellow_2
    {
      digitalWrite(ledg_2,0);           // Green_2 OFF - Yellow_2 ON
      digitalWrite(ledy_2,1);
      h = 0;
      countDown();
      blink();
      dk = 1;
    }
    else if (dk == 0)       // Yellow_1 - Yellow_2
    {
      digitalWrite(ledg_1,0);           // Green_1 OFF - Yellow_1 ON
      digitalWrite(ledy_1,1);
      digitalWrite(ledr_2,0);           // Red_2 OFF - Yellow_2 ON
      digitalWrite(ledy_2,1);
      h = 0;
      countDown();
      blink();
      number = 1510;
    }

  case 0: 
    if(dk == 1)       // Yellow_1 - Red_2
    number = 515;
    else if(dk == 0) number = 505;      // Yellow_1 - Yellow_2

  case 515: // Yellow_1 - Red_2
    digitalWrite (ledr_1,0);            // Red_1 OFF - Yellow_1 ON
    digitalWrite (ledy_1,1); 
    digitalWrite (ledy_2,0);           // Yellow_2 OFF - Red_2 ON
    digitalWrite (ledr_2,1); 
    h = 10;
    countDown(); 
    blink();

  case 10: 
    number = 1010;

  case 1010: // Green_1 - Red_2
    digitalWrite (ledy_1,0);            // Yellow_1 OFF - Green_1 ON
    digitalWrite (ledg_1,1); 
    h = 0;
    countDown(); 
    blink(); 
    dk = 0; 
  }
}