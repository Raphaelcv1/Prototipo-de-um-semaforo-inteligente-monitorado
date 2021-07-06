#include <PubSubClient.h>
#include <WiFi.h>

const char* ssid = "NOME_REDE";
const char* password = "SENHA_REDE";

const char* mqttServer = "ENDEREÇO_BROKER";
const int mqttPort = PORTA_DE_ACESSO;
const char* mqttUser = "USUÁRIO_BROKER";
const char* mqttPassword = "SENHA_DO_USUÁRIO";

const char* mqttTopic_G1 = "esp/G1";
const char* mqttTopic_G2 = "esp/G2";
const char* mqttTopic_G3 = "esp/G3";
const char* mqttTopic_G4 = "esp/G4";

const char* mqttTopic_Tmp1 = "esp/Tmp1";
const char* mqttTopic_Tmp2 = "esp/Tmp2";
const char* mqttTopic_Tmp3 = "esp/Tmp3";
const char* mqttTopic_Tmp4 = "esp/Tmp4";
const char* mqttTopic_Tmp5 = "esp/Tmp5";
const char* mqttTopic_Tmp6 = "esp/Tmp6";

String clientId = "DENTIFICAÇÃO_DO_USUÁRIO";

int ledG_G1 = 12;
int ledY_G1 = 14;
int ledR_G1 = 27;

int ledG_G2 = 26;
int ledY_G2 = 25;
int ledR_G2 = 33;

int ledG_G3 = 32;
int ledY_G3 = 22;
int ledR_G3 = 23;

int ledG_G4 = 18;
int ledY_G4 = 19;
int ledR_G4 = 21;

int tmp_it1 = 21000;
int tmp_it2 = 3000;
int tmp_it3 = 2000;
int tmp_it4 = 15000;
int tmp_it5 = 3000;
int tmp_it6 = 2000;

char buf[33];

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(ledG_G1, OUTPUT);
  pinMode(ledY_G1, OUTPUT);
  pinMode(ledR_G1, OUTPUT);
  
  pinMode(ledG_G2, OUTPUT);
  pinMode(ledY_G2, OUTPUT);
  pinMode(ledR_G2, OUTPUT);

  pinMode(ledG_G3, OUTPUT);
  pinMode(ledY_G3, OUTPUT);
  pinMode(ledR_G3, OUTPUT);

  pinMode(ledG_G4, OUTPUT);
  pinMode(ledY_G4, OUTPUT);
  pinMode(ledR_G4, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Connecting to WiFi:");
        Serial.println(ssid);
    }

    Serial.println("Connected to the WiFi network");
    Serial.println("");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    while (!client.connected()) {
        Serial.println("Connecting to MQTT…");
        
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqttUser, mqttPassword )) {
            Serial.println("connected");

            client.subscribe(mqttTopic_Tmp1);
            client.subscribe(mqttTopic_Tmp2);
            client.subscribe(mqttTopic_Tmp3);
            client.subscribe(mqttTopic_Tmp4);
            client.subscribe(mqttTopic_Tmp5);
            client.subscribe(mqttTopic_Tmp6);
            
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    client.publish(mqttTopic_Tmp1, convert(tmp_it1));
    client.publish(mqttTopic_Tmp2, convert(tmp_it2));
    client.publish(mqttTopic_Tmp3, convert(tmp_it3));
    client.publish(mqttTopic_Tmp4, convert(tmp_it4));
    client.publish(mqttTopic_Tmp5, convert(tmp_it5));
    client.publish(mqttTopic_Tmp6, convert(tmp_it6));
    
}

void loop() {
  if (!client.connected()) {
    reconect();
  }
  t1();
  client.loop();
  t2();
  client.loop();
  t3();
  client.loop();
  t4();
  client.loop();
  t5();
  client.loop();
  t6();
  client.loop();  

}

void t1(){
  
  digitalWrite(ledG_G1, HIGH);
  digitalWrite(ledY_G1, LOW);
  digitalWrite(ledR_G1, LOW);
  client.publish(mqttTopic_G1, "G");
  
  digitalWrite(ledG_G3, HIGH);
  digitalWrite(ledY_G3, LOW);
  digitalWrite(ledR_G3, LOW);
  client.publish(mqttTopic_G3, "G");
  
  digitalWrite(ledG_G2, LOW);
  digitalWrite(ledY_G2, LOW);
  digitalWrite(ledR_G2, HIGH);
  client.publish(mqttTopic_G2, "R");

  digitalWrite(ledG_G4, LOW);
  digitalWrite(ledY_G4, LOW);
  digitalWrite(ledR_G4, HIGH);
  client.publish(mqttTopic_G4, "R");

  delay(tmp_it1);
}

void t2(){
  
  digitalWrite(ledG_G1, LOW);
  digitalWrite(ledY_G1, HIGH);
  client.publish(mqttTopic_G1, "Y");
  
  digitalWrite(ledG_G3, LOW);
  digitalWrite(ledY_G3, HIGH);
  client.publish(mqttTopic_G3, "Y");

  delay(tmp_it2);
}

void t3(){
  
  digitalWrite(ledY_G1, LOW);
  digitalWrite(ledR_G1, HIGH);
  client.publish(mqttTopic_G1, "R");
  
  digitalWrite(ledY_G3, LOW);
  digitalWrite(ledR_G3, HIGH);
  client.publish(mqttTopic_G3, "R");

  delay(tmp_it3);
}

void t4(){
    
  digitalWrite(ledG_G2, HIGH);
  digitalWrite(ledR_G2, LOW);
  client.publish(mqttTopic_G2, "G");

  digitalWrite(ledG_G4, HIGH);
  digitalWrite(ledR_G4, LOW);
  client.publish(mqttTopic_G4, "G");

  delay(tmp_it4);
}

void t5(){
  
  digitalWrite(ledG_G2, LOW);
  digitalWrite(ledY_G2, HIGH);
  client.publish(mqttTopic_G2, "Y");
  
  digitalWrite(ledG_G4, LOW);
  digitalWrite(ledY_G4, HIGH); 
  client.publish(mqttTopic_G4, "Y");

  delay(tmp_it5);
  
}

void t6(){
  
  digitalWrite(ledY_G2, LOW);
  digitalWrite(ledR_G2, HIGH);
  client.publish(mqttTopic_G2, "R");

  digitalWrite(ledY_G4, LOW);
  digitalWrite(ledR_G4, HIGH);
  client.publish(mqttTopic_G4, "R");

  delay(tmp_it6);
}

void callback(char* topic, byte* payload, unsigned int length) {

    String message;

    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
  #ifdef DEBUG
  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  Serial.println();
  Serial.println("-----------------------");
  #endif

  if(String(topic) == "esp/Tmp1"){
    if (message.toInt() != tmp_it1){        
       tmp_it1 = message.toInt(); 
    }
  }

  if(String(topic) == "esp/Tmp2"){
    if (message.toInt() != tmp_it2){         
       tmp_it2 = message.toInt();
    }
  }

if(String(topic) == "esp/Tmp3"){
    if (message.toInt() != tmp_it3){         
       tmp_it3 = message.toInt();
    }
  }

  if(String(topic) == "esp/Tmp4"){
    if (message.toInt() != tmp_it4){        
       tmp_it4 = message.toInt(); 
    }
  }

    if(String(topic) == "esp/Tmp5"){
    if (message.toInt() != tmp_it5){        
       tmp_it5 = message.toInt();
    }
  }

  if(String(topic) == "esp/Tmp6"){
    if (message.toInt() != tmp_it6){        
       tmp_it6 = message.toInt();
    }
  }
}

void reconect() {
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.print("Tentando conectar ao servidor MQTT");
    #endif
     
    bool conectado = strlen(mqttUser) > 0 ?
                     client.connect(clientId.c_str(), mqttUser, mqttPassword) :
                     client.connect(clientId.c_str());
 
    if(conectado) {
      #ifdef DEBUG
      Serial.println("Conectado!");
      #endif

        client.subscribe(mqttTopic_Tmp1);
        client.subscribe(mqttTopic_Tmp2);
        client.subscribe(mqttTopic_Tmp3);
        client.subscribe(mqttTopic_Tmp4);
        client.subscribe(mqttTopic_Tmp5);
        client.subscribe(mqttTopic_Tmp6);

    } else {
      #ifdef DEBUG
      Serial.println("Falha durante a conexão.Code: ");
      Serial.println( String(client.state()).c_str());
      Serial.println("Tentando novamente em 10 s");
      #endif
      delay(10000);
    }
  }
}

char* convert(int i){
  itoa(i, buf,10);
  return buf;
}
