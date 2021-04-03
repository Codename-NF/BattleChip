const express = require('express');
const router = express.Router();

const matchController = require('../controllers/match');
const auth = require('../middleware/auth');

// Get list of matches where the user participated
router.get('/', auth.checkToken, matchController.getMatches);

// Create a new record of a match
router.post('/', matchController.createMatch);

module.exports = router;
