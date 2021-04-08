const User = require("../../models/user");
const loginController = require("../../controllers/login");
const testDB = require("../db/mongodb");

/* Test users to be inserted in test DB */
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
    wins: 0,
    losses: 0,
    player_id: 2
}
const testUser2 = {
    first_name: "TestUser",
    last_name: "Two",
    email: "test2@gmail.com",
    wins: 0,
    losses: 0,
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
    first_name: "",
    last_name: "Four",
    email: "test4@gmail.com",
    wins: 0,
    losses: 0,
    player_id: 5
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

    /* Clear out the testDB */
    await User.deleteMany({});
    
    /* Populate DB */
    await User.create(easyAI);
    await User.create(hardAI);
    await User.create(testUser1);
});

/* Cleanup */
afterAll(async () => {
    /* Disconnect from DB */
    await testDB.close();
});

/* Login tests */
describe("Login", () => {
    it("Case: User exists already", async () => {
        const req = mockRequest();
        const res = mockResponse();
        
        /* Middleware stores email into res.locals */
        res.locals.email = testUser1.email;

        /* Test getUser functionality */
        await loginController.loginUser(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(200);
        expect(res.body).toMatchObject(testUser1);
    });

    it("Case: User is a new player", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* User creation uses fullname and email */
        res.locals.firstname = testUser2.first_name;
        res.locals.lastname = testUser2.last_name;
        res.locals.email = testUser2.email;

        /* Test getUser functionality */
        await loginController.loginUser(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(201);
        expect(res.body).toMatchObject(testUser2);
    });

    it("Case: User is a new player (no lastname)", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* User creation uses fullname and email */
        res.locals.firstname = testUser3.first_name;
        res.locals.email = testUser3.email;

        /* Test getUser functionality */
        await loginController.loginUser(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(201);
        expect(res.body).toMatchObject(testUser3);
    });

    it("Case: User is a new player (no firstname)", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* User creation uses fullname and email */
        res.locals.email = testUser4.email;

        /* Test getUser functionality */
        await loginController.loginUser(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(500);
    });
    
    it("Case: User is a new player (improper email format)", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* User creation uses fullname and email */
        res.locals.email = { fail: 777 };

        /* Test getUser functionality */
        await loginController.loginUser(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(500);
    });
});
