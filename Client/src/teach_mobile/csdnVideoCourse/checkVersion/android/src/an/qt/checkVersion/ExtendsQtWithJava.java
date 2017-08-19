package an.qt.checkVersion;
import android.content.Context;
import android.content.Intent;
import android.app.PendingIntent;
import android.util.Log;
import android.net.Uri;
import android.provider.Settings;
import android.os.Bundle;
import android.os.Environment;
import android.widget.Toast;
import android.os.Handler;
import android.os.Message;

public class ExtendsQtWithJava extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static ExtendsQtWithJava m_instance;
    private final static String TAG = "checkVersion";
    private static Handler m_handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case 1:
                String qtVersion = ExtendsQtNative.GetQtVersion();
                String toastText = String.format("%s, %s", (String)msg.obj, qtVersion);
                Log.d(TAG, "call Toast.makeText " + toastText);
                Toast toast = Toast.makeText(m_instance, toastText, Toast.LENGTH_LONG);
                toast.show();
                Log.d(TAG, "after toast.show");
                break;
            };
        }
    };

    public ExtendsQtWithJava(){
        m_instance = this;
    }

    public static void launchActivity(String action){
        m_instance.startActivity(new Intent(action));
    }

    public void makeToast(String s){
        m_handler.sendMessage(m_handler.obtainMessage(1, s));
    }
}
