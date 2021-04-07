const userController = require("../../controllers/user");
const User = require("../../models/user");

const testDB = require("../db/mongodb");

const testUser1 = {
    first_name: "TestUser",
    last_name: "One",
    email: "test1@gmail.com",
    wins: 0,
    losses: 0,
    player_id: 1
}

beforeAll(async () => {
    await testDB.connect();

    /* Clear out the testDB */
    await User.deleteMany({});
    
    /* Populate DB */
    await User.create(testUser1);
});

afterAll(async () => {
    await testDB.close();
});

// const mockRequest = () => {
//     const req = {};
//     req.params = {};
//     req.body = {};
//     return req;
// };

// const mockResponse = () => {
//     const res = {};
//     res.body = {};
//     res.status = jest.fn().mockReturnValue(res);
//     // res.send = jest.fn().mockReturnValue(res);
//     res.send = (result) => {
//         res.body = JSON.parse(JSON.stringify(result));
//     };

//     res.locals = {};
//     return res;
// };

describe("Sanity Check", () => {
    it("Get user from the database", async () => {
        const myUser = await User.findOne({player_id: 1}).catch((error) => null);
        
        expect(myUser.email).toEqual(testUser1.email);
        expect(myUser.first_name).toEqual(testUser1.first_name);
        expect(myUser.last_name).toEqual(testUser1.last_name);
        expect(myUser.losses).toEqual(testUser1.losses);
        expect(myUser.wins).toEqual(testUser1.wins);
        expect(myUser.player_id).toEqual(testUser1.player_id);
    });
});
