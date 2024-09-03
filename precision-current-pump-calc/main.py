
#see figure 4 Precision Current Pump

'''

Av = R4/R2
R2 = R3
R4 = R5

Iout = (Vin * Av)/R1
Iout * R1 = Vin * Av
R1 = (Vin * Av)/Iout

'''
ohm = 1
k = 1000
M = k * 1000

# Set up Calculation Variables
Vin = 27.0

I_out_mA = 2.0
I_out_A = I_out_mA/1000

R2 = 1.33 * M
R3 = R2
R4 = 100
R5 = R4

def calc_Iout(R1, Av, Vin):
    Iout = (Vin * Av)/R1
    Iout_mA = Iout * 1000
    return Iout_mA

def print_Iout_calc(R1, R2, R3, R4, R5, Vin):
    Av = calc_av(R2, R4)
    Iout = calc_Iout(R1, Av, Vin)
    R1 = scale_resistor(R1)
    R2 = scale_resistor(R2)
    R3 = scale_resistor(R3)
    R4 = scale_resistor(R4)
    R5 = scale_resistor(R5)
    print(f"Vin: {Vin} V")
    print(f"R1: {R1}")
    print(f"R2: {R2}")
    print(f"R3: {R3}")
    print(f"R4: {R4}")
    print(f"R5: {R5}")
    print(f"Av: {Av}")
    print(f"Iout: {Iout} mA")


def calc_av(R2, R4):
    Av = R4 / R2
    return Av


def calc_resistor(Av):
    R1 = (Vin * Av) / I_out_A
    return R1

def scale_resistor(val):
    if val >= M:
        val = val/M
        scale = "M"
    elif val >= k:
        val = val/k
        scale = "k"
    else:
        scale = "ohm"
    return val, scale




def main():
    #print("R1 = ")
    Av = calc_av(R2, R4)
    print(f"Av: {Av}")
    R1 = calc_resistor(Av)
    R1_print = scale_resistor(R1)
    print(f"R1: {R1_print}")
    #print(R1[0])
    print("\ncheck reverse calc\n")
    Iout = calc_Iout(R1, Av, Vin)
    print(f"Iout: {Iout} mA")
    print_Iout_calc(1, R2, R3, R4, R5, Vin)


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main()


# See PyCharm help at https://www.jetbrains.com/help/pycharm/
