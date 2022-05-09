# electrolysisMachine
 Schematics & Build Plan for DIY Electrolysis Machine for Hair Removal #transhealth



[Reddit Informational Source](https://www.reddit.com/r/electronics/comments/8ek9dt/i_built_an_electrolysis_machine_epilator/)

## System Description

_It's battery powered. epilator circuit runs off 3 9v batteries in series, ammeter and voltmeter run off a separate supply, from a usb battery pack. Device delivers a voltage thats adjustable from 6v to 22v. Current sink (at probe) is adjustable from 0mA to 2mA. Supply is run through ecg electrode which is placed right next to the site I'm working on. A 555 timer is used to pulse the current sink. I'm using professional electrolysis probes, F4 shank stainless steel. Probe housing is made from a d-sub female crimp pin soldered to a wire and covered with a piece of heat shrink tubing. (F4 size probe fits snug into the socket)._ (abbxrdy, 2018) Accessed: (Reddit 2022)


## System Use Guide

_To adjust current sink, toggle the test switch an turn the current knob, the value is displayed on the ammeter. To use, slide the test switch back. When the probe is inserted, you hit the trigger button and it delivers the current for a specified time. You can see how much it's sinking. If it's sinking less than what it was set for you can increase the voltage to overcome the resistance between the probe and the electrode. The setting that I use is 0.5mA at 15v. This will kill a pubic hair in 10 seconds without much discomfort if the probe is correctly inserted into the hair follicle.
For more information on what size probes to use for what hairs, and how much current to deliver over whatever time period you can search for a "units of lye" chart_

#### Exposure Requirements

_for me, the settings I'm using are 15 volts with 0.5mA sink. This setting treats a pubic hair in 10-15 seconds when needle is placed properly. It just slides right out._



## Needles & Additional Items Required

https://www.texaselectrolysissupply.com/

Ballet brand, f shank, size 4



### Future Improvements for Reddit Version

_second revision of this thing with a lot more safety considerations. The current source will be galvanically isolated from the main supply and have an adjustable current source configurable from 0mA to 2mA and a fixed current sink set at 2mA. The trigger circuit will use an opto isolator and there will be a current monitor on the isolated area that passes it's reading outside through an iso op amp. If the probe sinks more than 2mA the machine will shut down._


# Personal Thoughts and Reflection

## TODO:
- Replace 555 with microcontroller


## Current Schematic

