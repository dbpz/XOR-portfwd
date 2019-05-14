#!/bin/bash


path2xor=.
cfg=./portfwd.cfg

bl=$( ps fxa|grep  $cfg |grep portfwdXOR|wc -l)

if [[  $bl -lt 2 ]]
then
  killall -9 portfwdXOR
  sleep 1
  killall -9 portfwdXOR
  sleep 1
  $path2xor/portfwdXOR -c $cfg
fi

