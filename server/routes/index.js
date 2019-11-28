const express = require('express')
const router = express.Router()


router.use('/resources', require('./resources').router)

module.exports = router
