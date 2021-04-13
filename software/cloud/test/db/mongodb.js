const mongoose = require("mongoose");
const { MongoMemoryServer } = require("mongodb-memory-server");

const mongod = new MongoMemoryServer();

module.exports = {
    connect: async () => {
        const mongooseOpts = {
            useNewUrlParser: true,
            useUnifiedTopology: true
        };
        await mongoose.connect("mongodb://localhost:27018/test", mongooseOpts);
    },
    close: async () => {
        await mongoose.connection.dropDatabase();
        await mongoose.connection.close();
        await mongod.stop();
    }
};