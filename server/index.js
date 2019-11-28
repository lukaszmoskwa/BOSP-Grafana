const express = require('express');
const bodyParser = require('body-parser')
const chalk = require('chalk');
const boxen = require('boxen');


const app = express();

console.log(boxen("Grafana-BOSP server", { padding: 1, borderStyle: "round", borderColor: "red"}))

//app.use(express.static(__dirname + "/public"));
app.use(bodyParser.urlencoded({ limit: '1mb', extended: true }))
app.use(bodyParser.json({ limit: '1mb' }))
app.use(require('./routes'));

/*app.get('*', function (req, res) {
    res.sendFile(path.join(__dirname + '/public/index.html'));
});*/


var port = process.env.PORT || 80;
port = 3131;

console.log(chalk.white.bgCyan.bold.underline("Running on http://localhost:"+port+"\n"))

app.listen(port);