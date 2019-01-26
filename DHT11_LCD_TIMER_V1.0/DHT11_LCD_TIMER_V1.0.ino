/* Se modificaron: 
   - i2c para LCD
   - Tiempos sin interrupciones
   - Se quitaron los delays
   - Se agregó mensaje de bienvenida
*/

//#include <EEPROM.h> // Habilitar si se quiere setear los valores de temperatura 
//#include <LiquidCrystal.h>// include the library code
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //Direccion de LCD, Pines A4 (SDA) y A5 (SCL)

//------------------------------DHT

#define DHTPIN 2  //Pin al que deber ser conectado el DHT11
#define DHTTYPE DHT11  //DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

//------------------------------RELOJ

#include "RTClib.h"
RTC_DS1307 RTC;

//------------------------------E/S

#define led 8   //Pin para usar de salida de actuacion 

//------------------------------Variables

byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
float h ;  // humedad (Número entero)
float t ; // Temperatura (Celsius)
float f ; // Temperatura (Farenheit)

/**************Seteo de Tiempos*************/ 

//------------------------------Monitor

unsigned long previousMillis = 0;
const long interval = 250 ;

//------------------------------Reloj
/*
unsigned long previousMillis_1 = 0 ;
const long interval_1 = 150;
*/
//------------------------------DHT11

unsigned long previousMillis_2 = 0 ;
const long interval_2 = 250 ;

void setup() {
  
//------------------------------E/S

  pinMode (led, OUTPUT); // LED de encendido / apagado

  //----Mensaje de Bienvenida----//
  
  lcd.init(); 
  lcd.backlight(); 
  lcd.setCursor(2, 0);
  lcd.print("ElectroLEC");

  lcd.setCursor(2, 1);
  lcd.print(" Cargando.");
  delay(1000);
  lcd.setCursor(2, 1);
  lcd.print(" Cargando..");
  delay(1000);
  lcd.setCursor(2, 1);
  lcd.print(" Cargando...");
  delay(1000);
  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.print("ElectroLEC");

  lcd.setCursor(2, 1);
  lcd.print(" Cargando.");
  delay(1000);
  lcd.setCursor(2, 1);
  lcd.print(" Cargando..");
  delay(1000);
  lcd.setCursor(2, 1);
  lcd.print(" Cargando...");
  delay(100);
  lcd.clear();
  
//------------------------------Iniciamos DHT
  
  dht.begin();
  
//------------------------------Iniciamos el Reloj y comprobamos que funcione

if (! RTC.isrunning()) // se verifica si el modulo rtc esta en funcionamiento, de lo contrario, se imprime: RTC is NOT running!
          {
          lcd.println("RTC is NOT running!");
          delay(10000);
          }
          // RTC.adjust(DateTime(__DATE__, __TIME__)); // SI SE HABILITA, CADA VEZ QUE SE APAGA VUELVE A LA HROA QUE SE CREO EL PROGRAMA
}

void loop()       {



//------------------------------Monitor
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Monitor ();
  }

//------------------------------Reloj

 mostrarRTCLCD(); // Sin tiempos para que el segundero Funcione
/*
  unsigned long currentMillis_1 = millis();

  if (currentMillis_1 - previousMillis_1 >= interval_1) {
    previousMillis_1 = currentMillis_1;
    Seteo ();
  } 
*/
//------------------------------DHT11


  unsigned long currentMillis_2 = millis();

  if (currentMillis_2 - previousMillis_2 >= interval_2) {
    previousMillis_2 = currentMillis_2;
    SensorTemp_Hum ();
  } 
}

/**************Funciones*************/

  void Monitor (){
 
  lcd.setCursor(10,0);
  lcd.print("T:");
  lcd.print(t,1);
  lcd.print ((char)223);
  lcd.print("C"); 
  lcd.setCursor(10,1);
  lcd.print("H:");
  lcd.print(h,0);
  lcd.print("%"); 
 }


void SensorTemp_Hum (){

      if (isnan(h) || isnan(t) || isnan(f)){
        lcd.print("error sensor DHT !");
        return;
          }
          
   h = dht.readHumidity();  // Humedad (Se cambio a número entero para los fines de ahorrar espacio en la pantalla LCD)
   t = dht.readTemperature();  // Celcius
   f = dht.readTemperature(true);  // Fahrenheit      
  }

void mostrarRTCLCD() //Funcion que lee los datos de modulo RTC y despues los imprime en el display
{
    DateTime now = RTC.now(); //obtiene datos del modulo RTC
    //lcd.clear();
    lcd.setCursor(0,0);
    if(now.day() < 10)
    {
    lcd.print("0");
    }
    lcd.print(now.day(), DEC); //imprime dia
    lcd.print('/');
    if(now.month() < 10)
    {
    lcd.print("0");
    }
    lcd.print(now.month(), DEC); //imprime mes
    lcd.print('/');
    lcd.print(now.year()-2000, DEC);  //imprime el año
    
    lcd.setCursor(0,1);
    if(now.hour() < 10)
    {
    lcd.print("0");
    }
    lcd.print(now.hour(), DEC); //imprime hora
    lcd.print(':');
    if(now.minute() < 10)
    {
    lcd.print("0");
    }
    lcd.print(now.minute(), DEC); //imprime minutos
    lcd.print(':');
    if(now.second() < 10)
    {
    lcd.print("0");
    }
    lcd.print(now.second(), DEC); //imprime segundos
    }
