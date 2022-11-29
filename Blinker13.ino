// ATtiny13 LED blinker
// Copyright (c) 2021 Aleksandr.ru
// @see http://aleksandr.ru

#define LED1_PIN PB0
#define LED2_PIN PB1
#define LED3_PIN PB2
#define LED4_PIN PB4
#define MODE_PIN PB3

#define MAX_LAMP 4
#define MAX_MINUTES 60
#define LONG_DELAY 60
#define SHORT_DELAY 15

struct Lamp
{
    uint8_t pin;
    uint8_t minute;
    boolean state;
};

Lamp lamps [] = {
    {LED1_PIN, 0, false},
    {LED2_PIN, 0, false},
    {LED3_PIN, 0, false},
    {LED4_PIN, 0, false}
};

uint8_t old_minute = MAX_MINUTES;

uint32_t bootrandom __attribute__ ((section (".noinit")));

void setup() 
{
    pinMode(MODE_PIN, INPUT_PULLUP);

    randomSeed(bootrandom);
    
    for (uint8_t i = 0; i < MAX_LAMP; i++) {
        pinMode(lamps[i].pin, OUTPUT);
        lamps[i].state = !random(3); // 2/3 off     
    }
}

void loop() 
{
    // uint8_t minute = (millis() / 1000) % MAX_MINUTES;
    uint8_t minute = (millis() / 1000 / 60) % MAX_MINUTES;
 
    if (minute != old_minute) {
        old_minute = minute;

        for (uint8_t i = 0; i < MAX_LAMP; i++) {
            if (lamps[i].minute == minute) {
                change(lamps[i]);
            }
        }
    }
}

void change(Lamp &lamp)
{
    lamp.state = !lamp.state;
    digitalWrite(lamp.pin, lamp.state ? HIGH : LOW);
    uint8_t max_delay = !digitalRead(MODE_PIN) ? LONG_DELAY : SHORT_DELAY;    
    uint8_t new_delay = random(1, max_delay);
    if (!lamp.state) new_delay = new_delay / 2; // short off interval
    if (!new_delay) new_delay = 1;
    lamp.minute += new_delay;
    if (lamp.minute >= MAX_MINUTES) lamp.minute -= MAX_MINUTES;
}
