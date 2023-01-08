import tkinter as tk
from mqtt_server import mqtt_server


class inteface:
    def __init__(self):
        self.start = 0

        self.root = tk.Tk()
        self.root.title("interface")
        self.root.geometry("480x520")

        self.canvas = tk.Canvas(self.root, width = 480, height = 20)
        self.canvas.pack(anchor = tk.NW)

        self.start_btn = tk.Button(self.canvas, text="Start" , bg = "#FFFFFF", command = self.start_stop)
        self.start_btn.pack(side="left")

        self.auto_scroll_btn = tk.Button(self.canvas, text="auto scroll" , bg = "#FFFFFF", command = self.auto_scroll)
        self.auto_scroll_btn.pack(side="left")

        self.text = tk.Text(self.root, height = 20, width = 52)
        self.vsb = tk.Scrollbar(self.root, orient="vertical", command=self.text.yview)
        self.text.configure(yscrollcommand=self.vsb.set)
        self.vsb.pack(side="right", fill="y")
        self.text.pack(side="left", fill="both", expand=True)

        self.server = mqtt_server(self)

        self.root.mainloop()

    def auto_scroll(self):
        self.server.auto_scroll = not self.server.auto_scroll

    def start_stop(self):
        
        if(not self.start):
            self.start = not self.start
            self.server.loop_start()
            self.server.publish("group8/start_system","true")
        else:
            # self.server.loop_stop()
            self.server.publish("group8/start_system","false")


        

a = inteface()