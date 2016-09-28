import logging
import signal
import os
from settings import Settings
from zmqlistener import ZmqListener


def handler(signum, frame):
    logging.info("Caught SIGINT. Exiting")
    os._exit(0)


def main():
    signal.signal(signal.SIGINT, handler)

    settings = Settings()
    settings.loadFile("settings.set")
    log_path = settings.getKey("log.acq_path")
    if not log_path:
        log_path = "acq.txt"
    logging.basicConfig(filename=log_path, level=logging.INFO)
    # define a Handler which writes INFO messages or higher to the sys.stderr
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    # set a format which is simpler for console use
    formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
    # tell the handler to use this format
    console.setFormatter(formatter)
    # add the handler to the root logger
    logging.getLogger('').addHandler(console)
    logging.info("Staring up")
    zmq = ZmqListener(settings)
    zmq.run()


main()
