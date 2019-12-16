var express = require("express");
const router = express.Router();
const seq = require("../sequelize");
const BBQid = seq.BBQid;
const BBQresource = seq.BBQresource;
const isDevelopment = require('../environment').isDevelopment;
const chalk = require('chalk')

/**
 * Handler for the resources GET requests
 */
router.get("/", (req, res) => {
  BBQresource.findAll().then(result => {
    res.send(result).end();
  }).catch(err => {
    console.log(err)
    res.sendStatus(400).end()
  })
});

/**
 * Handler for the resources POST requests
 */
router.post("/", (req, res) => {
  const {
    bbq_id,
    model,
    occupancy,
    bbq_load,
    power,
    temp,
    fans
  } = req.body;

  bbq_id.type_string = getResourceType(bbq_id.unit_type) || "none";
  let sys_string = `sys${bbq_id.sys}.group${bbq_id.bbq_group}.${bbq_id.type_string.toLowerCase()}${bbq_id.unit_id}.pe${bbq_id.pe_id}`
  if (isDevelopment) {
    console.log(chalk.underline.greenBright('Message: ' + bbq_id.type_string + " for " + sys_string))
  }
  BBQid.findOrCreate({
    where: bbq_id
  }).then(([single_id, is_created]) => {
    single_id = single_id.get({
      plain: true
    });
    return single_id.id;
  }).then((bbq_id) => {
    BBQresource.create({
      bbq_id,
      model,
      occupancy,
      bbq_load,
      power,
      temp,
      fans
    }).then(() => {
      res.sendStatus(200).end()
    }).catch((err) => {
      console.log("Errore nella creazione di qualcosa " + err)
      res.sendStatus(400).end()
    })

  })

});

function getResourceType(res) {
  switch (res) {
    case 3:
      return "CPU";
    case 4:
      return "GPU";
    case 5:
      return "ACC";
    case 7:
      return "MEM";
    case 8:
      return "NET";
  }
}

module.exports = {
  router: router
};