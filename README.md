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
* field3	Status do sistema (heartbeat)
Acesse o dashboard: https://thingspeak.com/channels/3360059
