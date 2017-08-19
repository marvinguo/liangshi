package an.qt.useJar;
import java.lang.String;
import android.content.Context;
import android.content.Intent;
import android.app.PendingIntent;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.location.Criteria;
import android.provider.Settings;
import android.os.Bundle;
import android.os.Environment;
import java.io.File;
import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;

public class ExtendsQtWithJava extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static ExtendsQtWithJava m_instance;
    private final static String TAG = "extendsQt";
    private static String m_pageUri = null;
    private static Handler m_handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case 1:
                if(m_pageUri == null){
                    m_pageUri = (String)msg.obj;
                    m_instance.downloadText(m_pageUri);
                }else{
                    m_instance.notifyQt(0, (String)msg.obj, "Downloader is Busy now!");
                }
                break;
            };
        }
    };
    

    public ExtendsQtWithJava(){
        m_instance = this;
    }

    public static int networkState(){
        ConnectivityManager conMan = (ConnectivityManager) m_instance.getSystemService(Context.CONNECTIVITY_SERVICE);
        return conMan.getActiveNetworkInfo() == null ? 0 : 1;
    }

    public static AsyncHttpClient m_httpc = new AsyncHttpClient();
    public static ExtendsQtNative m_nativeNotify = null;

    public void downloadText(String uri){
        Log.d(TAG, "start downloadText");
        m_httpc.get(uri, null, new AsyncHttpResponseHandler(){
            @Override
            public void onSuccess(String data){
                notifyQt(1, m_pageUri, data);
                m_pageUri = null;
            }
            @Override
            public void onFailure(Throwable e, String data){
                notifyQt(-1, m_pageUri, data);
                m_pageUri = null;
            }
        });
    }
    
    public static void downloadWebPage(String uri){
        Log.d(TAG, "downloadWebPage");
        m_handler.sendMessage(m_handler.obtainMessage(1, uri));
    }
    
    private void notifyQt(int result, String uri, String data){
        if(m_nativeNotify == null){
            m_nativeNotify = new ExtendsQtNative();
        }
        m_nativeNotify.OnDownloaded(result, uri, data);
    }
}
