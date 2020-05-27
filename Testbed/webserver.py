from flask import Flask, render_template, request, jsonify, make_response
import json
import requests
app = Flask (__name__)


def api(request,path):
    print ("The request method is :{}").format(request.method)
    print (request.headers)
    print (request.headers["Content-Type"])
    print ("Request Path: {}".format(path))

    result = request.get_json(force=True, silent=True, cache=True)


@app.route('/')
def main():
    return 'hello'

@app.route('/on',methods=['GET'])
def on():
    message_body = {"on":True, "sat":254, "bri":254, "hue":10000} 

    url = 'http://143.248.49.87:88/api/XAI8Yvp26NTLSxP0uGurWuI091Qxj65C2VFjSsr2/lights/2/state'
    res = requests.put(url,data=json.dumps(message_body))
    return res.text

@app.route('/off',methods=['GET'])
def off():
    message_body = {"on":False}

    url = 'http://143.248.49.87:88/api/XAI8Yvp26NTLSxP0uGurWuI091Qxj65C2VFjSsr2/lights/2/state'
    res = requests.put(url,data=json.dumps(message_body))
    return res.text

@app.route('/api/<path:path>',methods=['PUT'])
def apiHandler(path):
	return api(request,path)

if __name__ == "__main__":
    app.run()
    #http://127.0.0.1:5000/
    #app.run(host='143.248.49.87', port=88)

