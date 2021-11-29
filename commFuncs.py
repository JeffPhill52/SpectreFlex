from bleak.backends.device import BLEDevice
import bluetooth
import asyncio
import bleak
import serial
import select
import sys
import pyautogui

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

