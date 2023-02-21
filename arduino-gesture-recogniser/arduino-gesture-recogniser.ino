/**
 * Demonstration of PAJ7620 Gesture Recogniser module
 */

// INCLUDES
// Arduino Wire library is for communicating with any I2C device
#include "Wire.h"
// PAJ7620 library, based on datasheet as described at 
// https://www.epsglobal.com/Media-Library/EPSGlobal/Products/files/pixart/PAJ7620F2.pdf?ext=.pdf
#include "paj7620.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3
#define FORWARD 4
#define BACK 5
#define CLOCK 6
#define ANTI 7
int led = 13;
int sound = 6;
const int NUM = 16;
int count = 0;

int *gestureData = new int[NUM];

int oppositeOf(int gesture)
{
  if (gesture == UP)
    return DOWN;  
  else if (gesture == DOWN)
    return UP;
  else if (gesture == LEFT)
    return RIGHT;
  else if (gesture == RIGHT)
    return LEFT;
  else if (gesture == FORWARD)
    return BACK;
  else if (gesture == BACK)
    return FORWARD;
  else if (gesture == CLOCK)
    return ANTI;
  else if (gesture == ANTI)
    return CLOCK;
}

void setup() {
  // Initialise serial connection to display results
	Serial.begin(38400);

  // Initialise the sensor
  int error = paj7620Init();
	if(error) {
    Serial.print(F("Initialisation error code: "));
    Serial.println(error);
	}
	else {
		Serial.println(F("Ready!"));
	}

  pinMode(led, OUTPUT);
  pinMode(sound, OUTPUT);  
}

void loop() {
  // Create a variable to hold the value of any gesture recognised
	byte gesture;
  // Error variable holds any error code
	int error;
  // Read Reg 0x43 of Bank 0 to get result of any recognised gesture, and store in 'gesture' variable
	error = paj7620ReadReg(0x43, 1, &gesture);
 
	if(!error) {
		switch (gesture) {
			case GES_RIGHT_FLAG:
        gestureData[count] = RIGHT;      
			  Serial.println(F("Right"));          
				break;
			case GES_LEFT_FLAG: 
        gestureData[count] = LEFT;      
				Serial.println(F("Left"));         
				break;
			case GES_UP_FLAG:
        gestureData[count] = UP;      
				Serial.println(F("Up"));        
				break;
			case GES_DOWN_FLAG:
        gestureData[count] = DOWN;      
				Serial.println(F("Down"));         
				break;
			case GES_FORWARD_FLAG:
        gestureData[count] = FORWARD;      
				Serial.println(F("Forward"));
				break;
			case GES_BACKWARD_FLAG:	
        gestureData[count] = BACK;      	  
				Serial.println(F("Backward"));
				break;
			case GES_CLOCKWISE_FLAG:		
        gestureData[count] = CLOCK;        
				Serial.println(F("Clock"));
				break;
			case GES_COUNT_CLOCKWISE_FLAG:
        gestureData[count] = ANTI;      		  
				Serial.println(F("Counter-Clock"));
				break;
      // Library also defines GES_CLOCKWISE_FLAG GES_COUNT_CLOCKWISE_FLAG and GES_WAVE_FLAG, but I found these unreliable
			default:
        count--; // no gesture
				break;
		}
	}
 else {
    Serial.print(F("Error code: "));
    Serial.println(error);
 }

 if (count == NUM - 1) {
   count = 0;
   int asymmetric = 0;
   for (int i = 1; i < NUM - 1; i++)
   {
     if(gestureData[i - 1] != oppositeOf(gestureData[i])) 
     {
       asymmetric++;
     }
     if (gestureData[i - 1] != gestureData[i + 1])
     {
       asymmetric++;
     }              
   }

   if (asymmetric < 0.4 * NUM * 2) // if asymmetricity less than 20 percent of the total numbers
                                   // we multiply NUM by 2 because we are incrementing asymmetric twice per iteration
   {
     Serial.println("Happy");
     digitalWrite(led, HIGH);
     tone(sound,6000,250);     
     delay(500);
     tone(sound,4000,250);
     delay(500);
     tone(sound,6000,250);     
     digitalWrite(led, LOW);
   }
   else
   {
     Serial.println("Stressed");
     digitalWrite(led, HIGH);
     for(int j = 0;j<6;j++ ){
       tone(sound,j*1000,300);
       delay(500);
       tone(sound,j*1000,300);       
       delay(100);
     }
     digitalWrite(led, LOW);
   }


  delete [] gestureData;
}

  // Introduce small delay before next polling the sensor 
  delay(200);
  count++;

}
