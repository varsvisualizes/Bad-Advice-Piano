/*
 * E9 - Bad Advice Piano
 * 
 * This project was inpsired by the magic 8 ball, but with a slighly more
 * malicious intent. Press a key on the aluminum foil keys of the piano,
 * and you'll get bad advice spat on the LCD. Take it all with a grain of
 * salt.
 * 
 * Every time you re-upload, the chances of getting each piece of advice 
 * changes.
 * 
 * The circuit:
 * 
 * On Breadboard 1 - 
 * 
 * LCD pin 1 : ground
 * LCD pin 2 : positive voltage
 * LCD pin 3 : middle pin of 10K potentiometer
 * LCD pin 4 : Shift Register pin 1
 * LCD pin 5 : Ground
 * LCD pin 6 : Shift Register pin 3
 * LCD pin 7 - 10 : no connection
 * LCD pin 11 : Shift Register pin 4
 * LCD pin 12 : Shift Register pin 5
 * LCD pin 13 : Shift Register pin 6
 * LCD pin 14 : Shift Register pin 7
 * LCD pin 15 : positive voltage
 * LCD pin 16 : ground

 * Shift Register pin 2 : No connection
 * Shift Register pin 9 : No connection
 * Shift Register pin 10 : positive voltage
 * Shift Register pin 11 : Arduino pin 13   
 * Shift Register pin 12 : Arduino pin 9    
 * Shift Register pin 13 : ground
 * Shift Register pin 14 : Arduino pin 11   
 * Shift Register pin 15 : no connection
 * Shift Register pin 16 : positive voltage
 * 
 * On Breadboard 2 - 
 * 
 * Send pin wire connected to pin 4
 * Receiver pin 1 wire connected to MCU pin 2
 * Receiver pin 2 wire connected to MCU pin 3
 * Receiver pin 3 wire connected to MCU pin 5
 * 
 * All receive pins connected to send pin with 100k Ohm resistors
 * 
 * Piezo buzzer connected to MCU pin 6 through 5V
 * 
 * 
 * Adapted from code written by David A. Mellis, Limor Fried, Tom Igoe,
 * Juan Hernandez, LadyK, and Paul Badger. 
 * 
 * 21 November 2021
 * Carmen Vargas
 * 
 */

// initialize the libraries
#include <SPI.h>
#include <LiquidCrystal.h>
#include <CapacitiveSensor.h>

// initialize speaker
int speakerPin = 6;

// initialize the LCD through the shift register latch pin
LiquidCrystal lcd(9);

// initalize the capacitive sensors
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        
CapacitiveSensor   cs_4_3 = CapacitiveSensor(4,3);
CapacitiveSensor   cs_4_5 = CapacitiveSensor(4,5); 

// values to calibrate the touch sensor
int threshold1 = 100;
int threshold2 = 50;

// array of possible fortunes
char *fortunes[] = {"Slap them", "Be petty", "Key that car",
                     "Quit your job", "Block their number",
                     "Escape the county", "Change your name",
                     "Try the shrimp", "Don't ask again",
                     "Repress emotions", "Just scream",
                     "Chop your hair"};
                     
// 12 items are in the fortunes array, so make that a constant variable
const int fortunesTotal = 12;

// create an empty array to hold random numbers later
int fateNumbers[fortunesTotal];
  
void setup() {

  // more touch sensor calibration
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF); 
  Serial.begin(9600);

  // set up the LCD
  lcd.begin(16, 2);

  // make sure the values that we generate are random
  randomSeed(analogRead(0));

  // generate random numbers to fill the fateNumbers array
  for(int i = 0; i < (fortunesTotal - 1); i++) {
    fateNumbers[i] = random(fortunesTotal - 1);
    Serial.println(fateNumbers[i]);
  }
}

void loop() {

  // assign the capacitive sensors to the aluminum foil keys
  long pin1 =  cs_4_2.capacitiveSensor(30);
  long pin2 =  cs_4_3.capacitiveSensor(30);
  long pin3 =  cs_4_5.capacitiveSensor(30);

  // continually test the sensor values to calibrate appropriately 
  Serial.print(pin1);                  
  Serial.print("\t");

  Serial.print(pin2);
  Serial.print("\t");

  Serial.print(pin3);
  Serial.println("\t");

  if(pin1 > threshold1 && pin2 < threshold1 && pin3 < threshold1) { 
    // if only pin1 is pressed
    lcd.clear();
    tellFortune(1);
    tone(speakerPin, 1915);
  }
  else if(pin1 < threshold1 && pin2 > threshold1 && pin3 < threshold1) {
    // if only pin2 is pressed
    lcd.clear();
    tellFortune(2);
    tone(speakerPin, 1700);
  }
  else if(pin1 < threshold1 && pin2 < threshold1 && pin3 > threshold1) {
    // if only pin3 is pressed
    lcd.clear();
    tellFortune(3);
    tone(speakerPin, 1519);
  }
  else if(pin1 > threshold2 && pin2 > threshold2 && pin3 < threshold2) {
    // if pins 1 and 2 pressed
    lcd.clear();
    tellFortune(4);
    tone(speakerPin, 1432);
  }
  else if(pin1 < threshold2 && pin2 > threshold2 && pin3 > threshold2) {
    // if pins 2 and 3 pressed
    lcd.clear();
    tellFortune(5);
    tone(speakerPin, 1275);
  }
  else if(pin1 > threshold2 && pin2 < threshold2 && pin3 > threshold2) {
    // if pins 1 and 3 pressed
    lcd.clear();
    tellFortune(6);
    tone(speakerPin, 1136);
  }
  else {
    noTone(speakerPin);
    startPage();
  }

  // set the LCD cursor to column 0, line 1
  lcd.setCursor(0, 1);
}

void tellFortune(int pinCase) {

  // depending on the key(s) pressed, display one of
  // the messages from the fortunes array based on
  // the random number generated in the fatedNumbers
  // array
  switch (pinCase) {
    case 1:
      lcd.print(fortunes[fateNumbers[0]]);
      break;
    case 2:
      lcd.print(fortunes[fateNumbers[1]]);
      break;
    case 3:
      lcd.print(fortunes[fateNumbers[2]]);
      break;
    case 4:
      lcd.print(fortunes[fateNumbers[3]]);
      break;
    case 5:
      lcd.print(fortunes[fateNumbers[4]]);
      break;
    case 6:
      lcd.print(fortunes[fateNumbers[5]]);
      break;
  }

}

void startPage() {
  // our generic starting message on the MCU
  lcd.clear();
  lcd.print("Press a pin!");
}
