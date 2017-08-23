
#coding=utf-8
import os
import hashlib
import time
import ConfigParser
import urllib2
import json

#get config 
config = ConfigParser.ConfigParser()
config.readfp(open('config.ini'))
packageUrl_debug = config.get("UPDATE","packageUrl_debug")
packageUrl_release = config.get("UPDATE","packageUrl_release")

bRelease = 'false'
packageUrl = packageUrl_release
if bRelease == 'false':
	packageUrl = packageUrl_debug
	print "package:debug"
else:
	print "package:release"

remoteVersionUrl = packageUrl + config.get("UPDATE","remoteVersionUrl")
remoteManifestUrl = packageUrl + config.get("UPDATE","remoteManifestUrl")
engineVersion = config.get("UPDATE","engineVersion")

## get small version config
## 外网小版本
def http_get():
    url=remoteVersionUrl  
    response = urllib2.urlopen(url)       
    return response.read()
    
ret = http_get()
jsonVal = json.loads(ret)
version = jsonVal["version"]

## 小版本号加1
tb = version.split('.');
a = int(tb[0])
b = int(tb[1])
c = int(tb[2])

if c < 9:
	c = c + 1
elif b < 9:
	c = 0
	b = b + 1
else:
	c = 0
	b = 0
	a = a + 1

## 小版本号
version = str(a) + "." + str(b) + "." + str(c)

def getFileMd5(filename):
    if not os.path.isfile(filename):
        return
    myhash = hashlib.md5()# create a md5 object
    f = file(filename,'rb')
    while True:
        b = f.read(8096)# get file content.
        if not b :
            break
        myhash.update(b)#encrypt the file
    f.close()
    return myhash.hexdigest()

def walk(path, prefix):
    global xml
    fl = os.listdir(path) # get what we have in the dir.
    for f in fl:
        if os.path.isdir(os.path.join(path,f)): # if is a dir.
            if prefix == '':
                walk(os.path.join(path,f), f)
            else:
                walk(os.path.join(path,f), prefix + '/' + f)
        else:
            md5 = getFileMd5(os.path.join(path,f))
            xml += "\n\t\t\"%s\" : {\n\t\t\t\"md5\" : \"%s\"\n\t\t}, " % (prefix + '/' + f, md5) # output to the md5 value to a string in xml format.
if __name__ == "__main__": 
    timeStr = time.strftime("%Y%m%d%H%M%S",time.localtime(time.time()))
    if not os.path.exists(os.getcwd() + '\\manifest'):
        os.mkdir(os.getcwd() + '\\manifest')
    #generate project.manifest
    xml = '{\
    \n\t"packageUrl" : "%s",\
    \n\t"remoteVersionUrl" : "%s",\
    \n\t"remoteManifestUrl" : "%s",\
    \n\t"version" : "%s",\
    \n\t"engineVersion" : "%s",\
    \n\n\t"assets" : {' % (packageUrl, remoteVersionUrl, remoteManifestUrl, version, engineVersion)
    walk(os.getcwd() + '\\version', '')
    xml = xml[:-2]
    xml += '\n\t},\
    \n\t"searchPaths" : [\
    \n\t]\
    \n}'
    f = file("manifest\project_dev.manifest", "w+")
    f.write(xml)
    print 'generate version.manifest finish.'
    #generate version.manifest
    xml = '{\
    \n\t"packageUrl" : "%s",\
    \n\t"remoteVersionUrl" : "%s",\
    \n\t"remoteManifestUrl" : "%s",\
    \n\t"version" : "%s",\
    \n\t"engineVersion" : "%s"\n}' % (packageUrl, remoteVersionUrl, remoteManifestUrl, version, engineVersion)
    f = file("manifest\\version_dev.manifest", "w+")
    f.write(xml)
    print 'generate version.manifest finish.'