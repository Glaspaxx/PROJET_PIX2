#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
class Led {
  public:
    int r;
    int g;
    int b;
    Led(int red, int green, int blue) {
      r = red;
      g = green;
      b = blue;
    }
    void InitLed() {
      pinMode(r, OUTPUT);
      pinMode(g, OUTPUT);
      pinMode(b, OUTPUT);
    }
};
void displayColor(byte r, byte g, byte b, Led led) {
  // Assigne l'état des broches
  // Version cathode commune
  analogWrite(led.r, r);
  analogWrite(led.g, g);
  analogWrite(led.b, b);
}
Servo servo_flag;  // create servo object to control a servo
Servo servo_sweetie;
int pos = 0;
const int val_seuil = 11;
int turn = 0;
int buttonstate = 0;
int led_mode_1 = 52; // mode mémoire
int led_mode_2 = 53; // mode rapidité
int mode;
Led leds[5] = {
  Led(23, 22, 24),  // Led 1
  Led(27, 26, 28),  // Led 2
  Led(31, 30, 32),  // Led 3
  Led(35, 34, 36),  // Led 4
  Led(39, 38, 40)  // Led 5
};
int button[] = {A11, A12, A13, A14, A15}; //The four button input pins
int MyScore = 0;
int highscore = 0;

//Mode 1
int randomArray[50]; //long to store up to 50 inputs
int inputArray[50];

// Mode 2
int readingSensor[5];
int targetTimer[] = {0, 0, 0, 0, 0};
int timer = 100;
unsigned long interval = 100; // the time we need to wait
unsigned long previousMillis = 0; // millis() returns an unsigned long.

void setup() {
  servo_flag.attach(2);
  servo_sweetie.attach(3);
  lcd.init();                      // initialize the lcd
  Serial.begin(9600);
  for (int x = 0; x < 5; x++) // Bouton Input
  {
    pinMode(button[x], INPUT);
  }
  // Initialise les broches des leds RGB et des deux leds des modes
  pinMode(led_mode_1, OUTPUT);
  pinMode(led_mode_2, OUTPUT);
  for (int i = 0; i < 5; i++) {
    leds[i].InitLed();
  }
  //ShowModes();
}

void loop() {
  if (mode == 1) {
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("YOUR SCORE: " + (String) (MyScore));
    lcd.setCursor(0, 0);
    lcd.print("HIGH SCORE: " + (String) (highscore));
    mode_1();
  }
  if (mode == 2) {
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);  //(Column,Row)
    lcd.print("SCORE: " + (String) (MyScore));
    lcd.setCursor(0, 1);  //(Column,Row)
    lcd.print("TIME: " + (String) (timer));
    mode_2();
  }
}

void mode_1() {
  for (int y = 0; y <= 2; y++)
  {
    //function for generating the array to be matched by the player
    for (int i = 0; i < 5; i++) {
      displayColor(0, 255, 0, leds[i]);
    }
    delay(1000);
    for (int i = 0; i < 5; i++) {
      displayColor(0, 0, 0, leds[i]);
    }
    delay(1000);

    for (int y = turn; y <= turn; y++) //untuk sekali random
    { //Limited by the turn variable
      Serial.println(""); //Some serial output to follow along
      Serial.print("Turn: ");
      Serial.print(y);
      Serial.println("");
      lcd.clear();
      MyScore = y;
      lcd.setCursor(0, 1);  //(Column,Row)
      lcd.print("YOUR SCORE: " + (String) (MyScore));

      if (MyScore > highscore && MyScore != 0) {
        highscore = y;
        //Servo pour flag
        //Sort le drapeau vert
        servo_flag.write(180);
        delay(3000);
        servo_flag.write(90);
      }
      lcd.setCursor(0, 0);
      lcd.print("HIGH SCORE: " + (String)(highscore));
      Serial.print(y);
      Serial.println("");
      if (MyScore % 5 == 0 && MyScore != 0) {
        //Servo pour bonbon
        //Sort un bonbon si le score est multiple de 5
        servo_sweetie.write(180);
        delay(1000);
        servo_sweetie.write(90);
      }
      delay(1000);
      randomArray[y] = random(1, 6); //Assigning a random number (1-5) to the randomArray[y], y being the turn count
      for (int x = 0; x <= turn; x++)
      {
        Serial.print(randomArray[x]);

        for (int y = 0; y < 5; y++)
        {
          if (randomArray[x] == 1 && leds[y].r == 23)
          { //if statements to display the stored values in the array
            displayColor(0, 0, 255, leds[0]);
            delay(400);
            displayColor(0, 0, 0, leds[0]);
            delay(100);
          }
          if (randomArray[x] == 2 && leds[y].r == 37)
          {
            displayColor(0, 0, 255, leds[1]);
            delay(400);
            displayColor(0, 0, 0, leds[1]);
            delay(100);
          }

          if (randomArray[x] == 3 && leds[y].r == 31)
          {
            displayColor(0, 0, 255, leds[2]);
            delay(400);
            displayColor(0, 0, 0, leds[2]);
            delay(100);
          }

          if (randomArray[x] == 4 && leds[y].r == 35)
          {
            displayColor(0, 0, 255, leds[3]);
            delay(400);
            displayColor(0, 0, 0, leds[3]);
            delay(100);
          }
          if (randomArray[x] == 5 && leds[y].r == 39)
          {
            displayColor(0, 0, 255, leds[4]);
            delay(400);
            displayColor(0, 0, 0, leds[4]);
            delay(100);
          }
        }
      }
    }
    input_mode_1();
  }
}

void input_mode_1() {
  for (int x = 0; x <= turn;) {
    for (int y = 0; y < 5; y++) {
      buttonstate = analogRead(button[y]);
      if (buttonstate < val_seuil && button[y] == A11) {
        displayColor(0, 255, 0, leds[0]);
        delay(200);
        displayColor(0, 0, 0, leds[0]);
        inputArray[x] = 1;
        delay(250);
        if (inputArray[x] != randomArray[x]) {
          fail_mode_1();
        }
        x++;
      }
      if (buttonstate < val_seuil && button[y] == A12) {
        displayColor(0, 255, 0, leds[1]);
        delay(200);
        displayColor(0, 0, 0, leds[1]);
        inputArray[x] = 2;
        delay(250);
        if (inputArray[x] != randomArray[x]) {
          fail_mode_1();
        }
        x++;
      }
      if (buttonstate < val_seuil && button[y] == A13) {
        displayColor(0, 255, 0, leds[2]);
        delay(200);
        displayColor(0, 0, 0, leds[2]);
        inputArray[x] = 3;
        delay(250);
        if (inputArray[x] != randomArray[x]) {
          fail_mode_1();
        }
        x++;
      }
      if (buttonstate < val_seuil && button[y] == A14) {
        displayColor(0, 255, 0, leds[3]);
        delay(200);
        displayColor(0, 0, 0, leds[3]);
        inputArray[x] = 4;
        delay(250);
        if (inputArray[x] != randomArray[x]) {
          fail_mode_1();
        }
        x++;
      }
      if (buttonstate < val_seuil && button[y] == A15) {
        displayColor(0, 255, 0, leds[4]);
        delay(200);
        displayColor(0, 0, 0, leds[4]);
        inputArray[x] = 5;
        delay(250);
        if (inputArray[x] != randomArray[x]) {
          fail_mode_1();
        }
        x++;
      }
    }
  }
  delay(500);
  turn++;
}
void fail_mode_1() {
  // Flashe les leds pour indiquer l'echec
  for (int i = 0 ; i <= 3; i++) {
    for (int i = 0; i < 5; i++) {
      displayColor(255, 0, 0, leds[i]);
    }
    delay(200);
    for (int i = 0; i < 5; i++) {
      displayColor(0, 0, 0, leds[i]);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  GAME OVER !!");
  }
  //Sort le drapeau rouge
  servo_flag.write(0);
  delay(3000);
  servo_flag.write(90);
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HIGH: " + (String) (highscore));
  lcd.setCursor(9, 0);
  lcd.print("YOU: " + (String) (MyScore));
  lcd.setCursor(14, 0);
  lcd.setCursor(0, 1);
  lcd.print(" <PRESS TO PLAY>");
  WaitResponse();
  delay(1000);
  turn = -1;
}
void mode_2() {
  lcd.clear();
  lcd.setCursor(0, 0);  //(Column,Row)
  lcd.print("SCORE: " + (String) (MyScore));
  lcd.setCursor(0, 1);  //(Column,Row)
  lcd.print("TIME: " + (String) (timer));

  unsigned long currentMillis = millis(); // grab current time

  // check if "interval" time has passed (100 milliseconds)
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {

    for (int sensorNumber = 0; sensorNumber < 5; sensorNumber++) {
      readingSensor[sensorNumber] = analogRead(button[sensorNumber]);
    }
    previousMillis = millis();
    Serial.print(readingSensor[1]);
    Serial.println(readingSensor[2]);
  }
  LightLed();
  LighOffLed();
  ifZero();
  Countdowntimer();
}
void Countdowntimer() {
  timer--;
  delay(1000);
}
void LightLed() {
  int target = random(1, 6);
  if (target == 1) {
    targetTimer[0]++;
    displayColor(0, 255, 0, leds[0]);
  } else if (target == 2) {
    targetTimer[1]++;
    displayColor(0, 255, 0, leds[1]);
  } else if (target == 3) {
    targetTimer[2]++;
    displayColor(0, 255, 0, leds[2]);
  } else if (target == 4) {
    targetTimer[3]++;
    displayColor(0, 255, 0, leds[3]);
  } else if (target == 5) {
    targetTimer[4]++;
    displayColor(0, 255, 0, leds[4]);
  }
}
void LighOffLed() {
  if (readingSensor[0] < val_seuil) {
    MyScore++;
    displayColor(0, 0, 0, leds[0]);
  }
  if (readingSensor[1] < val_seuil) {
    MyScore++;
    displayColor(0, 0, 0, leds[1]);
  }
  if (readingSensor[2] < val_seuil) {
    MyScore = MyScore + 5;
    displayColor(0, 0, 0, leds[2]);
  }
  if (readingSensor[3] < val_seuil) {
    MyScore++;
    displayColor(0, 0, 0, leds[3]);
  }
  if (readingSensor[4] < val_seuil) {
    MyScore++;
    displayColor(0, 0, 0, leds[4]);
  }
}
void ifZero() {
  if (timer == 0) {
    for (int i = 0; i < 5; i++) {
      displayColor(0, 0, 0, leds[i]);
    }
    if (MyScore > highscore) {
      highscore = MyScore;
      servo_sweetie.write(180);
      delay(1000);
      servo_sweetie.write(0);
    }
    lcd.clear();
    lcd.print(" YOUR SCORE: " + (String) (MyScore));
    lcd.setCursor(0, 1);
    lcd.print(" HIGH SCORE: " + (String) (highscore));
    WaitResponse();
    delay(450);
    MyScore = 0;
    timer = 100;
  }
}
void WaitResponse() {
  while (analogRead(button[0]) > val_seuil && analogRead(button[1]) > val_seuil && analogRead(button[3]) > val_seuil && analogRead(button[4]) > val_seuil) {
    displayColor(255, 0, 0, leds[0]);
    delay(250);
    displayColor(0, 0, 0, leds[0]);
    displayColor(0, 255, 0 , leds[1]);
    delay(250);
    displayColor(0, 0, 0, leds[1]);
    displayColor(0, 0, 255, leds[2]);
    delay(250);
    displayColor(0, 0, 0, leds[2]);
    displayColor(0, 255, 0, leds[3]);
    delay(250);
    displayColor(0, 0, 0, leds[3]);
    displayColor(255, 0, 0, leds[4]);
    delay(250);
    displayColor(0, 0, 0, leds[4]);
    delay(100);
    if (analogRead(button[2]) < val_seuil) {
      ShowModes();
      break;
    }
  }
}
void ShowModes() {
  digitalWrite(led_mode_1, LOW);
  digitalWrite(led_mode_2, LOW);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("< Memoire");
  lcd.setCursor(0, 1);
  lcd.print("       Reflexe >");
  bool valider = true;
  while (valider) {
    int lecture_1 = analogRead(button[0]);
    Serial.println(lecture_1);
    displayColor(0, 0, 255, leds[0]);
    int lecture_5 = analogRead(button[4]);
    Serial.println(lecture_5);
    displayColor(0, 0, 255, leds[4]);
    mode = 0;
    if (lecture_1 < val_seuil) {
      valider = false;
      digitalWrite(led_mode_1, HIGH);
      digitalWrite(led_mode_2, LOW);
      displayColor(0, 0, 0, leds[0]);
      displayColor(0, 0, 0, leds[4]);
      MyScore = 0;
      highscore = 0;
      mode = 1;
    }
    if (lecture_5 < val_seuil) {
      valider = false;
      digitalWrite(led_mode_2, HIGH);
      digitalWrite(led_mode_1, LOW);
      displayColor(0, 0, 0, leds[0]);
      displayColor(0, 0, 0, leds[4]);
      MyScore = 0;
      highscore = 0;
      mode = 2;
    }
    delay(25);
  }
  delay(1000);
}
