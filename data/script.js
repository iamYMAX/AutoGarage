let ws;

function connectWebSocket() {
    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const wsUrl = `${protocol}//${window.location.host}/ws`;

    ws = new WebSocket(wsUrl);

    ws.onopen = () => {
        document.getElementById('connection-status').innerText = 'Connected';
        document.getElementById('connection-status').style.color = '#4caf50';
    };

    ws.onclose = () => {
        document.getElementById('connection-status').innerText = 'Disconnected';
        document.getElementById('connection-status').style.color = '#f44336';
        setTimeout(connectWebSocket, 2000);
    };

    ws.onmessage = (event) => {
        try {
            const data = JSON.parse(event.data);
            if (data.rpm !== undefined) document.getElementById('rpm-val').innerText = data.rpm;
            if (data.vBat !== undefined) document.getElementById('vbat-val').innerText = Number(data.vBat).toFixed(2);
            if (data.can !== undefined) document.getElementById('can-val').innerText = data.can;
            if (data.events !== undefined) document.getElementById('events-val').innerText = data.events;
            if (data.dataMode !== undefined) {
                const modes = ['SIMULATION', 'LOOPBACK', 'REAL', 'HYBRID'];
                document.getElementById('mode-display').innerText = modes[data.dataMode] || 'UNKNOWN';
            }
        } catch (e) {
            console.error('Error parsing WebSocket message:', e);
        }
    };
}

document.getElementById('set-rpm-btn').addEventListener('click', () => {
    const rpm = parseInt(document.getElementById('rpm-input').value, 10);
    if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({ rpm: rpm }));
    }
});

document.getElementById('set-mode-btn').addEventListener('click', () => {
    const mode = parseInt(document.getElementById('mode-select').value, 10);
    if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({ dataMode: mode }));
    }
});

window.onload = connectWebSocket;
