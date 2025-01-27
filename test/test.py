from NFDRS4Py import do_some_NFDRS_stuff, NFDRS4
import numpy as np

def main():
    air_temp = np.ones((4,2), dtype="float64")
    relhum = np.ones((4,2), dtype="float64") * 2.0
    precip = np.ones((4,2), dtype="float64") * 3.0
    wspd = np.ones((4,2), dtype="float64") * 4.0

    do_some_NFDRS_stuff(air_temp, relhum, precip, wspd)

    obj = NFDRS4()
    print(obj)
    print(type(obj))
    

if __name__ == "__main__":
    main()
