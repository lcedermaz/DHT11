#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);//Direccion de LCD

#include <Wire.h>
#include <math.h>

#include <avr/pgmspace.h> // Función PROGMEM, sirve para que no ocupe datos en la memoria sdram, pero si la flash

#include <OneWire.h>                
//#include <DallasTemperature.h>

//--------------------MicroSD

#include <SPI.h>
#include <SD.h>
const int chipSelect = 10; // Es el 10 para el modulo shield con SD
File myFile;

Sd2Card card;
SdVolume volume;
SdFile root;

// -- Variables Globales--
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

//--------------------RTC
#include "RTClib.h"
RTC_DS1307 RTC;
#define DS1307_I2C_ADDRESS 0x68  // Dirección I2C 

//------------------------------ DHT11

#include "DHT.h"

#define DHTPIN 6 // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

int h = 85 ; // humedad (Número entero)
float t = 23 ; // Temperatura (Celsius)
float f = 65 ; // Temperatura (Farenheit)              

//------------------------------ LCD

unsigned long previousMillis = 0;        
const long interval = 2000 ;  

//----------------------------- SD

unsigned long previousMillis_1 = 0 ; 
const long interval_1 = 5000;

//------------------------------ DHT11

unsigned long previousMillis_2 = 0;        
const long interval_2 = 500 ;  

void setup() 

{
 //----Mensaje de Bienvenida----//
  
  lcd.init(); 
  lcd.backlight(); 
 
  lcd.setCursor(2, 0);
  lcd.print("Provefarma SA");

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
  
//------Iniciamos DHT11 ------//
  
  dht.begin();
    
//------ Iniciamos Monistor Serial ------//
    
 Serial.begin(9600);

//------ Iniciamos SD ------//
   
  Serial.print(F("Iniciando SD ..."));
  
  if ((!SD.begin(10))) {
    Serial.println(F("No se pudo inicializar"));
    return;
  }
    Serial.println(F("inicializacion exitosa"));
          
    if(!SD.exists("datalog.csv"))
      {
      myFile = SD.open("datalog.csv", FILE_WRITE);
      if (myFile) {
        Serial.println(F("Archivo nuevo, Escribiendo encabezado(fila 1)"));
        myFile.println("Fecha,Hora,T,F,h");
        myFile.close();
      } else {
        
        Serial.println(F("Error creando el archivo datalog.csv"));
      }
    }
  }

// -- Conversor BCD ==> DECIMAL --

byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
} 

// -- Imprime con formato de 2 dígitos
String fprint (int dato)
{
String retorno = String(dato);
if (retorno.length() < 2) 
  retorno = "0" + retorno;
return retorno;
  
            
}

void loop() {

//------------------------------LCD

  unsigned long currentMillis = millis();   
 
if(currentMillis - previousMillis >= interval) {
    
  previousMillis = currentMillis;
  
  LCD_Monitor ();
}
  
//------------------------------SD

  unsigned long currentMillis_1 = millis();   
 
if(currentMillis_1 - previousMillis_1 >= interval_1) {
    
  previousMillis_1 = currentMillis_1;

  SDlog ();
}

//------------------------------DHT11

  unsigned long currentMillis_2 = millis();   
 
if(currentMillis_2 - previousMillis_2 >= interval_2) {
    
  previousMillis_2 = currentMillis_2;
  
   // SensorTemp_Hum ();
}

//------------------------------RTC

  reloj_RTC ();

}



//----------------------------- Sub - Funciones  
//----------------------------- RTC (Reloj)

void reloj_RTC () {

// -- Proceso de Lectura --
  Wire.beginTransmission(DS1307_I2C_ADDRESS);   // Linea IC2 en modo escitura
  Wire.write((byte)0x00);                       // Setea el punto de registro a(0x00)
  Wire.endTransmission();                       // Final de la escritura de transmición
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);      // Abre I2C en modo de envío
  second     = bcdToDec(Wire.read() & 0x7f);    // Lee 7 bytes de datos
  minute     = bcdToDec(Wire.read());
  hour       = bcdToDec(Wire.read() & 0x3f);  
  dayOfWeek  = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month      = bcdToDec(Wire.read());
  year       = bcdToDec(Wire.read());
}

//------------------------------DHT11 

void SensorTemp_Hum (){

    if (isnan(h) || isnan(t) || isnan(f)){
        lcd.print("error sensor DHT !");
        return;
          }
          
   h = dht.readHumidity();  // Humedad (Se cambio a número entero para los fines de ahorrar espacio en la pantalla LCD)
   t = dht.readTemperature();  // Celcius
   f = dht.readTemperature(true);  // Fahenheit      

  }

void LCD_Monitor () {
  
//------------------------------ HORA / MINUTO

lcd.setCursor(10,0);
lcd.print(fprint(hour)); //imprime hora
lcd.print(':');
lcd.print(fprint(minute)); //imprime minutos

lcd.setCursor(8,1);
lcd.print(fprint(dayOfMonth)); //Día
lcd.print('/');
lcd.print(fprint(dayOfWeek)); //Mes
lcd.print('/');
lcd.print(fprint(year)); //Mes
//------------------------------Temperatura
  
  lcd.setCursor(3,0);
  lcd.print(t,0);
  lcd.setCursor(0,0); 
  lcd.print("T :");
  lcd.setCursor(5,0);
  lcd.print((char)223);
  lcd.setCursor(6,0);
  lcd.print("C"); 

  /*
  if (t < 50) {       // si es mayor a 50, OL, se hace por que se corre la letra TH o TS cuando empieza a contar para el promedio
  lcd.setCursor(4,0);
  lcd.print(t,0);
  lcd.setCursor(0,0); 
  lcd.print("T :");
  lcd.setCursor(6,0);
  lcd.print((char)223);
  lcd.setCursor(7,0);
  lcd.print("C"); 
  } else {
  lcd.setCursor(0,0);
  lcd.print("OL");
  lcd.print("T ");
} 
*/
//------------------------------humedad Relativa

  lcd.setCursor(3,1);
  lcd.print(h,1); 
  lcd.setCursor(0,1);
  lcd.print("HR:");
  lcd.setCursor(5,1);
  lcd.print("%");


  /*
  if (h<100){
  lcd.setCursor(3,1);
  lcd.print(h,1); 
  lcd.setCursor(0,1);
  lcd.print("HR:");
  lcd.setCursor(5,1);
  lcd.print(" %");
                } else {     // Con el dht 11 no es necesaria esta funcion
  lcd.setCursor(8,1);
  lcd.print("100.0"); 
  lcd.setCursor(13,1);
  lcd.print("HR%");
                }
  */
 }


 //------------------------------ESCRITURA MICROSD
 
 void SDlog () {
  
  myFile = SD.open("datalog.csv", FILE_WRITE);//abrimos  el archivo
  
  if (myFile) { 
        
        Serial.print("Escribiendo SD: ");           
        myFile.print(fprint(dayOfMonth)); // Día
        myFile.print("/");      
        myFile.print(fprint(month)); // Mes
        myFile.print("/");
        myFile.print(fprint(year)); // Año
        myFile.print(" ");
        myFile.print(fprint(hour)); // Hora
        myFile.print(":");
        myFile.print(fprint(minute)); // Minuto
        myFile.print(" ");
        myFile.print(t);
        myFile.print(",");
   /*   myFile.print(THumedo);
        myFile.print(",");
    */  myFile.println(h); 
        myFile.close(); //cerramos el archivo

        //-----------------------------------------MONITOR SERIAL
        
        Serial.print(F("Fecha ")); 
        Serial.print(fprint(dayOfMonth)); // Días
        Serial.print(F("/"));
        Serial.print(fprint(month)); // Mes
        Serial.print("/");
        Serial.print(fprint(year)); // Año
        Serial.print(F(" Hora "));
        Serial.print(fprint(hour)); // Hora
        Serial.print(":");
        Serial.print(fprint(minute)); // Minutos
        Serial.print(F(" "));
        Serial.print(F(" T:"));
        Serial.print(t);
  //      Serial.print(F(" TH:"));
  //      Serial.print(THumedo);       
        Serial.print(F(" h:"));
        Serial.println(h);            
  } else {
    // if the file didn't open, print an error:
    Serial.println("Error al abrir el archivo");
  }
  
 }
