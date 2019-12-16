var express = require('express');
const router = express.Router();
const seq = require('../sequelize');
const BBQid = seq.BBQid;
const BBQresource = seq.BBQresource;
const BBQapp = seq.BBQapp;
const isDevelopment = require('../environment').isDevelopment;
const chalk = require('chalk');

/**
 * Handler for the applications GET requests
 */
router.get('/', (req, res) => {
  BBQapp.findAll()
    .then((result) => {
      //result = result.get({plain: true})
      res.send(result).end();
    })
    .catch((err) => {
      console.log(err);
      res.sendStatus(400).end();
    });
});

/**
 * Handler for the applications POST requests
 */
router.post('/', async (req, res) => {
  const { id, name, state, mapping, tasks } = req.body;
  for (let resource_id of mapping) {
    try {
      let single_id = await BBQid.findOrCreate({
        where: resource_id,
      });
      single_id = single_id[0].get({
        plain: true,
      });
      const bbq_id = single_id.id;
      const succ = await BBQapp.create({
        pid: id,
        name,
        state,
        bbq_id,
      });
      if (isDevelopment) {
        resource_id.type_string = getResourceType(bbq_id.unit_type) || 'none';
        let sys_string = `sys${resource_id.sys}.group${
          resource_id.bbq_group
        }.${resource_id.type_string.toLowerCase()}${resource_id.unit_id}.pe${
          resource_id.pe_id
        }`;
        console.log(
          chalk.underline.redBright(
            'Message: ' + resource_id.type_string + ' for ' + sys_string,
          ),
        );
      }
    } catch (err) {
      console.log(err);
    }
  }
  res.sendStatus(200).end();
});

function getResourceType(res) {
  switch (res) {
    case 3:
      return 'CPU';
    case 4:
      return 'GPU';
    case 5:
      return 'ACC';
    case 7:
      return 'MEM';
    case 8:
      return 'NET';
  }
}

module.exports = {
  router: router,
};
