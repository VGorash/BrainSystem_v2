#pragma once

////////////////// PIN SETTINGS | НАСТРОЙКИ ПИНОВ ////////////////////////
// Button pins mode | Режим пинов кнопок
// INPUT PULLUP or INPUT (if board has pullup resistors)
#define BUTTONS_PIN_MODE INPUT

// LEDs for players 1-4 | Светодиоды для игроков 1-4
#define LED_PLAYER_4 6
#define LED_PLAYER_3 5
#define LED_PLAYER_2 8
#define LED_PLAYER_1 7

// LED for game signals | Светодиод сигнала старта игры
#define LED_SIGNAL 13
// Buttons for players 1-4 | Кнопки игроков 1-4
#define BUTTON_PLAYER_4 A3
#define BUTTON_PLAYER_3 A2
#define BUTTON_PLAYER_2 A1
#define BUTTON_PLAYER_1 A0

// Control buttons | Управляющие кнопки
#define BUTTON_START 2
#define BUTTON_FUNC 4
#define BUTTON_STOP 3

// Buzzer | Динамик
#define BUZZER 9 // buzzer pin should support PWM | пин должен поддерживать генерацию ШИМ сигнала
