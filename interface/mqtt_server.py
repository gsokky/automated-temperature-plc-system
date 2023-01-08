import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import os
import time
import tkinter as tk



class mqtt_server():
    def __init__(self,root) :   
        
        SERVER_IP_ADDRESS = "160.75.154.100"
        self.root = root
        self.auto_scroll = 0

        self.client = mqtt.Client("mqtt_test")
        
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

        self.client.username_pw_set("iturockwell", "963258741")
        
        self.client.connect(SERVER_IP_ADDRESS, 1884)

        

    def loop_start(self):
        self.client.loop_start()
    
    def loop_stop(self):
        self.client.loop_stop()
    
    def on_connect(self,client, userdata, flags, rc):
        print("Connected with result code "+str(rc))

        self.client.subscribe("group8/temperature")
        self.client.subscribe("group8/SystemOut")
        self.client.subscribe("group8/tsgo")
        self.client.subscribe("group8/start")

    def on_message(self,client, userdata, msg):
        payload = str(msg.payload.decode("UTF-8"))
        topic = msg.topic
        t = "Topic: {} / Message: {}\n".format(topic,payload)
        self.root.text.insert(tk.END,t)
        if(self.auto_scroll):
            self.root.text.see(tk.END)

        if(topic == "group8/start"):
            if(payload == "false"):
                self.root.start = 0
            if(payload == "true"):
                self.root.start = 1

    def publish(self, topic,msg):
        self.client.publish(topic, msg)

def main():
    a = mqtt_server()
    while(1):
        a.publish("group8/start1","true")
        time.sleep(1)

if __name__ == "__main__":
    main()