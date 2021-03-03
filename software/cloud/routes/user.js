const express = require('express');
const router = express.Router();

const userController = require('../controllers/user');
const auth = require('../middleware/auth');

// Get info for specific user
router.get('/:email/', auth.checkToken, userController.getUser);

module.exports = router;
