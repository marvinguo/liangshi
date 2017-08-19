package com.liangshiyy.teaching;

import java.io.*;
import java.util.*;

import android.os.Environment;
import android.text.*;

public class Util {
	/**  
     * 获取外置SD卡路径  
     *   
     * @return  
     */  
    public static List<String> getSDCardPaths() {  
        List<String> sdcardPaths = new ArrayList<String>();  
        String cmd = "cat /proc/mounts";  
        Runtime run = Runtime.getRuntime();// 返回与当前 Java 应用程序相关的运行时对象  
        try {  
            Process p = run.exec(cmd);// 启动另一个进程来执行命令  
            BufferedInputStream in = new BufferedInputStream(p.getInputStream());  
            BufferedReader inBr = new BufferedReader(new InputStreamReader(in));  
  
            String lineStr;  
            while ((lineStr = inBr.readLine()) != null) {  
                // 获得命令执行后在控制台的输出信息  
  
                if(lineStr.contains("secure") || lineStr.contains("asec"))
                	continue;
                if(!lineStr.contains("fat"))
                	continue;
				//2015.8.4, 操，发现张老师的板子有private的东西可以逃出
            	if(lineStr.contains("mnt/private"))
                	continue;
                String[] temp = TextUtils.split(lineStr, " ");  
                // 得到的输出的第二个空格后面是路径  
                String result = temp[1];  
                File file = new File(result);  
                if (file.isDirectory() && file.canRead() && file.canWrite()) {  
                    // 可读可写的文件夹未必是sdcard，我的手机的sdcard下的Android/obb文件夹也可以得到  
                    sdcardPaths.add(result);  
  
                }  
  
                // 检查命令是否执行失败。  
                if (p.waitFor() != 0 && p.exitValue() == 1) {  
                    // p.exitValue()==0表示正常结束，1：非正常结束  
                }  
            }  
            inBr.close();  
            in.close();  
        } catch (Exception e) {  
            sdcardPaths.add(Environment.getExternalStorageDirectory()  
                    .getAbsolutePath());  
        }  
  
        optimize(sdcardPaths);  
        
        if(sdcardPaths.size() > 1)
        {
        	int hasExt = hasCertain(sdcardPaths, "/mnt/extsd");
        	if(hasExt >= 0)
        	{
        		int hassd = hasCertain(sdcardPaths, "/mnt/sdcard");
            	if(hassd >= 0)
            	{
            		sdcardPaths.remove(hassd);
            	}
        	}
        	
        }
        return sdcardPaths;  
    }  
    
    
    private static int hasCertain(List<String> sdcaredPaths, String text) {  
        for (int i = 0; i < sdcaredPaths.size(); i++) {  
            if (sdcaredPaths.get(i).contains(text)) {  
                return i; 
            }  
        } 
        return -1;
    }
  
    private static void optimize(List<String> sdcaredPaths) {  
	    if (sdcaredPaths.size() == 0) {  
	        return;  
	    }  
        int index = 0;  
        while (true) {  
            if (index >= sdcaredPaths.size() - 1) {  
                String lastItem = sdcaredPaths.get(sdcaredPaths.size() - 1);  
                for (int i = sdcaredPaths.size() - 2; i >= 0; i--) {  
                    if (sdcaredPaths.get(i).contains(lastItem)) {  
                        sdcaredPaths.remove(i);  
                    }  
                }  
                return;  
            }  
  
            String containsItem = sdcaredPaths.get(index);  
            for (int i = index + 1; i < sdcaredPaths.size(); i++) {  
                if (sdcaredPaths.get(i).contains(containsItem)) {  
                    sdcaredPaths.remove(i);  
                    i--;  
                }  
            }  
  
            index++;  
        }  
  
    }  
}