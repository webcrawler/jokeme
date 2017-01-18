
package org.cocos2dx.cpp;

import android.util.Log;
import java.net.HttpURLConnection;
import java.net.URL;

public class WebFileSize
{
    static String getWebFileSize(String linkURL) 
    {
        URL url;
        HttpURLConnection urlConnection;
        try {
            url = new URL(linkURL);
            urlConnection = (HttpURLConnection) url.openConnection();
			//int length = url.getContentLength();  
			String len = urlConnection.getHeaderField("content-length");  
			System.out.println("length: == " + len);
			return len;
		
        } catch (Exception e) {
            Log.e("URLConnection exception", e.toString());
            return "0";
        }
    }
}
