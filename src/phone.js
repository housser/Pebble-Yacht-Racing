//var geoWatch;

function locationSuccess(pos) {
  // Assemble dictionary using our keys
  var dictionary = {
    "KEY_SPEED": pos.speed
  };
  
  // Send to Pebble
  Pebble.sendAppMessage(dictionary,
    function(e) {
      console.log("Position info sent to Pebble successfully!");
    },
    function(e) {
      console.log("Error sending position info to Pebble!");
    }
  );
}

function locationError(err) {
  console.log("Error requesting location!");
}

function getSpeed() {
  /*
  geoWatch = navigator.geolocation.watchPosition(
    locationSuccess, 
    locationError, 
    {timeout: 15000, maximumAge: 60000}
  );*/
  
  
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
  
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getSpeed();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getSpeed();
  }                     
);