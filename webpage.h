#ifndef WEBPAGE_H
#define WEBPAGE_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Robot Dashboard</title>
  <style>
    body { font-family: Arial; text-align: center; margin: 0; background-color: #222; color: white; }
    h1 { background-color: #e63946; padding: 20px; margin: 0; }
    .card { background: #333; max-width: 400px; margin: 20px auto; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.5); }
    .data { font-size: 1.5rem; font-weight: bold; color: #4cc9f0; }
    .alert { color: #ff4d4d; font-weight: bold; animation: blink 1s infinite; }
    @keyframes blink { 50% { opacity: 0; } }
    button { padding: 15px 30px; font-size: 18px; background: #4cc9f0; border: none; border-radius: 5px; cursor: pointer; }
  </style>
</head>
<body>
  <h1>🤖 Robot Security Dashboard</h1>
  
  <div class="card">
    <h2>📍 GPS Location</h2>
    <p>Latitude: <span id="lat" class="data">Loading...</span></p>
    <p>Longitude: <span id="lon" class="data">Loading...</span></p>
    <p><a id="maps" href="#" target="_blank" style="color: #fff; text-decoration: underline;">Open in Google Maps</a></p>
  </div>

  <div class="card">
    <h2>🚨 Status</h2>
    <p>PIR Motion: <span id="pir">Checking...</span></p>
    <p>Obstacle: <span id="obs">Checking...</span></p>
  </div>

  <script>
    // Function to fetch data from Arduino every 2 seconds
    setInterval(function() {
      fetch('/data')
        .then(response => response.json())
        .then(data => {
          document.getElementById('lat').innerText = data.lat;
          document.getElementById('lon').innerText = data.lon;
          document.getElementById('maps').href = "https://www.google.com/maps/place/" + data.lat + "," + data.lon;
          
          // Update PIR Status
          let pirEl = document.getElementById('pir');
          if(data.motion) {
            pirEl.innerText = "DETECTED!";
            pirEl.className = "alert";
          } else {
            pirEl.innerText = "Safe";
            pirEl.className = "data";
          }

          // Update Obstacle Status
          let obsEl = document.getElementById('obs');
          obsEl.innerText = data.obstacle + " cm";
        });
    }, 2000);
  </script>
</body>
</html>
)rawliteral";

#endif