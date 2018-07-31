#!/usr/bin/python

from flask import Flask
from models import Person, Transaction, Group
app = Flask(__name__)

@app.route('/')
def root():
    return "Hello"

if __name__ == '__main__':
    app.run()

