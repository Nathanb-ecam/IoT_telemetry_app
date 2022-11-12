let express = require('express');
let router = express.Router();

let messages = [];

router.get('/', function (request, response) { // pour recuperer des parametres de l'url 
   response.render("home.ejs",{messages:messages});
    
 });
 


 module.exports = router; // pour pouvoir importer les routes dans le serveur