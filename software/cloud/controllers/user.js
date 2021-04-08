const User = require('../models/user');

module.exports = {
    getUser: async (req, res) => {
        //try {
            const myUser = await User.findOne({ email: res.locals.email }).catch(() => null);

            if (myUser) {
                res.status(200).send(myUser);
            }
            else {
                res.status(404).send({});
            }
        // }
        // catch (err) {
        //     res.status(500).send({});
        // }
    },
}