package com.example.gtk_mqtt

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import org.eclipse.paho.android.service.MqttAndroidClient

class MainActivity : AppCompatActivity() {

    //Variables
    val MQTTHOST = "tcp://192.168.0.27:1883"
    val USERNAME = ""
    val PASSWORD = ""
    var topicStr = ""
    //var MqttAndroidClient clientMQTT
    var clientMQTT = MqttAndroidClient()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }
}