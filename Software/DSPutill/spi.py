import RPi.GPIO as GPIO
import opq_pb2
import datetime
import Queue
import array
import sys
import isp
import time
from threading import Thread, Event
import zmq

class Acquisition(Thread):
    # Data ready gpio number
    PI_GPIO_READY_FLAG_PIN = 18
    DSP_PACKET_SIZE = 408

    def __init__(self, outputQueue):
        Thread.__init__(self)
        self.daemon = True
        self.outputQueue = outputQueue
        isp.init()
        # Setup spi
        self.device = open("/dev/opq0", "r")
        # setup exit event
        self.event = Event()

    def __del__(self):
        self.device.close()

    # Make a microsecond unix timestamp in UTC. There has to be a better way....
    def make_timestamp(self):
        now = datetime.datetime.utcnow()
        epoch = datetime.datetime.utcfromtimestamp(0)
        return int((now - epoch).total_seconds() * 1000)

    # This is a threaded callback that runs every time the DSP signals that there is data available
    def read_adc(self):
        message = opq_pb2.DataMesaage()
        for i in range(0,60):
            cycle = message.cycles.add();
            # Read some bytes. Deserialize them into packets.
            bytes = self.device.read(self.DSP_PACKET_SIZE)
            arr = array.array('h')
            arr.fromstring(bytes[0:self.DSP_PACKET_SIZE])
            cycle.time = self.make_timestamp()
            cycle.data.extend(arr.tolist())
        message.id = 0;
        self.outputQueue.put(message, True, 0.5)

    def run(self):
        while not self.event.is_set():
            self.read_adc()

    def stop(self):
        self.event.set()
        self.join()


#if __name__ == '__main__':
def main():
    q = Queue.Queue()
    dsp = Acquisition(q)

    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect("tcp://192.168.1.144:5555")
    dsp.start()
    count  = 0

    while(count < 600):
        message = q.get()
        socket.send(message.SerializeToString())
    dsp.join()



import cProfile
import re
cProfile.run('main()')