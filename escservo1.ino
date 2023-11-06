#include <Servo.h>

Servo ESCA;
Servo ESCB;
Servo ESCC;
Servo ESCD;

int speedA = 0;
int speedB = 0;
int speedC = 0;
int speedD = 0;

int led = 11;
bool on = false;

int inputSpeed[4] ;
String escName = "";
bool escRead = true;

String inputString = "34;45;67;54"; // Updated input string      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
String inputSpeeds[4]; // Assuming you want to split into 4 elements

long timer = 0;
long timeout = 500;

void setup() {
  Serial.begin(57600); // opens serial port, sets data rate to 9600 bps
  inputString.reserve(500);
  Serial.println(("started"));
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  ESCA.attach(5, 1000, 2000);
  ESCB.attach(6, 1000, 2000);
  ESCC.attach(9, 1000, 2000);
  ESCD.attach(10, 1000, 2000);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    // Clear the string:
    stringComplete = false;

    inputString.trim();

    // Split the inputString into inputSpeeds using the semicolon as a delimiter
    int semicolonIndex = inputString.indexOf(';');
    if (semicolonIndex != -1) {
      inputSpeeds[0] = inputString.substring(0, semicolonIndex);
      inputString = inputString.substring(semicolonIndex + 1);
      inputSpeed[0] = inputSpeeds[0].toInt();
      semicolonIndex = inputString.indexOf(';');
      if (semicolonIndex != -1) {
        inputSpeeds[1] = inputString.substring(0, semicolonIndex);
        inputString = inputString.substring(semicolonIndex + 1);
        inputSpeed[1] = inputSpeeds[1].toInt();
        semicolonIndex = inputString.indexOf(';');
        if (semicolonIndex != -1) {
          inputSpeeds[2] = inputString.substring(0, semicolonIndex);
         
          inputSpeed[2] = inputSpeeds[2].toInt();

          inputSpeeds[3] = inputString.substring(semicolonIndex + 1);

          inputSpeed[3] = inputSpeeds[3].toInt();

        }
      }
    }


    //escName = inputString.substring(0, 4);
    if (inputSpeed[0] > 0 || inputSpeed[1] > 0 || inputSpeed[2] > 0 || inputSpeed[3] > 0) {
      escRead = true;
      Serial.println("done");
      if (on) {
        digitalWrite(led, HIGH);
        on = false;
      } else {
        digitalWrite(led, LOW);
        on = true;
      }
    }

    if (escRead) {
      Serial.print("Speed A :");
      Serial.println(inputSpeed[0]);
      Serial.print("Speed B :");
      Serial.println(inputSpeed[1]);
      Serial.print("Speed C :");
      Serial.println(inputSpeed[2]);
      Serial.print("Speed D :");
      Serial.println(inputSpeed[3]);

      setESCSpeed("ESCA", inputSpeed[0]);
      setESCSpeed("ESCB", inputSpeed[1]);
      setESCSpeed("ESCC", inputSpeed[2]);
      setESCSpeed("ESCD", inputSpeed[3]);

      escName = "";
      escRead = false;
    }
    inputString = "";
    timer = millis();
  }

  if (millis() > timer + timeout) {

  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void setESCSpeed(String esc, int fanSpeed) {
  if (esc.equals("ESCA")) {
    speedA = fanSpeed;
    int potValue = map(fanSpeed, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
    ESCA.write(potValue);
  }
  if (esc.equals("ESCB")) {
    speedB = fanSpeed;
    int potValue = map(fanSpeed, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
    ESCB.write(potValue);
  }
  if (esc.equals("ESCC")) {
    speedC = fanSpeed;
    int potValue = map(fanSpeed, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
    ESCC.write(potValue);
  }
  if (esc.equals("ESCD")) {
    speedD = fanSpeed;
    int potValue = map(fanSpeed, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
    ESCD.write(potValue);
  }
}
