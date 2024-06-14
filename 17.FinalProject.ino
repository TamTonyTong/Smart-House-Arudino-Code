#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//Set the communication address of I2C to 0x27, display 16 characters every line, two lines in total
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo servo_9;
Servo servo_10;
int val_light_sensor;  //Read the voltage value of the photodiode
int val_steam_sensor;  //Read the vapor sensor of the steam sensor
int val_gas_sensor;    //Read the gas sensor value
int val_soil_sensor;   //Read the soil sensor value
int buzzer = 3;
int GasS = A0;
int SoilS = A2;
int PIRS = 2;
int val_button;
int val_bluetooth;
String servo9;//string type variable servo1
volatile int servo9_angle;//set variable light
String servo10;//string type variable servo2
volatile int servo10_angle;//set variable servo2_angle
String led2;//string type variable led2
volatile int value_led2;//set variable value_led2
String fans_char;           //string type variable fans_char
volatile int fans_val;      //set variable fans_char

bool val_gas_sensor_bool;
bool val_light_sensor_bool;
bool val_soil_sensor_bool;
bool val_steam_sensor_bool = false;

//define name of every sound frequency
#define D0 -1
#define D1 262
#define D2 293
#define D3 329
#define D4 349
#define D5 392
#define D6 440
#define D7 494
#define M1 523
#define M2 586
#define M3 658
#define M4 697
#define M5 783
#define M6 879
#define M7 987
#define H1 1045
#define H2 1171
#define H3 1316
#define H4 1393
#define H5 1563
#define H6 1755
#define H7 1971

#define WHOLE 1
#define HALF 0.5
#define QUARTER 0.25
#define EIGHTH 0.25
#define SIXTEENTH 0.625

const int birthdayNoteDurations[] = {250, 250, 250, 250, 500, 250, 250, 250, 250, 500, 250, 250, 250, 250, 250, 250, 375, 250, 250, 250, 500};
const int birthdayNotes[] = {294, 440, 392, 532, 494, 392, 440, 392, 587, 532, 392, 784, 659, 532, 494, 440, 698, 659, 532, 587, 532};
const int numBirthdayNotes = sizeof(birthdayNoteDurations) / sizeof(birthdayNoteDurations[0]);

const int odeToJoyTune[] = {D4, D4, D5, D6, D6, D5, D4, D3, D2, D2, D3, D4, D4, D3, D3, D4, D4, D5, D6, D6, D5, D4, D3, D2, D2, D3, D4, D3, D2, D2};
const float odeToJoyDurations[] = {QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER};
const int odeToJoyLength = sizeof(odeToJoyTune) / sizeof(odeToJoyTune[0]);

bool playing = false;
bool isBirthdaySong = false;  // Flag to determine which song to play
bool isOdeToJoy = false;      // Flag to determine if "Ode to Joy" is playing
unsigned long startMillis;
int currentNote = 0;

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);       // Open serial port, set baud rate at 9600bps
  
  pinMode(3, OUTPUT);       // Buzzer
  pinMode(A0, INPUT);       // Gas sensor
  pinMode(A2, INPUT);       // Soil sensor
  pinMode(2, INPUT);        // PIR sensor
  pinMode(4, INPUT);        // Button 1
  pinMode(5, INPUT);        // LED control (Yellow)
  pinMode(6, OUTPUT);       // Fan control
  pinMode(7, OUTPUT);       // Fan control
  pinMode(9, OUTPUT);       // Button 2
  pinMode(12, OUTPUT);      // Relay control
  pinMode(13, OUTPUT);      // Another LED control

  servo_9.attach(9);        // Door servo
  servo_9.write(0);         // Default position
  servo_10.attach(10);      // Window servo
  servo_10.write(0);        // Default position

  
}

void loop()
{
  if (Serial.available() > 0) //serial reads the characters
    {
      val_bluetooth = Serial.read();//set val to character read by serial
      Serial.println(val_bluetooth);//output val character in new lines
      pwm_control();
    
      switch (val_bluetooth) {

      case 'a'://if val is character 'a'，program will circulate
        digitalWrite(13, HIGH); //set digital 13 to high level，LED lights up
        break;//exit loop

      case 'b':
        digitalWrite(13, LOW);
        break;//exit loop

      case 'c'://if val is character 'c'，program will circulate
        digitalWrite(12, HIGH); //set digital 12 to high level，NO of relay is connected to COM
        break;//exit loop

      case 'd'://if val is character 'd'，program will circulate
        digitalWrite(12, LOW); //set digital 12 to low level，NO of relay is disconnected to COM
        break;//exit loop

      case 'e'://if val is character 'e'，program will circulate
        playing = true;
        isBirthdaySong = true;
        isOdeToJoy = false;
        currentNote = 0;
        startMillis = millis(); // Reset the timer
        break; // Exit loop

      case 'f':
        playing = true;
        isBirthdaySong = false;
        isOdeToJoy = true;
        currentNote = 0;
        startMillis = millis(); // Reset the timer
        break; // Exit loop

      case 'g'://if val is character 'g'，program will circulate
        playing = false;
        noTone(3);
        break;//exit loop

      case 'h'://if val is character 'h'，program will circulate
        val_light_sensor_bool = true;//output the value of variable light in new lines
        delay(100);
        break;//exit loop
        
      case 'i'://if val is character 'i'，program will circulate
        val_gas_sensor_bool = true;//output the value of variable gas in new lines
        delay(100);
        break;//exit loop

      case 'j'://if val is character 'j'，program will circulate
        val_soil_sensor_bool = true;//output the value of variable soil in new lines
        delay(100);
        break;//exit loop
      
      case 'k':
        val_steam_sensor_bool = true;//output the value of variable soil in new lines
        delay(100);
        break;//exit loop

      case 'l'://if val is character 'l'，program will circulate
        servo_9.write(180);//set servo connected to digital 9 to 180°
        delay(500);
        break;//exit loop

      case 'm'://if val is character 'm'，program will circulate
        servo_9.write(0);;//set servo connected to digital 9 to 0°
        delay(500);
        break;//exit loop

      case 'n'://if val is character 'n'，program will circulate
        servo_10.write(180);//set servo connected to digital 10 to 180°
        delay(500);
        break;//exit loop

      case 'o'://if val is character 'o'，program will circulate
        servo_10.write(0);//set servo connected to digital 10 to 0°
        delay(500);
        break;//exit loop

      case 'p':
        digitalWrite(5, HIGH);
        break;
      
      case 'q':
        digitalWrite(5, LOW);
        break;//exit loop
      
      case 'r'://if val is character 'r'，program will circulate
        digitalWrite(7, HIGH);
        digitalWrite(6, LOW); //fan rotates clockwise at the fastest speed
        break;//exit loop
      
      case 's'://if val is character 's'，program will circulate
        digitalWrite(7, LOW);
        digitalWrite(6, LOW); //fan stops rotating
        break;//exit loop
      
      case 'S':
        val_soil_sensor_bool = false;
        val_gas_sensor_bool = false;
        val_light_sensor_bool = false;
        val_steam_sensor_bool = false;
        break;//exit loop
      }
    }
  //Sensors
  //Light Sensors
  //bool val_light_sensor = true;
  if (val_light_sensor_bool == true)
  {
  val_light_sensor = analogRead(A1);
  lcd.setCursor(11, 11);
  lcd.print("Light");
  delay(1000);
  if (val_light_sensor < 100)  //If light souce is low, led will be on and otherwise. or the pirmotion is activated
  {
    digitalWrite(13, HIGH);
    digitalWrite(5, HIGH);
  } else {
    digitalWrite(13, LOW);
    digitalWrite(5, LOW);
  }
  delay(10);
  }
  else
  {
    val_light_sensor = 0;
  }

  //Steam Sensors
  if (val_steam_sensor_bool)
  {
  int val_steam_sensor = analogRead(A3);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Moisture: ");
    lcd.setCursor(9, 0);
    lcd.print(val_steam_sensor);
    delay(1000); // Consideration because display is flashy
  }
  else
  {
    val_steam_sensor = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
  }

  //Gas Sensor
  if (val_gas_sensor_bool) {
  val_gas_sensor = analogRead(GasS);
  lcd.setCursor(12, 0);
  lcd.print("Gas");
  delay(500);
  if (val_gas_sensor > 450) {
    // LCD shows "Danger" at the first row and column
    lcd.setCursor(0, 1);
    lcd.print("Danger");

    tone(3, 440);
    delay(125);
    noTone(3);
    delay(100);

    tone(3, 440);
    delay(125);
    noTone(3);
    delay(100);

    tone(3, 440);
    delay(125);
    noTone(3);
    delay(100);
  } else {
    noTone(3);
  }
} else {
  val_gas_sensor = 0;
}


  //Soil Sensor
  //bool val_soil_sensor = true;
  if (val_soil_sensor_bool == true)
  {
  val_soil_sensor = analogRead(SoilS);
  if (val_soil_sensor < 300)
  {
    lcd.clear();
    lcd.setCursor (0,0);
    lcd.print("Humidity:");
    lcd.setCursor(9,0);
    lcd.print(val_soil_sensor);
    lcd.setCursor(0,1);
    lcd.print("Dry soil");
    delay(1000);
  }
  else if ((val_soil_sensor >= 300) && (val_soil_sensor <= 700))
  {
    lcd.clear();
    lcd.setCursor (0,0);
    lcd.print("Humidity:");
    lcd.setCursor(9,0);
    lcd.print(val_soil_sensor);
    lcd.setCursor(0,1);
    lcd.print("Humid soil");
    delay(1000);
  }
  else
  {
    lcd.clear();
    lcd.setCursor (0,0);
    lcd.print("Humidity:");
    lcd.setCursor(9,0);
    lcd.print(val_soil_sensor);
    lcd.setCursor(0,1);
    lcd.print("In Water");
    delay(1000);
  }
  }
  else
  {
    val_soil_sensor = 0;
  }
if (playing) {
    unsigned long currentMillis = millis();

    if (isBirthdaySong) {
      if (currentMillis - startMillis >= birthdayNoteDurations[currentNote]) {
        // Move to the next note
        currentNote++;
        if (currentNote < numBirthdayNotes) {
          tone(3, birthdayNotes[currentNote]);
          startMillis = currentMillis; // Reset the timer for the next note
        } else {
          playing = false; // Stop playing after the last note
          noTone(3);
        }
      }
    } else if (isOdeToJoy) {
      if (currentMillis - startMillis >= 300 * odeToJoyDurations[currentNote]) {
        // Move to the next note
        currentNote++;
        if (currentNote < odeToJoyLength) {
          tone(3, odeToJoyTune[currentNote]);
          startMillis = currentMillis; // Reset the timer for the next note
        } else {
          playing = false; // Stop playing after the last note
          noTone(3);
        }
      }
    }
  }
}


void pwm_control() {
  switch (val_bluetooth) {
    case 't': // if val is 't', program will circulate
      servo9 = Serial.readStringUntil('#');
      servo9_angle = servo9.toInt();
      servo_9.write(servo9_angle); // set the angle of servo connected to digital 9 to servo10_angle
      delay(300);
      break;

    case 'u': // if val is 'u', program will circulate
      servo10 = Serial.readStringUntil('#');
      servo10_angle = servo10.toInt();
      servo_10.write(servo10_angle); // set the angle of servo connected to digital 10 to servo8_angle
      delay(300);
      break;

    case 'v': // if val is 'v', program will circulate
      led2 = Serial.readStringUntil('#');
      value_led2 = String(led2).toInt();
      analogWrite(5, value_led2); // set PWM value of digital 9 to value_led2
      break;

    case 'w': // if val is 'w', program will circulate
      fans_char = Serial.readStringUntil('#');
      fans_val = fans_char.toInt();
      digitalWrite(7, LOW);
      analogWrite(6, fans_val); // set PWM value of digital 6 to fans_val
      break;
  }
}