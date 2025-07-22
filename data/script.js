console.log("Script loaded");

const rpmValueElem = document.getElementById('rpm-value');
const genModeElem = document.getElementById('gen-mode');
const genDutyElem = document.getElementById('gen-duty');
const wifiStatusElem = document.getElementById('wifi-status');
const logBoxElem = document.getElementById('log-box');

let gateway = `ws://${window.location.hostname}/ws`;
let websocket;

function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
    wifiStatusElem.textContent = 'Connected';
    wifiStatusElem.style.color = '#4caf50';
}

function onClose(event) {
    console.log('Connection closed');
    wifiStatusElem.textContent = 'Disconnected. Retrying...';
    wifiStatusElem.style.color = '#f44336';
    setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
    console.log('Message from server ', event.data);
    addToLog(event.data);

    try {
        const data = JSON.parse(event.data);
        if (data.rpm !== undefined) {
            rpmValueElem.textContent = data.rpm;
        }
        if (data.gen_mode !== undefined) {
            genModeElem.textContent = data.gen_mode;
        }
        if (data.gen_duty !== undefined) {
            genDutyElem.textContent = data.gen_duty;
        }
    } catch (e) {
        // Not a JSON object, just a log message
    }
}

function addToLog(message) {
    logBoxElem.innerHTML += message + '\n';
    logBoxElem.scrollTop = logBoxElem.scrollHeight;
}

window.onload = initWebSocket;
