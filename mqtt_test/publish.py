import paho.mqtt.client as mqtt

print('hello')
c = mqtt.Client("Ubuntu")
c.connect("localhost", 1883)                                 #establish connection
c.loop_start()
c.publish("topic/A4PTest","hello")  
