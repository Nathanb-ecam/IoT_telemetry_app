
"__________________________________________________________________________________________________________________________________________________________________________________________"




var express = require('express'); 
let app = express();


//Middleware; css/img/js
app.use(express.static('public'));



//Server
var server = require("http").Server(app);
var io = require("socket.io")(server); 

//Routes
let routes = require('./routes');
app.use('/', routes);


// mqtt credentials
var secrets  = require('./secrets.json'); 

// client mqtt
const mqtt = require('mqtt');

var host = 'eu1.cloud.thethings.network';
var port = '1883';
var connectUrl = `mqtt://${host}:${port}`;


let messages = [];

var client = mqtt.connect(connectUrl,{
   clean: true,
   connectTimeout: 4000,
   username: secrets.username,
   password: secrets.password,
   reconnectPeriod: 1000,
});


client.on('connect',  () =>{
   var topic = "v3/moisture-application-2022@ttn/devices/eui-a8610a353028630a/up";
   console.log('Connected')
   client.subscribe(topic); //single topic

   
   
})

client.on("error",  function(error){
   console.log("Can't correct"+error);
   process.exit(1);
})


client.on("message", function (topic, message, packet) {
   var getDataFromTTN = JSON.parse(message);
   uplink_message = getDataFromTTN.uplink_message;
   data = getDataFromTTN.uplink_message.decoded_payload;
   // console.log("data :");
   // console.log(uplink_message);

   // console.log(data);
   // messages.push(data);

   // console.log("received from topic :");
   // console.log(topic);

   io.emit('message',uplink_message);
   
});




server.listen(process.env.PORT || 3000, () => { 
   console.log('J ecoute au port 3000 socket');
});