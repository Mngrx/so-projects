import os
import socket
import time as t
from threading import Thread


HOST = '192.168.7.1'     # Endereco IP do Servidor
PORT = 13000            # Porta que o Servidor esta

class digitalPort:

    def __init__(self, port, mode):
        os.system("echo " + str(port) + " > /sys/class/gpio/export")
        os.system("echo " + str(mode) + " > /sys/class/gpio/gpio" + str(port) +"/direction")
        self.port = port
        self.mode = mode
        self.path = "/sys/class/gpio/gpio" + str(port) +"/value"

    def readValue(self):
        if self.mode == "in":
            file = open(self.path)
            return int(file.readline())
        else:
            return "Invalid"

    def setValue(self, value):
        if self.mode == "out":
            os.system("echo " + str(value) + " > " + self.path)            
        else:
            return "Invalid"
        

class analogPort:
    
    def __init__(self, port):
        self.path = "/sys/bus/iio/devices/iio:device0/in_voltage"+ str(port) +"_raw"
        self.port = port
    
    def readValue(self):
        file = open(self.path)
        return int(file.readline())


class Enviador(Thread):
    def __init__(self, button, ldr, pot):
        Thread.__init__(self)
        self.button = button
        self.ldr = ldr
        self.pot = pot

    def run(self):        
        tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        dest = (HOST, PORT)
        tcp.connect(dest)
        
        while 1:
            msg = "0," + str(self.button.readValue())
            tcp.send(msg)
            msg = "1," + str(self.ldr.readValue())
            tcp.send(msg)
            msg = "2," + str(self.pot.readValue())
            tcp.send(msg)
            t.sleep(0.5)

        tcp.close()



def main():
    
    button = digitalPort(115, "in")
    ldr = analogPort(3)
    pot = analogPort(5)

    enviador = Enviador(button, ldr, pot)

    enviador.start()

main()
