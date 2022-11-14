var express = require('express');
var secrets  = require('./secrets.json'); 
   
var app = express();

app.use(express.urlencoded());// pour récupérer le contenu du corps de message de la méthode post

let routes = require("./routes.js");

app.use('/',routes); // on associe les routes à la racine

let messages = []

// client mqtt
const mqtt = require('mqtt');

var host = 'eu1.cloud.thethings.network';
var port = '1883';
var connectUrl = `mqtt://${host}:${port}`;

var client = mqtt.connect(connectUrl,{
   clean: true,
   connectTimeout: 4000,
   username: secrets.username,
   password: secrets.password,
   reconnectPeriod: 1000,
})

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
   data = getDataFromTTN.uplink_message.decoded_payload;
   // console.log("data :");
   // console.log(data);
   messages.push(data);
   // console.log("received from topic :");
   // console.log(topic);
   
})


app.post('/', function(req,resp){
   // console.log(req.body);
   resp.render("home.ejs",{messages:messages});
});





app.use(express.static('public')); // permet d'avoir acces au doissier "public" qui contient css/images

app.listen(3000, function () {
   console.log("Server running on port 3000")
});


