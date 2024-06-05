from flask import Flask, request
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

lat = "33.6"
lon = "117.8"
sound = ""
light = ""

@app.route("/")
def location():
    global lat
    global lon
    global sound
    global light
    lat = str(request.args.get("latitude"))
    lon = str(request.args.get("longitude"))
    print("\nLatitude:", lat, "Longitude:", lon)
    print("Action:", request.args.get("action"))
    return sound + ',' + light

@app.route("/action")
def action():
    global lat
    global lon
    global sound
    global light
    sound = request.args.get("sound")
    light = request.args.get("light")
    print("Sound:", sound, "Light:", light)
    lat += "l"
    return str(lat + ',' + lon)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000)
