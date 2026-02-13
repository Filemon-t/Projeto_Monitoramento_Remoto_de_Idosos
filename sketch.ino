#include "CTBot.h";
#include <Adafruit_MPU6050.h>
#include <ArduinoJson.h>

Adafruit_MPU6050 mpu;

float x = 0, y = 0, z = 0; //eixos

#define ssid_wifi "Wokwi-GUEST"
#define password_wifi "" //na simulação tem que ficar dessa forma pois não pega o wifi do dispositivo


int maxValue = 9; //variação maior q 9 = possível queda

// para não sobrecarregar o dispositivo e a rede pelo excesso de verificações do telegram
const long MessagesInterval = 30000; // 30s
unsigned long VerifyTelegram = 0;

//TELEGRAM API TOKEN
const char* BotToken = "SEU_TOKEN";  // crie um bot no Telegram pelo @BotFather, copie o token gerado e substitua no código. Não utilize tokens de terceiros
CTBot myBot;
TBMessage msg;

void ReadValues() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  x = a.acceleration.x < 0 ? a.acceleration.x * -1 : a.acceleration.x; // if simplificado
  y = a.acceleration.y < 0 ? a.acceleration.y * -1 : a.acceleration.y; // tirar o valor negativo que não vai ser utilizado nesse projeto
  z = a.acceleration.z < 0 ? a.acceleration.z * -1 : a.acceleration.z;
}

void SendMessage() {
  Serial.println("POSSÍVEL QUEDA!"); // aqui envia a mensagem no TERMINAL
  myBot.sendMessage(SEU_ID, "POSSÍVEL QUEDA"); // em "SEU_ID" precisa se informar sobre seu user ID do telegram para colocar nesse campo, o bot @userinfobot consegue pegar essa informação
} // aq em cima envia a mensagem no telegram

void CheckFall(float v1, float v2) {
  float vt = v1 - v2; // difereça entre o valor antigo e o novo
  if (vt <= maxValue * -1 || vt >= maxValue) { //Se a diferença for muito positiva ou muito negativa significa movimento brusco
    SendMessage(); // caso o valor acima esteja correto chama a função de mandar mensagem e atualiza valores para evitar spam
    ReadValues();
  }
}

void setup(void) { // setup é usada para inicializar o arduino/esp 
  Serial.begin(115200); // conecta o esp com pc, precisa ser nessa velocidade
  Serial.println("testando MPU6050");
  delay(10);

  if (!mpu.begin()) { // Se não encontrar o mpu
    Serial.println("Sensor MPU6050 não identificado"); //avisa isso
    while (1) { // evita que o programa fique rodando sem o sensor
      delay(10);
    }
  }
  Serial.println("MPU6050 Identificado!"); // confirmando que encontrou

  myBot.wifiConnect(ssid_wifi, password_wifi); // conectando o wifi com o bot
  myBot.setTelegramToken(BotToken); // o token de cima 

  if (myBot.testConnection())
    Serial.println("\nConexão OK");
  else
    Serial.println("\nSem conexão");


  mpu.setAccelerometerRange(MPU6050_RANGE_16_G); // velocidade normal c/ gravidade
  Serial.print("Faixa do acelerômetro definida para: "); 
  switch (mpu.getAccelerometerRange()) { // if else mas organizado para essa função
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G"); // apenas confirmando qual a aceleração usada, podendo retirar, melhor para verificar o funcionamento correto
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); // rotação s/ gravidade
  Serial.print("Faixa do giroscópio definida para: "); 
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_260_HZ); // filtra o ruído, outros movimentos para desconsiderar nos de cima
  Serial.print("Largura da frequência definida para: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;

      delay(1000);

  }

  Serial.println("");
  delay(500);

  ReadValues();

}

void loop() { //colocando para funcionar agr!!

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float x1, y1, z1;

  x1 = a.acceleration.x < 0 ? a.acceleration.x * -1 : a.acceleration.x;
  y1 = a.acceleration.y < 0 ? a.acceleration.y * -1 : a.acceleration.y;
  z1 = a.acceleration.z < 0 ? a.acceleration.z * -1 : a.acceleration.z;

  CheckFall(x, x1);
  CheckFall(y, y1);
  CheckFall(z, z1);

  x = x1;
  y = y1;
  z = z1;

  delay(200);

}