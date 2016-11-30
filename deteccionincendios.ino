#include <SPFD5408_Adafruit_GFX.h>    // Librerías TFT
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware específico de la pantalla
#include <SPFD5408_TouchScreen.h> // Biblioteca táctil
#include <GSM.h> // Biblioteca àra el módulo GSM


//Definimos el pin de la tarjeta SIM
#define PINNUMBER "<PIN>"
#define MAX_POS 16


GSM gsmAccess;
GSM_SMS sms;




#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif




#define YP A3
#define XM A2
#define YM 9
#define XP 8




// Valores de calibración
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0


#define LCD_RESET A4


#define  BLACK   0x0000
#define RED     0xF800
#define WHITE   0xFFFF




Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;


float tempC;
int tempPin = 5; // Definimos la entrada en pin A5
float Histo[MAX_POS]; // Historial de temperauras
int conta = 0;
float inc = 0;
char txtMsg[] = "INCENDIO\0";
int FrecuenciaMuestras = 3 * 1000;


void drawBorder();


void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Paint!"));


  //GSM
  //Serial.begin(9600);
  while (!Serial) {
    ;
    Serial.println("SMS Messages Sender");
  }

  boolean notConnected = true;


  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  //FIN GSM



  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(3);


  drawBorder();

  tft.setCursor (80, 55);
  tft.setTextSize (3);
  tft.setTextColor(BLACK);
  tft.println("TitanPics");


  tft.setCursor (30, 150);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println("Deteccion de Incendios");


  delay(1000);
}


void loop() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);


  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);


  tft.fillScreen(WHITE);
  tempC = analogRead(tempPin);
  char remoteNum[20] = "684111324";


  // Convierte el valor a temperatura
  tempC = (5.0 * tempC * 100.0) / 1024.0;


  // Se almacena el dato recogido
  if (conta < MAX_POS) {
    Histo[conta] = tempC;
  } else {
    conta = 0;
    Histo[conta] = tempC;
  }


  // Pintar la pantalla
  tft.setCursor (20, 20);
  tft.setTextSize (3);
  tft.setTextColor(BLACK);
  tft.println("Temperatura: ");


  tft.setCursor (90, 60);
  tft.setTextSize (4);
  tft.setTextColor(BLACK);
  tft.println(tempC);
  tft.setCursor (215, 60);
  tft.setTextSize (4);
  tft.setTextColor(BLACK);
  tft.println(" C");
  int x = 5, y = 125;
  for (int h = 0; h < conta; h++) {
    tft.setCursor (5, 100);
    tft.setTextSize (2);
    tft.setTextColor(BLACK);
    tft.println("Historico de temperaturas:");



    tft.setCursor (x, y);
    tft.setTextSize (2);
    tft.setTextColor(BLACK);
    tft.println(Histo[h]);
    if (x >= 200) {
      y += 30;
      x = 5;
    } else {
      x += 80;
    }
  }
  delay(3000);


  if (conta >= 1) {
    inc = Histo[conta] - Histo[conta - 1];
  }


  if ((tempC >= 60) || (inc >= 10)) {
    for (int i = 0; i < 3; i++) {
      tft.fillScreen(WHITE);
      tft.setCursor (60, 100);
      tft.setTextSize (4);
      tft.setTextColor(RED);
      tft.println("INCENDIO");
      delay(1000);
      tft.fillScreen(WHITE);
    }
    // send the message
    /*sms.beginSMS(remoteNum);
      sms.print(txtMsg);
      sms.endSMS();*/
    tft.fillScreen(WHITE);
    tft.setCursor (60, 100);
    tft.setTextSize (3);
    tft.setTextColor(BLACK);
    tft.println("SMS ENVIADO");
    delay(FrecuenciaMuestras);
  } else {
    delay(FrecuenciaMuestras);
  }
  conta++;
}


void drawBorder () {
  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 5;


  tft.fillScreen(RED);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);
}

