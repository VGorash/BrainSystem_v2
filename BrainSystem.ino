#include "settings.h"
#include "Game.h"

#include <EncButton.h>


Button btnStart(BUTTON_START, BUTTONS_PIN_MODE);
Button btnStop(BUTTON_STOP, BUTTONS_PIN_MODE);
Button btnFunc(BUTTON_FUNC, BUTTONS_PIN_MODE);

Button btnP1(BUTTON_PLAYER_1, BUTTONS_PIN_MODE);
Button btnP2(BUTTON_PLAYER_2, BUTTONS_PIN_MODE);
Button btnP3(BUTTON_PLAYER_3, BUTTONS_PIN_MODE);
Button btnP4(BUTTON_PLAYER_4, BUTTONS_PIN_MODE);

Game *game;

void setup() {
  Serial.begin(9600);
  initPins();
  
  btnStart.attach(startButtonCallback);
  btnStop.attach(stopButtonCallback);
  btnFunc.attach(funcButtonCallback);

  btnP1.attach([](){playerButtonCallback(&btnP1, LED_PLAYER_1);});
  btnP2.attach([](){playerButtonCallback(&btnP2, LED_PLAYER_2);});
  btnP3.attach([](){playerButtonCallback(&btnP3, LED_PLAYER_3);});
  btnP4.attach([](){playerButtonCallback(&btnP4, LED_PLAYER_4);});

  game = new Game(false);
  game->showGreeting();
}

void loop() {
  game->tick();
  
  btnStart.tick();
  btnStop.tick();
  btnFunc.tick();

  btnP1.tick();
  btnP2.tick();
  btnP3.tick();
  btnP4.tick();
}

void initPins() {
  pinMode(LED_PLAYER_1, OUTPUT);
  pinMode(LED_PLAYER_2, OUTPUT);
  pinMode(LED_PLAYER_3, OUTPUT);
  pinMode(LED_PLAYER_4, OUTPUT);
  pinMode(LED_SIGNAL, OUTPUT);
}

void playerButtonCallback(Button *button, int led){
  if(button->press()) {
    game->onPlayerButtonPress(led);
  }
}

void startButtonCallback(){
  if(btnStart.press()){
    game->onStartButtonPress();
  }
}

void stopButtonCallback(){
  if(btnStop.press()){
    game->onStopButtonPress();
  }
}

void funcButtonCallback(){
  if(btnFunc.click()){
    game->switchSound();
  }
  else if(btnFunc.hold()){
    Game *temp = game;
    game = game->nextGame();
    delete temp;
    game->showType();
  }
}
