# project-log: Electrolysis Machine
_Project log showing complete development notes_
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

tested with additionally diode stage and suitable load:
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/8c3fedfa-c0ec-44b7-ad5d-a58bdc3bba4f)

going to try adding one more diode stage
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/26914682-de31-4aea-aa29-c1ed5b355d51)
 Peak voltage of ~30v, capable of just over 30mA of current, 

Decreased R8 t0 4k7, increased frequency to 114 KHz

## Simulating pulse in LTspice
_Finally learned to set waveform using frequency & duty cycle_

At duty cycle of:
0.2 = 21.3v output
other duty cycles no difference

but lower input v of 5v = lower output voltage. need additional diode stage or higher PWM voltage
   
### Current of PWM Signal
I decided to check the current required to deliver the simulations shown above, during inrush

- First looking at inrush current:
- -![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/9c0d51df-91b0-468d-bce9-eb18cf90f545)
- ~100 A


- Then looking at each pulse as steady state is reached:
- ![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/27de97b1-40ea-42b9-877b-afaa9c00eec6)
- Still pulling about 2.5 A

This is absolutly not suitable, microcontroller pins can source about 20-40mA at max. For longevity, we wil try and keep it to about 20 mA

- R = V/I
- R = 5/0.020 = 250 ohm
- So lets put a series resistor in the PWM path to limit the current to this value

-Current limited to 18mA BUT Vout now limited to 11.5 v
- ![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/defa17e2-9757-4180-9f73-e390d0654a81)

- So PWm needs to be amplified, simple transistor will do or does this charge pump approach have a fundemental flaw?

