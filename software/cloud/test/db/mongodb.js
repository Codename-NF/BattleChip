const mongoose = require("mongoose");
const { MongoMemoryServer } = require("mongodb-memory-server");

const mongod = new MongoMemoryServer();

module.exports = {
    connect: async () => {
        const uri = await mongoServer.getUri()
        const mongooseOpts = {
            useNewUrlParser: true,
            useUnifiedTopology: true
        };
        await mongoose.connect(uri, mongooseOpts);
    },
    close: async () => {
        await mongoose.connection.dropDatabase();
        await mongoose.connection.close();
        await mongod.stop();
    }
};