# -*- coding: utf-8 -*-
"""
Created on Thu Sep 29 21:06:27 2022

@author: lucio
"""

import paho.mqtt.client as mqtt
import json
import base64


# gives connection message
def on_connect(mqttc, _mosq, _obj, rc):
    print("Connected with result code:" + str(rc))
    # subscribe for device
    mqttc.subscribe(
        "application/c1e60d40-66ea-40e5-81e2-2f4321c0a7fc/device/6cfd83fee3e12c05/event/up"
    )


# gives message from device
def on_message(_mqttc, _obj, msg):
    try:
        x = json.loads(msg.payload.decode("utf-8"))

        payload_raw = x["data"]
        payload_bytes = base64.b64decode(payload_raw)

        print("Data received:", payload_bytes)

    except Exception as e:
        print(e)
        pass


mqttc = mqtt.Client()
# Assign event callbacks
mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.connect("172.233.13.219", 1883, 60)


# and listen to server
run = True
while run:
    mqttc.loop()
