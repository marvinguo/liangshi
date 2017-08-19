package com.mtn.mes;
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
import android.widget.Toast;
import java.util.Date;

import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import com.amap.api.location.AMapLocation;
import com.amap.api.location.AMapLocationListener;
import com.amap.api.location.LocationManagerProxy;
import com.amap.api.location.LocationProviderProxy;
import com.baidu.location.BDLocation;
import com.baidu.location.BDLocationListener;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.baidu.location.LocationClientOption.LocationMode;

public class GpsService extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static GpsService m_instance;
    private final static String TAG = "GpsService";
    private final static int MSG_STR_GPS_LOC = 0;
    private final static int MSG_RPT_GPS_INF = 1;

    ////////////////////////////////////////////////////////////////
    //                        定位相关
    ////////////////////////////////////////////////////////////////
    private LocationManagerProxy aMapManager;

    public GpsService(){
        m_instance = this;
    }

    public static void calledByCpp() {
        System.out.println("[0]hello world!");
        Message msg = new Message();
        msg.what = MSG_STR_GPS_LOC;
        m_instance.handler.sendMessage(msg); // 消息触发，启动GPS定位
        //m_instance.handler.sendEmptyMessage(0); // 消息触发，启动GPS定位
    }

    public static void calledByCpp(int arg0) {
         System.out.println("[1]hello world!");
    }

    private void startAmap() {
        aMapManager = LocationManagerProxy.getInstance(this);
        /*
         * mAMapLocManager.setGpsEnable(false);
         * 1.0.2版本新增方法，设置true表示混合定位中包含gps定位，false表示纯网络定位，默认是true Location
         * API定位采用GPS和网络混合定位方式
         * ，第一个参数是定位provider，第二个参数时间最短是2000毫秒，第三个参数距离间隔单位是米，第四个参数是定位监听者
         */
        aMapManager.requestLocationUpdates(LocationProviderProxy.AMapNetwork, 2000, 10, mAMapLocationListener);
    }

    private AMapLocationListener mAMapLocationListener = new AMapLocationListener() {

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {

        }

        @Override
        public void onProviderEnabled(String provider) {

        }

        @Override
        public void onProviderDisabled(String provider) {

        }

        @Override
        public void onLocationChanged(Location location) {

        }

        @Override
        public void onLocationChanged(AMapLocation location) {
            if (location != null) {
                Double geoLat = location.getLatitude();
                Double geoLng = location.getLongitude();
                String cityCode = "";
                String desc = "";
                Bundle locBundle = location.getExtras();
                if (locBundle != null) {
                    cityCode = locBundle.getString("citycode");
                    desc = locBundle.getString("desc");
                }

                String str = (
                "location ok:(" + geoLng + "," + geoLat + ")"+
                "\nAccuracy   :" + location.getAccuracy() + "Meter" +
                "\nPositioning:" + location.getProvider() +
                "\nPositioning time:" + new Date(location.getTime()).toLocaleString() +
                "\nCity coding :" + cityCode +
                "\nLocation Description:" + desc +
                "\nProvince:" + location.getProvince() +
                "\nCity:" + location.getCity() +
                "\nDistrict (county):" + location.getDistrict() +
                "\nRegional Coding:" + location.getAdCode());

                //Toast.makeText(getApplicationContext(), "高德定位\n" + str, Toast.LENGTH_SHORT).show();

                // 发送位置信息到handler, hander处再转发给Qt
                Message msg = new Message();
                Bundle data = new Bundle();
                data.putString("value", str);
                msg.setData(data);
                msg.what = MSG_RPT_GPS_INF;
                handler.sendMessage(msg);
            }
        }
    };

    private Handler handler = new Handler() {

        @Override
        public void handleMessage(Message msg) {
            switch(msg.what) {
               case MSG_STR_GPS_LOC:
                   m_instance.startAmap();
               break;

               case MSG_RPT_GPS_INF:
                   ExtendsQtNative  m_nativeNotify = new ExtendsQtNative();
                   Bundle data = msg.getData();
                   System.out.println(data.getString("value"));
                   m_nativeNotify.reportGpsInfo(0, data.getString("value"));
               break;

               default:
                   System.out.println("msg type error!");
               break;
            }
        }
    };
}
