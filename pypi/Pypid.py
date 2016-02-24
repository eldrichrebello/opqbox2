import logging
import net
from tasks import Tasks
from threading import Event
import sys
import time

logger = logging.getLogger(__name__)

if __name__ == "__main__":
    # Setup logging
    logging.basicConfig(format='%(asctime)s -- %(levelname)s:%(name)s/%(filename)s.%(funcName)s[%(lineno)d]:%(message)s', level=logging.DEBUG)

    # Fire up the internet connection task
    conn_task_stopped = Event()
    conn_task = Tasks.NetworkConnectionTask(conn_task_stopped, 5)
    conn_task.start()

    # Fire up SSID tast
    ssid_task_stopped = Event()
    ssid_task = Tasks.SsidTask(ssid_task_stopped, 20)
    ssid_task.start()

    while True:
        sys.stdout.write("pypid # ")
        c = raw_input()
        if c == "q":
            logger.info("Killing tasks")
            conn_task_stopped.set()
            ssid_task_stopped.set()

            while conn_task.isAlive() or ssid_task.isAlive():
                logger.info("Waiting for all threads to die...")
                time.sleep(2)

            logger.info("Goodbye")
            exit(0)


