#!/bin/bash
echo "user=$(whoami)" | base64 > proof
echo "hostname=$(hostname)" | base64 >> proof
