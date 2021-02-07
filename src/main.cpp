#include <Arduino.h>
#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management
#include <avr/wdt.h>      // Watchdog timer

soft_ring_buffer* buf;

// const int RX_PIN=4;
// const int TX_PIN=5;
#define RX_PIN 4
#define TX_PIN 5
TinySoftwareSerial mySerial(buf,RX_PIN,TX_PIN); //RX, TX

// connected to pin 12 (ADKey1) of DFPLayer (pulling it LOW starts playing)
#define PLAY_PIN 0
// connected to PIR (HIGH = Movement, LOW = No Movement)
#define PIR_PIN 1
// connected to pin 16 (BUSY) of DFPlayer (LOW = playing, HIGH = Idle)
#define STATUS_PIN 2

// STATUS LEDs - testing only
#define POWER_STATUS 3
//#define PLAY_STATUS 4

#define watchdogRegister WDTCR


ISR (WDT_vect) 
{
   wdt_disable();  // disable watchdog
}  // end of WDT_vect


void setup() {
    pinMode(RX_PIN,INPUT);
    pinMode(TX_PIN,OUTPUT);
    mySerial.begin(9600);
    Serial.println(F("Setup started"));
    wdt_reset();  

    pinMode(PLAY_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);
    pinMode(STATUS_PIN, INPUT);
    pinMode(POWER_STATUS, OUTPUT);
    digitalWrite(POWER_STATUS, LOW);
//    pinMode(PLAY_STATUS, OUTPUT);
//    digitalWrite(PLAY_STATUS, LOW);
    //delay(3000); // wait for DFPlayer to start

// DEBUG
//    ADCSRA = 0;            // turn off ADC
//    power_all_disable ();  // power off ADC, Timer 0 and 1, serial interface
    Serial.println(F("Setup done"));
}

// the loop function runs over and over again forever
void loop() {
    Serial.println(F("Loop started"));
// DEBUG
//    power_timer0_enable ();  
    delay (1);

    int pir;
    int status;
    pir = digitalRead(PIR_PIN);
    status = digitalRead(STATUS_PIN);

    if ((pir == LOW) && (status == HIGH)){
        Serial.println(F("Power off DFPlayer"));
        digitalWrite(POWER_STATUS, LOW); // switch off DFPlayer if no motion detected and playing has ceased
    }

    // only hit play if PIR detects motion and we're not already playing
    if (pir== HIGH){
        Serial.println(F("Motion detected, powering on DFPlayer"));
        // always switch on DFPlayer (POWER_STATUS) on pir==HIGH (movement detected)
        digitalWrite(POWER_STATUS, HIGH);
        delay(3000); // wait for DFPlayer to initialize
        Serial.println(F("DFPlayer presumably ready"));
        if (status== HIGH) {
            Serial.println(F("DFPlayer needs to start playing!"));
            // if the device is not playing, we need to start playing. Light the PLAY_STATUS LED to indicate we're hitting play
//            digitalWrite(PLAY_STATUS, HIGH);
            pinMode(PLAY_PIN, OUTPUT);
            digitalWrite(PLAY_PIN, LOW); // drive low for short time to start play
            delay(100);
            // write high, then disconnect to avoid continous playing
            digitalWrite(PLAY_PIN, HIGH);
            pinMode(PLAY_PIN, INPUT); //Tristate the pin
//            digitalWrite(PLAY_STATUS, LOW); // turn off the pullup
            delay(100); // give the DFPlayer time to start playing
            Serial.println(F("DFPlayer presumably now playing"));
        }
    }
    Serial.println(F("Would go to sleep now"));
    delay(4000);
// DEBUG
//    power_timer0_disable ();
//    goToSleep ();
}

// DEBUG
/*
void goToSleep (){
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    noInterrupts ();       // timed sequence coming up

    // pat the dog
    wdt_reset();  

    // clear various "reset" flags
    MCUSR = 0;     
    // allow changes, disable reset, clear existing interrupt
    watchdogRegister = bit (WDCE) | bit (WDE) | bit (WDIF);
    // set interrupt mode and an interval (WDE must be changed from 1 to 0 here)
    //watchdogRegister = bit (WDIE) | bit (WDP2) | bit (WDP1) | bit (WDP0);    // set WDIE, and 2 seconds delay
    watchdogRegister = bit (WDIE) | bit (WDP3) ;    // set WDIE, and 4 seconds delay


    sleep_enable ();       // ready to sleep
    interrupts ();         // interrupts are required now
    sleep_cpu ();          // sleep                
    sleep_disable ();      // precaution
}
*/