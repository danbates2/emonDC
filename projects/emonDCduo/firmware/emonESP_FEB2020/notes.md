Terminal commands to flash firmware, the first two are optional depending on your ESP8266:

    $ pip install esptool.py (Python required)
    $ esptool.py erase_flash
    $ pio run -t upload && pio run -t uploadfs
    $ pio device monitor