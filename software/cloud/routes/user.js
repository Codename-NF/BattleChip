const express = require('express');
const router = express.Router();

const userController = require('../controllers/user');

router.get('/:username/', userController.getUser);

router.post('/', userController.createUser);

module.exports = router;