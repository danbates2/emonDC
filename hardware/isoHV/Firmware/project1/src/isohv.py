pi@emonpistm:~/rfm69_working $ cat isohv.py 
# RFM69 radio bonnet receiver test
import time
import busio
import board
import adafruit_ssd1306
import adafruit_rfm69
from digitalio import DigitalInOut, Direction, Pull

import requests

# Create the I2C interface.
i2c = busio.I2C(board.SCL, board.SDA)
 
# 128x32 OLED Display
display = adafruit_ssd1306.SSD1306_I2C(128, 32, i2c, addr=0x3c)
# Clear the display.
display.fill(0)
display.show()
width = display.width
height = display.height
packetcounter = 0

# RFM69 Configuration
packet = None
CS = DigitalInOut(board.CE1)
RESET = DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm69 = adafruit_rfm69.RFM69(spi, CS, RESET, 433.0)
rfm69.encryption_key = None

# emonCMS post config

apiwritekey = "33d44b9aade0623efc4c995bb785f54e"
emoncmsaddress = "localhost/emoncms"
node_id = "isoHV"
inputkey_id = "test"
testvalue = 1234 #for testing
test_string = str (testvalue)
emon_api_url_string = "http://" + emoncmsaddress + "/input/post?node=" + node_id + "&fulljson={\"" + inputkey_id + "\":" + test_string + "}" + "&apikey=" + apiwritekey

print("Posting data via: " + emon_api_url_string)
r = requests.get(emon_api_url_string)
print (r)

while True:
    # initialization
    packetRX = None
    packetTX = None
    display.fill(0)
    display.text('RFM69: init', 0, 0, 1)
    display.fill(0)
    time.sleep(2)

    while (packetRX == None):
        display.text('Listening for packet...', 0, 0, 1)
        display.show()
        packetRX = rfm69.receive()
        time.sleep(0.1)

    packet = packetRX
    packetcounter += 1
    print (packet)
    print (packetcounter)


    displayPacket = str(packet, "utf-8")
    display.fill(0)
    display.text('Packet = ', 0, 0, 1)
    display.text(displayPacket, 25, 10, 1)
    display.show()
    emon_api_url_string = "http://" + emoncmsaddress + "/input/post?node=" + node_id + "&json={" + displayPacket + "}" + "&apikey=" + apiwritekey
    r = requests.get(emon_api_url_string)
    print (r)
    time.sleep(1)