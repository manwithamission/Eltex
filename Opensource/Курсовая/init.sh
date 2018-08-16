#!/bin/bash

xterm -e "./server" & xterm -e "./clientsender" & xterm -e "./clientreciever"