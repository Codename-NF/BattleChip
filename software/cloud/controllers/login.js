const User = require('../models/user');

module.exports = {
	loginUser: async (req, res) => {
		// Check if user exists
        const myUser = await User.findOne({ email: res.locals.email }).catch(() => null);
        
        // If so, send a positive response
        if (myUser) {
            res.status(200).send(myUser)
        }
        // If not, create a new user from the token info
        else {
            // Get count of users in DB, assign unique id as count + 1
            const userCount = await User.countDocuments({});
            const lastName = (res.locals.lastname) ? res.locals.lastname : " ";
            
            const newUser = {
				player_id: userCount, // Not userCount + 1 since id is zero indexed
                first_name: res.locals.firstname,
                last_name: lastName,
                email: res.locals.email,
				wins: 0,
				losses: 0,
			}
            try {
                await User.create(newUser);
                res.status(201).send(newUser)
            }
            catch (err) {
                console.log(err)
                res.status(500).send({});
            }
		}
	}
}