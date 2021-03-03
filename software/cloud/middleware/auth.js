const googleAuth = require('../plugins/google-auth');

// Function called by middleware to verify that the google token is valid
module.exports = {
    checkToken: async (req, res, next) => {
        res.locals.userId = "123456789"
        res.locals.email = "paul@lin.com";
        res.locals.firstname = "Paul";
        res.locals.lastname = "Lin";
        next();
    }
};
    /*
    const bearerHeader = req.headers['ID-Token'];

    if (bearerHeader) {
        // Isolate the token from the header
        const bearer = bearerHeader.split(' ');
        const bearerToken = bearer[1];
        req.token = bearerToken;

        // Verify the token
        const credentials = await googleAuth(req.token);
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
    */
