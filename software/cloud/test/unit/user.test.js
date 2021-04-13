const User = require("../../models/user");
const userController = require("../../controllers/user");
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

/* User tests */
describe("Get user", () => {
    it("Case: User exists", async () => {
        const req = mockRequest();
        const res = mockResponse();
        
        /* Middleware stores email into res.locals */
        res.locals.email = testUser1.email;

        /* Test getUser functionality */
        await userController.getUser(req, res);

        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(200);
        expect(res.body).toMatchObject(testUser1);
    });

    it("Case: User does not exist", async () => {
        const req = mockRequest();
        const res = mockResponse();
        
        /* Middleware stores email into res.locals */
        res.locals.email = "fakeuser@gmail.com";

        /* Test getUser functionality */
        await userController.getUser(req, res);
        
        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(404);
    });
    
    it("Case: Email parameter is missing", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* Test getUser functionality */
        await userController.getUser(req, res);
        
        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(404);
    });
    
    it("Case: Email parameter is improperly formatted", async () => {
        const req = mockRequest();
        const res = mockResponse();

        /* Intentional set bad parameter */
        res.locals.email = { fail: 777 };

        /* Test getUser functionality */
        await userController.getUser(req, res);
        
        /* Expected response */
        expect(res.status).toHaveBeenCalledWith(404);
    });
});
