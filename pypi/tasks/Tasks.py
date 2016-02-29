import logging
import threading
import net

from bottle import app as server

logger = logging.getLogger(__name__)

class NetworkConnectionTask(threading.Thread):
    def __init__(self, event, delay):
        super(NetworkConnectionTask, self).__init__()
        self.stopped = event
        self.delay = delay

    def run(self):
        logger.debug("NetworkConnectionTask started")
        while not self.stopped.wait(self.delay):
            if not net.check_connection():  # todo and not already hot spot
                net.connect_hotspot()

        logger.debug("NetworkConnectionTask stopped")


class SsidTask(threading.Thread):
    def __init__(self, event, delay):
        super(SsidTask, self).__init__()
        self.stopped = event
        self.delay = delay

    def run(self):
        logger.debug("SsisTask started")
        while not self.stopped.wait(self.delay):
            pass
            # Update SSID list

        logger.debug("SsisTask stopped")


class BottleTask(threading.Thread):
    def __init__(self, event, delay):
        super(BottleTask, self).__init__()
        self.stopped = event
        self.delay = delay

    def run(self):
        logger.debug("BottleTask started")
        server.run_server(5000)
        while not self.stopped.wait(self.delay):
            pass

        logger.debig("BottleTask stopped")
