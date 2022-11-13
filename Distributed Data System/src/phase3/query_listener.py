import mysql.connector
import config
import requests

from flask import Flask, jsonify, request

app = Flask(__name__)

def createTableQueryGenUtil(reln_name):
    QUERY = " describe {}".format(reln_name)
    db = mysql.connector.connect(host = config.HOSTNAME, user = config.USERNAME, password = config.PASSWORD, database=config.DB)
    cursor = db.cursor()
    cursor.execute(QUERY)
    res = cursor.fetchall()
    cursor.close()
    db.close()
    return res

def getRelnData(reln_name):
    QUERY = " select * from {}".format(reln_name)
    db = mysql.connector.connect(host = config.HOSTNAME, user = config.USERNAME, password = config.PASSWORD, database=config.DB)
    cursor = db.cursor()
    cursor.execute(QUERY)
    res = cursor.fetchall()
    cursor.close()
    db.close()
    return res

def semiJoinUtil(reln_name, values, attr):
    QUERY = " select * from {} where {} IN ({})".format(reln_name, attr, ','.join(values))
    db = mysql.connector.connect(host = config.HOSTNAME, user = config.USERNAME, password = config.PASSWORD, database=config.DB)
    cursor = db.cursor()
    cursor.execute(QUERY)
    res = cursor.fetchall()
    cursor.close()
    db.close()
    return res

@app.route('/', methods=['GET'])
def healthlink():
    return "Hello World"

@app.route('/move_sj_table', methods=['POST'])
def moveSemiJoinTable():
    try:
        # IP Addresses
        URLS = {}
        URLS['1'] = 'http://10.3.5.215:8081/query'
        URLS['2'] = 'http://10.3.5.214:8081/query'
        URLS['3'] = 'http://10.3.5.213:8081/query'

        request_data = request.get_json()

        print(request_data)

        R = request_data['R']
        S = request_data['S']
        S_site = request_data['site']
        join_attr = request_data['join_attr']
        URL = URLS[str(S_site)]

        QUERY = """ select DISTINCT {} from {} """.format(join_attr, S)
        req_obj = {'query': QUERY}
        query_response = requests.post(URL, json = req_obj)
        values = [ str(row[0]) for row in query_response ]

        attrs = createTableQueryGenUtil(R)

        QUERY = " CREATE TABLE IF NOT EXISTS {} ( {} );".format(R, ','.join([ '{} {}'.format(x[0], x[1].decode("utf-8").upper()  ) for x in attrs]))
        req_obj = {'query': QUERY}
        query_response = requests.post(URL, json = req_obj)

        QUERY = " INSERT INTO {} VALUES {};".format(R, ','.join( [ str(row) for row in semiJoinUtil(R,values,join_attr) ]) )
        req_obj = {'query': QUERY}
        query_response = requests.post(URL, json = req_obj)

        print("{}_PRIME sent to {}".format(R, S_site))

        return jsonify({'response': "QUERY SUCCESSFULL"})
    except Exception as err:
        print(err)
        return jsonify({ 'response': 'Error : {}'.format(err) })
@app.route('/move_table', methods=['POST'])
def moveTable():
    try:
        # IP Addresses
        URLS = {}
        URLS['1'] = 'http://10.3.5.215:8081/query'
        URLS['2'] = 'http://10.3.5.214:8081/query'
        URLS['3'] = 'http://10.3.5.213:8081/query'

        request_data = request.get_json()

        print(request_data)

        reln_name = request_data['table']
        result_site = request_data['result_site']
        URL = URLS[str(result_site)]

        attrs = createTableQueryGenUtil(reln_name)

        QUERY = " CREATE TABLE IF NOT EXISTS {} ( {} );".format(reln_name, ','.join([ '{} {}'.format(x[0], x[1].decode("utf-8").upper()  ) for x in attrs]))
        req_obj = {'query': QUERY}
        query_response = requests.post(URL, json = req_obj)

        QUERY = " INSERT INTO {} VALUES {};".format(reln_name, ','.join( [ str(row) for row in getRelnData(reln_name) ]) )
        req_obj = {'query': QUERY}
        query_response = requests.post(URL, json = req_obj)

        print("{} sent to {}".format(reln_name, result_site))

        return jsonify({'response': "QUERY SUCCESSFULL"})
    except Exception as err:
        print(err)
        return jsonify({ 'response': 'Error : {}'.format(err) })

@app.route('/query', methods=['POST'])
def runQuery():
    try:
        request_data = request.get_json()
        print(request_data)
        query = request_data['query']
        db = mysql.connector.connect(host = config.HOSTNAME, user = config.USERNAME, password = config.PASSWORD, database=config.DB)
        cursor = db.cursor()
        cursor.execute(query)
        res = cursor.fetchall()
        cursor.close()
        db.commit()
        db.close()
        print(res)
        return jsonify({'response': res})
    except Exception as err:
        print(err)
        return jsonify({ 'response': 'Error : {}'.format(err) })

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port='8081')