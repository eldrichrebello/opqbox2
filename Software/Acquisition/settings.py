import logging
import os

class Settings:
    def __init__(self):
        self.store = {}

    def loadFile(self, fname):
        try:
            file = open(fname)
            for line in file:
                line.strip()
                if len(line) == 0 or line[0] == '#':
                    continue
                items = line.split(":", 2)
                items = map(lambda item: item.strip(), items)
                if len(items) < 3:
                    continue
                key = items[0]
                val = items[2]
                if len(key) == 0 or len(val) == 0:
                    logging.warning("Malformed line in the config file:" + line)
                    continue
                self.store[key] = val
        except IOError:
            logging.fatal("Could not open the settings file " + fname)
            os._exit(1)
    def getKey(self, key):
        if key in self.store:
            return self.store[key]
        else:
            return None
