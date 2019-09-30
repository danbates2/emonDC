##emonDCduo changelog
This changelog has been written retrospectively and will miss out detail.

v0.1 old versions were basically changes to the emonTX with a two shunt-monitors introduced. A dedicated power supply for DC-DC buck conversion was designed in with help from Ken Boak for the electrical theory side of things.

v0.4 contained in the _previous version archive attempts to integrate an atmel chip, rfm, and esp8266 module, and microSD card.

v1.1 does away with the atmel chip for cost reasons, and incorporates the 8-channel 12 bit ADC MCP3208. Also the integrated RFM69 is replaced with a 10 pin header for connecting an RMFpi.

v1.2 by this version there’s a worked out RTC chip based on Bob Lemaire IoTaWatt design. Also changed is the power supply, involving three regs.

v1.3 minor changes for production. The resistor divider is changed for the reference voltage for the bi-directional monitoring. The resistors are solderable through-hole type now, to enable the end-user to easily change the reference voltage. This philosophy could apply to the voltage dividers for the channel A and B voltage monitoring.

v1.4 ‘designed by Megni’ changed to ‘designed by Daniel Bates’. The shunts and PCB traces have more isolation and 4.5mm drill hole via have been introduced for soldering directly 4mm cable to the board, for higher current applications.

v2.0 Fuse and transient protection added. Board reorganised. Connectors reorganised.

v3.0 All inputs fused. DC-DC Buck removed, in favour of module, dubbed emonBuck. Mounting options diversified. RTC moved under-side.
