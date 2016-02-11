def get_ssids():
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
    print "Connecting hotspot mode"


def disconnect_hotspot():
    print "Disconnecting hotspot mode"


def connect_to_network(ssid, skey=None):
    print "Attempting to connect to {}".format(ssid)


def get_connection_type():
    return ""
