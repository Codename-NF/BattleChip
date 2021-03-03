const googleAuth = require('../plugins/google-auth');

// Function called by middleware to verify that the google token is valid
module.exports = {
    checkToken: async (req, res, next) => {
        if (req.headers.id_token) {
            const token = req.headers.id_token;

            // Verify the token
            const credentials = await googleAuth.checkGoogle(token);
            console.log(credentials);
            if (credentials) {
                // Store credentials
                res.locals.userId = credentials.userId;
                res.locals.email = credentials.email;
                res.locals.firstname = credentials.given_name;
                res.locals.lastname = credentials.family_name;

                // Grant access to endpoint
                next();
            } else {
                // Bad user
                res.status(403).send('Bad Token.');
            }
        } else {
            // No token
            res.status(401).send('Token missing.');
        }
    }
};
    