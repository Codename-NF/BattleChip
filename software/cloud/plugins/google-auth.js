const { OAuth2Client } = require('google-auth-library');
const client = new OAuth2Client(process.env.GOOGLE_CLIENT_ID);

module.exports = {
    checkGoogle: async (token) => {
        // Attempt to verify the token
        let ticket = null;
        try {
            ticket = await client.verifyIdToken({
                idToken: token,
                audience: process.env.GOOGLE_CLIENT_ID,
            });
        } catch (err) {
            console.log(err);
            return null;
        }

        // Token is good, so extract the credentials
        const payload = ticket.getPayload();
        const { sub, email, given_name, family_name } = payload;
        const userId = sub;

        return { userId, email, given_name, family_name };
    }
}