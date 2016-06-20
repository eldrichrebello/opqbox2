#!/usr/bin/python

# This script shows how to connect to a WPA protected WiFi network
# by communicating through D-Bus to NetworkManager 0.9.
#
# Reference URLs:
# http://projects.gnome.org/NetworkManager/developers/
# http://projects.gnome.org/NetworkManager/developers/settings-spec-08.html

import dbus
import time

def get_ssids():
    bus = dbus.SystemBus()
    manager_bus_object = bus.get_object("org.freedesktop.NetworkManager",
                                        "/org/freedesktop/NetworkManager")
    manager = dbus.Interface(manager_bus_object,
                             "org.freedesktop.NetworkManager")

    # Get path to the 'wlan0' device. If you're uncertain whether your WiFi
    # device is wlan0 or something else, you may utilize manager.GetDevices()
    # method to obtain a list of all devices, and then iterate over these
    # devices to check if DeviceType property equals NM_DEVICE_TYPE_WIFI (2).
    device_path = manager.GetDeviceByIpIface("wlan0")
    print "wlan0 path: ", device_path

    # Connect to the device's Wireless interface and obtain list of access
    # points.
    device = dbus.Interface(bus.get_object("org.freedesktop.NetworkManager",
                                           device_path),
                            "org.freedesktop.NetworkManager.Device.Wireless")
    accesspoints_paths_list = device.GetAccessPoints()

    our_ap_path = None
    ssids = []
    for ap_path in accesspoints_paths_list:
        ap_props = dbus.Interface(
            bus.get_object("org.freedesktop.NetworkManager", ap_path),
            "org.freedesktop.DBus.Properties")
        ap_ssid = ap_props.Get("org.freedesktop.NetworkManager.AccessPoint",
                               "Ssid")
        ap_wpa =  int(ap_props.Get("org.freedesktop.NetworkManager.AccessPoint",
                               "WpaFlags"))
        ap_wep =  int(ap_props.Get("org.freedesktop.NetworkManager.AccessPoint",
                               "RsnFlags"))
        ap_strength =  int(ap_props.Get("org.freedesktop.NetworkManager.AccessPoint",
                                "Strength"))
        security = int(ap_wpa) | int(ap_wep)
        # Returned SSID is a list of ASCII values. Let's convert it to a proper
        # string.
        str_ap_ssid = "".join(chr(i) for i in ap_ssid)
        if security == 0:
	    ssids.append((str_ap_ssid, "NONE", ap_strength))
            print "SSID = ", str_ap_ssid , " Security = NONE ", ap_strength , "%" 
        elif security & (0x1 | 0x2 | 0x10 | 0x20):
            ssids.append((str_ap_ssid, "WEP", ap_strength))
            print "SSID = ", str_ap_ssid , " Security = WEP", ap_strength , "%" 
        else:
            ssids.append((str_ap_ssid, "WPA", ap_strength))
            print "SSID = ", str_ap_ssid , " Security = WPA", ap_strength , "%"

     return ssids 
