import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    print("connected")
    client.subscribe("topic/A4PTest")

def on_message(client, userdata, msg):
    print(msg.payload.decode())
    client.disconnect()

c = mqtt.Client()
#c.connect("192.168.0.10")
c.connect("localhost")
c.on_connect = on_connect
c.on_message = on_message

c.loop_forever()