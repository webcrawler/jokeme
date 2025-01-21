
批量生成渠道包：https://github.com/GavinCT/AndroidMultiChannelBuildTool

只能使用 v1签名

apk文件放当前目录下
渠道flag在info/channel.txt 下

java读取渠道flag信息：

	// 从apk中获取渠道信息
	private String getChannelFromApk(Context context, String attributionTag) {
		//从apk包中获取
		ApplicationInfo appinfo = context.getApplicationInfo();
		String sourceDir = appinfo.sourceDir;
		//默认放在meta-inf/里， 所以需要再拼接一下
		String key = "META-INF/" + attributionTag;
		String ret = "";
		ZipFile zipfile = null;
		try {
			zipfile = new ZipFile(sourceDir);
			Enumeration<?> entries = zipfile.entries();
			while (entries.hasMoreElements()) {
				ZipEntry entry = ((ZipEntry) entries.nextElement());
				String entryName = entry.getName();
				if (entryName.startsWith(key)) {
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
		String channel = "0";
		if (split != null && split.length >= 2) {
			channel = ret.substring(split[0].length() + 1);
		}
		Log.i(TAG, "apk attributionId: " + channel);
		return channel;
	}
}

