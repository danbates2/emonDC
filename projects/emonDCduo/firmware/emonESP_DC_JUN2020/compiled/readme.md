Pre-compiled binaries,
Use **esptool.py** to upload.
Typical command executed from current directory:<br>

    $ esptool.py --baud 460800 write_flash 0x0 ./firmware.bin
    or
    $ esptool.py --baud 921600 write_flash 0x0 .pio/build/emonesp-dc/firmware.bin

SET BOTH VREF ON-BOARD TO BI-DIRECTIONAL