const User = require('../models/user');

module.exports = {
	loginUser: async (req, res) => {
		// Check if user exists
        const myUser = await User.findOne({ email: res.locals.email }).catch(() => null);
        
        // If so, send a positive response
        if (myUser) {
            res.status(200).send({"status" : "OK"})
        }
        // If not, create a new user from the token info
        else {
            const newUser = {
				first_name: res.locals.firstname,
                last_name: res.locals.lastname,
                email: res.locals.email,
				wins: 0,
				losses: 0,
			}
            try {
                await User.create(newUser);
                res.status(201).send({"status" : "OK"})
            }
            catch (err) {
                console.log(err)
                res.status(500).send({"status" : "ERR"});
            }
		}
	}
}