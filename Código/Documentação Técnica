# Documentação Técnica – Sistema de Detecção de Quedas com ESP32

## Visão Geral do Sistema

Este projeto consiste em um sistema de detecção de quedas utilizando o microcontrolador **ESP32** e o sensor **MPU6050** (usando especificamente o acelerômetro). O sistema monitora continuamente os movimentos do usuário e envia um alerta via **Telegram** caso detecte uma variação brusca que possa indicar uma queda.

## Componentes Utilizados

- ESP32;
- Sensor MPU6050;
- Comunicação Wi-Fi;
- Biblioteca CTBot (integração com Telegram);
- Programação em C++;
- Simulação no Wokwi.

## Arquitetura do Código

O código foi organizado em funções específicas:

### ReadValues()

Responsável por:

- Ler os dados do acelerômetro;
- Converter valores negativos em positivos;
- Armazenar os valores dos eixos X, Y e Z.

### CheckFall(v1, v2)

Responsável por:

- Calcular a diferença entre o valor anterior e o atual;
- Verificar se a variação ultrapassa o limite definido (`maxValue`);
- Acionar envio de alerta caso detecte movimento brusco.

A condição utilizada é:

```
if (vt <= -maxValue || vt >= maxValue)
```

O valor atual do limite é:

```
int maxValue = 9;
```

### SendMessage()

Responsável por:

- Exibir aviso no Monitor Serial;
- Enviar mensagem para o Telegram utilizando o ID do usuário.

## Inicialização do Sistema (setup)

Durante a execução da função `setup()`:

1. A comunicação serial é iniciada (115200 baud).
2. O sensor MPU6050 é verificado.
3. A conexão Wi-Fi é estabelecida.
4. O token do bot do Telegram é configurado.
5. É definida a faixa do acelerômetro.

Caso o sensor não seja identificado, o sistema interrompe a execução para evitar funcionamento incorreto.

## Loop Principal

No `loop()` o sistema:

1. Realiza nova leitura dos dados do sensor.
2. Compara os valores atuais com os anteriores.
3. Executa a função `CheckFall()` para cada eixo (X, Y e Z).
4. Atualiza os valores armazenados.
5. Aguarda 200 ms antes da próxima leitura.

Esse processo ocorre continuamente enquanto o dispositivo estiver ligado.

## Lógica de Detecção

A detecção é baseada na variação brusca de aceleração entre duas leituras consecutivas. Se a diferença ultrapassar o limite configurado, o sistema interpreta como possível queda e envia alerta.

## Considerações Técnicas

O uso do ESP32 permite:

- Baixo custo;
- Conectividade Wi-Fi integrada;
- Boa capacidade de processamento;
- Integração com múltiplos sensores.

A simulação no Wokwi permitiu validar a lógica antes da implementação física.
