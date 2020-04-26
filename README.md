Nissan 370Z (2009) eZ Navi Delete
==================================
Custom hardware/software solution to remove the factory navigation system from a Nissan 370Z and install an aftermarket solution while retaining AC control over the CAN bus.

 - [Wiki] for more info.
 - [eZ Navi-Delete Thread on the 370Z.com] with discussion.
 - [Stereo Thread on the370Z.com] showing my new stereo installation paird with the eZ Navi-Delete

### Introduction

I have been working on a way to delete the factory Navi in my ‘09 370Z while still retaining AC control that did not involve replacing the CAN based AC Amp, modifying existing wiring harnesses, or running new wires to the ECU and fuze box. The hardware platform is designed to be compatible with a wide range of interfaces. Only a subset of the functionality would be utilized in a custom stereo installation depending on the type of installation being done, but any or all combinations could be used (with the exception of multiple AC controllers). There are custom interface ports defined below and two connectors to interface with the car's existing wire harness via the connectors that went to the stock HU and control panel.

Custom interfaces available:
- Dial Control Connector: Base model AC control module interface (current scope)
  - Plug and play usage of the Base Model AC controller module with Navi Model CAN based AC systems.
- Steering Wheel Control Connector: Steering wheel control signals from resistive switch network (future firmware work)
  - Analog steering wheel control interface to monitor buttons and communicate states to tablet based systems.
- User Out Connector: Breaks out potentially useful signals from the wiring harness to be used in custom stereo installs and custom expandability
  - Breaks out signals on the car's wiring harness and microprocessor for future expandability. 
- USB: Programming interface and wired data communication port (data communication is future firmware work)
  - Allows for a wired connection to program the board (or flash updates) and to be able to communicate CAN messages to the HU. This communication can be used to update an app with the AC status when using the AC dial module, or it can be used as bidirectional communication between an app running on the HU to control the AC and update status on the screen (no AC dial module used, all touch screen).
- Bluetooth: Wireless data interface with apps running on custom HU or tablet installs (future firmware work)
  - Intended to be an alternative to the USB connection for normal operation (not for programming) in systems that cannot accept a USB conenction.

### Intended Configuration Scenarios

The idea is to be able to configure the eZ Navi-Delete board to a custom stereo installation need. Some installations will use actual car stereo Head Units (HU), while others may be using tablet based installs like the Braniac or custom varient. Some may wish to use physical controls, while others may wish to have a latger screen and control the AC via an app. Below attepts to list the most common configurations:

- Car stereo with physical controls
- Mini tablet with physical controls
- Large tables with virutal controls (app) over USB via OTG
- Large tables with virutal controls (app) over Bluetooth

### Implementation Videos
- [Initial Harwdare Test]
- [Rev 0.1 PCB Hardware Test]

### Code Prerequisites
- [STM32duino Bootloader] - Bootloader to allow USB programming
- [Arduino STM32] - Core library
- [Everytime Library] - Scheduler library
- [MCP2515 Library] - CAN Controller library

### Prebuilt Products
- [OSH PARK PCB] - Current PCB revision for sale
- [OSH Stencil] - Current stencil for sale (trying to figure out if I can share projects like OSH Park)
- [Built Firmware] - Current firmware file

[Wiki]:https://github.com/radensb/eZ_Navi-Delete/wiki
[Stereo Thread on the370Z.com]: http://www.the370z.com/audio-video/133687-joying-android-9-7-hu-install-90-complete.html
[eZ Navi-Delete Thread on the 370Z.com]: http://www.the370z.com/audio-video/130471-ez-navi-delete-project.html
[Initial Harwdare Test]:https://www.youtube.com/watch?v=1-dMO8ove7c&feature=youtu.be
[Rev 0.1 PCB Hardware Test]:https://youtu.be/FeSBLwDEIZM
[Everytime Library]: https://www.arduinolibraries.info/libraries/everytime
[MCP2515 Library]: https://github.com/autowp/arduino-mcp2515
[STM32duino Bootloader]: https://github.com/rogerclarkmelbourne/STM32duino-bootloader
[Arduino STM32]: https://github.com/rogerclarkmelbourne/Arduino_STM32
[OSH PARK PCB]: https://oshpark.com/shared_projects/BYueYDq3
[OSH Stencil]: https://www.oshstencils.com/
[Built Firmware]: https://github.com/radensb/eZ_Navi-Delete/blob/master/build/eZ_Navi_Delete_v0p5.elf

