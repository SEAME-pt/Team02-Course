#!/bin/bash

# Initialize submodules
git submodule init

# Synchronize submodules
git submodule sync

# Update submodules to the latest commit on the remote branch
git submodule update --remote --merge