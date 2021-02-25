const User = require('../models/user');

module.exports = {
    getUser: async (req, res) => {
        try {
            const myUser = await User.findOne({ username: req.params.username }).catch(() => null);

            if (myUser) {
                res.status(200).send(myUser);
            }
            else {
                res.status(404).send({ "message": "User does not exist" });
            }
        }
        catch (err) {
            res.status(500).send();
        }
    },
	createUser: async (req, res) => {
		try {
			const newUser = {
				username: req.body.username,
				wins: 0,
				losses: 0,
			}
			await User.create(newUser);
			res.status(200).send({"status" : "OK"})
		}
		catch (err) {
			console.log(err)
			res.status(500).send();
		}
	}
}