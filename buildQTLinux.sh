#!/bin/bash

# Update package list and install prerequisites
apt-get update
apt-get install -y ca-certificates curl gnupg

# Set up the Docker repository
install -m 0755 -d /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | tee /etc/apt/keyrings/docker.asc > /dev/null
chmod a+r /etc/apt/keyrings/docker.asc

echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
  tee /etc/apt/sources.list.d/docker.list > /dev/null

# Update package list again and install Docker
apt-get update
apt-get install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# Add the current user to the docker group
usermod -aG docker ${SUDO_USER}

# Enable Docker service
systemctl enable docker
systemctl start docker

# Build the Docker image
docker build -t qtcrossbuild .

echo "Docker installation and image 