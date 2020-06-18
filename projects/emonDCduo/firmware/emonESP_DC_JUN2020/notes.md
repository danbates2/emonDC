Terminal commands to flash firmware, the first two are optional depending on your ESP8266:

    $ pip install esptool.py (Python required)
    $ esptool.py erase_flash
    $ pio run -t upload && pio run -t uploadfs
    $ pio device monitor

    Alternatively:
    $ esptool.py --baud 921600 write_flash 0x0 .pio/build/emonesp-dc/firmware.bin
    or
    pio run && esptool.py --baud 921600 write_flash 0x0 .pio/build/emonesp-dc/firmware.bin && pio device monitor

    extra pio device monitor commands untested.
    $ -f colorize -f send_on_enter