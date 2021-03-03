const User = require('../models/user');
const Match = require('../models/match');

const updateUserRecord = async (email, didWinGame) => {
    if (didWinGame) {
        // Increment wins by 1
        await User.findOneAndUpdate({ email: email }, { $inc: { "wins": 1 }});
    }
    else {
        // Increment losses by 1
        await User.findOneAndUpdate({ email: email }, { $inc: { "losses": 1 }});
    }
}

module.exports = {
	getMatches: async (req, res) => {
		const userEmail = req.params.email
        
        // Check if user exists
        const myUser = await User.findOne({ email: userEmail }).catch(() => null);
        
        // If so, get an array of that user's matches
        if (myUser) {
            const myMatchesP1 = await Match.find({ player_one: userEmail }).catch(() => []);
            const myMatchesP2 = await Match.find({ player_two: userEmail }).catch(() => []);
            const allMyMatches = myMatchesP1.concat(myMatchesP2);
            
            res.status(200).send(
                {
                    "status" : "OK",
                    "matches" : allMyMatches,
                }
            )
        }
        // If not, respond with a 404
        else {
            res.status(404).send({"status" : "ERR"});
		}
	},
    
    createMatch: async (req, res) => {
        // Check if player1 exists
        const playerOneEmail = req.body.player_one;
        const playerTwoEmail = req.body.player_two;
        
        const player1 = await User.findOne({ email: playerOneEmail }).catch(() => null);
        if (player1) {
            
            // If player two is an AI
            if (req.body.player_two === "AI") {
                // Create a record of the match
                const newMatch = {
                    player_one: playerOneEmail,
                    player_two: playerTwoEmail,
                    winner: req.body.winner,
                    player_one_score: req.body.player_one_score,
                    player_two_score: req.body.player_two_score,
                }
                await Match.create(newMatch);
                
                // Update only player one's personal stats
                await updateUserRecord(playerOneEmail, (playerOneEmail === req.body.winner));
                
                // Respond with a 201
                res.status(201).send(
                    {
                        "status" : "OK"
                    }
                );
            }
            // If player two is NOT an AI
            else {
                // Verify that player two exists as a user
                const player2 = await User.findOne({ email: playerTwoEmail }).catch(() => null);
                if (player2) {
                    // Create a record of the match
                    const newMatch = {
                        player_one: playerOneEmail,
                        player_two: playerTwoEmail,
                        winner: req.body.winner,
                        player_one_score: req.body.player_one_score,
                        player_two_score: req.body.player_two_score,
                    }
                    await Match.create(newMatch);
                    
                    // Update both players' personal stats
                    await updateUserRecord(playerOneEmail, (playerOneEmail === req.body.winner));
                    await updateUserRecord(playerTwoEmail, (playerTwoEmail === req.body.winner));
                    
                    // Respond with a 201
                    res.status(201).send(
                        {
                            "status" : "OK"
                        }
                    );
                }
                // Respond with an error if player two doesn't exist
                else {
                    res.status(404).send(
                        {
                            "status" : "ERR",
                            "message" : req.body.player_two + " is not a user."
                        }
                    );
                }
            }
        }
        // Respond with an error if player one doesn't exist
        else {
            res.status(404).send(
                {
                    "status" : "ERR",
                    "message" : req.body.player_one + " is not a user."
                }
            );
		}
    }
}