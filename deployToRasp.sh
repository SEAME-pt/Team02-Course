#!/bin/bash

# Check if the hostPath argument is provided
if [ -z "$1" ] || [ -z "$2" ]; then
  echo "Usage: $0 <Project dir> <Executable>"
  exit 1
fi

projectDir=$1

# Check if the provided path exists
if [ ! -d "$projectDir" ]; then
  echo "Error: The provided path ($projectDir) does not exist."
  exit 1
fi

executable=$2
piUserName=team02
piIpAddress=10.21.221.19
piPath=/home/team02
piPass=seameteam2

echo "build docker image to build app"
docker buildx build --platform linux/arm64 --load -f DockerfileDeployRasp \
    --build-arg projectDir=/$projectDir \
    -t final-app .
echo "Remove tmpapp container if it is exist"
docker rm -f tmpapp
echo "Create a tmp container to copy binary"
docker create --name tmpapp final-app
echo "Copy the binary from tmp container"
docker cp tmpapp:/home/$projectDir/$executable ./$executable
echo "Send binary to rasp over scp"
sshpass -p "$piPass" scp $executable "$piUserName"@"$piIpAddress":"$piPath"
