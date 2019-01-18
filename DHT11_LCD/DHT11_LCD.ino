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

//------------------------------E/S

#define led 8   //Pin para usar de salida de actuacion 
/*
#define Puls_arrib_min 6   //Pulsador seteo en pin 7 (TMIN)
#define Puls_abaj_min 7   // Pulsador seteo en pin 8 (TMIN)

#define Puls_arrib_max 9  //Pulsador seteo en pin 9 (TMAX)
#define Puls_abaj_max 10  // Pulsador seteo en pin 10 (TMAX)


//------------------------------Variables

int t_min = 23 ; //variable de seteo para temperatura ideal (TMIN)
int t_max = 28 ; //variable de seteo para temperatura ideal (TMAX)
*/
float h ;  // humedad (Número entero)
float t ; // Temperatura (Celsius)
float f ; // Temperatura (Farenheit)

/**************Seteo de Tiempos*************/ 

//------------------------------Monitor

unsigned long previousMillis = 0;
const long interval = 50 ;

//------------------------------Seteo
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
  /*pinMode (Puls_arrib_min,INPUT); 
  pinMode (Puls_abaj_min, INPUT); 
  pinMode (Puls_arrib_max, INPUT); 
  pinMode (Puls_abaj_max, INPUT); */

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
}

void loop()       {

//------------------------------Monitor
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Monitor ();
  }

//------------------------------Seteo

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
 
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(t,1);
  lcd.print ((char)223);
  lcd.print("C"); 
  lcd.setCursor(0,1);
  lcd.print("Hum : ");
  lcd.print(h,1);
  lcd.print(" %"); 
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





//----------------VALORES SETEADOS----------------
/*
  lcd.setCursor (0,1);  //setear el cursor a 0,1
  lcd.print ("T1:");  //imprimir el valor en grados celsius
  lcd.print (t_min,1);  //valor 1
  lcd.print ((char)223);
  //lcd.print ("C");
  lcd.setCursor (10,1);
  lcd.print ("T2:");  //imprimir el valor en grados celsius  
  lcd.print (t_max,1);  // valor 2
  lcd.print ((char)223); 
  //lcd.print ("C");    
 }
*/

 /*
 void Seteo () { 
  
//-----------------------------SET (TMIN)-------------------------------------

             if (digitalRead (Puls_arrib_max)==true)// si el pulsador se activa, setea 1 grado
             {
             (t_min ++); // esta es la temperatura seteada 
             }

              
            if(digitalRead (Puls_abaj_max)==true)// si el otro pulsador se activa, setea 1 grado
            {
            (t_min--); // pero quita 1 grado del t_min
            }
            
               
//-----------------------------SET(TMAX)----------------------------
        
             if (digitalRead (Puls_arrib_min)== true) // si el pulsador se activa, setea 1 grado
             {
             (t_max++);  // esta es la temperatura seteada 
             }
         

            if(digitalRead (Puls_abaj_min)== true) // si el otro pulsador se activa, setea 1 grado
            {
            (t_max --); //pero quita 1 grado del t_min
            }

//-----------------------------Funcion----------------------------
      
      if (( t_min < t )||( t < t_max ))  // si la temperatura esta dentro de los valores seteados
            
            {
            digitalWrite (led,HIGH);  // prender el led
            }
            
            else  // si eso no ocurre, apagar el led
            
            {
            digitalWrite (led,LOW);
            }
      
}
*/
