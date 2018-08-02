#!/usr/bin/python

from flask import Flask, jsonify, Response, request
from models import Management

ms = Management()
app = Flask(__name__)

def json_msg(success, msg, status_code=200):
    response = {
            'success': success,
            'msg': msg
            }
    response = jsonify(response)
    response.status_code = status_code
    return response

@app.route('/')
def root():
    return "Hello"

@app.route('/api/save', methods=['POST'])
def save():
    ms.save_status()
    return "Save" 

@app.route('/api/load', methods=['POST'])
def load():
    ms.load_status('.')
    return "Load"

@app.route('/api/group/create/<group_name>', methods=['POST'])
def api_create_group(group_name):
    if ms.add_group(group_name):
        return json_msg(True, "Add Group %s Success!" % group_name)
    else:
        return json_msg(False, "Fail when add Group", 400)
    
@app.route('/api/group/update/<group_name>', methods=['POST'])
def api_update_group(group_name):
    people_names = request.form.get('people')
    people_names = people_names.strip().split(',')
    if ms.people_join_group(people_names, group_name):
        return json_msg(True, "Update Group %s success!" % group_name)
    else:
        return json_msg(False, "Fail when updating group")

@app.route('/api/group/status/<group_name>', methods=["GET"])
def api_status_group(group_name):
    msg = ms.query_group_status(group_name)
    return jsonify(msg)

@app.route('/api/group/transaction/<group_name>', methods=['POST'])
def api_transaction_group(group_name):
    transaction_info = request.get_json()
    if ms.add_transaction(group_name, transaction_info):
        return json_msg(True, "Add Transaction to Group %s Success!" % group_name)
    else:
        return json_msg(False, "Fail when adding Transaction", 400)

@app.route('/api/user/create/<user_name>', methods=['POST'])
def api_create_user(user_name):
    if ms.add_person(user_name):
        return json_msg(True, "Add User %s Success!" % user_name)
    else:
        return json_msg(False, "User Already Exist!", 400)



if __name__ == '__main__':
    app.run()

