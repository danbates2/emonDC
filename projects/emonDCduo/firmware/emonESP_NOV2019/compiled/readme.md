Pre-compiled binaries,
Use **esptool.py** to upload.
Typical command executed from current directory:<br>

    $ esptool.py --baud 460800 write_flash 0x0 ./firmware.bin 0x300000 ./spiffs.bin
