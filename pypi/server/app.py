import os
from bottle import route, run, static_file, template
from net import get_ssids


@route('/js/<filename>')
def js_static(filename):
    return static_file(filename, root='/home/pi/opqbox2/pypi/server/js')


@route('/img/<filename>')
def img_static(filename):
    return static_file(filename, root='/home/pi/opqbox2/pypi/server/img')


@route('/css/<filename>')
def img_static(filename):
    return static_file(filename, root='/home/pi/opqbox2/pypi/server/css')


@route("/")
def hello():
    return template('/home/pi/opqbox2/pypi/server/views/networks.tpl', ssids=get_ssids())


def run_server(host='0.0.0.0', port=5000):
    port = int(os.environ.get("PORT", port))
    run(host=host, port=port)
