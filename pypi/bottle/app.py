#!/usr/bin/env python

import os
from bottle import route, run, static_file, template
from __init__ import get_ssids


@route('/js/<filename>')
def js_static(filename):
    return static_file(filename, root='./js')


@route('/img/<filename>')
def img_static(filename):
    return static_file(filename, root='./img')


@route('/css/<filename>')
def img_static(filename):
    return static_file(filename, root='./css')


@route("/")
def hello():
    return template('main1.tpl', List1=get_ssids())


def run_server(port=5000):
    port = int(os.environ.get("PORT", port))
    run(host='localhost', port=port)
