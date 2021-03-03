const User = require('../models/user');

module.exports = {
    getUser: async (req, res) => {
        try {
            const myUser = await User.findOne({ email: req.params.email }).catch(() => null);

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
}