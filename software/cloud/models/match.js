const mongoose = require('mongoose');
const Schema = mongoose.Schema;

let Match = new Schema({
    player_one: {
        type: String,
        required: true,
    },
	player_two: {
        type: String,
        required: true,
    },
	winner: {
        type: String,
        required: true,
    },
    player_one_score: {
        type: Number,
		required: true,
    },
    player_two_score: {
        type: Number,
		required: true,
    },
    player_one_name: {
        type: String,
		required: true,
    },
    player_two_name: {
        type: String,
		required: true,
    },
    date: {
        type: Number,
        required: true,
    }
});

module.exports = mongoose.model('Match', Match);