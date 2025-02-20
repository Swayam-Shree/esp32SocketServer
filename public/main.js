const socket = new WebSocket("wss://esp32socketserver.onrender.com/");

socket.onopen = () => console.log("Connected to WebSocket server");
socket.onmessage = (event) => {
    const messagesDiv = document.getElementById("messages");
    messagesDiv.innerHTML += `<p>${event.data}</p>`;
};

function sendMessage() {
    const input = document.getElementById("messageInput");
    if (input.value) {
        socket.send(input.value);
        input.value = "";
    }
}
