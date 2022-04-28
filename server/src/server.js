const { EventEmitter } = require("events");
const { Server } = require("ws");
const express = require("express");
const cors = require("cors");
let server = express();

server.use(cors());

server = server.listen(process.env.PORT || 3000, () => console.log(`Listening on ${process.env.PORT || 3000}`));
const ws = new Server({ server });
const bus = new EventEmitter();

const subscribers = [];
let publisher = null;

bus.on("update", async (data) => {
    // console.log(JSON.parse(msg));
    subscribers.forEach((sub) => {
        sub.send(data, { binary: false });
    });
});

bus.on("command", (cmd) => {
    type = String(cmd);
    if (publisher) {
        publisher.send(cmd, { binary: false });
    }
});

ws.on("connection", (socket, req) => {
    switch (req.url) {
        case "/sensor":
            publisher = socket;
            console.log("[SENSOR]:[CONNECTED]");
            socket.on("message", (msg) => {
                console.log(msg);
                bus.emit("update", msg);
            });
            socket.on("close", (msg) => {
                console.log("[SENSOR]:[DISCONNECTED]");
            });
            break;
            case "/client":
            subscribers.push(socket);
            console.log("[CLIENT]:[CONNECTED]");
            socket.on("message", (cmd) => {
                bus.emit("command", cmd);
            });
            break;
            default:
                break;
    }
});