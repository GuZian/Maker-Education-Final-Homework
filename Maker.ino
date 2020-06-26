/*

            This program is only used for the final homework of Maker Education.
                           本程序仅供创客教育课程期末作业使用


                                    by Zian Gu
                                    06/22/2020
  Notes: statusLight shows the work status of relay.It'll turn red if there's no people in the room
         activationLight shows the acvation of the system
         bufferDuraion means the duration that the room has no people or heat source


*/
#define photoResistance 0
#define statusLight 2      //Red
#define relay 5
#define infrared 7
#define buzzer 8
#define activationLight 10 //Green
#define switch 12         //visual switch

#define bufferDuraion 10000

enum light {dark, bright};
int flag = 0;
int flagActivate = 0;
unsigned long changeTime;

void activate();//activate the system
int envBrightness();//return 0 if dark, return 1 if bright

void setup()
{
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps

  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(statusLight, OUTPUT);
  pinMode(activationLight, OUTPUT);
  pinMode(switch, INPUT);
  pinMode(infrared, INPUT);

  digitalWrite(relay, HIGH);
  digitalWrite(statusLight, LOW);
  digitalWrite(activationLight, LOW);

}
void loop()
{
  int statusSwitch = digitalRead(switch);
  Serial.print("statusSwitch：");
  Serial.println(statusSwitch);
  delay(100);
  if (envBrightness() == bright && statusSwitch == LOW || envBrightness() == dark && statusSwitch == LOW)
  {
    digitalWrite(relay, HIGH);
    digitalWrite(statusLight, LOW);
    digitalWrite(activationLight, LOW);
  }
  else if (envBrightness() == bright && statusSwitch == HIGH)
  {
    delay(1000);
    activate();
    digitalWrite(activationLight, HIGH);
  }
  else
  {
    digitalWrite(statusLight, LOW);
    digitalWrite(activationLight, LOW);
    Serial.println("The light is broken!");
    tone(buzzer,1000);
    delay(1000);
    noTone(buzzer);
    delay(2000);
  }
}

//activate the system
void activate()
{
  int val_infrared = digitalRead(infrared);
  Serial.print("val_infrared:");
  Serial.println(val_infrared);
  delay(100);

  if ((val_infrared == LOW) && ((millis() - changeTime) >= bufferDuraion) && (!flagActivate))
  {

    tone(buzzer, 2000);
    delay(1000);
    noTone(buzzer);
    digitalWrite(relay, LOW);
    digitalWrite(statusLight, HIGH);
    //    changeTime = millis();
    flagActivate = 1;
  }
  else if (val_infrared == HIGH)
  {
    digitalWrite(relay, HIGH);
    digitalWrite(statusLight, LOW);
    flagActivate = 0;
    changeTime = millis();
    delay(1000);
  }
  flag = 1;
}

//return 0 if dark, return 1 if bright
int envBrightness()
{
  int status;
  int val_photoResistance = analogRead(photoResistance);
  if (val_photoResistance > 1000)
  {
    status = dark;
    return status;
  }
  else
  {
    status = bright;
    return status;
  }
}
