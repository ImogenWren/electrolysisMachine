# electrolysisMachine
 Schematics & Build Plan for DIY Electrolysis Machine for Hair Removal #transhealth


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
 - 1b. Model Power Supply []
2. Construct Current source prototype & Test. []
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

# Project Log

## Step 1: Model Voltage Controlled Current Source
- Current Servo, Voltage Controlled Current Source
 ![Op-Amp-Voltage-Controlled-Current-Source-Circuit-Diagram](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/14538ed3-4064-43b1-9a8e-5d472b6c1dae)
"The third requirement is the shunt resistor. Let's stick into 1ohms 2watt resistor. Additional two resistors are required, one for the MOSFET gate resistor and the other one is the feedback resistor. These two are required for reducing the loading effect. However, the drop between these two resistors is negligible.

Now, we need a power source, it is a bench power supply. There are two channels available in the bench power supply. One of them, the first channel is used to provide power to the Circuit and the other one which is the second channel used to provide the variable voltage to control the source current of the circuit. As the control voltage is applied from an external source, both channels need to be in the same potential, thus the ground terminal of the second channel is connected across the first channel ground terminal."

### LTspice modelling
Most linear response found. Will work with 3v3 logic
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/24817674-8046-4430-89f8-011a9acf5891)
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/ee70646f-9a43-44bd-b7c0-aad7430b6b55)

Increasing voltage to 27 to match origional (3x 9v)
Commands Used:
- `.dc V2 0 5 0.1`
- Now shows almost linear response across the entire control voltage range
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/08384ca2-f880-4f20-8bc2-eaac25e927ce)

Testing current setpoint given change in R3 (sweep from 0R to 10M)
Commands used:
- `.step param R3 list 0R 1R 10R 100R 1K 10k 100k 1000k 10000k`
- `.op`
- Shows constant current through changing load
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/4b596783-c94c-46f2-bc84-4b8f3fc0266a)

### Calculating DAC Resolution (10 bit)
- ADC max val = 1023
- Max Current output = 2.36 mA
- Max Current / Max ADC = 0.00231 mA per ADC
- Resolution of 0.002 mA is possible using Arduino ADC, though slight non-liniarity at the lowest control voltage
- MEETS DESIGN GOAL [G2] Resolution of 0.2mA
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/95e89b84-3562-49b1-9dfa-3c0feba3fbb1)
(Source - [eee-calculator](https://github.com/PanGalacticTech/engineering-calculator-2023)

### Accounting for PWM
As the control voltage from the microcontroller will be PWM, rather than a descrete voltage, this should be added to the simulation. This may not matter, as average current delivered will be the same, however in the origional design the current supplied was constant, not pulsed and I wish to at least attempt to replicate that. A decision can be made later which outcome is better, though this might require testing which will be hard to quantify if I am the only test subject.

### Origional Design modelling PWM control voltage
Arduino nano PWM frequency = 490.2 (this can be changed but why change if not required)
Period = 1/490.2 = 2.04 mS 
Model PWM frequency at 2 mS period, with 50% Duty
Using Commands: 
`.tran 0 1 0 0.1 startup` // DC Transfer for 1 second after startup <br>
`**PULSE(0 5 0 0 0 1m 2m 0)**` // Pulse voltage source <br>
` step param R3 list 0R 1R 10R 100R 1K 10k 100k 1000k 10000k` // step R3 to show constant current regardsless of load <br>
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/4bfb3f53-d18b-41ef-97b9-bb0ef0713afe)
This shows that the output will also be pulsed, the first attempt to remove this pulse will be the addition of a capacitor to the control voltage. Placed after the 100k resistor, this will form a low pass filter, for which the cutoff frequency must be pushed as low as possible to block all but DC.

Selecting 
> capacitor: 1 uF = cutoff_frequency: 1.59 Hz
> capacitor: 10 uF = cutoff_frequency: 0.16 Hz
> 
 Modelling with 1uF capacitor as these are cheaper and easier to source without going to electrolytic.
Using Commands: 
`.tran 0 1 0 0.1 startup` // DC Transfer for 1 second after startup <br>
`**PULSE(0 5 0 0 0 1m 2m 0)**` // Pulse voltage source <br>
` step param R3 list 0R 1R 10R 100R 1K 10k 100k 1000k 10000k` // step R3 to show constant current regardsless of load <br>

Tested with a few different C1 values, nothing showed significant DC smoothing untill 100 uF, shown below:

DC Voltage Ripple:

DC Current Ripple: 1.30 - 1.18 = 12 mA

Next try modelling with smaller value capacitors in paralellel

![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/f38141b7-cebf-494b-8194-32d6db9f25b3)

DC Current Ripple: 1.30 - 1.18 = 12 mA

NO change!
 100uF is likely optimal for this purpose, though introduces a slower current rise & fall time. 

### Testing capacative filter on output of OP-AMP to mosfet
Now lowpass filter with 10k, 

100uF = 0.16 cutoff frequency

![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/2d016e8b-6273-4a3a-913c-91facf9f598c)
Ramp takes over half a second to reach full power, this may assit in reducing pain or shock, or may prolong pain. Certainly will not be suitable for cutting power to probe.

![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/eab666d7-24b3-4b8b-b559-8453b0b1913d)


At 1u, ramp up is only 60mS, current ripple of: 1.26 - 1.224 = 0.036mA ripple this is very acceptable. Wave has also been smoothed to sinusoildal. Could this have an effect on pain?
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/1c03c5b5-0295-44e5-af2b-37906526d532)

## Step 1b: Model The Power Supply

_The power supply needs to be capable of providing the correct current considering all of the possible loads that a paitents skin could offer._
In order to specify the full requirements for the power supply, we could take data of different peoples skin resistivity, calculate averages, min & max, however. For the first prototypes we will bypass this, and attempt to replicate the power supply used in the origional project. 

This comprised of 3x 9v batteries, or 27v rail-to-rail.

For this implementation we wish to achieve this from a single 12v power supply at most, as these are the most common available. For this we could use a charge pump.

_For previous, failed attempts at specifying the power supply circuit, see the additional file `step-1b-charge-pump-circuits.md`_ <br>

### Attempt 2: 555 based charge pump
_The initial modelling using a charge pump IC with a +18 and -9v output seemed to be a dead end, so next we will try to model a 555 based charge pump with an output of ~+30v_

This next attempt will use this basic building block: <br>
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/edfd8b9f-4ee1-4631-9991-48a0bd2a2d45)
(circuits-diy.com)

![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/58773af9-933d-455b-8537-376ab458d9b0)
https://www.circuits-diy.com/charge-pump-circuit-getting-higher-voltage-from-low-voltage-source/


We could potentially use the microcontroller itself to provide the PWM for the charge pump, this may be explored later, but first we will keep things as simple as possible and model with a 555 timer. For the 555 circuit, I will be adapting a circuit origionally designed for a high voltage SMPS for use with Nixie Tubes, documented here:
https://pangalactictech.com/12v-dc-to-300v-dc-smps/
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/e9e7b69d-567d-44fb-a2f5-6acb06813e88)


NE555 Working Voltage: 4.5-16v, fits perfectly with the 12v supply voltage specification.

#### Modelling Charge Pump
_Initial Results Promising_
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/b9f7da40-d6aa-4cde-872a-96d45d52f22a)

stable 36v output given 12v input

next test CV frequency sweep - cant set up correctly
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/eacac20e-c940-4e67-9644-a53552f214a6)
Tested at different CV, rise time differed but max V output did not


Then test with varying loads

Decreased R8 t0 4k7, increased frequency to 114 KHz

## Simulating pulse in LTspice
_Finally learned to set waveform using frequency & duty cycle_

At duty cycle of:
0.2 = 21.3v output
other duty cycles no difference

but lower input v of 5v = lower output voltage. need additional diode stage or higher PWM voltage
   

- 

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

