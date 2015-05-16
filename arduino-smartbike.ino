// neopixel
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define PIN            9
#define NUMPIXELS      64
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int led_matrixRight[] = {0,0,0,0,1,0,0,0,
                         0,0,0,0,1,1,0,0,
                         0,0,0,0,1,1,1,0,
                         1,1,1,1,1,1,1,1,
                         1,1,1,1,1,1,1,1,
                         0,0,0,0,1,1,1,0,
                         0,0,0,0,1,1,0,0,
                         0,0,0,0,1,0,0,0};
                    
int led_matrixLeft[] = {0,0,0,1,0,0,0,0,
                        0,0,1,1,0,0,0,0,
                        0,1,1,1,0,0,0,0,
                        1,1,1,1,1,1,1,1,
                        1,1,1,1,1,1,1,1,
                        0,1,1,1,0,0,0,0,
                        0,0,1,1,0,0,0,0,
                        0,0,0,1,0,0,0,0};
                        
// pins
const int turn_sense[] = {2, 3}, blinker_btn[] = {5, 6}, blinker_led[] = {7, 8};  

// states
int button_state[] = {0, 0}, turn_state[] = {0, 0}, use_delay = 0;
int state;
                        
void setup() 
{
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  
  Serial.begin(9600);
  
  Serial.print("initializing neopixels...");
  pixels.begin();
  Serial.println("OK");
  
  Serial.print("initializing pins...");
  for(int x = 0; x <= 1; x++)
  {
    pinMode(turn_sense[x], INPUT);
    pinMode(blinker_btn[x], INPUT);
    pinMode(blinker_led[x], OUTPUT);
  }
  Serial.println("OK");
}

void loop() 
{
  run_turns();
  run_blinker();
  logging();
  if(use_delay){delay(500); use_delay = 0;}
}

void logging()
{
  /*
  Serial.print("btn state: ");
  Serial.print(button_state[0]);
  Serial.print(" ");
  Serial.println(button_state[1]);
  */
   
  Serial.print("turn state: ");
  Serial.print(turn_state[0]);
  Serial.print(" ");
  Serial.println(turn_state[1]);
}

void read_blinkers()
{
  int blinker_btn_state[2];
  for(int x = 0; x <= 1; x++){blinker_btn_state[x] = digitalRead(blinker_btn[x]);}
  
  if(blinker_btn_state[0] && blinker_btn_state[1]){button_state[0] = 0; button_state[1] = 0; use_delay = 1;}
  else if(blinker_btn_state[0] || blinker_btn_state[1])
  {
    for(int x = 0; x <= 1; x++)
    {
      if(blinker_btn_state[x] && button_state[x] != 1){button_state[x] = 1; use_delay = 1;}
      else if(blinker_btn_state[x] && button_state[x] == 1){button_state[x] = 0; use_delay = 1;}
      else{button_state[x] = 0;}
    }
  }
}

void read_turns()
{
  for(int x = 0; x <= 1; x++)
  {
    if(button_state[x] && turn_state[x] && !digitalRead(turn_sense[x]))
    {
      button_state[0] = 0;
      button_state[1] = 0;
    }
    else
    {
      turn_state[x] = digitalRead(turn_sense[x]);
    }
  }  
}

void run_blinker()
{
  read_blinkers();
  
  if(button_state[0]){state = 2;}
  else if(button_state[1]){state = 1;}
  else{state = 0;}
  tail_light(0); // reset 
  tail_light(state);
}

void run_turns()
{
  read_turns();
}

void tail_light(int state) 
{
  for(int i=0;i<NUMPIXELS;i++)
  {
    switch(state)
    {
      case 1: 
        if(led_matrixRight[i] == 1){pixels.setPixelColor(i, pixels.Color(0,100,0));}
        break;
      case 2:
        if(led_matrixLeft[i] == 1){pixels.setPixelColor(i, pixels.Color(0,100,0));}
        break;
      default:
        pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
        
    pixels.show();
  }
  
  if(state == 1 || state == 2)
  {
    if(state == 1){digitalWrite(blinker_led[0], HIGH); digitalWrite(blinker_led[1], LOW);}
    else{digitalWrite(blinker_led[0], LOW); digitalWrite(blinker_led[1], HIGH);}
    delay(500);
  }
  else
  {
    digitalWrite(blinker_led[0], LOW); 
    digitalWrite(blinker_led[1], LOW);
  }
}
