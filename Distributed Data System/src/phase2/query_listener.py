import mysql.connector
import config
import requests

from flask import Flask, jsonify, request

app = Flask(__name__)

@app.route('/', methods=['GET'])
def healthlink():
    return "Hello World"


@app.route('/query', methods=['POST'])
def runQuery():
    request_data = request.get_json()
    print(request_data)
    query = request_data['query']
 
    db = mysql.connector.connect(host = config.HOSTNAME, user = config.USERNAME, password = config.PASSWORD, database=config.DB)
    cursor = db.cursor()
    cursor.execute(query)
    res = cursor.fetchall()
    cursor.close()
    db.close()
 
    return res

if __name__ == '__main__':
    app.run(debug=True, host='127.0.0.1', port='8081')