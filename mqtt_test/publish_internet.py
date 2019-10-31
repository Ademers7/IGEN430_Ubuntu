#import paho.mqtt.client as mqtt

#c = mqtt.Client("Ubuntu")
#c.username_pw_set("topinstca", "t2y5a2")
#c.connect("70.77.10.139", 1883)                                 #establish connection
#c.loop_start()
#c.publish("canada","Hello from Kelvin")  

import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    print("connected")
    client.subscribe("canada")

def on_message(client, userdata, msg):
    print(msg.payload.decode())
    #client.disconnect()

c = mqtt.Client()
c.username_pw_set("topinstca", "t2y5a2")
c.connect("70.77.10.139", 1883)
c.on_connect = on_connect
c.on_message = on_message

c.loop_forever()