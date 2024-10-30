# TC1004B_IOT_Project

## Run and build the container
```
git clone https://github.com/GerardoFJ/TC1004B_IOT_Project.git
cd TC1004B_IOT_Project/
chmod +x container.sh
```
### To build the container
` ./container.sh build `

### To create the container
` ./container.sh create YOUR_SQL_PASSWORD`

### To start and enter to the container
` ./container.sh start `

### To stop the container
` ./container.sh stop `

### To remove the container
` ./container.sh clean `

### To set ip
  `./container set-ip YOUR-IP`


#### Setup Mysql
```
#Create database
create database nodedb;
use nodedb;

#Create table devices
create table devices(
    device_id int primary key auto_increment,
    device_name varchar(20)
);
insert into devices
(device_name)
values('Temperatura');
select * from devices;

#CReate table temperatures
create table temperatures (
    log_id int primary key auto_increment,
    log_date datetime,
    device_id int,
    foreign key (device_id)
    references devices (device_id),
    temp float

);
select * from temperatures;

#Create Function sp_get_devices
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_get_devices`()
BEGIN
    SELECT * FROM devices;
END ;;
DELIMITER ;

#Create Function sp_add_device
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_add_device`(
    IN d_device_id int,
    IN d_device_name varchar(20)
)

BEGIN
    INSERT INTO devices
    (device_id, device_name)
    VALUES (d_device_id, d_device_name);
END ;;
DELIMITER ;

#Create sp_get_temp
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_get_temp`()
BEGIN
    SELECT * FROM temperatures;
END ;;
DELIMITER ;

#Create sp_add_temp
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_add_temp`(
    IN t_log_id int,
    IN t_log_date datetime,
    IN t_device_id int,
    IN t_temp float
)
BEGIN
    INSERT INTO temperatures
    (log_id, log_date, device_id, temp)
    VALUES (t_log_id,t_log_date, t_device_id, t_temp);
END ;;
DELIMITER ;

```

#### Notes
Link to the FASTAPI:
http://YOUR_IP:5195/swagger/index.html
