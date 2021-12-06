from bleak.backends.device import BLEDevice
import bluetooth
import asyncio
import bleak
import serial
import select
import sys
import math
import pyautogui
import pydirectinput
import PySimpleGUI as sg


# Oversees BLE Communication with Spectre Flex Controller
class BLE_Controller:
    """ Any method preceeded by '__' is a private method and should not be accessed outside the class """

    # The init function by default has all of the information it needs to connect to a standard spectre flex device
    def __init__(self, device_name="Spectre Flex", service_UUID = "6F457C49-2D99-4334-B087-B7CD9E62D807", characteristic_UUID = "6F457C49-2D99-4334-B087-B7CD9E62D808") -> None:
        self.device_name = device_name
        self.service_UUID = service_UUID
        self.char_UUID = characteristic_UUID

    # Attempts to discover the Spectre Flex Controller
    """ If the device is found returns a BLEDevice else returns None """
    async def __discoverDevice(self) -> BLEDevice:
        devices = await bleak.BleakScanner.discover()
        for d in devices:
            if self.device_name == d.name:
                return d
        return None

    def discoverDevice(self) -> bool:
        self.device = asyncio.run(self.__discoverDevice())
        if self.device != None:
            return True
        return False

    async def __connectDevice(self) -> bool:
        self.client = bleak.BleakClient(self.device.address)
        try:
            await self.client.connect()
            return True
        except Exception as e:
            print(e)
            return False
    
    def connectDevice(self):
        return asyncio.run(self.__connectDevice())

    async def __readDevice(self):
        char_Response = await self.client.read_gatt_char(self.char_UUID)
        return char_Response
    
    def readDevice(self):
        return asyncio.run(self.__readDevice())

    async def __disconnectDevice(self):
        await self.client.disconnect()
    
    def disconnectDevice(self):
        asyncio.run(self.__disconnectDevice())

    #     24         0         44          6
    # 0001 1000  0000 0000  0010 1100  0000 0110
    # Expected: 0.48 0.01 0.9
    # Received: 0.48 0.01 0.88

    # Decodes the x, y, and z values of the accelerometer from a 4 byte array
    def floatDecoder(self, acclState):
        bitArr0 = list('{0:08b}'.format(acclState[0]))
        bitArr1 = list('{0:08b}'.format(acclState[1]))
        bitArr2 = list('{0:08b}'.format(acclState[2]))
        bitArr3 = list('{0:08b}'.format(acclState[3]))
        bitArr0.reverse()
        bitArr1.reverse()
        bitArr2.reverse()
        bitArr3.reverse()

        if int(bitArr0[7]) == 1:
            x_sign = -1
        else:
            x_sign = 1

        if int(bitArr1[7]) == 1:
            y_sign = -1
        else:
            y_sign = 1

        if int(bitArr2[7]) == 1:
            z_sign = -1
        else:
            z_sign = 1

        x = float(bitArr0[6])
        y = float(bitArr1[6])
        z = float(bitArr2[6])

        x_dec = float(bitArr3[0])
        y_dec = float(bitArr3[1])
        z_dec = float(bitArr3[2])

        for i in range(0,6):
            x_dec += float(bitArr0[i])*pow(2,i + 1)
            y_dec += float(bitArr1[i])*pow(2,i + 1)
            z_dec += float(bitArr2[i])*pow(2,i + 1)

        x_dec = float(x_dec)/float(100)
        y_dec = float(y_dec)/float(100)
        z_dec = float(z_dec)/float(100)


        x += x_dec
        y += y_dec
        z += z_dec

        x = x*x_sign
        y = y*y_sign
        z = z*z_sign

        return [x, y, z]

    # Calculates the roll and pitch of the spectre flex
    # acclState: [x, y, z]
    # returns: [roll, pitch]
    def rollAndPitch(self, acclState):
        # roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
        # pitch = atan(-1 * X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
        return [math.atan(acclState[1] / math.sqrt(pow(acclState[0] + 0.0001, 2) + pow(acclState[2] + 0.0001, 2))) * 180 / math.pi, math.atan(-1 * (acclState[0]) / math.sqrt(pow(acclState[0]+0.0001, 2) + pow(acclState[2] + 0.0001, 2))) * 180 / math.pi ]


class controllerCommands:

    def __init__(self) -> None:
        #self.screenSize = pyautogui.size()
        pyautogui.moveTo(1920, 1080)
        #print("Screensize: ",self.screenSize)
        pyautogui.FAILSAFE = False

        #self.Position = [0, 0, 0]

        #self.tolerance = 0.1

    # Takes the roll and pitch of the spectre flex and uses it to move the mouse
    def moveMouse(self, gyroScope, time, flexBytes, roll_and_pitch):
        # math.sqrt(pow(gyroScope[1], 2) + pow(gyroScope[2], 2))
        
        #if gyroScope[0] > self.tolerance:
        #    self.Position[0] = (self.Position[0] + gyroScope[0]*time)
        #if gyroScope[1] > self.tolerance:
        #    self.Position[1] = (self.Position[1] + gyroScope[1]*time)
        #if gyroScope[2] > self.tolerance:
        #    self.Position[2] = (self.Position[2] + gyroScope[2]*time)

        #print("Position: ", self.Position)        
        pydirectinput.move(int(-gyroScope[2]*8000), int(-gyroScope[1]*6000))

    def buttonClicks(self, flexBytes):
        if int(flexBytes[2]) == 1:
            pydirectinput.click()

        if int(flexBytes[3]) == 1:
            pydirectinput.press('r')
        
        if int(flexBytes[1]) == 0:
            pydirectinput.keyDown('w')

        elif int(flexBytes[1]) == 1:
            pydirectinput.keyUp('w') 

    def GUI(self):
        UPKEY = 'z'
        RIGHTKEY = 'z'
        LEFTKEY = 'z'
        DOWNKEY = 'z'
        sg.theme('Reds')
        layout = [ [sg.Image('SPECTREFLEX.png', 'center' )]  , [sg.Text("Thank you for using spectre flex. Please input your controls below!")], #INTRODUCTION
        [sg.Text("Please Input Keyboard Up Control"),sg.InputText( key = "UPBOX")],
        [sg.Text("Please Input Keyboard Right Control"),sg.InputText( key = "RIGHTBOX")],
        [sg.Text("Please Input Keyboard Down Control"),sg.InputText( key = "DOWNBOX")],
        [sg.Text("Please Input Keyboard Left Control"),sg.InputText( key = "LEFTBOX")],
        [sg.Button('OK') , sg.Button('Close Window')]  ] #BUTTONS

        window = sg.Window("Spectre-Flex Controller GUI", layout, margins=(100,100) )


        while True:
            event, values = window.read()
            if event == "Close Window" or event == sg.WIN_CLOSED:
                break 
            
            if event == "OK":
                UPKEY = values["UPBOX"]
                RIGHTKEY = values["RIGHTBOX"]
                DOWNKEY = values["DOWNBOX"]
                LEFTKEY = values["LEFTBOX"]
                layout2 = [[sg.Text("You have changed your controls. Please Exit the spectreflex GUI completely and continue with your experience!")]]
                CONFIRM = sg.Window("Confirmation" , layout2 )
                while True: 
                    event, values = CONFIRM.read()
                    if event == sg.WIN_CLOSED:
                        break


        window.close()

        print("Control Values: ", UPKEY, RIGHTKEY,DOWNKEY,LEFTKEY)



class serConnection:
    def __init__(self, incomingPort, outgoingPort, deviceAddr) -> None:
        self.deviceAddr = deviceAddr

        #test size of monitor. then move cursor down slightly RELATIVE to current placement.
        #print(pyautogui.size())
        #pyautogui.moveRel(0, 500, duration = 5)
        #pyautogui.click()
        #pyautogui.press('W')
        # Device connection
        print("Connecting to BT device")
        try:  #Attempting to Connect
            self.sock = bluetooth.BluetoothSocket()
            self.sock.connect((self.deviceAddr, 1))
        except Exception: #connection Failed
            print("Failed to connect to the device")
            print("Make sure the device is already paired")
            print("The program will now exit\n")
            sys.exit(1)
        #connection Sucessful
        print("BT device connected!\n")

        #self.incoming = serial.Serial(incomingPort, 9600, timeout = 2)
        #self.outgoing = serial.Serial(outgoingPort, 9600, timeout = 2)

    def readGyro(self) -> str:
        try:
            r, _, _ = select.select([self.sock], [], [])
            if r:
                # ready to receive
                return self.sock.recv(1)
        except:	
            return 'exception'

    
    def readFlex(self) -> str:
        try:
            r, _ = select.select([self.sock], [], [])
            if r:
                # ready to receive
                return self.sock.recv(1)
        except:	
            return 'exception'


    def close(self) -> None:
        self.sock.close


