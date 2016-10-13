import os

from bottle import route, run, static_file, template, request, redirect, post
from WIFIutill.wifi_ssids import wifi_ssids
from WIFIutill.wifi_connect import wifi_connect
from WIFIutill.wifi_check import wifi_check
from WIFIutill.wifi_ap import wifi_ap


@route('/js/<filename>')
def js_static(filename):
    return static_file(filename, root='js/')


@route('/img/<filename>')
def img_static(filename):
    return static_file(filename, root='img/')


@route('/css/<filename>')
def img_static(filename):
    return static_file(filename, root='css/')


@route("/")
def networks():
    return template('views/networks.tpl', ssids=wifi_ssids())

@post("/")
def connect():
    if request.POST.get("sub_OPEN","").strip():
        mSSID = request.forms.get('fOPEN')


    elif request.POST.get("sub_WPA","").strip():
        password = request.forms.get('pwdWPA')
        mSSID = request.forms.get('fWPA')
        wifi_ap(False, "wlan0")
        wifi_connect({"ssid" : mSSID, "Sec" : "WPA", "password" : password})

    elif request.POST.get("sub_WEP","").strip():
        password = request.forms.get('pwdWEP')
        mSSID = request.forms.get('fWEP')

    redirect("/")
def run_server(host='0.0.0.0', port=5001):
    port = int(os.environ.get("PORT", port))
    run(host=host, port=port)

if not wifi_check():
    wifi_ap(True, "wlan0")
run_server()
