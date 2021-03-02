const mongoose = require('mongoose');
const Schema = mongoose.Schema;

let User = new Schema({
    username: {
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