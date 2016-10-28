#include <TimerOne.h>
#include <stdio.h>
#define NUM_DISPLAYS 5
#define DISPLAY_DIGITS 6

//TimerOne timer;

void displayConvert()
{
  long displayVal[NUM_DISPLAYS] = {0L, 0L, 0L, 0L, 0L};
  char displayDigits[NUM_DISPLAYS][DISPLAY_DIGITS] = {"000000", "000000", "000000", "000000", "000000"};
  // I thought this needed an extra char on DISPLAY_DIGITS for the terminate character...
  // but it seems to get the values right
  
  for(int x = 0; x < NUM_DISPLAYS; x++)
  {
    while(displayVal[x]+8353 < 1000000)
    {
      displayVal[x] += 8353; //% 1000000 later when it continously runs?
      Serial.println(displayVal[x]); //DEBUG
      sprintf(displayDigits[x], "%06lu", displayVal[x]);
      for(int test = 0; test < DISPLAY_DIGITS; test++) //DEBUG
        Serial.print(displayDigits[x][test]); //DEBUG
      Serial.println(); //DEBUG
      Serial.println(); //DEBUG
      delay(1000); //DEBUG

      
    }
    Serial.println();
  }
}

void setup()
{
  Serial.begin(9600); //DEBUG

}

void loop()
{
  displayConvert();
  
}
