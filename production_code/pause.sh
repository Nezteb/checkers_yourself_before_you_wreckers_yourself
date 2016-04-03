#!/bin/bash

kill -STOP $(ps aux | grep './main.o' | head -1 | awk '{print $2}')   