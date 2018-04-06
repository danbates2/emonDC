## emonDC

A dual-channel bidirectional internet connected power data-logger and meter for DC applications. With integral DC-DC buck power supply, 12-bit ADC, configurable reference voltage for bidirectional measurement, backup battery powered RTC, local microSD card data-logging, and extensibility through RFM radio module and LCD.

### Project status: prototype release, Summer 2018.

### Target use:

- 48V Solar PV systems.
- Battery monitoring.
- Any generator / battery system up to 76V.

Several spinoff boards have derived of this project, in various stages of completion, these include and isolated FTDI adaptor, an emonDCmini targeted at single channel DC monitoring with WiFi capabilities, and an emonDC shield for Arduino pattern.

#### Features

Latest spec:

- High-side DC sensing based on two LMP8481 chips, using either external shunts for higher currents, or an internal shunt soldered across a bridge on board, now rated at 20 amps, pending testing.
- Bidirectional monitoring achieved with onboard reference voltage, set by R10 & R11, easily modified by the end-user. 
- ESP-12S wireless module for internet connectivity. The module also manages the sampling through the external ADC.
- 12-bit 8 channel ADC used for monitoring two channels of current, two channels of voltage, the bidirectional reference voltage, and the 3.3V for another reference.
- MicroSD card slot for storing data locally.
- Suitable for up to 76V DC systems. Onboard DC-DC step-down (buck) switching power supply based on MAX5035 set at 7V.

Also:
- Extendable with RFM69pi module
- I2C connector for LCD screen.

- Designed for clear top waterproof case.


### Measurement of direct current (DC).

This is a project description but also a collection of ideas and findings.
The diversity of approaches could be illustrated by these two websites:
- https://www.coolcomponents.co.uk/attopilot-voltage-and-current-sense-breakout-180a.html
- http://www.victronenergy.com/upload/documents/Datasheet-Battery-monitoring-EN.pdf

### Safety

Safety needs consideration in several ways. Firstly, short circuit protection needs to exist at different levels of the hardware design to avoid inadvertent damage to the unit, by mal-assembly on part of the manufacturer (PCB and assembly) or user (installation). This can be achieved with adding fuses and buffering inputs with resistors. The layout of the components will factor in safety also.
Secondly, protection of connected devices and users from floating ground voltages in certain applications. The approach at present is to make use of double isolation and warning labels. Bright warning labels stating ‘disconnect DC system before attaching USB/FTDI’ or ‘ensure common ground between DC system and other devices to be connected’ or something similar, pending advice.

### Approach
- Hall-effect DC monitoring ICs are simple and provide a degree of electrical isolation, however, they are inflexible and costly.
- Shunt monitoring provides much greater flexibility, accuracy and cost-effectiveness.

1. The range of requirements in DC monitoring applications require a flexible approach because of:
a. Unidirectional and bidirectional requirements.
b. Amperage Ranges and associated cable cross sectional areas (CSA). For example, household 100A cables are 25mm2 or 16mm2, whereas wire for headers and breadboard carrying up to about 5A are about 0.9mm2. The target measurement range has significant physical design implications. 
c. Different cable/wire CSAs require terminations suitable to their size and application (screw terminals, bolted bus bars, soldered connections, etc.) there is no single solution.
d. Whole battery systems can be monitored with one shunt or individual cells of the system can require a multi-cell monitoring unit.
e. There are also high humidity, marine and automotive applications.

2. If we consider cable sizes and suitable cable terminations, the issues make it difficult to see any one PCB realistically and cost-effectively meeting ALL possible requirements. Unlike in many AC requirements where a different CT and burden resistor can be selected, DC demands a more targeted approach.
This possibly leads to the solution of designing several different boards according to the different requirements. First suggestions might look like:
emonDC50 – Bidirectional DC monitor with integrated yet replaceable / bypassable shunt, accepting up to 6mm2 cable up to 50A, but with customisable sensitivity down for micro amps.
emonDC500 – Unidirectional shunt monitor with external shunt rated up to 500A.
emonDC500duo – Two unidirectional shunt monitors in parallel for measuring two external shunts rated up to 500A, for battery/generator applications.

The approach selected for first production has been dubbed emonDCduo, onboard shunts and pheonix connectors for external shunts ensure some flexibility.

3. Another option might be to have a two-part system where Tx functions are separated from shunt monitoring functions in two separate PCBs, connected via RJ11 or ribbon cable. This could lend itself to multi-source monitoring effectively. Also, galvanic isolation would become easier. Also, the standalone Tx PCB may be compatible with other emon projects in the future.

4. Given the nature of solar, wind and various DC applications there a requirement for a buck regulating supply, probably isolated. There needs to be a regulated and smooth output for powering the shunt monitor and other ICs.

5. Failsafe monitoring... Fuses between the shunt and monitor to protect from short circuit conditions.

6. High-side vs. low-side sensing… This shunt monitor needs to be capable of handling both applications, which means using a shunt monitor IC capable of high-side measurement. The device should ideally handle a wide common-mode voltage range, say up to 80V, and protection be in place for voltage over the common-mode voltage limit of the chip.

### Project Aims

Create as flexible as possible unit. Aiming primarily at domestic solar, wind and battery applications.
Compare hall-effect type to shunt monitor type DC measuring approaches, particularly in relation to safety, accuracy and flexibility.
Integrate wireless Tx function using RFM69pi and ESP8266.
Explore potential for AC monitoring applications.
To ascertain for certain the viability or necessity of a two part system. Such as Part A: Tx PCB and Part B: DC monitoring shield.
Investigate a calibration routine.
Look at case design.
Look at the energy efficiency of different manufacturing approaches.
Make a unit!


Questions…
What’s the relationship to different amperages, cables sizes and suitable cable terminations at the device?
Would a particular amperage range require soldered connection? Do specific applications require soldered connections?
Could the PCB safely handle the required amps? What’s the current carrying capacity of header pins and PCB track?
Can PCB track be used as the shunt?
How does heat dissipate through the PCB and how does this effect the value being monitored? Is a temperature sensor required for factoring in temperature drift?
How much responsibility does the installer have in installing correct cable terminations for this board? What are the divisions of responsibility?


### Licence
- Hardware licensed according to the TAPR Open Hardware Licence v1.0. Documentation free to use and change.
https://www.tapr.org/TAPR_Open_Hardware_License_v1.0.txt
- Software licensed licensed to GNU General Public License v3. Free to use and change.
https://www.gnu.org/licenses/gpl.html