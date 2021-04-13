const User = require("../../models/user");
const Match = require("../../models/match");
const matchController = require("../../controllers/match");
const testDB = require("../db/mongodb");

/* Test data to be inserted in test DB */
const easyAI = {
    first_name: "Easy",
    last_name: "AI",
    email: "Easy_AI_Email",
    wins: 0,
    losses: 0,
    player_id: 0,
}
const hardAI = {
    first_name: "Hard",
    last_name: "AI",
    email: "Hard_AI_Email",
    wins: 0,
    losses: 0,
    player_id: 1,
}
const testUser1 = {
    first_name: "TestUser",
    last_name: "One",
    email: "test1@gmail.com",
    wins: 2,
    losses: 0,
    player_id: 2
}
const testUser2 = {
    first_name: "TestUser",
    last_name: "Two",
    email: "test2@gmail.com",
    wins: 0,
    losses: 2,
    player_id: 3
}
const testUser3 = {
    first_name: "TestUser",
    last_name: " ",
    email: "test3@gmail.com",
    wins: 0,
    losses: 0,
    player_id: 4
}
const testUser4 = {
    first_name: "TestUser",
    last_name: "Four",
    email: "test4@gmail.com",
    wins: 0,
    losses: 0,
    player_id: 5
}
const testMatch1 = {
    player_one: testUser1.email,
    player_two: testUser2.email,
    winner: testUser1.email,
    player_one_score: 5,
    player_two_score: 3,
    player_one_name: `${testUser1.first_name} ${testUser1.last_name}`,
    player_two_name: `${testUser2.first_name} ${testUser2.last_name}`,
    date: 6
}
const testMatch2 = {
    player_one: testUser2.email,
    player_two: testUser1.email,
    winner: testUser2.email,
    player_one_score: 2,
    player_two_score: 5,
    player_one_name: `${testUser2.first_name} ${testUser2.last_name}`,
    player_two_name: `${testUser1.first_name} ${testUser1.last_name}`,
    date: 7
}

/* Returns a mocked incoming request object */
const mockRequest = () => {
    const req = {};
    req.params = {};
    req.body = {};
    return req;
};

/* Returns a mocked outgoing response object */
const mockResponse = () => {
    const res = {};
    res.body = {};
    res.status = jest.fn().mockReturnValue(res);
    res.locals = {};
    res.send = (result) => {
        res.body = JSON.parse(JSON.stringify(result));
    };
    return res;
};

/* Initialization */
beforeAll(async () => {
    /* Connect to MongoDB memory server */
    await testDB.connect();
});

beforeEach(async () => {
    /* Clear out the testDB */
    await User.deleteMany({});
    await Match.deleteMany({});

    /* Populate DB */
    await User.create(easyAI);
    await User.create(hardAI);
    await User.create(testUser1);
    await User.create(testUser2);
    await User.create(testUser3);
    await User.create(testUser4);
    await Match.create(testMatch1);
    await Match.create(testMatch2);
})

/* Cleanup */
afterAll(async () => {
    /* Disconnect from DB */
    await testDB.close();
});

/* Match retrieval tests */
describe("Get user's matches", () => {
    it("Case: User has two matches", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* Middleware stores email into res.locals */
        res.locals.email = testUser1.email;

        /* We expect the following sorted array of matches */
        const expectedMatches = [testMatch2, testMatch1]

        /* Test getUser functionality */
        await matchController.getMatches(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(200);
        expect(res.body.matches).toMatchObject(expectedMatches);
    });
    it("Case: User has no matches", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* Middleware stores email into res.locals */
        res.locals.email = testUser4.email;

        /* We expect the following sorted array of matches */
        const expectedMatches = [testMatch2, testMatch1]

        /* Test getUser functionality */
        await matchController.getMatches(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(200);
        expect(res.body.matches).toMatchObject([]);
    });
    it("Case: User doesn't exist", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* Test getUser functionality */
        await matchController.getMatches(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(404);
    });
    it("Case: Badly formatted email", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* Middleware stores email into res.locals */
        res.locals.email = 777;

        /* Test getUser functionality */
        await matchController.getMatches(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(404);
    });
});

/* Match creation tests */
describe("Create match", () => {
    it("Case: P1 beats Easy AI", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* P1 beats easy AI */
        const sampleMatchReqObj = {
            player_one: testUser1.player_id,
            player_two: easyAI.player_id,
            winner: testUser1.player_id,
            player_one_score: 5,
            player_two_score: 3,
        }
        req.body = sampleMatchReqObj;

        const expectedMatchObj = {
            player_one: testUser1.email,
            player_two: `${easyAI.first_name} ${easyAI.last_name}`,
            winner: testUser1.email,
            player_one_score: 5,
            player_two_score: 3,
            player_one_name: `${testUser1.first_name} ${testUser1.last_name}`,
            player_two_name: `${easyAI.first_name} ${easyAI.last_name}`,
            /* Arbitrary date for test */
            date: 90909090909,
        }

        /* Test getUser functionality */
        await matchController.createMatch(req, res);

        const createdMatch = res.body;
        createdMatch.date = 90909090909;

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(201);
        expect(createdMatch).toMatchObject(expectedMatchObj);

        /* Verify that user's record has been updated */
        const playerOne = await User.findOne({ email: testUser1.email });
        expect(playerOne.wins).toEqual(testUser1.wins + 1);
        expect(playerOne.losses).toEqual(testUser1.losses);
    });
    it("Case: P1 loses to Easy AI", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* P1 loses to easy AI */
        const sampleMatchReqObj = {
            player_one: testUser1.player_id,
            player_two: easyAI.player_id,
            winner: easyAI.player_id,
            player_one_score: 0,
            player_two_score: 5,
        }
        req.body = sampleMatchReqObj;

        const expectedMatchObj = {
            player_one: testUser1.email,
            player_two: `${easyAI.first_name} ${easyAI.last_name}`,
            winner: easyAI.email,
            player_one_score: 0,
            player_two_score: 5,
            player_one_name: `${testUser1.first_name} ${testUser1.last_name}`,
            player_two_name: `${easyAI.first_name} ${easyAI.last_name}`,
            /* Arbitrary date for test */
            date: 90909090909,
        }

        /* Test getUser functionality */
        await matchController.createMatch(req, res);

        const createdMatch = res.body;
        createdMatch.date = 90909090909;

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(201);
        expect(createdMatch).toMatchObject(expectedMatchObj);

        /* Verify that user's record has been updated */
        const playerOne = await User.findOne({ email: testUser1.email });
        expect(playerOne.wins).toEqual(testUser1.wins);
        expect(playerOne.losses).toEqual(testUser1.losses + 1);
    });
    it("Case: P1 beats Hard AI", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* P1 beats hard AI */
        const sampleMatchReqObj = {
            player_one: testUser1.player_id,
            player_two: hardAI.player_id,
            winner: testUser1.player_id,
            player_one_score: 5,
            player_two_score: 3,
        }
        req.body = sampleMatchReqObj;

        const expectedMatchObj = {
            player_one: testUser1.email,
            player_two: `${hardAI.first_name} ${hardAI.last_name}`,
            winner: testUser1.email,
            player_one_score: 5,
            player_two_score: 3,
            player_one_name: `${testUser1.first_name} ${testUser1.last_name}`,
            player_two_name: `${hardAI.first_name} ${hardAI.last_name}`,
            /* Arbitrary date for test */
            date: 90909090909,
        }

        /* Test getUser functionality */
        await matchController.createMatch(req, res);

        const createdMatch = res.body;
        createdMatch.date = 90909090909;

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(201);
        expect(createdMatch).toMatchObject(expectedMatchObj);

        /* Verify that user's record has been updated */
        const playerOne = await User.findOne({ email: testUser1.email });
        expect(playerOne.wins).toEqual(testUser1.wins + 1);
        expect(playerOne.losses).toEqual(testUser1.losses);
    });
    it("Case: P1 loses to Hard AI", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* P1 loses to hard AI */
        const sampleMatchReqObj = {
            player_one: testUser1.player_id,
            player_two: hardAI.player_id,
            winner: hardAI.player_id,
            player_one_score: 0,
            player_two_score: 5,
        }
        req.body = sampleMatchReqObj;

        const expectedMatchObj = {
            player_one: testUser1.email,
            player_two: `${hardAI.first_name} ${hardAI.last_name}`,
            winner: hardAI.email,
            player_one_score: 0,
            player_two_score: 5,
            player_one_name: `${testUser1.first_name} ${testUser1.last_name}`,
            player_two_name: `${hardAI.first_name} ${hardAI.last_name}`,
            /* Arbitrary date for test */
            date: 90909090909,
        }

        /* Test getUser functionality */
        await matchController.createMatch(req, res);

        const createdMatch = res.body;
        createdMatch.date = 90909090909;

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(201);
        expect(createdMatch).toMatchObject(expectedMatchObj);

        /* Verify that user's record has been updated */
        const playerOne = await User.findOne({ email: testUser1.email });
        expect(playerOne.wins).toEqual(testUser1.wins);
        expect(playerOne.losses).toEqual(testUser1.losses + 1);
    });
    it("Case: P1 beats P2", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* P1 beats easy AI */
        const sampleMatchReqObj = {
            player_one: testUser1.player_id,
            player_two: testUser2.player_id,
            winner: testUser1.player_id,
            player_one_score: 5,
            player_two_score: 3,
        }
        req.body = sampleMatchReqObj;

        const expectedMatchObj = {
            player_one: testUser1.email,
            player_two: testUser2.email,
            winner: testUser1.email,
            player_one_score: 5,
            player_two_score: 3,
            player_one_name: `${testUser1.first_name} ${testUser1.last_name}`,
            player_two_name: `${testUser2.first_name} ${testUser2.last_name}`,
            /* Arbitrary date for test */
            date: 90909090909,
        }

        /* Test getUser functionality */
        await matchController.createMatch(req, res);

        const createdMatch = res.body;
        createdMatch.date = 90909090909;

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(201);
        expect(createdMatch).toMatchObject(expectedMatchObj);

        /* Verify that user records have been updated */
        const playerOne = await User.findOne({ email: testUser1.email });
        const playerTwo = await User.findOne({ email: testUser2.email });
        expect(playerOne.wins).toEqual(testUser1.wins + 1);
        expect(playerOne.losses).toEqual(testUser1.losses);
        expect(playerTwo.wins).toEqual(testUser2.wins);
        expect(playerTwo.losses).toEqual(testUser2.losses + 1);
    });
    it("Case: P2 beats P1", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* P1 beats easy AI */
        const sampleMatchReqObj = {
            player_one: testUser1.player_id,
            player_two: testUser2.player_id,
            winner: testUser2.player_id,
            player_one_score: 2,
            player_two_score: 5,
        }
        req.body = sampleMatchReqObj;

        const expectedMatchObj = {
            player_one: testUser1.email,
            player_two: testUser2.email,
            winner: testUser2.email,
            player_one_score: 2,
            player_two_score: 5,
            player_one_name: `${testUser1.first_name} ${testUser1.last_name}`,
            player_two_name: `${testUser2.first_name} ${testUser2.last_name}`,
            /* Arbitrary date for test */
            date: 90909090909,
        }

        /* Test getUser functionality */
        await matchController.createMatch(req, res);

        const createdMatch = res.body;
        createdMatch.date = 90909090909;

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(201);
        expect(createdMatch).toMatchObject(expectedMatchObj);

        /* Verify that user records have been updated */
        const playerOne = await User.findOne({ email: testUser1.email });
        const playerTwo = await User.findOne({ email: testUser2.email });
        expect(playerOne.wins).toEqual(testUser1.wins);
        expect(playerOne.losses).toEqual(testUser1.losses + 1);
        expect(playerTwo.wins).toEqual(testUser2.wins + 1);
        expect(playerTwo.losses).toEqual(testUser2.losses);
    });
    it("Case: P1 wins but doesn't exist", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* P1 beats P2 */
        const sampleMatchReqObj = {
            player_one: 777,
            player_two: testUser2.player_id,
            winner: 777,
            player_one_score: 5,
            player_two_score: 3,
        }
        req.body = sampleMatchReqObj;

        /* Test getUser functionality */
        await matchController.createMatch(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(404);

        /* Verify user records remain unchanged by bad match */
        const playerOne = await User.findOne({ email: testUser1.email });
        const playerTwo = await User.findOne({ email: testUser2.email });
        expect(playerOne.wins).toEqual(testUser1.wins);
        expect(playerOne.losses).toEqual(testUser1.losses);
        expect(playerTwo.wins).toEqual(testUser2.wins);
        expect(playerTwo.losses).toEqual(testUser2.losses);
    });
    it("Case: P2 wins but doesn't exist", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* P2 beats P1 */
        const sampleMatchReqObj = {
            player_one: testUser1.player_id,
            player_two: 777,
            winner: 777,
            player_one_score: 0,
            player_two_score: 5,
        }
        req.body = sampleMatchReqObj;

        /* Test getUser functionality */
        await matchController.createMatch(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(404);

        /* Verify user records remain unchanged by bad match */
        const playerOne = await User.findOne({ email: testUser1.email });
        const playerTwo = await User.findOne({ email: testUser2.email });
        expect(playerOne.wins).toEqual(testUser1.wins);
        expect(playerOne.losses).toEqual(testUser1.losses);
        expect(playerTwo.wins).toEqual(testUser2.wins);
        expect(playerTwo.losses).toEqual(testUser2.losses);
    });
    it("Case: P2 loses but doesn't exist", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* P2 loses to P1 */
        const sampleMatchReqObj = {
            player_one: testUser1.player_id,
            player_two: 777,
            winner: testUser1.player_id,
            player_one_score: 5,
            player_two_score: 3,
        }
        req.body = sampleMatchReqObj;

        /* Test getUser functionality */
        await matchController.createMatch(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(404);

        /* Verify user records remain unchanged by bad match */
        const playerOne = await User.findOne({ email: testUser1.email });
        const playerTwo = await User.findOne({ email: testUser2.email });
        expect(playerOne.wins).toEqual(testUser1.wins);
        expect(playerOne.losses).toEqual(testUser1.losses);
        expect(playerTwo.wins).toEqual(testUser2.wins);
        expect(playerTwo.losses).toEqual(testUser2.losses);
    });
    it("Case: P1 loses but doesn't exist", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* P1 loses to P2 */
        const sampleMatchReqObj = {
            player_one: 777,
            player_two: testUser2.player_id,
            winner: testUser2.player_id,
            player_one_score: 0,
            player_two_score: 5,
        }
        req.body = sampleMatchReqObj;

        /* Test getUser functionality */
        await matchController.createMatch(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(404);

        /* Verify user records remain unchanged by bad match */
        const playerOne = await User.findOne({ email: testUser1.email });
        const playerTwo = await User.findOne({ email: testUser2.email });
        expect(playerOne.wins).toEqual(testUser1.wins);
        expect(playerOne.losses).toEqual(testUser1.losses);
        expect(playerTwo.wins).toEqual(testUser2.wins);
        expect(playerTwo.losses).toEqual(testUser2.losses);
    });
});
