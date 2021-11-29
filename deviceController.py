from socket import socket
import commFuncs

def main():
    bleController = commFuncs.BLE_Controller()
    if bleController.discoverDevice():
        if bleController.connectDevice():
            while(True):
                controllerState = bleController.readDevice()
                flexState = controllerState[0:4]
                acclState = controllerState[4:8]
                print("Flex State: ", flexState)
                print("Accl State: ", acclState)                



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
