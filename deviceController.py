from socket import socket
import commFuncs

def main():
    bleController = commFuncs.BLE_Controller()
    if bleController.discoverDevice():
        if bleController.connectDevice():
            while(True):
                controllerState = bleController.readDevice()
                print(controllerState)
                gyroState = controllerState[0:3]
                flexState = controllerState[3:7]
                acclState = controllerState[7:10]
                



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
