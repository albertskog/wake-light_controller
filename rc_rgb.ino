/*
  Modified version of RecieveDemo_Simple for controlling my RGB wake-up ligh
  
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
  
  Need help? http://forum.ardumote.com
*/


/*
Remote codes extracted from 
receiver example output:
     A
1361 - 1364
     B
4433 - 4436
     C
5201 - 5204
     D
5393 - 5396
 ON  - OFF
 
    "E"
5441 - 5444
*/

#include <RCSwitch.h>

// Arduino pins hooked up to the mosfets driving the light
#define PIN_RED        10
#define PIN_GREEN      9
#define PIN_BLUE       11

// Just for readability later..
#define RED            0
#define GREEN          1
#define BLUE           2

// "Keycodes" for the remote
#define REMOTE_A_ON    1361
#define REMOTE_A_OFF   1364
#define REMOTE_B_ON    4433
#define REMOTE_B_OFF   4436
#define REMOTE_C_ON    5201
#define REMOTE_C_OFF   5204
#define REMOTE_D_ON    5393
#define REMOTE_D_OFF   5396

#define REMOTE_ALARM_ON  5441
#define REMOTE_ALARM_OFF  5444


#define ALARM_ENABLE     true
#define ALARM_DISABLE    false

RCSwitch mySwitch = RCSwitch();
byte color[3] = {0, 0, 0};
bool alarmEnabled = false;
int increment = 4000;

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
}

void loop() {
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.print("Unknown encoding");
    }
    else 
    {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
      
      
      
      switch (mySwitch.getReceivedValue())
      {
        
        case REMOTE_A_ON:
          //cycle();
          color[RED]   = 60;
          color[GREEN] = 25;
          color[BLUE]  = 5;
          alarmEnabled = false;
          break;
          
        case REMOTE_A_OFF:
          color[RED]   = 0;
          color[GREEN] = 0;
          color[BLUE]  = 0;
          alarmEnabled = false;
          break;
      
        case REMOTE_B_ON:
          color[RED]   += 20;
          color[GREEN] += 20;
          color[BLUE]  += 20;
          alarmEnabled = false;
          break;
          
        case REMOTE_B_OFF:  
          color[RED]   -= 20;
          color[GREEN] -= 20;
          color[BLUE]  -= 20;
          alarmEnabled = false;
          break;
        
        case REMOTE_ALARM_ON:
          alarmEnabled = true;
          increment = 4000;
          break;
          
        case REMOTE_ALARM_OFF:
          alarmEnabled = true;
          increment = 100;
          break;
          
        default:
          break;
          
      }//switch
      
    }//else
    
    mySwitch.resetAvailable();
  }//if (mySwitch.available())
  
  alarm();
  
  analogWrite( PIN_RED, color[RED] );
  analogWrite( PIN_GREEN, color[GREEN] );
  analogWrite( PIN_BLUE, color[BLUE] );

}//loop


void alarm()
{
  //Timestamp for last increase in light
  static long lastTime = 0;
  
  //Milliseconds between increases in light
  //static int increment = 4000;
  
  //dimming state
  static int state = 0;
  
  if ( alarmEnabled )
  {
    if ( millis() > (lastTime + increment) )
    {
      
      switch (state)
      {
        case 0:
          color[BLUE]++;
          if ( color[BLUE] > 10 ) state++;
          break;
          
        case 1:
          color[BLUE]  = max(color[BLUE]--, 0);
          color[RED]++;
          if ( color[RED] > 15 ) state++;
          break;
          
        case 2:
          color[GREEN]++;
          if ( color[GREEN] > 5 ) state++;
          break;
          
        case 3:
          color[RED]    = min(color[RED]+3, 255);;          
          color[GREEN]++;
          if ( color[RED] > 180 ) state++;
          break;

        case 4:
          color[RED]   = min(color[RED]++, 255);
          color[GREEN] = min(color[GREEN]++, 255);
          color[BLUE]  = min(color[BLUE]+2, 255);
          if ( color[BLUE] > 254 ) state++;
          break;       
          
        default:
          alarmEnabled = false;
          break;
      }
      
      lastTime = millis();
    }
  }
  else
  {
    state = 0;
  }

}


void cycle()
{
  static int state = 0;
  
  state = (state +1)%3;
  
  int i;
  for(i=0; i<3; i++)
  {
    color[i]=0;
  }
  color[state]=200;
}
