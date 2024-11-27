CREATE DATABASE SecureSense;

USE SecureSense;
CREATE TABLE HOUSE (
house_id INT AUTO_INCREMENT PRIMARY KEY,
direction_ip VARCHAR(15),
direction VARCHAR(255)
);

CREATE TABLE ROOM (
room_id INT AUTO_INCREMENT PRIMARY KEY,
num_windows INT,
num_doors INT
);

CREATE TABLE PERSON (
room_id INT,
name VARCHAR(100),
PRIMARY KEY (room_id, name),
FOREIGN KEY (room_id) REFERENCES ROOM(room_id)
);

CREATE TABLE SENSOR(
type_ VARCHAR(50),
unit VARCHAR(10),
sensor_id INT AUTO_INCREMENT PRIMARY KEY,
room_id INT,
FOREIGN KEY (room_id) REFERENCES ROOM(room_id)
);

CREATE TABLE LOG_SENSOR(
log_id INT AUTO_INCREMENT,
date_ DATETIME,
measure INT,
sensor_id INT,
PRIMARY KEY (log_id),
FOREIGN KEY (sensor_id) REFERENCES SENSOR(sensor_id)
);

CREATE TABLE ACTUATOR(
actuator_id INT AUTO_INCREMENT PRIMARY KEY,
condition_ INT,
sensor_id INT,
FOREIGN KEY (sensor_id) REFERENCES SENSOR(sensor_id)
);

CREATE TABLE LOG_ACTUATOR(
log_id INT AUTO_INCREMENT PRIMARY KEY,
date_ DATETIME,
active_ BOOL,
actuator_id INT,
FOREIGN KEY (actuator_id) REFERENCES ACTUATOR(actuator_id)
);

INSERT INTO HOUSE (direction_ip, direction) VALUES
('192.168.1.1', '123 Main St'),
('192.168.1.2', '456 Elm St'),
('192.168.1.3', '789 Oak St');

INSERT INTO ROOM (num_windows, num_doors) VALUES
(2, 1),
(3, 2),
(1, 1);

SET FOREIGN_KEY_CHECKS=0;
INSERT INTO PERSON (room_id, name) VALUES
(1, 'Alice'),
(1, 'Bob'),
(2, 'Carlos'),
(3, 'Diana');

USE SecureSense;
INSERT INTO SENSOR (type_, unit, room_id) VALUES
('Temperature', 'Celsius', 1),
('Humidity', '%', 1),
('Light', 'Lux', 2),
('CO2', 'ppm', 3);

INSERT INTO LOG_SENSOR (date_, measure, sensor_id) VALUES
('2024-11-01 12:00:00', 22, 1),
('2024-11-01 12:05:00', 45, 2),
('2024-11-01 12:10:00', 300, 3),
('2024-11-01 12:15:00', 400, 4);

INSERT INTO ACTUATOR (condition_, sensor_id) VALUES
(25, 1),
(60, 2),
(200, 3),
(350, 4);

INSERT INTO LOG_ACTUATOR (date_, active_, actuator_id) VALUES
('2024-11-01 12:30:00', TRUE, 1),
('2024-11-01 12:35:00', FALSE, 2),
('2024-11-01 12:40:00', TRUE, 3),
('2024-11-01 12:45:00', FALSE, 4);



