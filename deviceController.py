from socket import socket
import commFuncs

def main():
    BTconnect = commFuncs.serConnection("COM3", "COM4", '98:D3:21:F4:98:50')

    try:
        while True:
            print(BTconnect.readGyro())
            print('bruh')
    except KeyboardInterrupt:
        BTconnect.close()

if __name__ == "__main__":
    main()
