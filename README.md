# Comedouro Inteligente para Pets com IoT

![Versão](https://img.shields.io/badge/versao-1.0-blue)
![Plataforma](https://img.shields.io/badge/plataforma-ESP32-green)
![HTTP](https://img.shields.io/badge/IoT-ThingSpeak-orange)

## Sobre o Projeto
Este projeto consiste em um comedouro inteligente que monitora automaticamente a quantidade de ração consumida por um pet em cada refeição. Utilizando sensores de peso e conectividade Wi-Fi, o sistema registra os dados em tempo real e envia para a nuvem (ThingSpeak), permitindo que o tutor acompanhe os hábitos alimentares do animal remotamente.

## Problema Solucionado
Muitos tutores não conseguem monitorar com precisão a quantidade de ração que seus pets consomem diariamente, seja por rotina corrida ou por múltiplos animais na mesma residência. O comedouro inteligente automatiza esse processo, gerando alertas e estatísticas de consumo.

## Tecnologias de IoT Aplicadas

* Sensoriamento → Célula de carga + HX711 para medição precisa do peso

* Conectividade → Wi-Fi integrado ao ESP32

* Computação em Nuvem → ThingSpeak para armazenamento e visualização dos dados

*  Horário Sincronizado → NTP para timestamps precisos

## Tecnologias Gerais Utilizadas
* Componente	Tecnologia	Finalidade
* Microcontrolador	ESP32	Processamento e conectividade Wi-Fi
* Sensor de Peso	Célula de carga + HX711	Medição da quantidade de ração
* Sensor de Presença	Botão (simulado) / PIR	Detectar quando o pet se aproxima
* Atuador	LED	Indicador visual de presença do pet
* Plataforma IoT	ThingSpeak	Dashboard e armazenamento em nuvem
* Protocolo	HTTP (GET)	Envio de dados para a nuvem
* Sincronização	NTP (Network Time Protocol)	Timestamps precisos das refeições
* Linguagem	C++ (Arduino Framework)	Programação do ESP32

## Funcionalidades
- Leitura precisa do peso da ração com célula de carga
- Detecção automática da presença do pet com o sensor de  presença
- Cálculo do consumo por refeição
- Contador diário de refeições (reseta automaticamente à meia-noite)
- Timestamp via NTP (horário de Brasília - UTC-3)
- Envio automático para dashboard ThingSpeak
- Feedback visual via LED
- Filtragem de erros (ignora consumos menores que 5g, no caso da presença e não consumo)

## Dashboard ThingSpeak
### Os dados são enviados para os seguintes campos:

Campo	Descrição
* field1	Quantidade consumida na refeição (gramas)
* field2	Total de refeições no dia atual
* field3	Status do sistema (heartbeat)\
  
Acesse o dashboard: https://thingspeak.com/channels/3360059

⚠️ Importante: O canal é público! Você pode visualizar os dados em tempo real sem necessidade de login ou configuração.


## Instruções de Uso no Wokwi

### Passo 1: Acessar o Wokwi
Clique no link abaixo para criar um novo projeto ESP32:

https://wokwi.com/projects/new/esp32

### Passo 2: Importar o Código
No editor do Wokwi, substitua todo o conteúdo da parte sketch.ino com o conteudo do arquivo sketch.ino desse repositorio.

Clique em "Save" (Ctrl+S)

### Passo 3: Configurar o Circuito (Diagram.json)
Faça o mesmo com o o diagram.json substitua o conteúdo da paarte diagram.json pelo conteudo do arquivo diagram.json desse repositorio.

### Passo 4: Executar a Simulação
Clique em "Start Simulation" (botão Play ▶)


## Como Operar (Passo a Passo no Wokwi)

| Passo | Ação no Simulador | O que acontece |
|-------|-------------------|----------------|
| 1 | Clique no **Botão Branco** (ZERAR) | Balança é tarada (0g) |
| 2 | Clique na célula de carga e escolha um valor (ex: 250) | Simula a ração no prato |
| 3 | Clique no **Botão Verde** (CAPTURAR) | Sistema salva o peso inicial |
| 4 | CLique no **Sensor PIR** | Simula pet chegando → LED acende |
| 5 | Clique na célula de carga e reduza o valor (ex: 180) | Simula o consumo de ração |
| 6 | Aguarde calcular | LED apaga → sistema calcula o consumo e envia para o ThingSpeak |

## 📈 Resultados Parciais

| Funcionalidade | Status | Observação |
|----------------|--------|-------------|
| Conexão Wi-Fi |  Funcional | Rede Wokwi-GUEST automática |
| Leitura da célula de carga (HX711) |  Funcional | Média de 10 leituras para estabilidade |
| Sensor PIR (presença) |  Funcional | Detecta aproximação do pet |
| LED indicador |  Funcional | Acende com pet presente, apaga ao sair |
| Botão ZERAR (branco) |  Funcional | Tara a balança corretamente |
| Botão CAPTURAR (verde) |  Funcional | Salva o peso inicial da refeição |
| Cálculo de consumo |  Funcional | Peso_inicial - Peso_final |
| Filtro anti-ruído |  Funcional | Ignora consumos menores que 5g |
| Contador diário de refeições |  Funcional | Reseta automaticamente à meia-noite |
| Sincronização NTP |  Funcional | Horário de Brasília (UTC-3) |
| Envio para ThingSpeak |  Funcional | HTTP GET a cada refeição |
| Dashboard público |  Disponível | [Clique aqui](https://thingspeak.com/channels/3360059) |
| Simulação completa no Wokwi |  Funcional | Testada e validada |

## Menu inicial
<img width="516" height="634" alt="Captura de tela 2026-05-23 194315" src="https://github.com/user-attachments/assets/98521bd9-23db-4562-a9dc-ba6af3cd9370" />

## Botão de zerar balança
<img width="833" height="758" alt="Captura de tela 2026-05-23 194521" src="https://github.com/user-attachments/assets/8df8deae-c835-4b49-9026-3ed1fd3576a1" />

## Botão de registrar peso (colocando ração)
<img width="697" height="848" alt="Captura de tela 2026-05-23 194746" src="https://github.com/user-attachments/assets/cb2b4252-e6b4-419a-b8d4-9a58e3b90a86" />

## Sensor de presença acionado e diminuição do peso
<img width="743" height="399" alt="Captura de tela 2026-05-23 194836" src="https://github.com/user-attachments/assets/c0629720-0324-455c-9fa0-c16a583e2aad" />
<img width="717" height="886" alt="Captura de tela 2026-05-23 194913" src="https://github.com/user-attachments/assets/461dfa6f-fcf3-4088-9a43-d6b946c688c3" />



