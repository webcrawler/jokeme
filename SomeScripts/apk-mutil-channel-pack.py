#!/usr/bin/python
# add file "channel_123" to "META-INF" dir. "INF/channel_123"
# 1.txt is a empty file
import zipfile
zipped = zipfile.ZipFile("E:/MyGithub/jokeme/SomeScripts/123.apk", 'a', zipfile.ZIP_DEFLATED) # sys.argv[1]
empty_channel_file = "META-INF/channel_123"
zipped.write("E:/MyGithub/jokeme/SomeScripts/1.txt", empty_channel_file)
zipped.close()