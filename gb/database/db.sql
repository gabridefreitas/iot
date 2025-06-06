CREATE USER iot_user WITH PASSWORD '';
ALTER USER iot_user CREATEDB;

CREATE TABLE IF NOT EXISTS sensor_readings (
    id SERIAL PRIMARY KEY,
    sensor_id TEXT NOT NULL,
    timestamp TIMESTAMPTZ NOT NULL,
    water_level_cm NUMERIC NOT NULL
);

CREATE TABLE IF NOT EXISTS alerts (
    id SERIAL PRIMARY KEY,
    sensor_id TEXT NOT NULL,
    timestamp TIMESTAMPTZ NOT NULL,
    alert_type TEXT NOT NULL,
    value NUMERIC NOT NULL,
    message TEXT
);