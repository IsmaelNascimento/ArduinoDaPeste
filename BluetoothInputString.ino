int led = 13;
String recebido[3];
int n = 0;
int iNum1, iNum2, iNum3;

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
    
    // Fazendo conversão de String para Int
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
