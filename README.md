# IoT

## Atividade 1

### Grupo

- Christian Plentz
- Gabriel de Freitas
- Luís Dorr

### Proposta

Você foi designado para criar uma solução IoT que resolva um problema específico em um contexto urbano. O objetivo é desenvolver um dispositivo que monitore uma variável e utilize um cliente em Python ou Java para processar e responder a esses dados.

### Requisitos do Exercício

1. Protocolo de Comunicação: A solução deve utilizar o protocolo MQTT para comunicação entre o dispositivo e um servidor de dados.
1. Dispositivo: Utilize a ferramenta Wokwi para projetar e simular seu dispositivo IoT. Este dispositivo deve incluir:
   - Um Sensor: Escolha um sensor apropriado que será o foco do monitoramento.
   - Um Atuação: O dispositivo deve ser capaz de executar uma ação baseada nos dados recebidos.
1. Cliente em Python ou Java:
   - Desenvolva um cliente que se conecte ao broker MQTT para receber e processar dados do sensor.
   - O cliente deve ser capaz de realizar operações de leitura, processamento e armazenamento dos dados recebidos.
1. Ação Baseada em Dados: Implemente uma lógica que, ao detectar condições específicas nos dados recebidos, dispare a ação previamente definida.
1. Documentação: Elabore um relatório que descreva o contexto do problema que será resolvido, a arquitetura da sua solução, o funcionamento do dispositivo e a implementação do cliente em Python ou Java. Inclua imagens da simulação feita no Wokwi.

### Possíveis Casos a Serem Implementados

#### Sistema de Monitoramento da Qualidade do Ar

- Sensor: Sensor de gases (ex: MQ-135) para medir poluentes.
- Ação: Enviar alertas quando os níveis de poluição excederem um limite seguro.

#### Sistema de Irrigação Inteligente

- Sensor: Sensor de umidade do solo.
- Ação: Ativar a irrigação automaticamente quando o solo estiver seco.

#### Sistema de Iluminação Pública Automatizado

- Sensor: Sensor de luminosidade.
- Ação: Ajustar a intensidade da luz pública com base na luminosidade externa.

#### Gestão de Resíduos Sólidos

- Sensor: Sensor de nível em contêineres de lixo.
- Ação: Notificar serviços de coleta quando o nível de resíduos estiver alto.

#### Monitoramento de Saúde Pública

- Sensor: Monitor de temperatura e umidade em locais públicos.
- Ação: Alertar as autoridades sobre condições ambientais que possam afetar a saúde pública.

Esse exercício visa proporcionar uma experiência prática no desenvolvimento de soluções IoT, abordando problemas relevantes nas cidades modernas. Boa sorte!

### Caso Aplicado

Implementação de um sensor de luminosidade via WOKWI que publica a leitura em um tópico MQTT.

O cliente python lê o valor publicado pelo sensor e publica em outro tópico um comando para ligar ou desligar um led com base na leitura de luminosade do sensor.

O sensor assina o tópico de comando e liga ou desliga o led de acordo com o comando recebido.

Link Wokwi: <https://wokwi.com/projects/426672379174481921>
