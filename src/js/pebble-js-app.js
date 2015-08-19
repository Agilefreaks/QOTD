Pebble.addEventListener('ready',
  function(e) {
    console.log('PebbleKit JS ready!');
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
  }
);
// function qotd(){
//   var url = 'http://damp-temple-5257.herokuapp.com/today.json'
//   xhrRequest(url, 'GET', quoteReceived)A;
// }
//
// function quoteReceived(responseText) {
//   var json=JSON.parse(responseText);
//   var quote= json.quote;
//   var author= json.author;
// }
// var dictionary = {
//   'Q': quote,
//   'A': author
// };
// Pebble.sendAppMessage(dictionary,
//   function(e) {
//     console.log('Quto sent to Pebble successfully!');
//   },
//   function(e) {
//     console.log('Error sending quote to Pebble!');
//   }
// );
// Pebble.addEventListener('appmessage',
//   function(e) {
//     console.log('AppMessage received!');
//
//   }
// );
