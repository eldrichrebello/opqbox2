import NetworkManager
import logging
import urllib2

import wifiap as ap

logger = logging.getLogger(__name__)


def get_ssids():
    logger.debug("Retrieving SSIDs")
    for dev in NetworkManager.NetworkManager.GetDevices():
        if dev.DeviceType != NetworkManager.NM_DEVICE_TYPE_WIFI:
            continue
        return map(lambda ap: ap.Ssid, dev.SpecificDevice().GetAccessPoints())


def connect_hotspot():
    ap.connect_hotspot(True)


def disconnect_hotspot():
    logger.debug("Disconnecting as hotspot")
    ap.connect_hotspot(False)


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

if __name__ == "__main__":
    print get_ssids()
