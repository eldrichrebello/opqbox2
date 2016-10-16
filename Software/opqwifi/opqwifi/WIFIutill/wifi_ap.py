#!/usr/bin/env python
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
# Copyright (C) 2010 - 2012 Red Hat, Inc.
#

#
# This example starts or stops a wifi hotspot
#
# Configuration settings are described at
# https://developer.gnome.org/NetworkManager/1.0/ref-settings.html
#

import dbus, sys, time

def wifi_ap(up=True, iface="wlan0"):

    our_uuid = '2b0d0f1d-b79d-43af-bde1-7174462564EE'

    s_con = dbus.Dictionary({
        'type': '802-11-wireless',
        'uuid': our_uuid,
        'id': 'OPQ'})

    s_wifi = dbus.Dictionary({
        'ssid': dbus.ByteArray("OPQBox"),
        'mode': "ap"})

    s_ip4 = dbus.Dictionary({'method': 'shared'})
    s_ip6 = dbus.Dictionary({'method': 'ignore'})

    con = dbus.Dictionary({
        'connection': s_con,
        '802-11-wireless': s_wifi,
        'ipv4': s_ip4,
        'ipv6': s_ip6
         })



    bus = dbus.SystemBus()
    service_name = "org.freedesktop.NetworkManager"
    proxy = bus.get_object(service_name, "/org/freedesktop/NetworkManager/Settings")
    settings = dbus.Interface(proxy, "org.freedesktop.NetworkManager.Settings")
    iface = iface
    proxy = bus.get_object(service_name, "/org/freedesktop/NetworkManager")
    nm = dbus.Interface(proxy, "org.freedesktop.NetworkManager")
    devpath = nm.GetDeviceByIpIface(iface)

    # Find our existing hotspot connection
    connection_path = None
    for path in settings.ListConnections():
        proxy = bus.get_object(service_name, path)
        settings_connection = dbus.Interface(proxy, "org.freedesktop.NetworkManager.Settings.Connection")
        config = settings_connection.GetSettings()
        if config['connection']['uuid'] == our_uuid:
            settings_connection.Update(con)
            connection_path = path
            break

    # If the hotspot connection didn't already exist, add it
    if not connection_path:
        connection_path = settings.AddConnection(con)
    # Now start or stop the hotspot on the requested device
    proxy = bus.get_object(service_name, devpath)
    device = dbus.Interface(proxy, "org.freedesktop.NetworkManager.Device")
    operation = "up" if up else "down"
    if operation == "up":
        try:
            acpath = nm.ActivateConnection(connection_path, devpath, "/")
            proxy = bus.get_object(service_name, acpath)
            active_props = dbus.Interface(proxy, "org.freedesktop.DBus.Properties")
        except Exception as e:
            return False
    else:
        try:
            device.Disconnect()
            proxy = bus.get_object(service_name, connection_path)
            settings_connection = dbus.Interface(proxy, "org.freedesktop.NetworkManager.Settings.Connection")
            settings_connection.Delete()
        except Exception as e:
            print e
            return True

     # Wait to connect
    start = time.time()
    while time.time() < start + 30:
        try:
            state = active_props.Get("org.freedesktop.NetworkManager.Connection.Active", "State")
            if state == 2:  # NM_ACTIVE_CONNECTION_STATE_ACTIVATED
                print "Connected to access point"

                return True
        except Exception as e:
            pass
        time.sleep(1)
    return False
