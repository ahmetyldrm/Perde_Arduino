
#include <StepMotor.h>
#include <SoftwareSerial.h>

StepMotor stepMotor = StepMotor(9, 10, 11, 12);
SoftwareSerial btSerial = SoftwareSerial(3, 2); // RX, TX

// Constants
const char END_CHAR = '#';
const char DIR_CW_CHAR  = '+';
const char DIR_CCW_CHAR = '-';
const char SET_DELAY_CHAR = '*';

// Step motor variables
bool stepRunning = false;
int pos = 0;
int maxPos = 0;
int delayValueX10 = 15; // delay is between 1.0 and 5.0 ms.
                        // delay = delayValueX10 / 10.0 = 2.0 ms

// Modes (if motor is in returning phase or setting delay)
bool returning = false;
bool setting_delay = false;
bool goto_startpos = false;

// Incoming chars stored here.
char* tempString = "";
bool tempDir;

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);
  stepMotor.set_delay(delayValueX10 / 10.0);  // 1.5 ms
}

void loop() {
  if (btSerial.available()) {
    char data = btSerial.read();
    Serial.print("data = ");
    Serial.print(data);
    Serial.print("\n");

    if (data == DIR_CCW_CHAR){
      if (stepMotor.get_direction() != false && returning == false) {
        if (stepRunning == true){
          goto_startpos = true;
          tempDir = false;
          stepMotor.set_direction( ! stepMotor.get_direction());
          returning = true;
        }
        else{
          stepMotor.set_direction(false);
        }        
      }      
    }
    else if (data == DIR_CW_CHAR) {
      if (stepMotor.get_direction() != true && returning == false) {
        if (stepRunning == true){
          goto_startpos = true;
          tempDir = true;
          stepMotor.set_direction( ! stepMotor.get_direction());
          returning = true;
        }
        else{
          stepMotor.set_direction(true);
        }        
      }      
    }
    else if (data == SET_DELAY_CHAR){
      setting_delay = true;
    }
    else if (data == END_CHAR) {
      Serial.print("tempString = ");
      Serial.print(tempString);
      Serial.print("\n");
      if (strlen(tempString) > 1 && isNumeric(tempString)){
        if (setting_delay == true){
          delayValueX10 = atoi(tempString);
          if (delayValueX10 >= 10 && delayValueX10 <= 50){
            stepMotor.set_delay(delayValueX10 / 10.0);
          }          
          setting_delay = false;
        }
        else {
          maxPos = atoi(tempString);
          stepRunning = true; 
        }            
      }
      tempString = "";
    }
    else if (isdigit(data)){
      tempString = append(tempString, data);
    }
  }
  if (stepRunning == true) {
    if (pos == maxPos && ! returning) {
      stepMotor.set_direction( ! stepMotor.get_direction());
      returning = true;  
    }
    else {       
      stepMotor.one_step_wo_flush();
          
      if (returning){
        if (pos == 0){
          returning = false;
          if (goto_startpos == true) {
            stepMotor.set_direction(tempDir);
            goto_startpos = false;
          }
          else {
            stepRunning = false;          
            stepMotor.flush();
          }          
        }
        else
          pos--;        
      }
      else
        pos++;
    }
  }
}
