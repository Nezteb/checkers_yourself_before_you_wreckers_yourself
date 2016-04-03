#!/bin/bash

kill -9 $(ps aux | grep './main.o' | head -1 | awk '{print $2}')   
