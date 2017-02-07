
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
        String len = "0";
        try {
            url = new URL(linkURL);
            urlConnection = (HttpURLConnection) url.openConnection();
            urlConnection.setRequestMethod("HEAD");
            if (urlConnection.getResponseCode() == 200) 
            {
    			len = urlConnection.getHeaderField("content-length");  
            }
            System.out.println("length: == " + len);
			url = null;
			return len;
		
        } catch (Exception e) {
            Log.e("URLConnection exception", e.toString());
            url = null;
            return len;
        }
    }
}
