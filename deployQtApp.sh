#!/bin/bash

# Check if the hostPath argument is provided
if [ -z "$1" ] || [ -z "$2" ]; then
  echo "Usage: $0 <Repo Path> <Project Path>"
  exit 1
fi

projectDir=$2
repoPath=$1
hostPath=$repoPath/$projectDir

# Check if the provided path exists
if [ ! -d "$hostPath" ]; then
  echo "Error: The provided path ($hostPath) does not exist."
  exit 1
fi

executableCluster=speedometer
executableZenoh=zenoh
piUserName=team02
piIpAddress=10.21.221.64
piPath=/home/team02
piPass=seameteam2
qtPathOnTarget=/usr/local/qt6/lib/


echo "build docker image to build app"
docker build -f Dockerfile.app \
    --build-arg projectDir=/$projectDir \
    --build-arg repoPath=$repoPath \
    -t final-app .
echo "Remove tmpapp container if it is exist"
docker rm -f tmpapp
echo "Create a tmp container to copy binary"
docker create --name tmpapp final-app
echo "Copy the binary from tmp container"
docker cp tmpapp:$hostPath/$executableCluster ./$executableCluster
docker cp tmpapp:$hostPath/$executableZenoh ./$executableZenoh
echo "Send binary to rasp over scp"
sshpass -p "$piPass" scp $executableCluster "$piUserName"@"$piIpAddress":"$piPath"
sshpass -p "$piPass" scp $executableZenoh "$piUserName"@"$piIpAddress":"$piPath"
