from logging import warn
from socket import socket

import pyautogui
import pydirectinput
import commFuncs
import time

def main(): 
    pyautogui.MINIMUM_SLEEP = 0
    pyautogui.PAUSE = 0
    pydirectinput.PAUSE = 0
    pydirectinput.MINIMUM_SLEEP = 0
    pydirectinput.FAILSAFE = False
    bleController = commFuncs.BLE_Controller()
    commands = commFuncs.controllerCommands()
    KeyValues = commands.GUI()   # KEYVALUES [ thumb , pointer , middle , ring]
    #print ("KeyValues",KeyValues)  # FOR TESTING KEYVALUES VARIABLES... PASSED AS LIST...

    if bleController.discoverDevice():
        if bleController.connectDevice():
            start = time.time()
            while(True):
                controllerState = bleController.readDevice()
                flexBytes = controllerState[0:4]
                switchByte = controllerState[4]
                acclBytes = controllerState[5:9]
                gyroBytes = controllerState[9:13]
                acclState = bleController.floatDecoder(acclBytes)
                gyroState = bleController.floatDecoder(gyroBytes)
                # print(controllerState)
                # print("Switch: ", switchByte)
                # print("Gyroscope: ", gyroState)
                roll_and_pitch = bleController.rollAndPitch(acclState)
                # print("Roll and Pitch: ", roll_and_pitch)
                end = time.time()
                commands.moveMouse(gyroState, end - start, flexBytes, roll_and_pitch)
                start = time.time()
                commands.buttonClicks(flexBytes, switchByte,KeyValues)



                    



    """
    try:
        while True:
            print(BTconnect.readGyro())
            print('bruh')
    except KeyboardInterrupt:
        BTconnect.close()
    """

if __name__ == "__main__":
    main()



