import React from 'react';
import './App.css';

function App() {
  const handleClick = (action) => {
    fetch(`http://ec2-107-22-40-2.compute-1.amazonaws.com:5000/?action=${action}&latitude=${"None"}&longitude=${"None"}`)
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
        <button onClick={() => handleClick('sound')}>Sound</button>
        <button onClick={() => handleClick('light')}>Light</button>
      </header>
    </div>
  );
}

export default App;
