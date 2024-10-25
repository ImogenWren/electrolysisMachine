# electrolysisMachine
 Schematics & Build Plan for DIY Electrolysis Machine for Hair Removal #transhealth

## Notes
_I have just been made aware of someone who is following the origional reddit design, and is quite far into the process with PCBs already in production. Please find a link to this project here: https://hexbear.net/post/2795875_

However I am going to carry on with my design, as I believe my version will be far more useable for those that dont have electronic test equipment to hand.



## System Description
This project is an attempt to build a digitallly controlled version of this DIY Hair Electrolysis machine found on the reddit post [i_built_an_electrolysis_machine_epilator](https://www.reddit.com/r/electronics/comments/8ek9dt/i_built_an_electrolysis_machine_epilator/).
The origional design requires external ampmeters to calibrate, and relies on manually tuning and testing the circuit to ensure settings are correct before delivering current to the hair follical.

To make this project accessable, and build in the ability to add additional safety system, the analog elements should be replaced with digital electronics. The digital electronics should be based around the Arduino Framework to make it as easy as possible for others to build this project.
Once the circuit has been proven, a PCB will be designed to allow others to easy replicate the project.

The hope is to make this a complete open-source, safe, cheap and easy system to build and use, to maximise the impact to the communities that are currently denied access to private hair removal services.

This project is open-source and welcomes collaborators on everything from project goals, design, production and testing!

NOTE: _This is an attempt to design a medical product to deliver current through a paitents skin, within in an open source framework.
Any attempt to follow the designs or guides within this repo are undertaken ENTIRELY at the users own risk._ 

## Design Goals
- [G1] Safe & Consistant operation of a constant current source, able to deliver up to 2.0 mA in pulses of up to 10s between probes attached to users skin.
- [G2] Current Source should be controllable down to 0.0 mA, with a resolution of at least 0.2 mA and accuracy of +-0.2 mA. (Exact values to be reviewed)
- Timer pulse should be controllable down to 1.0 mS, with a resolution of at least 0.5 mS, and accuracy of +-0.2 mS. (Exact values to be reviewed)
- 0-2 mA current to voltage sensor with a resolution of at least 0.2 mA and accuracy of +-0.2 mA (Exact values to be reviewed)
- Operation from 3S or less LiPo battery pack. (Can be reviewed and voltage increased if nessissary to achieve other design goals)
- Battery Management System (BMS) with USB-C (If 5V is sufficient to charge) AND 2.1mm DC jack (or similar generic connector) included.
- Passive Safety system to cut power if 2.0 +-0.2 mA is exceeded.
- Passive safety system to cut power if 10s pulse time is exceeded.
- Microcontroller will be AtMega328p OR Arduino Nano Dev board for ease of replication. 
- Simple to operate user interface:
 - Large display for SET_CURRENT, PULSE_TIME.
 - Settings menu to change MAX_CURRENT, MAX_PULSELENGTH
 - 1x Rotary Encoder with button for user inputs.
 - 1x TRIGGER button
 - 1x CANCEL button
 - Save at least 3x User profiles for quick settings changes
- 3D printable case to contain both electronics, battery system and probes.
- Clear & Simple Build Manual
- Clear & Simple operation instructions (Will need to collaborate to ensure clear communication of medical information)

### Strech Goals
- 3D printable case to include hygenic supplies required for safe operation.
- Chargeable on 5V USB, or 12V power.

## Project Steps:
_These steps are each created with the intent of de-risking the project as quickly as possible,
by focusing on small functional prototypes that can be intergrated at the earliest opportunity,
avoiding increasing complexity untill individual sub-systems are proven._

1. Model voltage controlled, 0-2mA constant current driver in LTspice. [x]
 - 1b. Model Power Supply [x]
2. Construct Current source prototype & Test. []
   - 2b. Construct and test power supply. [x]
3. Model 0-2 mA current to 0-5 V sensing circuit in LTspice. []
4. Construct current sensor prototype & test. []
5. Integrate current sensor with Arduino controller and test ability to meet sensitivity requirements. []
6. Integrade constant current driver with Arduino controller, including manual (pot) controls, and test accuracy of output pulse against requirements. [] <br>
- 6b. Create Alpha Version firmware for manual operation of the PULSE_OUTPUT  [] 
7. Model circuit protection concepts in LTspice. []
8. Integrate passive circuit protection to existing prototypes (if possible) and test against requirements.[]
9. Integrate User Interface with existing prototypes and test function of dsiplay & all input devices. []
10. Finalise schematic & Design PCB [] <br>
- 10b. Start design of 3D printed enclosure, to coencide with PCB design (Though should not block PCB design or production) []
11. Develop Beta version firmware including Functional user interface. []
12. Build Production prototype. []
13. Test Beta Firmware with Production prototype. []
14. Release Verified V1.0.0 PCB design & Make PCBs available to purchase. []
15. Release production Firmware. []
16. Develop "DIY Kit" Including build & user guides. []

## Design: Schematic
_The schematic is the most up to date & tested version of each sub system_

### Power Supply
This circuit has been tested with a 12v power supply, driven from a microcontroller. Duty cycle did not affect voltage generated untill the extremes.
Tested using microcontroller for PWM signal, finished version will be driven by 555 timer. <br>

- Use LM317 to regulate to 30v (https://www.homemade-circuits.com/how-to-build-simplest-variable-power/) NOTE: This could maybe be used as current source
- ![image](https://github.com/user-attachments/assets/24e29666-7bad-4582-8390-38f74036ad16)
- ![image](https://github.com/user-attachments/assets/abcdb6d3-724f-4fe5-a21f-4f8256e90d5b)



![image](https://github.com/user-attachments/assets/d14e9ac6-de4b-4b2e-9438-3b823b4669ad)
![image](https://github.com/user-attachments/assets/a27c9824-72dd-4d0a-b0ae-ddccf8a522c5)
<br>
555 Circuit Values:
- R1: 100k
- R2: 1.5k (sets frequency to 48kHz)
- C1: 10n (sets frequency to 48kHz)

Breadboarded & tested, can deliver 2.09 mA @ 32v, unloaded maxV = 45



### Voltage Controlled Current Source
- Concept <br>
![328051958-14538ed3-4064-43b1-9a8e-5d472b6c1dae](https://github.com/user-attachments/assets/6d9a9827-29ac-48bd-b1d4-6e73d20aa809)

<br>
- execution <br>

_Shows circuit at Vcontrol == 5 V, use to validate against physical circuit_
![image](https://github.com/user-attachments/assets/3ad14dfc-0747-4236-9a32-62735d58c61e)

#### Alternative Option found by 410bdf
![image](https://github.com/user-attachments/assets/2e2d6040-d90c-4499-95a7-78fac6effc9c)

#### Parts Values:
```
Op Amp: TLV9304 
Iout: 2.0 mA 
Vin: 27.0 V
R1: (1, 'ohm')
R2: (1.33, 'M')
R3: (1.33, 'M')
R4: (100, 'ohm')
R5: (100, 'ohm')
Av: 7.518796992481203e-05
Iout: 2.030075187969925 mA
```
_Find the python script for this calculation in `precision-current-pump-calc` folder_ <br>
This did not model correctly, so adjusted values are:
```
Vin: 27 V
R1: (1, 'ohm')
R2: (220.0, 'k')
R3: (220.0, 'k')
R4: (100, 'ohm')
R5: (100, 'ohm')
Av: 0.00045454545454545455
Iout: 12.272727272727272 mA  ?? Python calc is order of magnitude out
```
![image](https://github.com/user-attachments/assets/8954ac02-a318-4e4e-9b0f-4eea0c766c71)
_Modelling Operating Point @ 5V control voltage, current limited to 2.35 mA_
- Stable current output across stepped resistor value
- Linear scaling between Vin & Iout

_Modelling I_out for V_ctrl = 0, 5 V_ <br>
![image](https://github.com/user-attachments/assets/e01eb0ce-68ae-4b9d-9c04-b90b4d97aef3)


_Modelling I_out @ V_ctrl = 5 V, R6 = 1, 1 M_<br>
![image](https://github.com/user-attachments/assets/d0bd63fc-7ccd-4970-87b9-9870a6172373)

## Functional Prototype Layout
### Power Supply 12v to ~30 V
_Need testing to find out actual limit of regulator output
![image](https://github.com/user-attachments/assets/94275945-bf1e-4862-9837-fafd1be8954e)

### Current Servo V1 (Untested)
_Untested_
![image](https://github.com/user-attachments/assets/be710a49-dcf4-48f8-8c7a-a6b947073019)


>
>
------------------------------------------------------------------------
>
>


# State-of-the-Art

### Description of the Existing Reddit Design

_It's battery powered. epilator circuit runs off 3 9v batteries in series, ammeter and voltmeter run off a separate supply, from a usb battery pack. 
Device delivers a voltage thats adjustable from 6v to 22v. Current sink (at probe) is adjustable from 0mA to 2mA. 
Supply is run through ecg electrode which is placed right next to the site I'm working on. A 555 timer is used to pulse the current sink. 
I'm using professional electrolysis probes, F4 shank stainless steel. Probe housing is made from a d-sub female crimp pin soldered
to a wire and covered with a piece of heat shrink tubing. (F4 size probe fits snug into the socket)._
(abbxrdy, 2018) Accessed: (Reddit 2022)


## Existing System Use Guide

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
![electrolysisMachine_Schematic_source](https://user-images.githubusercontent.com/53580358/167491659-b3adbdfa-fd45-4e4e-a239-ae5a97defdb3.png)

