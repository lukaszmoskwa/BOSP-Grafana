const express = require('express');
const bodyParser = require('body-parser')
const chalk = require('chalk');
const boxen = require('boxen');


const app = express();

console.log(boxen("BOSP-Grafana server", { padding: 1, borderStyle: "round", borderColor: "red"}))

app.use(bodyParser.urlencoded({ limit: '1mb', extended: true }))
app.use(bodyParser.json({ limit: '1mb' }))
app.use(require('./routes'));

app.get('/*', function (req, res) {
    res.send('<h3>BOSP-Grafana Node server</h3>');
});


var port = 3131;

console.log(chalk.white.bgCyan.bold.underline("Running on http://localhost:"+port+"\n"))

app.listen(port);