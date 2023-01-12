import tkinter as tk
from mqtt_server import mqtt_server 
from tkinter.scrolledtext import ScrolledText


class inteface:
    def __init__(self):
        self.start = 0
        

        self.root = tk.Tk()
        self.root.title("interface")
        self.root.geometry("640x640")

        self.show_start = tk.IntVar()

        self.canvas = tk.Canvas(self.root)
        self.canvas.pack(anchor = tk.NW,fill="x")

        self.connect_btn = tk.Button(self.canvas, text="Connect" , bg = "#FFFFFF", command = self.connect)
        self.connect_btn.pack(side="left")

        self.disconnect_btn = tk.Button(self.canvas, text="Disconnect" , bg = "#FFFFFF", command = self.disconnect)
        self.disconnect_btn.pack(side="left")

        self.start_btn = tk.Button(self.canvas, text="Start" , bg = "#FFFFFF", command = self.start_stop)
        self.start_btn.pack(side="left")

        self.select_topics_btn = tk.Button(self.canvas, text="Topics" , bg = "#FFFFFF", command = self.select_topics)
        self.select_topics_btn.pack(side="left")

        self.auto_scroll_btn = tk.Button(self.canvas, text="auto scroll" , bg = "#FFFFFF", command = self.auto_scroll)
        self.auto_scroll_btn.pack(side="left")
        
        self.publish_btn = tk.Button(self.canvas, text="publish command" , bg = "#FFFFFF", command = self.publish)
        self.publish_btn.pack(side="left")

        self.text_topic = tk.Text(self.canvas, height = 1, width = 5)
        self.text_msg = tk.Text(self.canvas, height = 1, width = 5)
        
        self.text_topic.pack(side="left",expand= True,padx = 3,fill="x")
        self.text_msg.pack(side="right",expand= True,padx = 3,fill="x")
       
        self.text = tk.Text(self.root, height = 20, width = 52)
        self.vsb = tk.Scrollbar(self.root, orient="vertical", command=self.text.yview)

        self.text.configure(yscrollcommand=self.vsb.set)
        self.vsb.pack(side="right", fill="y")
        self.text.pack(side="left", fill="both", expand=True)

        self.server = mqtt_server(self)

        self.root.mainloop()

    def select_topics(self):
        self.open_topics_popup()
    
    
    def open_topics_popup(self):
        self.topics_popup = tk.Toplevel(self.root)
        self.topics_popup.geometry("220x250")
        self.topics_popup.title("Topics")

        self.canvas_popup = tk.Canvas(self.topics_popup)
        self.canvas_popup.pack(anchor = tk.NW,fill="x")

        self.submit_topics_btn = tk.Button(self.canvas_popup, text="submit" , bg = "#FFFFFF", command = self.submit_topics)
        self.submit_topics_btn.pack(side= "left")

        self.add_topics_btn = tk.Button(self.canvas_popup, text="add" , bg = "#FFFFFF", command = self.add_topics)
        self.add_topics_btn.pack(side= "left")

        self.delete_topics_btn = tk.Button(self.canvas_popup, text="delete" , bg = "#FFFFFF", command = self.delete_topics)
        self.delete_topics_btn.pack(side= "left")

        self.text_topic_add_delete = tk.Text(self.canvas_popup, height = 1, width = 5)
        self.text_topic_add_delete.pack(side="left",expand= True,padx = 3,fill="x")
        

        self.topics = []

        self.scroll_frame = ScrolledText(self.topics_popup, width=25, height=15,cursor="arrow #0000FF")
        self.scroll_frame.pack(anchor = tk.NW ,side="left")

        self.fill_scroll_frame()
        
        
        
        
    def fill_scroll_frame(self):
        self.scroll_frame.config(state=tk.NORMAL)
        self.scroll_frame.delete('1.0', tk.END)
        self.topics = []

        for j,(key,i) in enumerate(self.server.topics.items()):
            if key == "group8/start":
                color = "#AAAAAA"
                value = 1
                button2 = tk.Checkbutton(self.scroll_frame,
                    text = "", 
                    variable = self.show_start,
                    bg = color,
                    onvalue = 1,
                    offvalue = 0
                    )
                self.scroll_frame.window_create('end', window = button2)
                self.scroll_frame.insert('end', '')
            else:
                color = "#FFFFFF"
                value = 0
            var = tk.IntVar()
            button = tk.Checkbutton(self.scroll_frame,
                    text = key, 
                    variable = var,
                    bg = color,
                    onvalue = 1,
                    offvalue = value
                    )
            if(i):
                button.select()
            button.pack()
            self.topics.append([button,var,key])

            self.scroll_frame.window_create('end', window = button)
            self.scroll_frame.insert('end', '\n')

        self.scroll_frame.config(state=tk.DISABLED)
        
        
    def add_topics(self):
        topic = self.text_topic_add_delete.get("1.0",'end-1c')
        self.server.topics[topic] = 1
        
        self.fill_scroll_frame()
        self.submit_topics()


    def delete_topics(self):
        
        topic = self.text_topic_add_delete.get("1.0",'end-1c')
        if(topic == "group8/start"):
            return

        self.server.topics.pop(topic)

        self.fill_scroll_frame()
        self.submit_topics()



    def submit_topics(self):
        
        for button,var,key in self.topics:
            self.server.topics[key] = var.get()
        self.connect()


    def connect(self):
        self.server.connect()
        self.server.loop_start()

    def disconnect(self):
        self.server.disconnect()
        self.server.loop_stop()
    
    def publish(self):
        topic = self.text_topic.get("1.0",'end-1c')
        msg = self.text_msg.get("1.0",'end-1c')
        print("published ", topic, " : ", msg)
        self.server.publish(topic,msg)
        

    def auto_scroll(self):
        self.server.auto_scroll = not self.server.auto_scroll

    def start_stop(self):
        
        if(not self.start):
            self.server.loop_start()
            self.server.publish("group8/start_system","true")
        else:
            # self.server.loop_stop()
            self.server.publish("group8/start_system","false")


        

a = inteface()