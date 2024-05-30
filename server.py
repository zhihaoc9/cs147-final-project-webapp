from flask import Flask
from flask import request
app = Flask(__name__)
@app.route("/")
def hello():
    print("\nLatitude:", request.args.get("latitude"), "Longitude:", request.args.get("longitude"))
    print("Action:", request.args.get("action"))
    return "We received value: "+str(request.args.get("latitude") + " + " + request.args.get("longitude") + "\n\n")
