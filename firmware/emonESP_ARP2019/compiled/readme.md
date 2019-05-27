Pre-compiled binaries,
Use **esptool.py** to upload.
Typical command executed from current directory:<br>

    $ esptool.py --baud 230400 write_flash 0x0 ./src.ino.bin 0x300000 ./src.spiffs.bin
