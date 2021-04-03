const express = require('express');
const router = express.Router();

const loginController = require('../controllers/login');
const auth = require('../middleware/auth');

// Create a user
router.post('/', auth.checkToken, loginController.loginUser);

module.exports = router;