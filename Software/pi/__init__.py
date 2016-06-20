import logging
import subprocess

logger = logging.getLogger(__name__)


def restart():
    logger.info("Restarting RaspberryPi!")
    subprocess.call(["reboot", "now"])


def restart_pypid():
    logger.info("Restarting pypid!")
    # TODO


def run_cmd(cmd, are_you_sure=False):
    if are_you_sure:
        logger.info("Running cmd {}".format(cmd))
        try:
            out = subprocess.check_output(cmd, shell=True)
            return out
        except subprocess.CalledProcessError as e:
            logger.error("Could not run {} got code {} and output {}".format(e.cmd, e.returncode, e.output))

if __name__ == "__main__":
    # Unit tests?
    print run_cmd("ls -alkoj", are_you_sure=True)