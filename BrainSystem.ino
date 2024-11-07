#include "settings.h"
#include "display.h"
#include "Game.h"
#include "SettingsWindow.h"
#include "Storage.h"

#include <EncButton.h>

#define VERSION 1

Button btnStart(BUTTON_START, BUTTONS_PIN_MODE);
Button btnStop(BUTTON_STOP, BUTTONS_PIN_MODE);
Button btnFunc(BUTTON_FUNC, BUTTONS_PIN_MODE);

Button btnP1(BUTTON_PLAYER_1, BUTTONS_PIN_MODE);
Button btnP2(BUTTON_PLAYER_2, BUTTONS_PIN_MODE);
Button btnP3(BUTTON_PLAYER_3, BUTTONS_PIN_MODE);
Button btnP4(BUTTON_PLAYER_4, BUTTONS_PIN_MODE);

Runnable *window;
Display display;

void setup() {
  Serial.begin(9600);
  display.init();
  display.textMode(BUF_REPLACE);
  Wire.setClock(800000L);
  initPins();
  
  btnStart.attach(startButtonCallback);
  btnStop.attach(stopButtonCallback);
  btnFunc.attach(funcButtonCallback);

  btnP1.attach([](){playerButtonCallback(&btnP1, 0);});
  btnP2.attach([](){playerButtonCallback(&btnP2, 1);});
  btnP3.attach([](){playerButtonCallback(&btnP3, 2);});
  btnP4.attach([](){playerButtonCallback(&btnP4, 3);});

  State state = Storage::getState();
  Sound sound = Sound(state.soundMode);

  showGreeting(sound);

  window = Game::fromState(state, display);
}

void loop() {
  checkUart();

  window->tick();
  
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

void showGreeting(Sound& sound){
  display.clear();
  display.setCursor(5, 2);
  display.setScale(2);
  display.print("System 2.");
  display.print(VERSION);
  display.setCursor(64, 6);
  display.setScale(1);
  display.print("by VGorash");
  digitalWrite(LED_PLAYER_1, 1);
  sound.playTone(1000, 300);
  delay(500);
  digitalWrite(LED_PLAYER_1, 0);
  digitalWrite(LED_PLAYER_2, 1);
  sound.playTone(1500, 300);
  delay(500);
  digitalWrite(LED_PLAYER_2, 0);
  digitalWrite(LED_PLAYER_3, 1);
  sound.playTone(2000, 300);
  delay(500);
  digitalWrite(LED_PLAYER_3, 0);
  digitalWrite(LED_PLAYER_4, 1);
  sound.playTone(2500, 300);
  delay(500);
  digitalWrite(LED_PLAYER_4, 0);
}

void playerButtonCallback(Button *button, int player){
  if(button->press()) {
    window->onPlayerButtonPress(player);
  }
}

void startButtonCallback(){
  if(btnStart.press()){
    window->onStartButtonPress();
  }
}

void stopButtonCallback(){
  if(btnStop.press()){
    window->onStopButtonPress();
  }
}

void funcButtonCallback(){
  if(btnFunc.click()){
    window->onFuncButtonPress();
  }
  else if(btnFunc.hold()){
    changeGame();
  }
}

void changeGame()
{
  bool isSettings = strcmp(window->getName(), "НАСТРОЙКИ") == 0;
  delete window;
  State state = Storage::getState();
  if(isSettings)
  {
    window = Game::fromState(state, display);
  }
  else
  {
    window = new SettingsWindow(state, display);
  }
}

void checkUart()
{
  if(Serial.available())
  {
    int data = Serial.read();
    if(data == -1)
    {
      return;
    }
    window->onUartDataReceive((byte)data);
  }
}
