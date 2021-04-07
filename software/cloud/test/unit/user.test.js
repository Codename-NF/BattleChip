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

// beforeAll(async () => {
//     await testDB.connect();

//     /* Clear out the testDB */
//     await List.deleteMany({});
//     await User.deleteMany({});

//     await User.create({
//         id: TestUserID,
//         name: "TestUserX",
//         firebaseToken: "FakeTokenX",
//     });
    
//     await List.create(testList1);
//     await List.create(testList2);
//     await List.create(testList4);
//     await List.create(testList5);
// });

// afterAll(async () => {
//     await testDB.close();
// });

describe("Basic", () => {
    it("This should pass", async () => {
        const thisWorks = true;
        expect(thisWorks).toEqual(true);
    });
});

describe("Get User", () => {
    it("Basic", async () => {
        const myUser = await User.find({player_id: 1}).catch((error) => {
            console.log(error);
            return null; }
        )
        expect(myUser).toEqual(testUser1);
    });
});