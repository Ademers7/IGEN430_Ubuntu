import paho.mqtt.client as mqtt

c = mqtt.Client("Ubuntu")
c.connect("localhost", 1883)                                 #establish connection
c.loop_start()
c.publish("topic/A4P_NET1","ENABLEdd")  
