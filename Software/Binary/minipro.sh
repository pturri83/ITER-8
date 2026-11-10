#!/bin/bash


# Bash script to upload a binary file to an AT28C256 ROM chip through a XGecu T48 programmer.
#
# Instructions:
# - Change the value of 'BIN_FILE' with the name of the binary file, without the '.bin' extension.
# - Start this script from terminal by typing './minipro.sh'.


BIN_FILE='test_via_lcd'

minipro -p 'AT28C256' -w $BIN_FILE.bin
