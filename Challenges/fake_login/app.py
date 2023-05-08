from flask import Flask, request, render_template, make_response, jsonify
from lxml import etree

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('login.html')

@app.route('/login', methods=['POST'])
def login():
    parser = etree.XMLParser(recover=True)
    xml_string = request.data
    tree = etree.fromstring(xml_string, parser)
    username = tree.find('username').text
    password = tree.find('password').text
    if username == 'admin' and password == 'admin':
        message = 'Oh! You guessed my username and password, but where is the flag?'
        response = make_response('', 200)
        response.headers['Content-Type'] = 'application/json'
        response.data = jsonify({'message': message}).data
        return response
    else :
        message = username + ' is not exist or password is wrong!'
        response = make_response('', 401)
        response.headers['Content-Type'] = 'application/json'
        response.data = jsonify({'message': message}).data
        return response

if __name__ == '__main__':
    app.run(host = "0.0.0.0", port = 8000 ,debug = True)
