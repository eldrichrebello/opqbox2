import time
import sys
import spidev


class DSP:
    def __init__(self):
        self.spi = spidev.SpiDev();
        self.spi.open(0,0);

        
def readAdc(channel):
    print spi.readbytes(1)
        
if __name__ == '__main__':
    try:
        while True:
            val = readAdc(0)
            time.sleep(1)
    except KeyboardInterrupt:
        spi.close() 
        sys.exit(0)
