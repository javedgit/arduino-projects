#include <IRremote.h>
IRsend irsend;

/* 
 * Arduino DIY project to play with IR receiver / sender , LED and push button on the Arduino Uno R3. The program is 
 * expected to execute the following operation :
 * 1. Detect that an IR signal has been sent , this will be printed to Serial Console
 *    , STATUS_LED will blink 2 times on receiving IR signal
 * 2. Send Sony (IR) power code to the IR emitter when button is pressed
 *    , STATUS_LED will blink 10 times on sending IR signal
 * 
 * Prequisites for this project:
 * 1. IR emittor
 * 2. IR receiver
 * 3. Push Button
 * 4. 3X 10K OHM resistors
 * 5. Arduino Uno R3
 * 6. BreadBoard
 *
 * IR library from https://github.com/shirriff/Arduino-IRremote needs to be downloaded and installed
 * 
 */


//Receiver PIN 
const int IR_RECV_PIN = 11;
IRrecv irrecv(IR_RECV_PIN);

//Decoder Results object
decode_results results;

//PIN where LED indicates IR signal received
const int LED_STATUS_PIN = 9;

//PIN which controls whether IR signal needs to be sent
const int BUTTON_PIN = 2;

void setup()
{
  Serial.begin(9600);
  
  // Start the receiver
  irrecv.enableIRIn(); 
}


/**
 * Method listens IR signals and emmits the Sony power code 
 * when buttons is pressed
 *
 */
void loop() {
  
  sendIRCodeOnButtonPress();
  
  readIRSignal();

}


/**
 * 
 *
 */
void readIRSignal(){
   //Will be triggered where there is a IR signal detected
  if (irrecv.decode(&results)) {
    Serial.println("Captured remote codes: [");
    Serial.println(results.value, HEX);
    Serial.println("]");
    
    irrecv.resume(); // Receive the next value
      blinkStatusLED(1000,500,2);
  } 
}


/**
 * When button attached to BUTTON_PIN is pressed then sony power codes 
 * are sent via IR emitter. This also turns on LED on LED_STATUS_PIN
 *  http://arduino.cc/it/Tutorial/Button
 *
 */
void sendIRCodeOnButtonPress(){
   //Will send sony code when 
   
   if (digitalRead(BUTTON_PIN) == HIGH) { 
  
      sendSonyPowerCode();
      
      blinkStatusLED(1000,500,10);
      
   }
   
}

/**
 * Sends out Sony power codes using IRremote library
 *
 */
void sendSonyPowerCode() {

    Serial.println("\n Sending Sony power code now...");
    
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xa90, 12); // Sony TV power code
      delay(100);
    }
}

/**
 * Causes LED Status to blink 5 times rapidly
 *
 */
void blinkStatusLED(int lightUp , int lightDown , int numberOfBlinks){

  for(int i=0; i < numberOfBlinks ; i++){ 
    digitalWrite(LED_STATUS_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(lightUp);               // wait for a second
    digitalWrite(LED_STATUS_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(lightDown);               // wait for a second
  }
  
  digitalWrite(LED_STATUS_PIN,LOW);
}
