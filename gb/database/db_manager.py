import psycopg2
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT
import datetime

# Connection parameters
db_name = "iot_db"
db_user = "iot_user"
db_password = ""
db_host = "localhost"
db_port = 5432

# Step 1: Connect to the default "postgres" database
conn = psycopg2.connect(
    dbname="postgres", user=db_user, password=db_password, host=db_host, port=db_port
)
conn.set_isolation_level(ISOLATION_LEVEL_AUTOCOMMIT)  # Needed to create databases
cur = conn.cursor()

# Step 2: Check if the target database exists
cur.execute(f"SELECT 1 FROM pg_database WHERE datname = %s", (db_name,))
exists = cur.fetchone()

# Step 3: Create the database if it doesn't exist
if not exists:
    cur.execute(f"CREATE DATABASE {db_name}")
    print(f"Database '{db_name}' created.")
else:
    print(f"Database '{db_name}' already exists.")

# Close connection to "postgres"
cur.close()
conn.close()

# Step 4: Connect to the target database
conn = psycopg2.connect(
    dbname=db_name, user=db_user, password=db_password, host=db_host, port=db_port
)
cur = conn.cursor()

# Step 5: Create tables
cur.execute(
    """
    CREATE TABLE IF NOT EXISTS sensor_readings (
        id SERIAL PRIMARY KEY,
        sensor_id TEXT NOT NULL,
        timestamp TIMESTAMPTZ NOT NULL,
        water_level_cm NUMERIC NOT NULL
    );
"""
)

cur.execute(
    """
    CREATE TABLE IF NOT EXISTS alerts (
        id SERIAL PRIMARY KEY,
        sensor_id TEXT NOT NULL,
        timestamp TIMESTAMPTZ NOT NULL,
        alert_type TEXT NOT NULL,
        value NUMERIC NOT NULL,
        message TEXT
    );
"""
)

conn.commit()

# Step 6: Insert a dummy reading
cur.execute(
    """
    INSERT INTO sensor_readings (sensor_id, timestamp, water_level_cm)
    VALUES (%s, %s, %s)
""",
    ("sensor_001", datetime.datetime.now(), 145.7),
)

conn.commit()

# Step 7: Fetch and print the last 5 readings
cur.execute("SELECT * FROM sensor_readings ORDER BY timestamp DESC LIMIT 5")
rows = cur.fetchall()

print("Last sensor readings:")
for row in rows:
    print(row)

# Cleanup
cur.close()
conn.close()
