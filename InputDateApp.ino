/*
 * Copyright (c) 2016 Arduino da Peste.  All rights reserved.
 * 
 * Abaixo link para baixar a biblioteca necessaria
 * https://drive.google.com/folderview?id=0B7LWVhDM1smbMFZfZ1NCUHRuS3c&usp=sharing
 */
 
#include <Wire.h>
#include <RTClib.h>
#include <CurieBLE.h>

// BLE Peripheral Device (the board you're programming)
BLEPeripheral blePeripheral;  

// BLE LED Service
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); 

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEUnsignedCharCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

// Objeto rtc (relogio de tempo real)
RTC_DS1307 relogio;

//Conectar no pino digital 8 do Arduino Uno (Pode mudar o pino de entrada, mas mude tambem o numero do pino no codigo)
int rele = 8;
int ledArduino = 13;
int TempoToque = 0;
// O primeiro é o tempo de toque da Sirene [0]
int horarios[21]; 
int n = 0;

void setup() {
  Serial.begin(9600);

  // set advertised local name and service UUID:
  blePeripheral.setLocalName("Sirene");
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  // add service and characteristic:
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(switchCharacteristic);

  // set the initial value for the characeristic:
  switchCharacteristic.setValue(0);

  // begin advertising BLE service:
  blePeripheral.begin();

  // A porta do rele esta configurada para liberar energia 
  pinMode(rele, OUTPUT);
  pinMode(ledArduino, OUTPUT);

  //inicia o objeto rtc
  relogio.begin(); 

  //Comente a linha abaixo depois da primeira compilacao.
  //Se seu RTC ja esta com as horas configuradas, deixe esta linha comentada !
  //relogio.adjust(DateTime(__DATE__, __TIME__));

  Serial.println("BLE LED Peripheral");
}

void loop() {
  
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
        if (switchCharacteristic.written()) {
          horarios[n] = switchCharacteristic.value();

          // Lê mais um valor do bluetooth
          n++;
          
       
        }
        
      }
      
      // when the central disconnects, print it out:
        Serial.print(F("Disconnected from central: "));
        Serial.println(central.address());
    }
    
    showInformations();
    horariosDeTocar();
}

void showInformations()
{
  DateTime now = relogio.now();
  
  //Date
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  
  //Time
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  
  //delay(1000);
}

void horariosDeTocar()
{
  DateTime now = relogio.now();
  TempoToque = horarios[0] * 1000;
  
  if(now.hour() == horarios[1] && now.minute() == horarios[2]) //Primeiro toque.
  {
    tocando();
  }
  else if(now.hour() == horarios[3] && now.minute() == horarios[4]) //Segundo toque.
  {
    tocando();
  }
  else if(now.hour() == horarios[5] && now.minute() == horarios[6]) //Terceiro toque.
  {
    tocando();
  }
  else if(now.hour() == horarios[7] && now.minute() == horarios[8]) //Quarto toque.
  {
    tocando();
  }
  else if(now.hour() == horarios[9] && now.minute() == horarios[10]) //Quinto toque.
  {
    tocando();
  }
  else if(now.hour() == horarios[11] && now.minute() == horarios[12]) //Sexto toque.
  {
    tocando();
  }
  else if(now.hour() == horarios[13] && now.minute() == horarios[14]) //Setimo toque.
  {
    tocando();
  }
  else if(now.hour() == horarios[15] && now.minute() == horarios[16]) //Oitavo toque.
  {
    tocando();
  }
  else if(now.hour() == horarios[17] && now.minute() == horarios[18]) //Nono toque.
  {
    tocando();
  }
  else if(now.hour() == horarios[19] && now.minute() == horarios[20]) //Decimo toque.
  {
    tocando();
  }
  else
  {
    // piscaLed();
    digitalWrite(rele, LOW); //Sirene desligada.
    
  }
}

void tocando()
{
    Serial.print("Esta tocando\n");
    Serial.print("O Leandro (e com acento agudo) lindo \n");
    digitalWrite(ledArduino, HIGH);
    digitalWrite(rele, HIGH);
    //Toca 15 segundos (15 * 1000)
    delay(TempoToque);    
    digitalWrite(rele, LOW);
}
