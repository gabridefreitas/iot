import paho.mqtt.subscribe as subscribe
import paho.mqtt.publish as publish

SERVER = "broker.emqx.io"
SUB_TOPIC = "praga-lum-checker"
PUB_TOPIC = "praga-on_off_int_level"

LOW_LIGHT_LIMIT = 400

def on_message_print(client, userdata, message):
    userdata["message_count"] += 1

    print("\nAtualização:")
    print("\n\t%s %s" % (message.topic, message.payload))
    
    if message == "turn_off":
        client.disconnect()

    valueAsNumber = int(message.payload)

    print("\tValor recebido: ", valueAsNumber)
    print("\tContador de mensagens: ", userdata["message_count"])

    if (valueAsNumber < LOW_LIGHT_LIMIT):
        print("\tLuz baixa, ligando o LED")
        publish.single(PUB_TOPIC, "on", hostname=SERVER)
    else:
        print("\tLuz alta, desligando o LED")
        publish.single(PUB_TOPIC, "off", hostname=SERVER)
    

def main():
    subscribe.callback(on_message_print, SUB_TOPIC, hostname=SERVER, userdata={"message_count": 0})


main()