package an.qt.callSettings;
import android.content.Context;
import android.content.Intent;
import android.app.PendingIntent;
import android.util.Log;
import android.net.Uri;
import android.provider.Settings;
import android.os.Bundle;
import android.os.Environment;

public class ExtendsQtWithJava extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static ExtendsQtWithJava m_instance;
    private final static String TAG = "extendsQt";

    public ExtendsQtWithJava(){
        m_instance = this;
    }

    public static void launchActivity(String action){
        m_instance.startActivity(new Intent(action));
    }
}
