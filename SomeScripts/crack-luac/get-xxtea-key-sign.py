#!/usr/bin/python

# https://www.shuax.com/archives/decryptcocos.html

import zipfile
import xxtea
import re
import os
import sys

apk = zipfile.ZipFile(sys.argv[1]); #"E:/WDJDownload/Apps/222.apk"

pro_file = []
for filename in apk.namelist():
	if filename.endswith(".luac"):
		pro_file.append(apk.read(filename))
		print('File:',filename)
		if len(pro_file)==2:
			break;

sign = []
for i in range(len(pro_file[0])):
	if pro_file[0][i]!=pro_file[1][i]:
		sign = pro_file[0][:i]
		break
print('Sign:',sign,len(sign))	


key = []
pattern = re.compile("(\w+)\0")
for filename in apk.namelist():
	if filename.endswith("libcocos2dlua.so"):
		print('file:', filename)
		content = apk.read(filename)
		pos = content.find(sign)
		subcontent = content[pos-512:pos+512]
	
		sign = sign.decode()
		strlist = pattern.findall(subcontent.decode("ascii", "ignore"))
		for index, found_str in enumerate(strlist):
			if found_str==sign:
				key = strlist[index-1].encode('ascii')
				

#key = key + '\0' * 9
#key = key.ljust(16, '\0')
print('Key:', key)

for filename in apk.namelist():
	if filename.endswith(".luac"):
		content = apk.read(filename)
		data = xxtea.decrypt(content[len(sign):], key)
		
		filename = os.path.splitext(filename)[0] + ".lua"
		os.makedirs(os.path.dirname(filename), exist_ok=true)
		print(filename)
		with open(filename, 'wb') as decrypted_file:
			decrypted_file.write(data)




























