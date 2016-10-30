int led = 13;
String recebido[3];
int n = 0;
int iNum1;
int iNum2;
int iNum3;

int tam;
  
void setup() { 
 pinMode(led, OUTPUT); 
 Serial.begin(9600);
}
  
void loop() {
  // Se receber algo pela serial
  if (Serial.available() > 0){
    // Lê toda string recebida
    recebido[n] = leStringSerial();
    
    String sNum1 = recebido[0];
    String sNum2 = recebido[1];
    String sNum3 = recebido[2];
    
    iNum1 = recebido[0].toInt();
    iNum2 = recebido[1].toInt();
    iNum3 = recebido[2].toInt();
    
    if(sNum3 != ""){
      Serial.println("Number1 + 1 = ");
     	Serial.println(iNum1 + 1);
      Serial.println("Number2 + 1 = ");
     	Serial.println(iNum2 + 1);
      Serial.println("Number3 + 1 = ");
     	Serial.println(iNum3 + 1);
    }
    
    n++;
  }
}

/**
 * Função que lê uma string da Serial
 * e retorna-a
 */
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
