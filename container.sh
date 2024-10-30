#!/bin/bash

if [ "$1" == "start" ]; then
	docker start iot
    xhost + && docker exec -it --user $(id -u):$(id -g) iot bash
elif [ "$1" == "stop" ]; then
    docker stop iot
elif [ "$1" == "build" ]; then
    docker build -t iot_image docker/
elif [ "$1" == "create" ]; then
    if [ -z "$2" ]; then
        echo "Please provide the MySQL root password as the second argument."
        exit 1
    fi
    docker run -d --name iot --net=host --privileged --env="QT_X11_NO_MITSHM=1" -e DISPLAY=$DISPLAY -eQT_DEBUG_PLUGINS=1 -v /tmp/.X11-unix:/tmp/.X11-unix --device /dev/video0:/dev/video0 -e MYSQL_ROOT_PASSWORD=$2 -v $(pwd):/workspace iot_image:latest
elif [ "$1" == "clean" ]; then
    docker stop iot
    docker rm iot
else
    echo "Invalid option. Please use start, stop, build or create"
fi