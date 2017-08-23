
#coding=utf-8
import os
import hashlib
import time
import ConfigParser
import urllib2
import json

#get config 
remoteVersionUrl = "http://192.168.0.111/update_resource/version_dev.manifest"

## get small version config
## 外网小版本
def http_get():
    url=remoteVersionUrl  
    response = urllib2.urlopen(url)       
    return response.read()
    
ret = http_get()
jsonVal = json.loads(ret)
version = jsonVal["version"]

print version

## 小版本号加1
tb = version.split('.');
a = int(tb[0])
b = int(tb[1])
c = int(tb[2])

if c < 999:
	c = c + 1
elif b < 999:
	c = 0
	b = b + 1
else:
	c = 0
	b = 0
	a = a + 1
	
version = str(a) + "." + str(b) + "." + str(c)

print version

