const WebSocket = require('ws');
const express = require('express');
const http = require('http');
const path = require('path');
const cors = require('cors');

const app = express();

app.use(cors());
app.use(express.static(path.join(__dirname, 'public')));
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

let espSocket = null;

let idCounter = 0;

wss.on('connection', (socket) => {
    socket.id = idCounter;
    ++idCounter;
    console.log('Client connected', socket.id);

    socket.on('message', (message) => {
        message = message.toString();

        if (message === "espInit") {
            espSocket = socket;
            console.log("ESP connected");
            espSocket.send("espInitAck");
        }

        if (espSocket && espSocket.id === socket.id) {
            console.log("ESP sent: " + message);
        } else {
            console.log("Browser sent: " + message);
            if (espSocket) {
                espSocket.send(message);
            } else {
                console.log("esp is not connected")
            }
        }
    });

    socket.on('close', () => console.log('Client disconnected'));
});

server.listen(6969, () => {
    console.log('Server running on http://localhost:6969');
});