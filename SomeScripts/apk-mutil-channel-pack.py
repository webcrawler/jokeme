#!/usr/bin/python
# -*- coding: utf-8 -*-

# add file "channel_123" to "META-INF" dir. "INF/channel_123"
# 1.txt is a empty file
import zipfile
zipped = zipfile.ZipFile("E:/MyGithub/jokeme/SomeScripts/123.apk", 'a', zipfile.ZIP_DEFLATED) # sys.argv[1]
empty_channel_file = "META-INF/channel_123"
zipped.write("E:/MyGithub/jokeme/SomeScripts/1.txt", empty_channel_file)
zipped.close()

"""
java 代码获取渠道123  "META-INF/channel_123" ：
public static String getChannel(Context context) {
	ApplicationInfo appinfo = context.getApplicationInfo();
	String sourceDir = appinfo.sourceDir;
	String ret = "";
	ZipFile zipfile = null;
	try {
		zipfile = new ZipFile(sourceDir);
		Enumeration<?> entries = zipfile.entries();
		while (entries.hasMoreElements()) {
			ZipEntry entry = ((ZipEntry) entries.nextElement());
			String entryName = entry.getName();
			if (entryName.startsWith("META-INF/channel")) {
				ret = entryName;
				break;
			}
		}
	} catch (IOException e) {
		e.printStackTrace();
	} finally {
		if (zipfile != null) {
			try {
				zipfile.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	String[] split = ret.split("_");
	if (split != null && split.length >= 2) {
		return ret.substring(split[0].length() + 1);

	} else {
		return "";
	}
}
测试：
String channel = getChannel(this);
showDialog("channel", channel);
"""
