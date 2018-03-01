#!/bin/bash
#####################################################
# Note: terminate the status with a ;
#####################################################
#echo "Active;"
RET="$(/etc/init.d/ds_agent status | grep Active: | awk '{print $2}')"
LE=";"
echo $RET$LE

