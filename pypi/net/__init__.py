import logging
import urllib2

logger = logging.getLogger(__name__)


def get_ssids():
    logger.debug("Retrieving SSIDs")
    return [
        ("CSLD", "WEP"),
        ("network1", "OPEN"),
        ("fedz Only", "WPA"),
        ("free doughnuts", "WPA"),
        ("Linksys", "OPEN"),
        ("Cisco", "OPEN"),
        ("HP Printer #308", "WEP")
    ]


def connect_hotspot():
    logger.debug("Connecting as hotspot")


def disconnect_hotspot():
    logger.debug("Disconnecting as hotspot")


def connect_to_network(ssid, skey=None):
    logger.debug("Connecting to network {}", ssid)


def get_connection_type():
    return ""


# Adapted from http://stackoverflow.com/a/3764660
def check_connection():
    try:
        response = urllib2.urlopen('http://manoa.hawaii.edu', timeout=3)
        return True
    except urllib2.URLError as err:
        pass
    return False
