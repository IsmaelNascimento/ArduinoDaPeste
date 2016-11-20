/*
 * Copyright (c) 2016 Arduino da Peste.  All rights reserved.
 * 
 * Abaixo link para baixar a biblioteca necessaria
 * https://drive.google.com/folderview?id=0B7LWVhDM1smbMFZfZ1NCUHRuS3c&usp=sharing
 */
//3000 é o código para emergência
//30001 é o código para update 
#include <Wire.h>
#include <RTClib.h>
#include <CurieBLE.h>
#include <EEPROM.h>
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
int horarios[10];
int tamanho = sizeof(horarios)/sizeof(int);
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
  horariosDeTocar();
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();
  // if a central is connected to peripheral:
  if (central) {
    /*
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    */
    // while the central is still connected to peripheral:
    while (central.connected()) {
         for(int i = 0; i < tamanho; i++) {
              // Serial.print("//========================\\");
              Serial.print("Valor ");
              Serial.print(i);
              Serial.print(" = ");
              Serial.println(EEPROM.read(i)); 
              // Serial.print("//========================\\");
        } 
        showInformations();
        // if the remote device wrote to the characteristic,
        // use the value to control the LED:
        if (switchCharacteristic.written()) {
          if (switchCharacteristic.value() == 255){
            tocando();   
          }
          else if(switchCharacteristic.value() == 254){
            while(switchCharacteristic.value() == 254){
              Serial.println("Digite outro valor");
              showInformations();
            }
            int horaUpdate = switchCharacteristic.value() - 1;
            int minutoUpdate = switchCharacteristic.value();

            while(minutoUpdate == switchCharacteristic.value()){
              Serial.println("Digite o valor da hora");
              showInformations();
            }
            horarios[horaUpdate] = switchCharacteristic.value();
            EEPROM.write(horaUpdate,horarios[horaUpdate]);
             while(horarios[horaUpdate] == switchCharacteristic.value()){
              Serial.println("Digite o valor dos minutos");
              showInformations();
            }
            horarios[minutoUpdate] = switchCharacteristic.value();
            EEPROM.write(minutoUpdate,horarios[minutoUpdate]);
          }
          else{
            horarios[n] = switchCharacteristic.value();
            // Lê mais um valor do bluetooth
            EEPROM.write(n,horarios[n]);
            n++;
          }
        

          
     }
     delay(1000);
    }
    
  }
    
    
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    showInformations();
}
void showInformations() {
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
}

//OBS DOWN
void horariosDeTocar() {
  DateTime agora = relogio.now();
  //Primeiro toque.
  for(int i = 1; i < tamanho; i++){  
    if(agora.hour() == horarios[i] && agora.minute() == horarios[i++] && agora.second() == 0) {
      tocando();
    }
  }
  piscaLed();
  
}

void tocando()
{
    TempoToque = EEPROM.read(0) * 1000;
    //TempoToque = horarios[0];
    
    Serial.println("Esta tocando");
    digitalWrite(rele, HIGH);
    digitalWrite(ledArduino, HIGH);
    delay(TempoToque);    
    digitalWrite(rele, LOW);
    digitalWrite(ledArduino, LOW);
}

void piscaLed()
{
  digitalWrite(ledArduino, HIGH);
  delay(500);
  digitalWrite(ledArduino, LOW);
  delay(500);
}
