#define Q 0
#define A 0
Pebble.addEventListener('ready',
  function(e) {
    console.log('QOTD ready!');
    qotd();
  }
);
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('QOTD updated!');
  }
);

function qotd(){
  var url = 'http://damp-temple-5257.herokuapp.com/today.json'
  xhrRequest(url, 'GET', quoteReceived);
}

function quoteReceived(responseText) {
  var json=JSON.parse(responseText);
  var quote= json.quote;
  var author= json.author;
}
var dictionary = {
  'Q': quote,
  'A': author
};
Pebble.sendAppMessage(dictionary,
  function(e) {
    console.log('Quto sent to Pebble successfully!');
  },
  function(e) {
    console.log('Error sending quote to Pebble!');
  }
);
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');

  }
);
