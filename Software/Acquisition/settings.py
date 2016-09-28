class Settings:
    def __init__(self):
        self.store = {}

    def loadFile(self, fname):
        file = open(fname)
        for line in file:
            if len(line) == 0 or line[0] == '#':
                continue
            items = line.split(":", 2)
            items = map(lambda item: item.strip(), items)
            if len(items) < 3:
                continue
            key = items[0]
            val = items[2]
            if len(key) == 0 or len(val) == 0:
                continue
            self.store[key] = val

    def getKey(self, key):
        if key in self.store:
            return self.store[key]
        else:
            return None
