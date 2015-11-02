// pins
const int blinker_btn[] = {2, 3}, blinker_led[] = {4, 5};  

// states
int button_state[] = {0, 0}, use_delay = 0;

void setup() 
{
  Serial.begin(9600);
  
  // turn buttons
  for(int x = 0; x <= 1; x++){pinMode(blinker_btn[x], INPUT);}
  
  // blinker leds
  for(int x = 0; x <= 1; x++){pinMode(blinker_led[x], OUTPUT);}
}

void loop() 
{
  handle_turns();
  run_blinker();
  logging();
  if(use_delay){delay(500); use_delay = 0;}
}

void handle_turns()
{
  /* Old code
  turn_state = map(analogRead(pseudo_turn), 1, 700, 270, 1);
  
  if(turned_state == "left" || turned_state == "right")
  {
    if(turn_state > 90 && turn_state < 180)
    {
      if(turned_state == "left"){blinker_state[0] = 0;}
      else if(turned_state == "right"){blinker_state[1] = 0;}
      turned_state = "";
    }
  }
  else
  {
    if(turn_state <= 90){turned_state = "left";}
    else if(turn_state >= 180){turned_state = "right";}
  }
  */
}

void logging()
{
  // blinker states
  Serial.print(button_state[0]);
  Serial.print(" ");
  Serial.println(button_state[1]);
  
  Serial.println("-------");
  Serial.println();
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

void run_blinker()
{
  read_blinkers();
  // blink function goes here
}
