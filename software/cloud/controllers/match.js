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
		const userEmail = res.locals.email
        
        // Get the page number from the page parameter (?page=)
        let pageNumber = parseInt(req.query.page);
        
        // Default to page 1 if requested page is invalid
        if (pageNumber === NaN) {
            pageNumber = 1;
        }
        else {
            // If user gave a page number with a decimal, just round down
            pageNumber = (Math.floor(pageNumber) > 1) ? Math.floor(pageNumber) : 1;
        }
        
        // Check if user exists
        const myUser = await User.findOne({ email: userEmail }).catch(() => null);

        // If User exists, get an array of that user's matches
        if (myUser) {
            const myMatchesP1 = await Match.find({ player_one: userEmail }).catch(() => []);
            const myMatchesP2 = await Match.find({ player_two: userEmail }).catch(() => []);
            const allMyMatches = myMatchesP1.concat(myMatchesP2);
            
            // Sort matches by most recent date
            allMyMatches.sort((a, b) => parseFloat(b.date) - parseFloat(a.date));
            
            // Get the requested page of up to 10 entries
            const firstIndex = 10 * (pageNumber - 1);
            const lastIndex = 10 * pageNumber;
            
            res.status(200).send(
                {
                    "status" : "OK",
                    "matches" : allMyMatches.slice(firstIndex, lastIndex),
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
                    player_one_name: `${player1.first_name} ${player1.last_name}`,
                    player_two_name: "AI",
                    date: Date.now() * 1000
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
                        player_one_name: `${player1.first_name} ${player1.last_name}`,
                        player_two_name: `${player2.first_name} ${player2.last_name}`,
                        date: Date.now() * 1000
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