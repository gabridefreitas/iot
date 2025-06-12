import json
import paho.mqtt.subscribe as subscribe
import paho.mqtt.publish as publish
import psycopg2
from datetime import datetime

# MQTT Configuration
SERVER = "broker.emqx.io"
SUB_TOPIC = "water_level"
PUB_TOPIC = "sensor_delay"

# Database Configuration
DB_CONFIG = {
    "host": "localhost",
    "port": 5432,
    "dbname": "iot_db",
    "user": "iot_user",
    "password": "",
}

STATUS = {
    "OK": 0,
    "LOW": 150,
    "CRITICALLY": 50,
}

global current_status
current_status = STATUS["OK"]


def connect_db():
    return psycopg2.connect(**DB_CONFIG)


def validate(payload):
    global current_status

    print("Current Status", current_status)

    if payload["distance"] > STATUS["LOW"] and current_status != STATUS["OK"]:
        current_status = STATUS["OK"]
        publish.single(PUB_TOPIC, "3000", hostname=SERVER)
        return

    if (
        payload["distance"] > STATUS["CRITICALLY"]
        and payload["distance"] < STATUS["LOW"]
        and current_status != STATUS["LOW"]
    ):
        current_status = STATUS["LOW"]
        publish.single(PUB_TOPIC, "2000", hostname=SERVER)
        return

    if (
        payload["distance"] < STATUS["CRITICALLY"]
        and current_status != STATUS["CRITICALLY"]
    ):
        current_status = STATUS["CRITICALLY"]
        publish.single(PUB_TOPIC, "1000", hostname=SERVER)

        conn = connect_db()
        cursor = conn.cursor()

        cursor.execute(
            """
            INSERT INTO alerts (sensor_id, timestamp, alert_type, value, message)
            VALUES (%s, %s, %s, %s, %s)
            """,
            (
                payload["id"],
                datetime.now(),
                "CRITICALLY",
                payload["distance"],
                "Nível da água crítico!",
            ),
        )

        conn.commit()


def save_to_database(message):
    try:
        conn = connect_db()
        cursor = conn.cursor()

        timestamp = datetime.now()

        json_string = message.payload.decode()

        print(f"📥 Received message at {timestamp}: {json_string}")

        payload = json.loads(json_string)

        print(f"📊 Payload: {payload}")

        cursor.execute(
            """
            INSERT INTO sensor_readings (sensor_id, timestamp, water_level_cm)
            VALUES (%s, %s, %s)
            """,
            (payload["id"], timestamp, payload["distance"]),
        )

        conn.commit()
        print(f"✅ Saved to DB: {timestamp}, {payload["id"]}, {payload["distance"]} cm")

        validate(payload)

    except Exception as e:
        print(f"❌ Error saving to DB: {e}")

    finally:
        if cursor:
            cursor.close()
        if conn:
            conn.close()


def on_message(_client, userdata, message):
    userdata["message_count"] += 1

    print("\n📡 New Message:")
    print("Message Count:", userdata["message_count"])
    print(f"Topic: {message.topic}, Message: {message}")

    save_to_database(message)


def main():
    print("\n🚀 Backend initialized and listening for MQTT messages...")

    subscribe.callback(
        on_message, SUB_TOPIC, hostname=SERVER, userdata={"message_count": 0}
    )


if __name__ == "__main__":
    main()
