#!/usr/bin/env python

import os
from bottle import route, run, static_file, template, view, debug
from __init__ import get_ssids

#fiveList=["BLEHP","BLOOP","BLEEP","BERDP","ASDF"]

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
#@view("main1")

#anyway this works well, for knowing that i can access the view folder.
#so basically @view needs to match the file name in view folder. 
#and __name__ = "__main__" just makes sure this is run when this is primary main.
#i think.  
#i was confused at first because I thought that the it had to me __main1__ but 
#no its now.
# the other method used in the other code is just using return template('main.tpl')
# i will try that right now to see if that works instead of view("main1")
# okay, so i discovered that it does work however, i came across an issue with was the 
# there was a python variable located in the main1.tpl code so it was preventing me 
# from just loading it the first time . And i also learned that the form below
# with the defining the variables title and conent in the template funcation actually
# works. I also tried defining it out side but i guess i didn't pass it as a variable
# in the function so it didn't work out. 
# so either way works.



def hello():
       return template('main1.tpl', List1=get_ssids())
   # return dict(title = "Hello", content = "Hello from Python!")


if __name__ == "__main__":
    port = int(os.environ.get("PORT", 5000))
    run(host='localhost', port=port)
