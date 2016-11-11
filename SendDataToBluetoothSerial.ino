/*
 * Copyright (c) 2016 Arduino da Peste.  All rights reserved.
 */

#include <CurieBLE.h>

BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEUnsignedCharCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int ledPin = 13; // pin to use for the LED
String recebido[3];
int v = 0;
int iNum1, iNum2, iNum3;

void setup() {
  Serial.begin(9600);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

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
          recebido[v] = switchCharacteristic.value();
    
          iNum1 = recebido[0].toInt();
          iNum2 = recebido[1].toInt();
          iNum3 = recebido[2].toInt();
          
          // Verificação de impressão em serial monitor
          if(recebido[2] != ""){
            Serial.println("Number1 + 1 = ");
             Serial.println(iNum1 + 1);
            Serial.println("Number2 + 1 = ");
            Serial.println(iNum2 + 1);
            Serial.println("Number3 + 1 = ");
            Serial.println(iNum3 + 1);
          }
          
          // Lê mais um valor do bluetooth
          v++;
    
          
          /*
          if (switchCharacteristic.written()) {
            if (switchCharacteristic.value() == 1) {   // any value other than 0
              Serial.println("LED on");
              digitalWrite(ledPin, HIGH);         // will turn the LED on
            } else {                              // a 0 value
              Serial.println(F("LED off"));
              digitalWrite(ledPin, LOW);          // will turn the LED off
            }
            */
        }
        
      }
      // when the central disconnects, print it out:
        Serial.print(F("Disconnected from central: "));
        Serial.println(central.address());
    }
}
    

String leStringSerial(){
  String conteudo = "";
  char caractere;
  
  // Enquanto receber algo pela serial
  while(Serial.available() > 0) {
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n'){
      // Concatena valores
      conteudo.concat(caractere);
    }
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  }
    
  Serial.print("Recebi: ");
  Serial.println(conteudo);
    
  return conteudo;
}
