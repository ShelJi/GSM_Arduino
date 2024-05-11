#include <SoftwareSerial.h>                         //Create software serial object to communicate with SIM800L
SoftwareSerial gsm(11, 10);                         //SIM800L Tx & Rx is connected to Arduino #8 & #9

#define light 3                                     //declare pin for light
#define fan 4                                       //declare pin for fan

void setup() { 
  Serial.begin(9600);                               //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  gsm.begin(9600);                                  //Begin serial communication with Arduino and SIM800L

  Serial.println("Initializing....");
  initModule("AT", "OK", 1000);                     //Once the handshake test is successful, it will back to OK

  gsm.println("AT+CMGF=1\r");                       // Set SMS text mode
  delay(2000);
  gsm.println("AT+CMGL=\"REC UNREAD\"\r");          // read messages
  delay(2000);
}

void loop(){
  read();
}

void read(){
  if (gsm.available() ) {                           // check for messages
    String msg=gsm.readString();                    // store readed message in msg variable
    Serial.print(msg);                              // print message in serial monitor

    if (msg.indexOf("LIGHT ON") != -1) {            // checks command
      digitalWrite(light,HIGH);                     // turn lights on
      gsm.println("AT+CMGS=\"+919488231905\"\r");   // Receiver’s Mobile Number
      delay(1000);
      gsm.println("LIGHT TURNED ON");               // send message as light turned on
      delay(100);
      gsm.println((char)26);
    }
    else if (msg.indexOf("LIGHT OFF") != -1) {
      digitalWrite(light,LOW);
      gsm.println("AT+CMGS=\"+919488231905\"\r"); 
      delay(1000);
      gsm.println("LIGHT TURNED OFF");
      delay(100);
      gsm.println((char)26);
    }
    else if (msg.indexOf("FAN ON") != -1) {
      digitalWrite(fan,HIGH);
      gsm.println("AT+CMGS=\"+919488231905\"\r"); 
      delay(1000);
      gsm.println("FAN TURNED ON");
      delay(100);
      gsm.println((char)26);
    }
    else if (msg.indexOf("FAN OFF") != -1) {
      digitalWrite(fan,LOW);
      gsm.println("AT+CMGS=\"+919488231905\"\r"); 
      delay(1000);
      gsm.println("FAN TURNED OFF");
      delay(100);
      gsm.println((char)26);
    }
  }
}

void initModule(String cmd, char *res, int t) {     // checks for communication between arduino and gsm module
  while (1) {
    Serial.println(cmd);
    gsm.println(cmd);
    delay(100);
    while (gsm.available() > 0) {
      if (gsm.find(res)) {
        Serial.println(res);
        delay(t);
        return;
      } else {
        Serial.println("Error");
      }
    }
    delay(t);
  }
}


