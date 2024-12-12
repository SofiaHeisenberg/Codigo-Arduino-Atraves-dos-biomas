#include <WiFi.h>
#include <HX711.h>

//
//#define THINGER_SERIAL_DEBUG
//#include <ThingerESP32.h>
//ThingerESP32 thing("sofiaheisenberg", "cell", "cy_+Jkc&TwgiM#Rf"); //Conexão com o device

WiFiServer server(80);
//#define WIFI_SSID "/D "augustnine"
//#define WIFI_SSID "Vingado sete vezes"
//#define WIFI_PASSWORD "123comer"
//#define WIFI_SSID "Diogo"
//#define WIFI_PASSWORD "oimeuchapa"
#define WIFI_SSID "OP"
#define WIFI_PASSWORD "onepiece"


// HX711_1 circuit wiring
const int LOADCELL_DOUT_PIN1 = 5;
const int LOADCELL_SCK_PIN1 = 18;
HX711 scale1;
// HX711_2 circuit wiring
const int LOADCELL_DOUT_PIN2 = 23;
const int LOADCELL_SCK_PIN2 = 22;
HX711 scale2;

//HX711_3 circuit wiring
const int LOADCELL_DOUT_PIN3 = 33;
const int LOADCELL_SCK_PIN3 = 32;
HX711 scale3;
  
int esq = 0;
int dir = 0;
int pulo = 0;
int andando = 0;
int k = 0;
String txt = "";
long reading1;
long reading2;
long reading3;
int valorInt;
int estado;
unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
int checagem[3] = {0,0,0};
bool check = false;
/*
IPAddress staticIP(192, 168, 43, 186);
IPAddress gateway(192, 168, 43, 151);   // Replace this with your gateway IP Addess
IPAddress subnet(255, 255, 255, 0);  // Replace this with your Subnet Mask
IPAddress dns(192, 168, 43, 151);   // Replace this with your DNS
*/
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  /*if (WiFi.config(staticIP, gateway, subnet, dns) == false) {
    Serial.println("Configuration failed.");
  }*/

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  server.begin(); //inicia o servidor
  scale1.begin(LOADCELL_DOUT_PIN1, LOADCELL_SCK_PIN1);
  scale2.begin(LOADCELL_DOUT_PIN2, LOADCELL_SCK_PIN2);
  scale3.begin(LOADCELL_DOUT_PIN3, LOADCELL_SCK_PIN3);

//  thing.add_wifi("Heisenberg ", "SofiaAxl");
//  thing["GPIO_2"] << digitalPin(2);
//  thing["dados"] >> [](pson & out)
//  {
//    out ["CELULA1"] =  scale1.read();
//    out ["CELULA2"] =  scale2.read();
//    out ["CELULA3"] = scale3.read();
//  };

}

void loop()
{
 /*reading1 = scale1.read();
  Serial.print("HX711_1 reading: ");
  Serial.println(reading1);
 

  reading2 = scale2.read();
  Serial.print("HX711_2 reading: ");
  Serial.println(reading2);

  reading3 = scale3.read();
  Serial.print("HX711_3 reading: ");
  Serial.println(reading3);*/

  WiFiClient client = server.available();
 // Serial.println(server.available());
  if (client)
  {
    client.print("Godot esta conectado a ESP32");
    Serial.println("ESP32 esta conectado ao godot");
    while (client.connected()) //Envia os valores para o Godot(Client)
    {
      delay(200);
      if (scale1.is_ready() or scale2.is_ready() or scale3.is_ready())
      {
        reading1 = scale1.read();
        //Serial.print("HX711_1 reading: ");
        //Serial.println(reading1);
 

        reading2 = scale2.read();
        //Serial.print("HX711_2 reading: ");
        //Serial.println(reading2);

        reading3 = scale3.read();
        //Serial.print("HX711_3 reading: ");
        //Serial.println(reading3);

        if (reading1 > 1120000 and dir == 0 ) //PÉ DIREITO
        {
          dir = 1; //PISOU EM CIMA

        }
        if (reading1 < 1120000 and dir == 1) {
          dir = 0; //SAIU DE CIMA

        }
        

        if (reading2 < 670000 and esq == 0) //PÉ ESQUERDO
        {
          esq = 1;

        }
        if (reading2 > 670000 and esq == 1) {
          esq = 0;

        }

        if (reading3 > 270000 and pulo == 0 ) //pulo
        {
          pulo = 1;

        }
        if (reading3 < 270000 and pulo == 1) {
          pulo = 0;
        }

//        inicia envio de sinais
        int leitura[3] = {dir,esq,pulo};
        for(int i = 0; i < 3; i++){
          
            Serial.println(leitura[i]);
            Serial.println(checagem[i]);
            if (leitura[i] != checagem[i]){
              check = false;
              break;
            }else{
              check = true;
            }
          }
       
        if (check == false ){
          Serial.println("LINDÃO----------");
          if (pulo == 1 and andando == 1){
            client.print("a parar\n");}
          else if (pulo == 1){
            client.print("a pular\n");
            Serial.println("......PULEI");
          }
          else if (dir ==1 and esq ==0){
            client.print("a direita\n");
            andando = 1;
            Serial.println("!!!!!!!!!DIREITA");
         
          }else if (dir ==0 and esq == 1){
            client.print("a esquerda\n");
            andando = 1;
            Serial.println("#######ESQUERDA");
          }
          else {
            client.print("a parar\n");
            andando = 0;


          }
          
          for(int i = 0; i < 3; i++){
          
            
            checagem[i] = leitura[i];
          }

  
      
      }
      }
      else
      {
        delay(500);
        Serial.println("HX711_1 or HX711_2 not found.");
      }
//      thing.handle(); //envia a informação para o THINGER.IO

    }
  
}
}