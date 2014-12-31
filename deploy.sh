#!/bin/bash

make && scp -r ./qml hosSmartHome hosSmartHomeResource.qrc execute.sh root@172.20.41.106:/root/apps/hosSmartHomeDeployed/
