import React from 'react';
import './App.css';

function App() {
  const handleClick = (temperature, humidity) => {
    fetch(`http://ec2-52-53-217-9.us-west-1.compute.amazonaws.com:5000/?temperature=${temperature}&humidity=${humidity}`)
      .then(response => response.text())
      .then(data => alert(data))
      .catch(error => console.error('Error:', error));
  };

  return (
    <div className="App">
      <header className="App-header">
        <h1>Item Finder</h1>
        <p>Longitude:</p>
        <p>Latitude:</p>
        <button onClick={() => handleClick('sound', 'sound')}>Sound</button>
        <button onClick={() => handleClick('light', 'light')}>Light</button>
      </header>
    </div>
  );
}

export default App;
