var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

Pebble.addEventListener('ready',
  function(e) {
    console.log('PebbleKit JS ready!');
    qotd();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
  }
);

function qotd(){
  var url = 'http://damp-temple-5257.herokuapp.com/today.json'
  xhrRequest(url, 'GET', codesmellReceived);
}

function codesmellReceived(responseText) {
  console.log('Request made. Response is: ')
  console.log(responseText)
  var json=JSON.parse(responseText);
  var content= json.content;
  var title= json.title;
  var dictionary = {
    "0": content,
    "1": title
  };
  // Send to Pebble
Pebble.sendAppMessage(dictionary,
  function(e) {
    // console.log('Code Smell sent to Pebble successfully!');
  },
  function(e) {
    // console.log('Error sending code smell to Pebble!');
  }
);
}


Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');

  }
);
