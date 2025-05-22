# IoT

## Atividade 2

### Grupo

- Christian Plentz
- Gabriel de Freitas
- Luís Dorr

### Proposta

Implementação de uma Rede IoT Real (LoRaWAN)

#### Etapa 1 - Gateway Canais 0-7  (Introduction - ChirpStack open-source LoRaWAN® Network Server documentation)

#### Etapa 2 - Servidor de Rede e Aplicação ( Debian / Ubuntu - ChirpStack open-source LoRaWAN® Network Server documentation)

#### Etapa 3 - Aplicação Cliente para consumir dados (Integrations - ChirpStack open-source LoRaWAN® Network Server documentation)

#### Etapa 4 - Dispositivos IoT  LoRAWAN  ABP (Heltec ESP32+LoRa Series Quick Start — esp32 latest documentation)

#### Etapa 5 - Dispositivos IoT  LoRAWAN OTAA (Heltec ESP32+LoRa Series Quick Start — esp32 latest documentation)

O objetivo é fazer o estudo de como implementar uma rede de IoT LoraWAN

O que deve ser entregue:  A documentação da rede (howto), explicando o que é necessário para implementar uma rede com esse padrão. Essa documentação servirá como guia para implantarmos a rede real. Quanto mais detalhada melhor.

Apresentar uma proposta dos mecanismos de segurança que podem ser configurados  no dispositivo, servidor de rede, servidor de aplicação e aplicação cliente.

### Caso Aplicado

Criar uma VPS Linux

#### Adicionar novo usuário

##### Passo 1 — Fazendo login em seu servidor

Acesse seu servidor via SSH como usuário root:

```bash
ssh root@your_server_ip_address
```

##### Passo 2 — Adicionando um novo usuário ao sistema

Use o comando adduser para adicionar um novo usuário ao seu sistema:

```bash
adduser iotuser
```

O iotuser deve ser substituído pelo nome de usuário que deseja criar. Será solicitado que você crie e verifique uma senha para o usuário:

```bash
Enter new UNIX password:
Retype new UNIX password:
passwd: password updated successfully
```

Em seguida, será solicitado que você preencha algumas informações sobre o novo usuário. Aceite as configurações padrão e deixe todas essas informações em branco:

##### Passo 3 — Adicionando o usuário ao grupo sudo

Use o comando usermod para adicionar o usuário ao grupo sudo:

```bash
usermod -aG sudo iotuser
```

Novamente, o sammy deve ser substituído pelo nome de usuário que acabou de adicionar. Por padrão, no Ubuntu, todos os membros do grupo sudo têm privilégios totais de sudo.

##### Passo 4 — Desabilitar Login SSH de Root

O Secure Shell (SSH) permite que os usuários acessem remotamente uma estação de trabalho Linux utilizando um par de usuário e senha válidos ou chaves SSH. Desabilitar o login de root via SSH encerra todas as solicitações de conexão do usuário root e aceita apenas conexões de usuários não-root. Siga os passos abaixo para desabilitar o login root via SSH em sua estação de trabalho.

Abra o arquivo /etc/ssh/sshd_config usando um editor de texto, como o nano.

```bash
sudo nano /etc/ssh/sshd_config
```

Encontre a diretiva PermitRootLogin e altere seu valor de yes para no para desabilitar o login do usuário root via SSH.

```bash
PermitRootLogin no
```

Salve e feche o arquivo.

Reinicie o serviço SSH para aplicar as alterações na configuração.

```bash
sudo systemctl restart ssh
```

#### Instalar ChirpStack

##### Instalar requisitos

O seguinte comando instala os requisitos do ChirpStack usando o gerenciador de pacotes apt:

```bash
sudo apt update

sudo apt install \
    mosquitto \
    mosquitto-clients \
    redis-server \
    redis-tools \
    postgresql

```

Se você deseja poder acessar o broker de outras máquinas, precisará editar explicitamente os arquivos de configuração para adicionar um novo listener que vincule ao endereço IP externo (ou 0.0.0.0) ou adicionar uma entrada de bind para o listener padrão.

No arquivo mosquitto.conf adicione as seguintes linhas

```bash
listener 1883 0.0.0.0
allow_anonymous true
```

reinicie o Mosquitto:

```bash
sudo service mosquitto restart
```

O ChirpStack fornece binários pré-compilados como pacotes Debian (.deb). Para ativar este repositório, execute os seguintes comandos:

```bash
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 1CE2AFD36DBCCA00

sudo echo "deb https://artifacts.chirpstack.io/packages/4.x/deb stable main" | sudo tee /etc/apt/sources.list.d/chirpstack_4.list

sudo apt update
```

Configuração do PostgreSQL
Para entrar no utilitário de linha de comando do PostgreSQL:

```bash
sudo -u postgres psql
```

Dentro deste prompt, execute as seguintes consultas para configurar o banco de dados ChirpStack. É recomendado mudar os nomes de usuário e senhas.

```bash
-- criar papel para autenticação
create role chirpstack with login password 'chirpstack';

-- criar banco de dados
create database chirpstack with owner chirpstack;

-- mudar para o banco de dados chirpstack
\c chirpstack

-- criar extensão pg_trgm
create extension pg_trgm;

-- sair do psql
\q
```

#### Instalar o servidor de rede

```bash
sudo apt install chirpstack

sudo cd /etc/chirpstack
```

editar arquivo chirpstack.toml

```bash
# ChirpStack Configuration File

# Configurações gerais
log_level = "info"

# Endereço do servidor MQTT
mqtt_server = "tcp://localhost:1883"
mqtt_username = ""
mqtt_password = ""

# Configurações do banco de dados PostgreSQL
postgresql_dsn = "postgres://chirpstack:chirpstack@localhost:5432/chirpstack?sslmode=disable"

# Estado do ChirpStack
enabled_regions = [
   # "as923",
   # "as923_2",
   # "as923_3",
   # "as923_4",
   "au915_0",
   # "cn470_10",
   # "cn779",
   # "eu433",
   # "eu868",
   # "in865",
   # "ism2400",
   # "kr920",
   # "ru864",
   # "us915_0",
   # "us915_1",
]
```

#### Instalar ChirpStack OS no Gateway

Todo:

#### Codificar o firmware dos dispositivos LoRaWAN

Todo:

#### Adicionar os dispositivos na redes

Todo:

#### Integrar a aplicação cliente

Todo:
