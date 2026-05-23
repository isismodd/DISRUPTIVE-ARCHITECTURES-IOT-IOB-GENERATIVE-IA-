#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"
#include "HX711.h"

// ========== CONFIGURAÇÕES THINGSPEAK ==========
const String thingSpeakApiKey = "MMSE4XL1J7MNEBDH";
const String thingSpeakUrl = "http://api.thingspeak.com/update";

// ========== CONFIGURAÇÕES WI-FI ==========
const char* ssid     = "Wokwi-GUEST";
const char* password = "";

// ========== CONFIGURAÇÕES NTP ==========
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

// ========== PINAGEM DOS COMPONENTES ==========
int ledPin = 14;           // LED indica pet presente
int botaoPetPin = 17;      // Botão que simula a PRESENÇA do pet
int botaoZerarPin = 16;    // Botão para zerar/tarar a balança
int botaoCapturarPin = 18; // Botão para capturar peso inicial
const int DOUT_PIN = 22;
const int SCK_PIN = 23;

// ========== VARIÁVEIS ==========
bool petPresente = false;
bool pesoInicialCapturado = false;

// Balança
HX711 balanca;
float fatorCalibracao = 0.42;
float pesoInicial = 0.0;
float pesoFinal = 0.0;
float quantidadeComida = 0.0;

// Contadores
int refeicoesHoje = 0;
int ultimoDiaAno = -1;  // Armazena o dia do ano (0-365) para detectar mudança de dia
unsigned long ultimoEnvioSaude = 0;
const unsigned long intervaloEnvioSaude = 3600000;

// ========== FUNÇÃO PARA ENVIAR AO THINGSPEAK ==========
void enviarParaThingSpeak(float consumo, int totalRefeicoes) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = thingSpeakUrl + "?api_key=" + thingSpeakApiKey + 
                 "&field1=" + String(consumo) +
                 "&field2=" + String(totalRefeicoes);
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode == 200) {
      Serial.println("✅ Dados enviados ao ThingSpeak!");
      Serial.print("   → Consumo: "); Serial.print(consumo); Serial.println("g");
      Serial.print("   → Refeições hoje: "); Serial.println(totalRefeicoes);
    } else {
      Serial.print(" Erro HTTP: "); Serial.println(httpCode);
    }
    http.end();
  }
}

// ========== FUNÇÃO PARA ZERAR A BALANÇA ==========
void zerarBalanca() {
  Serial.println("\n Zerando a balança...");
  balanca.tare();
  delay(500);
  Serial.println("✅ Balança zerada! Coloque o prato vazio e pressione o botão novamente se necessário.");
}

// ========== FUNÇÃO PARA CAPTURAR PESO INICIAL ==========
void capturarPesoInicial() {
  Serial.println("\n Capturando peso inicial...");
  
  // Faz múltiplas leituras para estabilizar
  float soma = 0;
  for(int i = 0; i < 10; i++) {
    soma += balanca.get_units(3);
    delay(50);
  }
  pesoInicial = soma / 10.0;
  
  if(pesoInicial < 0) pesoInicial = 0;
  
  Serial.print("✅ Peso inicial capturado: ");
  Serial.print(pesoInicial);
  Serial.println("g");
  Serial.println(" Agora pressione o BOTÃO DO PET para simular a chegada!");
  
  pesoInicialCapturado = true;
}

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  
  // Configura os pinos
  pinMode(ledPin, OUTPUT);
  pinMode(botaoPetPin, INPUT_PULLUP);      // Pull-up interno
  pinMode(botaoZerarPin, INPUT_PULLUP);    // Pull-up interno
  pinMode(botaoCapturarPin, INPUT_PULLUP); // Pull-up interno
  
  // Inicializa a balança
  Serial.println("Inicializando balança...");
  balanca.begin(DOUT_PIN, SCK_PIN);
  balanca.set_scale(fatorCalibracao);
  balanca.tare();
  Serial.println("✅ Balança pronta!");
  
  // Conecta Wi-Fi
  Serial.print("Conectando Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi Conectado!");
  
  // Configura horário
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n✅ Horário sincronizado!");
  
  // Inicializa o dia atual
  getLocalTime(&timeinfo);
  ultimoDiaAno = timeinfo.tm_yday;
  
  // Menu de instruções
  Serial.println("\n╔════════════════════════════════════════════════════╗");
  Serial.println("║          COMEDOURO INTELIGENTE - MANUAL          ║");
  Serial.println("╚════════════════════════════════════════════════════╝");
  Serial.println("\n CONTROLES:");
  Serial.println("   🔘 BOTÃO ZERAR    (pino 16) → Zera a balança");
  Serial.println("   🔘 BOTÃO CAPTURAR (pino 18) → Captura peso INICIAL");
  Serial.println("   🔘 BOTÃO PET      (pino 17) → Simula presença do pet");
  Serial.println("\n PASSO A PASSO CORRETO:");
  Serial.println("   1. Pressione ZERAR (balança com prato vazio)");
  Serial.println("   2. Coloque a ração no prato");
  Serial.println("   3. Pressione CAPTURAR (guarda o peso inicial)");
  Serial.println("   4. Pressione PET (simula pet chegando - LED acende)");
  Serial.println("   5. Retire um pouco de ração (simula que o pet comeu)");
  Serial.println("   6. Solte PET (simula pet saindo - LED apaga)");
  Serial.println("   7. Sistema calcula consumo automaticamente!");
  Serial.println("\n======================================================\n");
}

// ========== LOOP PRINCIPAL ==========
void loop() {
  // Lê os botões (LOW = pressionado)
  bool botaoZerar = (digitalRead(botaoZerarPin) == LOW);
  bool botaoCapturar = (digitalRead(botaoCapturarPin) == LOW);
  bool botaoPet = (digitalRead(botaoPetPin) == LOW);
  
  // ========== 1. BOTÃO ZERAR ==========
  if (botaoZerar) {
    zerarBalanca();
    pesoInicialCapturado = false;
    petPresente = false;
    digitalWrite(ledPin, LOW);
    delay(300);
  }
  
  // ========== 2. BOTÃO CAPTURAR PESO INICIAL ==========
  if (botaoCapturar && !petPresente) {
    capturarPesoInicial();
    delay(300);
  }
  
  // ========== 3. PET DETECTADO (botão pressionado) ==========
  if (botaoPet && !petPresente && pesoInicialCapturado) {
    petPresente = true;
    digitalWrite(ledPin, HIGH);
    
    Serial.print("   Peso inicial registrado: ");
    Serial.print(pesoInicial);
    Serial.println("g");
    
    delay(300);
  }
  
  // ========== 4. PET SAIU (botão solto) ==========
  if (!botaoPet && petPresente) {
    digitalWrite(ledPin, LOW);
    
    Serial.println("\n PET DETECTADO no comedouro!");
    Serial.println("   Aguardando 5 segundos para leitura final...\n");
    delay(5000);
    
    // Captura peso final (média de 10 leituras)
    float soma = 0;
    for(int i = 0; i < 10; i++) {
      soma += balanca.get_units(3);
      delay(50);
    }
    pesoFinal = soma / 10.0;
    if(pesoFinal < 0) pesoFinal = 0;
    
    Serial.print("   Peso final: ");
    Serial.print(pesoFinal);
    Serial.println("g");
    
    // Calcula consumo
    quantidadeComida = pesoInicial - pesoFinal;
    if(quantidadeComida < 0) quantidadeComida = 0;
    
    Serial.print("   🍽️ Consumo calculado: ");
    Serial.print(quantidadeComida);
    Serial.println("g");
    
    // Registra se consumiu mais de 5g
    if (quantidadeComida > 5.0) {
      struct tm timeinfo;
      getLocalTime(&timeinfo);
      
      // Verifica se mudou o DIA DO ANO (reseta contador apenas quando realmente mudar o dia)
      if (timeinfo.tm_yday != ultimoDiaAno) {
        refeicoesHoje = 0;
        ultimoDiaAno = timeinfo.tm_yday;
        Serial.println("\n NOVO DIA DETECTADO! Contador de refeições resetado.\n");
      }
      
      refeicoesHoje++;
      
      // Exibe relatório
      Serial.println("\n╔══════════════════════════════════════╗");
      Serial.println("║       REFEIÇÃO REGISTRADA!          ║");
      Serial.println("╚══════════════════════════════════════╝");
      Serial.printf("🕒 Horário: %02d:%02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
      Serial.printf(" Data: %02d/%02d/%04d\n", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
      Serial.print("  Peso antes: "); Serial.print(pesoInicial); Serial.println("g");
      Serial.print("  Peso depois: "); Serial.print(pesoFinal); Serial.println("g");
      Serial.print("  Consumo: "); Serial.print(quantidadeComida); Serial.println("g");
      Serial.print("  Refeições hoje: "); Serial.println(refeicoesHoje);
      Serial.println("═══════════════════════════════════════\n");
      
      // Envia para ThingSpeak
      enviarParaThingSpeak(quantidadeComida, refeicoesHoje);
      
    } else {
      Serial.println("\n⚠️ Consumo insuficiente (menos de 5g) - refeição ignorada.\n");
    }
    
    // Reseta estado
    petPresente = false;
    pesoInicialCapturado = false;
    delay(300);
  }
  
  // Mostra peso atual a cada 10 segundos (para debug)
 // static unsigned long ultimoDebug = 0;
 // if (millis() - ultimoDebug > 10000 && !petPresente) {
 //   float pesoAtual = balanca.get_units(5);
 //   Serial.print("🔍 [DEBUG] Peso atual na balança: ");
 //   Serial.print(pesoAtual);
//    Serial.println("g");
//    ultimoDebug = millis();
 // }
  
  delay(50);
}