#!/usr/bin/python

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

    # Event to kill threads
    stop_event = Event()

    conn_task = Tasks.NetworkConnectionTask(stop_event, 5)
    ssid_task = Tasks.SsidTask(stop_event, 20)
    bottle_task = Tasks.BottleTask(stop_event, 20)

    tasks = [conn_task, ssid_task, bottle_task]

    for task in tasks:
        task.start()

    def find_alive():
        alive = []
        for task in tasks:
            if task.isAlive():
                alive.append(task)
        return alive

    while True:
        sys.stdout.write("pypid # ")
        c = raw_input()
        if c == "q":
            logger.info("Killing tasks")
            stop_event.set()

            while len(find_alive()) > 0:
                logger.info("Waiting for all threads to die... " + str(find_alive()))
                time.sleep(2)

            logger.info("Goodbye")
            exit(0)


