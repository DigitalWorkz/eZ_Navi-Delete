Nissan 370Z eZ Navi-Delete
==================================
Custom hardware/software solution to remove the factory navigation system from a Nissan 370Z and install an aftermarket solution while retaining AC control over the CAN bus.

 - [Wiki] for more info. (NOTE: Information in the Wiki is still under development and is subjet to change until finalized.)
 - [eZ Navi-Delete Thread on the 370Z.com] with discussion.
 - [Stereo Thread on the370Z.com] showing my new stereo installation paird with the eZ Navi-Delete

### Introduction
<p align="center">
  <img src="/img/support/eZNaviDelete_current_cartoon.png" width="1080" alt="accessibility text">
</p>

I have been working on a way to delete the factory Navi in my ‘09 370Z while still retaining AC control that did not involve replacing the CAN based AC Amp, modifying existing wiring harnesses, or running new wires to the ECU and fuze box. The hardware platform is designed to be compatible with a wide range of interfaces. Only a subset of the functionality would be utilized in a custom stereo installation depending on the type of installation being done, but any or all combinations could be used (with the exception of multiple AC controllers). There are custom interface ports defined below and two connectors to interface with the car's existing wire harness via the connectors that went to the stock HU and control panel.

### Interfaces
- [Universal Wiring Harness Adapter]: Main interface to the 370Z.
  - There was a change in the factory Navi after 2009 for 2010+ that resulted in different connectors being used. To be compatible with any modle year, the eZ Navi-Delete uses a universal connector that adapts to a specific wiring harness connector based on the model year. 
- [Dial Control Connector]: Base model AC control module interface. (current scope)
  - Plug and play usage of the Base Model AC controller module with Navi Model CAN based AC systems.
- [Steering Wheel Control Connector]: Steering wheel control signals from resistive switch network (future firmware work)
  - Analog steering wheel control interface to monitor buttons and communicate states to tablet based systems over USB or Bluetooth.
- [User Out Connector]: Breaks out potentially useful signals from the wiring harness to be used in custom stereo installs and custom expandability
  - Breaks out signals on the car's wiring harness and microprocessor for future expandability. 
- USB: Programming interface and wired data communication port (data communication is future firmware work)
  - Allows for a wired connection to program the board (or flash updates) and to be able to communicate to the HU. This communication can be used to update an app with the AC status when using the AC dial module, or it can be used as bidirectional communication between an app running on the HU to control the AC and update status on the screen (no AC dial module used, all touch screen).
- Bluetooth: Wireless data interface with apps running on custom HU or tablet installs (future firmware work)
  - Intended to be an alternative to the USB connection for normal operation (not for programming) in systems that cannot accept a USB conenction.

### Intended Configuration Scenarios

The idea is to be able to configure the eZ Navi-Delete board to a custom stereo installation need. Some installations will use actual car stereo Head Units (HU), while others may be using tablet based installs like the Braniac or custom varient. Some may wish to use physical controls, while others may wish to have a latger screen and control the AC via an app. Below attepts to list the most common configurations:

- Car stereo with physical controls
- Mini tablet with physical controls
- Large tablets with virtual controls (app) over USB via OTG
- Large tablets with virtual controls (app) over Bluetooth

### Implementation Videos
- [Initial Harwdare Test]
- [Rev 0.1 PCB Hardware Test]

### Code Prerequisites
- [STM32duino Bootloader] - Bootloader to allow USB programming
- [Arduino STM32] - Core library (current trunk)
- [OFFICIAL Arduino STM32 CORE] - Core library (buildConfig dev branch)
- [Everytime Library] - Scheduler library
- [MCP2515 Library] - CAN Controller library

### Prebuilt Products
- [Current PCB revision] - Board only
- [Current PWA revision] - Current pre-assembled board (coming soon) 

[//]: <> (- [Built Firmware] - Current firmware file)

<h3>

```diff
-! DISCLAIMER: THIS IS CUSTOM HARDWARE AND SOFTWARE PROJECT. USE AT YOUR OWN RISK !-
```
</h3>

[Wiki]:https://github.com/radensb/eZ_Navi-Delete/wiki
[Stereo Thread on the370Z.com]: http://www.the370z.com/audio-video/133687-joying-android-9-7-hu-install-90-complete.html
[eZ Navi-Delete Thread on the 370Z.com]: http://www.the370z.com/audio-video/130471-ez-navi-delete-project.html
[Universal Wiring Harness Adapter]:https://github.com/radensb/eZ_Navi-Delete/wiki/Universal-Adapter-Wiring-Harness
[Dial Control Connector]:https://github.com/radensb/eZ_Navi-Delete/wiki/Base-Model-AC-Control-Module-Wiring-Harness
[Steering Wheel Control Connector]:https://github.com/radensb/eZ_Navi-Delete/wiki/Steering-Wheel-Control-Connector
[User Out Connector]:https://github.com/radensb/eZ_Navi-Delete/wiki/User-Out-Connector
[Initial Harwdare Test]:https://www.youtube.com/watch?v=1-dMO8ove7c&feature=youtu.be
[Rev 0.1 PCB Hardware Test]:https://youtu.be/FeSBLwDEIZM
[Everytime Library]: https://www.arduinolibraries.info/libraries/everytime
[MCP2515 Library]: https://github.com/autowp/arduino-mcp2515
[STM32duino Bootloader]: https://github.com/rogerclarkmelbourne/STM32duino-bootloader
[Arduino STM32]: https://github.com/rogerclarkmelbourne/Arduino_STM32
[OFFICIAL Arduino STM32 CORE]: https://github.com/stm32duino/Arduino_Core_STM32
[Current PCB revision]: https://github.com/radensb/eZ_Navi-Delete/wiki/7%29-How-to-Buy-an-Assembled-PWA-or-Bare-PCB#ez-navi-delete-pcbs-only
[Current PWA revision]: https://github.com/radensb/eZ_Navi-Delete/wiki/7%29-How-to-Buy-an-Assembled-PWA-or-Bare-PCB#how-to-purchase-an-ez-navi-delete-pwa
[OSH Stencil]: https://www.oshstencils.com/
[Built Firmware]: https://github.com/radensb/eZ_Navi-Delete/blob/master/build/

