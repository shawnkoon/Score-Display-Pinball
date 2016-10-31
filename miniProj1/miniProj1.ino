#include <TimerOne.h>
#include <stdio.h>
#define NUM_DISPLAYS 5
#define DISPLAY_DIGITS 6

long displayVal[NUM_DISPLAYS] = {0L, 0L, 0L, 0L, 0L};
String displayDigits[NUM_DISPLAYS] = {"000000", "000000", "000000", "000000", "000000"};
bool displayRunning[NUM_DISPLAYS] = {false, false, false, false, false};
bool allRunning = false;
int digitToPrint = 0;

TimerOne updatingTimer;
TimerOne displayingTimer;

/*
 * This function will increment the value at given [displayNumber]by 8353
 * Also stores corresponding score into String array of displayDigits array.
 */
void displayConvert(int displayNumber)
{
    displayVal[displayNumber] += 8353;
    displayVal[displayNumber] %= 1000000;
    
    displayDigits[displayNumber] = String(displayVal[displayNumber]);
}// End of displayConvert


/*
 * This function will be ran by updatingTimer
 */
void updateScores()
{
  if(allRunning == false)
  {
    /*
     * Find which display needs to start.
     * Once found, update 3 tables accordingly.
     * # need to search from backword. 
     */
     
    int index_need_run;
    bool index_found = false;
    
    for(index_need_run = (NUM_DISPLAYS - 1); index_need_run >= 0; index_need_run--)
    {
      if(displayRunning[index_need_run] == false)
      {
        index_found = true;
        break;
      }
    }

    if(index_found == true)
    {
      /*
       * change the boolean to true and update the long display array, String display array down to that index.
       * Ex) if found_index == 2, then update timer 5, 4, 3, 2.
       */

       displayRunning[index_need_run] = true;
    }
    else // if index was not found.
    {
      /*
       * Set the all Running  to true.
       * Still need to update all the numbers.
       */
      allRunning = true;
      index_need_run = 0;
    }

       int index;
       for(index = (NUM_DISPLAYS - 1); index >= index_need_run; index--)
       {
         displayConvert(index);
       }
    
  }
  else // if all of them are running.
  {
    /*
     * Increment all the displayVal & change displayDigits array accordingly.
     */
     int index;
     
     for(index = (NUM_DISPLAYS - 1); index >= 0; index --)
     {
      displayConvert(index);
     }
  }

  printAll();
  
}// End of updateScores()

/*
 * Debug printing funciton. Called right before updateScores() finishes.
 */
void printAll()
{
  int i = 0;
  for(i = 0; i < 5; i++)
  {
    Serial.println(displayDigits[i]);
  }

  Serial.print("\n\n\n\n\n");
}


void setup()
{
  Serial.begin(9600); //DEBUG
  updatingTimer.initialize(500000); // timer every half second. 
  displayingTimer.initialize(31000); // timer every 3.1 millisecond.

  updatingTimer.attachInterrupt(updateScores);
}

void loop()
{
  
}
