# Step 1 - Modelling a charge pump circuit
_Moving exploration done on virtual grounds here as it ended up being a dead end in the development tree_


### Attempt 1: Charge Pump IC

The Charge pump selected is based on the Klon charge pump, which delivers a +18v and -9v output, provided a 9v input, and is based on a readily available ILC7660S Charge pump IC. It can also handle voltages up to 12v, given extensive testing (although this may be slightly out of spec and not reccomended) It does offer an opporunity to regulate a 12v input voltage down to 9v to feed the charge pump a very consistant input voltage.

### Schematic
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/14f57c2c-94e5-44a7-b5dc-35a231df9f59)


### Issues with Charge Pump
- Unable to model circuit I have experience with as cannot find a model for ILC7660S or TL1044 IC
- In order to get the full 27v rail-to-rail voltage, the negative rail must be used as a virtual ground for the skin probe.
For this reason, I am not going to worry about modelling the charge pump, I know this circuit is cabable of outputting these voltages, however we must ensure that
we can modify the current servo implementation to account for the virtual ground, if this is even possible to do.

## First modelling with -9v virtual ground
- Note: To get this to run needed to use fixed power supply set to +9v, with positive rail grounded
![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/4676e8d9-eed6-4e2c-9d0a-c0c33432b56f)
- Showed no current controlability, but current output of ~1.7mA
- After boosting Vsupply to 18v (as if from charge pump) Then current output is able to deliver 2.2mA but still uncontrolled.

## Next trying different groundings (virtual/real)
- Ensure ap-amp is bipolar
- added protection diode to Vcontrol to protect microcontroller from negative voltages

Best option found so far using virtual grounds:

![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/8270e5dd-c864-42ee-b7d5-15ac27f09d07)

Not great: Controllable down to  ~0.65mA and up to ~1.8mA, must be able to go to 0.0 mA

## Trying to find best feedback resistor.
Used commands: <br>
- `.param Rval=0`
- `.step dec param Rval 0 100k 500` (Logarythmic step Rval
- Put `{Rval` in value to sweep

![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/6b9913ee-074c-4d83-bbd8-fc5da66994ee)

_Not quite sure what this graph is telling me, trying to expand the range available both up and down._
Immediate conclusions: 
- R1 should be less than 68k to avoid limiting current early.
- Run simulation with lower Vcontrol & compare.
- At Vcontrol = 2.5, current should be ~1.1 mA


![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/6edcc1e4-f4cd-434d-bc7d-47682173c56d)

- Actual results:
   - For any given feedback resistor value, current is above 1.25
   - Best guess from graph would require a 100k resistor but leaning towards 68k
 
- Arse

- Run again with Vcontrol=0v, should be 0mA
- At 100k, current is still 0.6mA

- I think 68k should be selected, then test R4 step value
- Step change R4 made no difference.
- Setting back to 10k
- try stepping R6 - currently 100k with R5 voltage divider 47r to GND
- INTERESTING RESULT:

### Playing around with voltage divider
_I like using a bigger resistor to GND if possible, so try increasing R6 to reduce control voltage
 ![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/f4139fc7-58e2-447e-8fd6-7d5222a55f6e)
This shows 0.0 mA reached at 220k input resistor. Will set to here then check upper Vcontrol output

max output is now only ~0.55 mA
- Try increasing R5
-  Must be above 120 to prevent limiting current. Very sensitive

![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/fa8068d1-3fce-4d40-bca5-a0edf3227206)

## Its nae goood

![image](https://github.com/ImogenWren/electrolysisMachine/assets/97303986/49355dd7-c073-47fd-ad7d-5b114d7c24da)


_Might have to think a little harder about whats happening in the circuit_
 
