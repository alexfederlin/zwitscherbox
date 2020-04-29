// connected to pin 12 (ADKey1) of DFPLayer (pulling it LOW starts playing)
#define PLAY_PIN 0
// connected to PIR (HIGH = Movement, LOW = No Movement)
#define PIR_PIN 1
// connected to pin 16 (BUSY) of DFPlayer (LOW = playing, HIGH = Idle)
#define STATUS_PIN 2

#define PIR_STATUS 3
#define PLAY_STATUS 4

void setup() {
    pinMode(PLAY_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);
    pinMode(STATUS_PIN, INPUT);
    pinMode(PIR_STATUS, OUTPUT);
    pinMode(PLAY_STATUS, OUTPUT);
    delay(1000);
}

// the loop function runs over and over again forever
void loop() {
    int pir;
    int status;
    pir = digitalRead(PIR_PIN);
    status = digitalRead(STATUS_PIN);
    // only hit play if PIR detects motion and we're not already playing
    if (pir== HIGH){
        // always light the PIR_STATUS LED on pir==HIGH (movement detected)
        digitalWrite(PIR_STATUS, HIGH);
        if (status== HIGH) {
            // if the device is not playing, we need to start playing. Light the PLAY_STATUS LED to indicate we're hitting play
            digitalWrite(PLAY_STATUS, HIGH);
            pinMode(PLAY_PIN, OUTPUT);
            digitalWrite(PLAY_PIN, LOW);
            delay(10);
            // write high, then disconnect to avoid continous playing
            digitalWrite(PLAY_PIN, HIGH);
            pinMode(PLAY_PIN, INPUT);
            digitalWrite(PLAY_STATUS, LOW);
        }
    }

    delay(10);                       // wait for a second
    digitalWrite(PIR_STATUS, LOW);

}