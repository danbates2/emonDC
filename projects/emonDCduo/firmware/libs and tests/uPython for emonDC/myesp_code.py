import network

sta_if = network.WLAN(network.STA_IF); sta_if.active(True)
sta_if.connect("Luxusgemeinschaft", "Essen&Trinken.24h.")
