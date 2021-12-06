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
    
    bleController = commFuncs.BLE_Controller()
    commands = commFuncs.controllerCommands()
    bleController.GUI()

    if bleController.discoverDevice():
        if bleController.connectDevice():
            start = time.time()
            while(True):
                controllerState = bleController.readDevice()
                flexBytes = controllerState[0:4]
                acclBytes = controllerState[4:8]
                gyroBytes = controllerState[8:12]
                acclState = bleController.floatDecoder(acclBytes)
                gyroState = bleController.floatDecoder(gyroBytes)
                #print("Gyroscope: ", gyroState)
                roll_and_pitch = bleController.rollAndPitch(acclState)
                #print("Roll and Pitch: ", roll_and_pitch)
                end = time.time()
                commands.moveMouse(gyroState, end - start, flexBytes, roll_and_pitch)
                start = time.time()
                commands.buttonClicks(flexBytes)



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



