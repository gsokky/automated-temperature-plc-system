import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import os
import time
import tkinter as tk
from datetime import datetime  



class mqtt_server():
    def __init__(self,root) :   
        
        self.SERVER_IP_ADDRESS = "160.75.154.100"
        self.SERVER_PORT = 1884
        self.root = root
        self.auto_scroll = 0

        self.topics = {
            "group8/start" : 1,
            "group8/temperature" : 0,
            "group8/SystemOut" : 1,
            "group8/tsgo" : 0,
            "group8/tserror" : 0,
           
        }

        self.client = mqtt.Client("mqtt_test")
        
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

        self.client.username_pw_set("iturockwell", "963258741")
        self.connect()
        

    
    def connect(self):
        self.client.connect(self.SERVER_IP_ADDRESS, self.SERVER_PORT)

    def disconnect(self):
        self.client.disconnect()


    def loop_start(self):
        self.client.loop_start()
    
    def loop_stop(self):
        self.client.loop_stop()
    
    def on_connect(self,client, userdata, flags, rc):
        print("Connected with result code "+str(rc))

        for key,i in self.topics.items():
            if(i == 1):
                self.client.subscribe(key)

            
    def on_message(self,client, userdata, msg):
        payload = str(msg.payload.decode("UTF-8"))
        topic = msg.topic
        time_stamp = time.time()
        date_time = datetime.fromtimestamp(time_stamp)
        str_date_time = date_time.strftime("%d.%m.%Y %H:%M:%S.%f")

        t = "{} | {} | {}\n".format(str_date_time,topic,payload)
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