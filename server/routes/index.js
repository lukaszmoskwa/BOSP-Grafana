const express = require('express')
const router = express.Router()


router.use('/resources', require('./resources').router)
router.use('/apps', require('./app').router)

module.exports = router
