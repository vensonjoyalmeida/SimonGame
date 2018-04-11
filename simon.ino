 #include <LiquidCrystal.h>
 #include <CapacitiveSensor.h>
 CapacitiveSensor   cs_11_13 = CapacitiveSensor(11,13); 
 CapacitiveSensor   cs_7_10 = CapacitiveSensor(7,10);
 LiquidCrystal lcd( 14, 15, 16, 17, 18, 19);
 int ledState = 0;
 int gameState = 0;
 #define WINNUM 5    //No of rounds
 int Order[WINNUM];
 int level = 0;
 int led[] = {12, 9, 5, 2}; 
 int button[] = {13, 10, 6, 3};
 int count = 4; 
 int buzzer = 8;
 int speed = 500;
 
//declare functions
void setLed(int ledNum);
void displayClue(int level);
boolean waitForInput(int buttonNum);
 
void setup () {
Serial.begin(9600);
lcd.begin (16,2);
lcd.clear();
lcd.print("Welcome to Simon!");

Serial.println("setup");
 
  for(int Pin = 0; Pin < count; Pin++) 
  {
  //set all leds to output
  pinMode(led[Pin], OUTPUT);
  }
    
  for(int Pin = 0; Pin < count; Pin++)
  {
  
  pinMode(button[Pin], INPUT_PULLUP);
  }
 //different seed numbers each time the sketch runs
 //randomSeed() will shuffle the random function that we call during playGame
 randomSeed(analogRead(0));
 }

void loop () {
  
  if (gameState == 0)
  {  
     
     long total1 =  cs_11_13.capacitiveSensor(30);
     long total2 =  cs_7_10.capacitiveSensor(30);  
     int button2 = digitalRead(button[2]); 
     int button3 = digitalRead(button[3]); 
     
    //if no buttons are pressed 
    if (total1<300 && total2<300 && button2 && button3)
    { 
      lcd.clear();
      lcd.print("Welcome to Simon!");
      lcd.setCursor(0,1);
      lcd.print("Press any button");
      delay(800);
      pre_game();
      premusic();
      
    }
    // if a button is pressed set up for the game
    else
    {
      Serial.println("button Pressed"); 
      //turn leds off before starting game --> function setLed() states that if ledNum is <0 led is LOW
      setLed(-1);
      //set the round number to zero so you start on the first round
      level = 0;
      delay (1000); 
      gameState = 1;
    }
  }

  if (gameState == 1)
  { 
    lcd.clear();
    lcd.print("LETS PLAY");
    
    playGame();
  }
  
  if (gameState == 2)
  {
    delay(500);
    lcd.clear();
    lcd.print("YOU WIN!!!");
    winning_sound();
    winDisplay();
    winDisplay();
    winDisplay();
    loseDisplay();
    Serial.println("winDisplay");
    delay (1000);
    //reset gameState to zero to call pre_game
    gameState = 0;
  }
  
  if (gameState == 3)
  {
    lcd.clear();
    lcd.print("YOU LOSE!!");
    lcd.setCursor(0,1);
    lcd.print("Try Again!");
    delay(500);
    losing_sound();
    loseDisplay();
    loseDisplay();
    loseDisplay();
    Serial.println("loseDisplay");
    delay (1000);
    //reset gameState to zero to call pre_game
    gameState = 0;
  } 
}


//pre_game
void pre_game()
{
  
  if (ledState == 0)
  {
    Serial.println("green");
    setLed(0);
    ledState = 1;
  
  }
  else if (ledState == 1)
  {
    Serial.println("red");
    setLed(1);
    ledState = 2;
    
  }
  else if (ledState == 2)
  {
    Serial.println("yellow");
    setLed(2);
    ledState = 3;
    
  }
  else if (ledState == 3)
  {
    Serial.println("blue");
    setLed(3);
    ledState = 0;
    
  }

  }

void playGame()
{
  Serial.println("playGame");
  //pre-load array with random numbers for game
  for ( int i=0; i<WINNUM; i++)
  {
    //use floor to round number down to nearest integer and random to choose random numbers from 0 to 3 to put in the Order array.
    Order[i] = floor(random(0,4));
    Serial.println(Order[i], DEC);
  }

  
  Serial.print("Starting Round ");
  Serial.println(level, DEC);
  for (int r=0; r<WINNUM; r++)
  {
    displayClue(level);
  
    for (int i=0; i<=level; i++)
    {      
      Serial.print("Waiting for input ");
      Serial.print(i+1, DEC);
      Serial.print(" of ");
      Serial.print(level+1, DEC);
      Serial.println("");
  
        if (waitForInput(Order[i]))
      {
        //correct input
        if (i==level)
        { 
          
          Serial.println("Correct");
          lcd.clear();
          lcd.print("CORRECT!");
          lcd.setCursor(0,1);
          lcd.print("Next Round");
          //check if final round 
          if (level == WINNUM-1)
          {
            Serial.println("You Win!");
           // set gameState to 2 to run winning fuction
           gameState = 2;
           speed = 500;
           return;
          }
          else 
          {
            Serial.println("Next Level");
          }
        }
      }
      else 
      {
        //incorrect input
        Serial.println("You Lose!");
        gameState = 3;
        speed = 500;
        return;
      }
    }
    level++;
    //Delay briefly so user has time to see start of next clue
    delay(1000);
  }
}

void setLed(int ledNum)
{
  if (ledNum<0) //no LED light
  {
   for (int i = 0; i<count; i++)
   {
   digitalWrite(led[i], LOW);
   }
  }
  else
  {
    for (int i = 0; i<count; i++) 
    {
      if (i == ledNum)
      {
        digitalWrite(led[i], HIGH);
        
      }
      else 
      {
        digitalWrite(led[i], LOW);
      }
    }
  }
 }


 void displayClue(int level)
 {
  //turn all lights off
  setLed(-1);
  for (int i=0; i<=level; i++)
  {
    //turn on led that matches current round
    setLed(Order[i]);
    tone(buzzer, 294, 250);
    speed = speed *3/4;
    delay(speed);
    setLed(-1);
    delay(250);
  }
 }

boolean waitForInput(int buttonNum)
{
  //read all the buttons/ capacitive sensors continuously until a button is pressed, if a button is pressed check if it's correct and if so return true. if not, return false. 
  while(true) 
  {
   long total1 =  cs_11_13.capacitiveSensor(30);
   long total2 =  cs_7_10.capacitiveSensor(30);  
   int button2 = digitalRead(button[2]); 
   int button3 = digitalRead(button[3]); 
   
   
   if (total1>300 || total2>300 || !button2 || !button3) // 2 capacitive cloths and 2 buttons
   {
      //Delay briefly to avoid double counting button press
      delay(250);

      //light up the corresponding led when a button is pressed
      if (buttonNum == 0)
      {
        digitalWrite(led[0], HIGH);
        tone(buzzer, 400, 250);
        delay(250);
        digitalWrite(led[0], LOW);
        
      }
      else if (buttonNum == 1)
      {
        digitalWrite(led[1], HIGH);
        tone(buzzer, 500, 250);
        delay(250);
        digitalWrite(led[1], LOW);
        
      }
      else if (buttonNum == 2)
      {
        digitalWrite(led[2], HIGH);
        tone(buzzer, 600, 250);
        delay(250);
        digitalWrite(led[2], LOW);
        
      }
      else if (buttonNum == 3)
      {
        digitalWrite(led[3], HIGH);
        tone(buzzer, 700, 250);
        delay(250);
        digitalWrite(led[3], LOW);
        
      }
      
      //check if correct button was pressed
      if (buttonNum == 0 && total1>300) 
      {
        Serial.println("Button 1 Pressed");
        return true;
      }
      else if (buttonNum == 1 && total2>300)
      {
        Serial.println("Button 2 Pressed");
        return true;
      }
      else if (buttonNum == 2 && button2 == 0)
      {
        Serial.println("Button 3 Pressed");
        return true;
      }
      else if (buttonNum == 3 && button3 == 0)
      {
        Serial.println("Button 4 Pressed");
        return true;
      }
      else
      {
        return false;
      }
    }
  }
}

void premusic(void) // standby music
{
  Serial.println("pre_game music");
  tone(buzzer, 200, 250);
  delay(200);
  tone(buzzer, 300, 250);
  delay(200);
  tone(buzzer, 400, 250);
  delay(200);
  tone(buzzer, 200, 250);
  delay(200);

}
void winDisplay() //Led flash in sequence
{
digitalWrite(led[0], HIGH);
delay(75);
digitalWrite(led[0], LOW);
delay(75);
digitalWrite(led[1], HIGH);
delay(75);
digitalWrite(led[1], LOW);
delay(75);
digitalWrite(led[2], HIGH);
delay(75);
digitalWrite(led[2], LOW);
delay(75);
digitalWrite(led[3], HIGH);
delay(75);
digitalWrite(led[3], LOW);
delay(75);
digitalWrite(led[2], HIGH);
delay(75);
digitalWrite(led[2], LOW);
delay(75);
digitalWrite(led[1], HIGH);
delay(75);
digitalWrite(led[1], LOW);
delay(75);
digitalWrite(led[0], HIGH);
delay(75);
digitalWrite(led[0], LOW);
delay(75);
}

void winning_sound(void) // winning sound
{
  Serial.println("buzzer1");
  tone(buzzer, 294, 250);
  delay(200);
  tone(buzzer, 294, 250);
  delay(200);
  tone(buzzer, 294, 250);
  delay(200);
  tone(buzzer, 392, 500);
  delay(500);
  Serial.println("second 392");
  tone(buzzer, 392, 250);
  delay(200);
  tone(buzzer, 440, 250);
  delay(200);
  tone(buzzer, 392, 250);
  delay(200);
  tone(buzzer, 440, 250);
  delay(200);
  tone(buzzer, 494, 500);
}


void loseDisplay() // High and low
{
  digitalWrite(led[0], HIGH);
  digitalWrite(led[1], HIGH);
  digitalWrite(led[2], HIGH);
  digitalWrite(led[3], HIGH);
  delay(200);
  digitalWrite(led[0], LOW);
  digitalWrite(led[1], LOW);
  digitalWrite(led[2], LOW);
  digitalWrite(led[3], LOW);
  delay(200);
}

void losing_sound(void)  //bethoven 5th
{
  Serial.println("losing_sound");
  tone(buzzer, 400, 250);
  delay(250);
   tone(buzzer, 400, 250);
  delay(250);
   tone(buzzer, 400, 250);
  delay(250);
  tone(buzzer, 320, 1000);
  delay(1250);
  tone(buzzer, 350, 250);
  delay(250);
   tone(buzzer, 350, 250);
  delay(250);
   tone(buzzer, 350, 250);
  delay(250);
  tone(buzzer, 300, 1000);
  delay(250);
}




