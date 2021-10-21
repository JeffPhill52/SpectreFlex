import bluetooth
import serial
import select
import sys
import pyautogui
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

