#define NUM_DISPLAYS 5
#define DISPLAY_DIGITS 6

long displayVal[NUM_DISPLAYS] = {0L, 0L, 0L, 0L, 0L};
String displayDigits[NUM_DISPLAYS] = {"000000", "000000", "000000", "000000", "000000"};
bool displayRunning[NUM_DISPLAYS] = {false, false, false, false, false};
bool allRunning = false;
int digitToPrint = 0;
int curDigit = 0;
int displayPin[NUM_DISPLAYS] = {26, 27, 28, 29, 38}; //Display 5:1; 29:26 are 4 MSBs of PORTA
int enablePin[DISPLAY_DIGITS] = {52, 51, 50, 41, 40, 39}; //Left out 53; MSB to LSB

/*
 * This function will increment the value at given [displayNumber]by 8353
 * Also stores corresponding score into String array of displayDigits array.
 */
void displayConvert(int displayNumber)
{
    displayVal[displayNumber] += 8353;
    displayVal[displayNumber] %= 1000000;

    String s = String(displayVal[displayNumber]);

    if(s.length() < DISPLAY_DIGITS)
    {
      int i;
      int digitsToAdd = DISPLAY_DIGITS - s.length();
      for(i = 0; i < digitsToAdd; i++)
      {
        s = 0 + s;
      }

      displayDigits[displayNumber] = s;
    }
    else
    {
      displayDigits[displayNumber] = s;
    }
}// End of displayConvert


/*
 * This function will run every 1/2 secs
 */
void updateScores()
{
  delay(2000);
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
  int y = 0;
  Serial.println("Displays");
  Serial.println("Read Down");
  Serial.println("1 2 3 4 5");
  Serial.println("---------");
  for(y = 0; y < 6; y++)
  {
    int x;
    for(x = 0; x < 5; x++)
    {
      Serial.print(displayDigits[x].charAt(y));
      Serial.print(" ");
    }
    Serial.println();
  }

  Serial.print("\n\n\n\n\n");
  
}

//printScores will run every 3.1 msecs
void printScores()
{
  digitalWrite(A15, HIGH); //Blank all displays
  digitalWrite(enablePin[curDigit], LOW);
  curDigit = (curDigit + 1) % 6;
  digitalWrite(A9, HIGH);
  for(int i = 0; i < NUM_DISPLAYS; i++)
  {
    PORTA = 0b00000000 | byte(displayDigits[i].charAt(curDigit));
    digitalWrite(displayPin[i], HIGH);
    digitalWrite(displayPin[i], LOW);
  }
  digitalWrite(A9, LOW);
  digitalWrite(enablePin[curDigit], HIGH);
  digitalWrite(A15, LOW);
}


void setup()
{
  Serial.begin(9600); //DEBUG
  pinMode(A15, OUTPUT);
  pinMode(A9, OUTPUT); //Display Strobe Enable pin
  pinMode(38, OUTPUT); //5th Score Display Strobe bit
  pinMode(39, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(52, OUTPUT);
  pinMode(53, OUTPUT);
  // pins 29:22 are taken care of by the DDRA statement below
  DDRA = 0b11111111;

  attachInterrupt(2, printScores, RISING);
}

void loop()
{
  updateScores();
  
}
