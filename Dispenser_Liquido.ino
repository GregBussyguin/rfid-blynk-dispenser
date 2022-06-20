#include <SPI.h>     
#include <MFRC522.h> 
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL);

extern uint8_t SmallFont[];
byte readCard[4];
char* myTags[5];
String tagID = "";
int ID_Number;
int RFID_Master = 0;
boolean successRead = false;
int moving = 3;
int selecting = 4;
int location = 15;
int press_button = 0;
int press_button2 = 0;
int screen = 1;
int button_selecting = 0;
int button_selecting_two = 0;
int addtagcounter = 1;
int addtagcounter_page = 1;
int removetagcounter = 1;
int removetagcounter_page = 1;
int relay1 = 8;
int relay2 = 7;
int botao_libera = 4;
int libera = 1;
int paglibera = 0;
int bebidas = 0;



#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);


void setup(){
  pinMode(moving, INPUT_PULLUP);
  pinMode(selecting, INPUT_PULLUP);
  pinMode(botao_libera, INPUT_PULLUP);
  pinMode(relay1, INPUT_PULLUP);
  pinMode(relay2, INPUT_PULLUP);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  if (!myOLED.begin(SSD1306_128X64))
    while (1);

  myOLED.setFont(SmallFont);

  myOLED.clrScr();
  myOLED.print("Dispenser Liquido", CENTER, 32);
  myOLED.update();
  delay(4000);
  myOLED.clrScr();
  myOLED.update();

}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  Serial.println(tagID);

  if (tagID == "53EB39B")
  {
    RFID_Master = 1;
    screen = 1;
    addtagcounter_page = 1;
    removetagcounter_page = 1;
    button_selecting = 0;
    button_selecting_two = 0;
    addtagcounter = 1;
    removetagcounter = 1;
    press_button = 0;
    press_button2 = 0;
  }
  else
  {
    if (tagID == myTags[0] || tagID == myTags[1] || tagID == myTags[2] || tagID == myTags[3] || tagID == myTags[4]){
      bebidas = 1;
      while(bebidas == 1){
        myOLED.clrScr();
        myOLED.print("-- Escolha sua bebida --", CENTER, 0);
        myOLED.drawLine(0, 10, 127, 10);
        myOLED.print("Bebida 1", 10, 15);
        myOLED.print("Bebida 2", 10, 28);
        myOLED.print(">", 0, location);

        if (press_button == 0 && digitalRead(moving) == LOW)
      {
        press_button = 1;
        if (location == 15)
        {
          location = 28;
        }
        else if (location == 28)
        {
          location = 15;
        }
      }
      else if (press_button == 1 && digitalRead(moving) == HIGH)
      {
        press_button = 0;
      }
      if (press_button2 == 0 && digitalRead(selecting) == LOW)
      {
        press_button2 = 1;
        if (location == 15)
        {
          paglibera = 1;
          delay(50);
        }
        else if (location == 28)
        {
          paglibera = 2;
          delay(50);
        }
      }
      else if (press_button2 == 1 && digitalRead(selecting) == LOW)
      {
        press_button2 = 0;
      }
      myOLED.update();
      int counting = 0, anti_furto = 0;
      if(paglibera == 1){
        while(paglibera == 1){
        if (libera == 1){
          myOLED.clrScr();
          myOLED.print("Tag liberada!", CENTER, 0);
          myOLED.drawLine(0, 10, 127, 10);
          myOLED.print("Segure o botao ", 10, 28);
          myOLED.print("para liberar", 10, 38);
          myOLED.print("o liquido", 10, 48);
          myOLED.update();
            while(digitalRead(selecting) == LOW && anti_furto < 200){
            digitalWrite(relay1, LOW);
            delay(50);
            counting = 0;
            anti_furto++;
            Serial.println(anti_furto);
            }
            digitalWrite(relay1, HIGH);
            counting++;
            
            
            if(counting > 150 || anti_furto >= 200) paglibera = 0;
        }
        bebidas = 0;
      }
    }

    if(paglibera == 2){
        while(paglibera ==2){
        if (libera == 1){
          myOLED.clrScr();
          myOLED.print("Tag liberada!", CENTER, 0);
          myOLED.drawLine(0, 10, 127, 10);
          myOLED.print("Segure o botao ", 10, 28);
          myOLED.print("para liberar", 10, 38);
          myOLED.print("o liquido", 10, 48);
          myOLED.update();
            while(digitalRead(selecting) == LOW && anti_furto < 200){
            digitalWrite(relay2, LOW);
            delay(50);
            counting = 0;
            anti_furto++;
            Serial.println(anti_furto);
            }
            digitalWrite(relay2, HIGH);
            counting++;
            
            
            if(counting > 150 || anti_furto >= 200) paglibera = 0;
        }
        bebidas = 0;
      }
    }
      }
      
    
    myOLED.clrScr();
    myOLED.drawLine(0, 10, 127, 10);
    myOLED.print("Tempo esgotado!", 10, 28);
    myOLED.update();
    delay(5000);
    myOLED.clrScr();
    myOLED.update();
    }
    else{
      delay(50);
    }
  }
  

    while (RFID_Master == 1){
    if (screen == 1)
    {
      myOLED.clrScr();
      myOLED.print("-- Menu Principal --", CENTER, 0);
      myOLED.drawLine(0, 10, 127, 10);
      myOLED.print("Adicionar tag", 10, 15);
      myOLED.print("Remover tag", 10, 28);
      myOLED.print(">", 0, location);

      if (press_button == 0 && digitalRead(moving) == LOW)
      {
        press_button = 1;
        if (location == 15)
        {
          location = 28;
        }
        else if (location == 28)
        {
          location = 15;
        }
      }
      else if (press_button == 1 && digitalRead(moving) == HIGH)
      {
        press_button = 0;
      }

      if (press_button2 == 0 && digitalRead(selecting) == LOW)
      {
        press_button2 = 1;
        if (location == 15)
        {
          screen = 2;
        }
        else if (location == 28)
        {
          screen = 3;
        }
      }
      else if (press_button2 == 1 && digitalRead(selecting) == LOW)
      {
        press_button2 = 0;
      }
      myOLED.update();

    }

    if (screen == 2)
    {
      if (addtagcounter_page == 1)
      {
        myOLED.clrScr();

        if (strlen(myTags[addtagcounter]) <= 1)
        {
          myOLED.print("ID vazio", LEFT, 54);
        }
        else
        {
          myOLED.print("ID cheio", LEFT, 54);
        }

        myOLED.print("Adicionar tag", CENTER, 0);
        myOLED.drawLine(0, 10, 127, 10);
        myOLED.print("Salvar tag no ID#:", 0, 15);
        myOLED.print("<    >", 46, 36);
        myOLED.print(String(addtagcounter), 61, 36);
        myOLED.print("Continuar", RIGHT, 54);

        if (digitalRead(moving) == LOW)
        {
          addtagcounter++;
          delay(200);
          if (addtagcounter == 5)
          {
            addtagcounter = 1;
          }
        }

        if (digitalRead(selecting) == LOW && button_selecting == 0)
        {
          button_selecting = 1;
        }
        else if (digitalRead(selecting) == HIGH && button_selecting == 1)
        {
          button_selecting = 2;
        }
        else if (digitalRead(selecting) == LOW && button_selecting == 2)
        {
          addtagcounter_page = 2;
        }
        myOLED.update();
      }

      if (addtagcounter_page == 2)
      {
        myOLED.clrScr();
        myOLED.print("Adicionar tag", CENTER, 0);
        myOLED.drawLine(0, 10, 127, 10);
        myOLED.print("Aproxime a tag...", 0, 15);
        myOLED.update();

        while (!successRead)
        {
          successRead = getID();
          if ( successRead == true)
          {
            myOLED.clrScr();
            myOLED.print("Adicionar tag", CENTER, 0);
            myOLED.drawLine(0, 10, 127, 10);
            myOLED.update();
            myOLED.print("Salvando tag no ID#", LEFT, 15);
            myOLED.print(String(addtagcounter), RIGHT, 15);
            myTags[addtagcounter] = strdup(tagID.c_str());
            myOLED.update();
            delay(1000);
            myOLED.print("Adicionando tag", LEFT, 28);
            myOLED.update();
            delay(500);
            myOLED.print("Adicionando tag.", LEFT, 28);
            myOLED.update();
            delay(500);
            myOLED.print("Adicionando tag..", LEFT, 28);
            myOLED.update();
            delay(500);
            myOLED.print("Adicionando tag...", LEFT, 28);
            myOLED.update();
            delay(500);
            myOLED.print("Tag adicionada", LEFT, 41);
            myOLED.update();
            delay(3000);
            myOLED.clrScr();
            myOLED.update();
            RFID_Master = 0;
          }
        }
      }
    }

    if (screen == 3)
    {
      if (removetagcounter_page == 1)
      {
        myOLED.clrScr();

        if (strlen(myTags[removetagcounter]) <= 1)
        {
          myOLED.print("ID vazio", LEFT, 54);
        }
        else
        {
          myOLED.print("ID cheio", LEFT, 54);
        }

        myOLED.print("Remover tag", CENTER, 0);
        myOLED.drawLine(0, 10, 127, 10);
        myOLED.print("Remover tag do ID#:", 0, 15);
        myOLED.print("<    >", 46, 36);
        myOLED.print(String(removetagcounter), 61, 36);
        myOLED.print("Continuar", RIGHT, 54);

        if (digitalRead(moving) == LOW)
        {
          removetagcounter++;
          delay(200);
          if (removetagcounter == 5)
          {
            removetagcounter = 1;
          }
        }

        if (digitalRead(selecting) == LOW && button_selecting_two == 0)
        {
          button_selecting_two = 1;
        }
        else if (digitalRead(selecting) == HIGH && button_selecting_two == 1)
        {
          button_selecting_two = 2;
        }
        else if (digitalRead(selecting) == LOW && button_selecting_two == 2)
        {
          removetagcounter_page = 2;
        }

        myOLED.update();
      }

      if (removetagcounter_page == 2)
      {
        myOLED.clrScr();
        myOLED.print("Remover tag", CENTER, 0);
        myOLED.drawLine(0, 10, 127, 10);
        myOLED.print("Removendo tag de ID#", LEFT, 15);
        myOLED.print(String(removetagcounter), RIGHT, 15);
        myTags[removetagcounter] = NULL;
        myOLED.update();
        delay(1000);
        myOLED.print("Removendo tag", LEFT, 28);
        myOLED.update();
        delay(500);
        myOLED.print("Removendo tag.", LEFT, 28);
        myOLED.update();
        delay(500);
        myOLED.print("Removendo tag..", LEFT, 28);
        myOLED.update();
        delay(500);
        myOLED.print("Removendo tag...", LEFT, 28);
        myOLED.update();
        delay(500);
        myOLED.print("Tag removida", LEFT, 41);
        myOLED.update();
        delay(3000);
        myOLED.clrScr();
        myOLED.update();
        RFID_Master = 0;
      }
    }
  }
  successRead = false;
}

uint8_t getID() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {
    readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return 1;
}
