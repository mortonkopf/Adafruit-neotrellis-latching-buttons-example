/* This example shows basic usage of the
MultiTrellis object controlling an array of
NeoTrellis boards

this example is different from the basic
usage example as it uses LATCHING BUTTONS
not momentary push. The buttons latch on,
then off, so push on - push-off.

both states use the rising edge to trigger.
mortonkopf
*/

#include "Adafruit_NeoTrellis.h"

#define Y_DIM 4 //number of rows of key
#define X_DIM 8 //number of columns of keys

//create a matrix of trellis panels
Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
  
  { Adafruit_NeoTrellis(0x2f), Adafruit_NeoTrellis(0x2e) } //address order here
  
};

int trellisButtonState[32] ={
  0,0,0,0, 0,0,0,0,
  0,0,0,0, 0,0,0,0,
  0,0,0,0, 0,0,0,0,
  0,0,0,0, 0,0,0,0,
};


//pass this matrix to the multitrellis object
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM/4, X_DIM/4);

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}

//define a callback for key presses ----- uses latching button state
TrellisCallback blink(keyEvent evt){
  
  if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
   
    if(trellisButtonState[evt.bit.NUM]==0){
      trellis.setPixelColor(evt.bit.NUM, Wheel(map(evt.bit.NUM, 0, X_DIM*Y_DIM, 0, 255))); //on rising
      trellisButtonState[evt.bit.NUM]=1;
        trellis.show();}
     
     else if(trellisButtonState[evt.bit.NUM]==1){
      trellis.setPixelColor(evt.bit.NUM, 0);//on rising
      trellisButtonState[evt.bit.NUM]=0;
      trellis.show();}
  }
  /*
  else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
    trellis.setPixelColor(evt.bit.NUM, 0); //off falling
    */
 // trellis.show();
  return 0;
}

void setup() {
  Serial.begin(9600);
  //while(!Serial);

  if(!trellis.begin()){
    Serial.println("failed to begin trellis");
    while(1);
  }

  /* the array can be addressed as x,y or with the key number */
  for(int i=0; i<Y_DIM*X_DIM; i++){
      trellis.setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255))); //addressed with keynum
      trellis.show();
      delay(50);
  }
  
  for(int y=0; y<Y_DIM; y++){
    for(int x=0; x<X_DIM; x++){
      //activate rising and falling edges on all keys
      trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
      trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
      trellis.registerCallback(x, y, blink);
      trellis.setPixelColor(x, y, 0x000000); //addressed with x,y
      trellis.show(); //show all LEDs
      delay(50);
    }
  }

}

void loop() {
    if(!digitalRead(20)){
  trellis.read();
    }
}
