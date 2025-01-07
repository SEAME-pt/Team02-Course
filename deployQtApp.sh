#!/bin/bash

# Check if the hostPath argument is provided
if [ -z "$1" ] || [ -z "$2" ]; then
  echo "Usage: $0 <Project Path> <Repo Path>"
  exit 1
fi

projectDir=$1
repoPath=$2
hostPath=$repoPath$projectDir

# Check if the provided path exists
if [ ! -d "$hostPath" ]; then
  echo "Error: The provided path ($hostPath) does not exist."
  exit 1
fi

executable=speedometer
piUserName=team02
piIpAddress=10.21.221.64
piPath=/home/team02
piPass=seameteam2
qtPathOnTarget=/usr/local/qt6/lib/


echo "build docker image to build app"
docker build -f Dockerfile.app \
    --build-arg projectDir=$projectDir \
    --build-arg repoPath=$repoPath \
    -t final-app .
echo "Remove tmpapp container if it is exist"
docker rm -f tmpapp
echo "Create a tmp container to copy binary"
docker create --name tmpapp final-app
echo "Copy the binary from tmp container"
docker cp tmpapp:$hostPath/$executable ./$executable
echo "Send binary to rasp over scp"
sshpass -p "$piPass" scp $executable "$piUserName"@"$piIpAddress":"$piPath"
