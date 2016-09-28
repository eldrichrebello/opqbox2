import os
from bottle import route, run, static_file, template, request, redirect, post

@route('/js/<filename>')
def js_static(filename):
    return static_file(filename, root='./server/js')


@route('/img/<filename>')
def img_static(filename):
    return static_file(filename, root='./server/img')


@route('/css/<filename>')
def img_static(filename):
    return static_file(filename, root='./server/css')


@route("/")
def networks():
    return template('/home/pi/opqbox2/pypi/server/views/networks.tpl', ssids=get_ssids())

@post("/")
def connect():

    if request.POST.get("sub_OPEN","").strip():
        mSSID = request.forms.get('fOPEN')
        connect_to_network(mSSID)

        #will return next template
        return '<h1 style="color:blue;">CONNECTED</h1>'

    elif request.POST.get("sub_WPA","").strip():
        password = request.forms.get('pwdWPA')

        #check if password matches
        if password == 'abcd':
            mSSID = request.forms.get('fWPA')
            connect_to_network(mSSID)

            return '<h1 style="color:blue;">CONNECTED</h1>'
        else:
            #should display invalid password try again
            redirect("/")

    elif request.POST.get("sub_WEP","").strip():
        password = request.forms.get('pwdWEP')

        #check if password matches
        if password == 'abcd':
            mSSID = request.forms.get('fWEP')
            connect_to_network(mSSID)

            return '<h1 style="color:blue;">CONNECTED</h1>'
        else:
            redirect("/")

    redirect("/")

def run_server(host='0.0.0.0', port=5000):
    port = int(os.environ.get("PORT", port))
    run(host=host, port=port)

run_server()
