#define BLYNK_TEMPLATE_ID "TMPLiMpddfQn"
#define BLYNK_DEVICE_NAME "Dispenser"
#define BLYNK_AUTH_TOKEN "8i36pYVk1faLBgb_g6BqrG_apMPZ_cM5"

#define BLYNK_PRINT Serial

#define Bebida1 D5
#define Bebida2 D6

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "gkbressam";
char pass[] = "gkb46505699";

WidgetTerminal terminal(V2);

void rele(){
  while(digitalRead(Bebida1)== LOW){
    Blynk.virtualWrite(V0, 1);
  }
  delay(50);
  Blynk.virtualWrite(V0, 0);
  while(digitalRead(Bebida2)== LOW){
    Blynk.virtualWrite(V1, 1);
  }
  delay(50);
  Blynk.virtualWrite(V1, 0);
}


void setup()
{
  terminal.clear();
  pinMode(Bebida1, INPUT_PULLUP);
  pinMode(Bebida2, INPUT_PULLUP);
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  terminal.println(F("Dispenser Ligado"));
  terminal.println();
  terminal.println();
  terminal.flush();
}

void loop()
{
  Blynk.run();
  rele();
}
