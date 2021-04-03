const mongoose = require('mongoose');
const Schema = mongoose.Schema;

let User = new Schema({
    player_id: {
        type: Number,
        required: true,
    },
    email: {
        type: String,
        required: true,
    },
	first_name: {
        type: String,
        required: true,
    },
	last_name: {
        type: String,
        required: true,
    },
    wins: {
        type: Number,
		required: true,
    },
    losses: {
        type: Number,
		required: true,
    }
});

module.exports = mongoose.model('User', User);