#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI

uint8_t frame = 0;

void setup() {
  Serial.begin(9600);
  MIDI.begin(1);
  MIDI.turnThruOff();
  
  // flip screen, if required
  //u8g.setRot180();
#if defined(ARDUINO)
  pinMode(13, OUTPUT);        
  digitalWrite(13, HIGH);  
#endif
}



int y = 0;

int btn_val;
int btn = 0;

int cursor_x;
int cursor_y;

int current_step = 0;

int steps[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
void step(){

  btn_val = analogRead(A0);


  if(btn_val < 80){ //LEFT
    btn = 1;
  }else if(btn_val < 200){ //UP
    btn = 3;
  }else if(btn_val < 400){ //DOWN
    btn = 4;
  }else if(btn_val < 600){ //RIGHT
    btn = 2;
  }else if(btn_val < 800){ //ENTER
    btn = 5;
  }else{
    btn = 0;
  }

  if(btn == 1){
    cursor_x -= 1;
    if(cursor_x < 0){
      cursor_x = 15;
    }
  }else if(btn == 2){
    cursor_x += 1;
    if(cursor_x > 15){
      cursor_x = 0;
    }
  }else if(btn == 3){
    cursor_y -= 1;
    if(cursor_y < 0){
      cursor_y = 31;
    }
  }else if(btn == 4){
    cursor_y += 1;
    if(cursor_y > 31){
      cursor_y = 0;
    }
  }else if(btn == 5){
    steps[cursor_x] = cursor_y;
  }


  MIDI.sendNoteOff(48 + 32 - steps[current_step],100,1);
  MIDI.sendNoteOn(48 + 32 - steps[current_step],100,1);
  
};


void draw(){

  for(int i=0; i < 16; i++){
    u8g.drawFrame(i*8 + 3, steps[i] * 2 + 3, 2, 2);
  }
  
  u8g.drawBox( current_step * 8 +1, steps[current_step] *2, 6, 6);
  
  u8g.drawFrame( cursor_x * 8 , cursor_y *2 ,8, 8);
}





void loop() {

  step();
  
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );

  frame++;
  current_step = frame%16;

}


