import React, { useState, useEffect } from 'react';
import Switch from 'react-switch';
import { MapContainer, TileLayer, Marker, Popup } from 'react-leaflet';
import 'leaflet/dist/leaflet.css';
import L from 'leaflet';
import './App.css';

import markerIcon2x from 'leaflet/dist/images/marker-icon-2x.png';
import markerIcon from 'leaflet/dist/images/marker-icon.png';
import markerShadow from 'leaflet/dist/images/marker-shadow.png';

L.Icon.Default.mergeOptions({
  iconRetinaUrl: markerIcon2x,
  iconUrl: markerIcon,
  shadowUrl: markerShadow,
});

function App() {
  const [latitude, setLatitude] = useState("34");
  const [longitude, setLongitude] = useState("-118.2");
  const [sound, setSound] = useState(false);
  const [light, setLight] = useState(false);

  useEffect(() => {
    const interval = setInterval(() => {
      handleClick(sound ? 'on' : 'off', light ? 'on' : 'off');
    }, 2000);

    return () => clearInterval(interval);
  }, [sound, light]);

  const handleSoundChange = (checked) => {
    setSound(checked);
  };

  const handleLightChange = (checked) => {
    setLight(checked);
  };

  const handleClick = (sound, light) => {
    fetch(`http://ec2-107-22-40-2.compute-1.amazonaws.com:5000/action?sound=${sound}&light=${light}`)
      .then(response => {
        if (!response.ok) {
          throw new Error('Network response was not ok');
        }
        return response.text();
      })
      .then(data => {
        const [lat, lon] = data.split(',');
        setLatitude(parseFloat(lat));
        setLongitude(parseFloat(lon));
      })
      .catch(error => console.error('Error:', error));
  };

  return (
    <div className="App">
      <header className="App-header">
        <h1>Item Finder</h1>
        <div className="controls">
          <div>
            <label>
              Sound
              <Switch
                onChange={handleSoundChange}
                checked={sound}
                uncheckedIcon={false}
                checkedIcon={false}
              />
            </label>
          </div>
          <div>
            <label>
              Light
              <Switch
                onChange={handleLightChange}
                checked={light}
                uncheckedIcon={false}
                checkedIcon={false}
              />
            </label>
          </div>
        </div>
        <p>Latitude: {latitude}</p>
        <p>Longitude: {longitude}</p>
        
        <div className="map-container">
          <MapContainer center={[latitude, longitude]} zoom={13} style={{ height: '100%', width: '100%' }}>
            <TileLayer
              url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
              attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
            />
            <Marker position={[latitude, longitude]}>
              <Popup>Item is here!</Popup>
            </Marker>
          </MapContainer>
        </div>
      </header>
    </div>
  );
}

export default App;
