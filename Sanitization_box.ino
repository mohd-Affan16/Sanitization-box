#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
/* ================= PIN DEFINITIONS ================= */
#define RELAY_PIN D1 // GPIO5
#define BUZZER_PIN D6 // GPIO12
#define IR_PIN D5 // GPIO14
/* ================= WIFI LIST ================= */
const char* ssidList[] = {
" WiFi_1",
" WiFi_2",
"WiFi_3"
};
const char* passList[] = {
" password1",
" password2",
"password3"
};
const int wifiCount = 3;
/* ================= VARIABLES ================= */
bool processRunning = false;

SANITIZATION BOX

Page 13 of 36

Department of Computer Science,PACE
unsigned long processEndTime = 0;
unsigned long buzzerEndTime = 0;
/* ================= HTML PAGE ================= */
const char MAIN_page[] PROGMEM = R"====(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1.0" />
<title>Process Log Book</title>
<style>
body {
font-family: Arial, sans-serif;
margin: 0;
padding: 20px;
background: #D2C1B6;
}
h2 {
text-align: center;
margin-bottom: 20px;
color:#1B3C53;
font-family: Georgia, 'Times New Roman', Times, serif;
font-size: 40px;
text-shadow: 2px 2px 4px gray;
}
.container {
max-width: 900px;
margin: auto;
}
.top-bar {
display: flex;
justify-content: space-between;
margin-bottom: 15px;
}
button {
padding: 10px 16px;
font-size: 16px;
background: #1B3C53;
border-color: #D2C1B6;
color: white;
border-radius: 7px;
cursor: pointer;
}
#cancelBtn {
background: red;
display: none;
}
button:disabled {
background: gray;
cursor: not-allowed;
}
.table-wrapper {
max-height: 450px;
overflow-y: auto;
border-radius: 8px;
}
table {
width: 100%;
background: #456882;
border-collapse: collapse;
box-shadow: 0px 0px 10px rgba(0,0,0,0.1);
}
th, td {
padding: 12px;
border-bottom: 1px solid #ddd;
text-align: center;
color: white;
}
th {
background: #1B3C53;
color: white;
}
td {
height: 55px;
}
.status-badge {
padding: 6px 12px;
border-radius: 5px;
color: white;
font-weight: bold;
}
.running { background: orange; }
.completed { background: green; }
.failed { background: red; }
/* Mobile responsive */
@media (max-width: 600px) {
.status-badge {
font-size: 7px;
padding: 4px;
}
td, th {
font-size: 10px;
padding: 6px;
height: 45px;
}
button {
width: 30%;
font-size: 12px;
height: 30px;
padding: 0;
}
body {
padding: 10px;
}
.table-wrapper {
max-height: 400px;
}
}
</style>
</head>
<body>
<div class="container">
<h2>Process Log Book</h2>
<div class="top-bar">
<button id="startBtn" onclick="openProcessPopup()">Start Process</button>
<button id="cancelBtn" onclick="cancelProcess()">Cancel Process</button>
</div>
<div class="table-wrapper">
<table>
<thead>
<tr>
<th>S.No</th>
<th>Date</th>
<th>Item</th>
<th>Duration</th>
<th>Status</th>
<th>Sanitization<br>Process</th>
</tr>
</thead>
<tbody id="logBody"></tbody>
</table>
</div>
</div>
<!-- START PROCESS POPUP -->
<div id="popup" style="display:none; position:fixed; inset:0; background:rgba(0,0,0,0.5);">
<div style="background:#1B3C53; width:90%; max-width:350px; margin:100px auto;
padding:20px; border-radius:10px;">
<div style="color:white; font-size:18px; font-weight:bold; margin-bottom:15px;">Start New
Process</div>
<div style="color:white;">Item Name:</div>
<input id="itemName" type="text" style="width:95%; padding:10px; margin-bottom:10px;
background:#D2C1B6;" />
<div style="color:white;">Duration (minutes):</div>

<input id="durationInput" type="number" style="width:95%; padding:10px; margin-
bottom:15px; background:#D2C1B6;" />
<button onclick="startNewProcess()">Start</button>
<button onclick="closePopup()" style="background:red; margin-left:10px;">Cancel</button>
</div>
</div>
<!-- NOTIFICATION POPUP (ADDED FEATURE) -->
<div id="notifyPopup" style="display:none; position:fixed; inset:0; background:rgba(0,0,0,0.5);">
<div style="background:#1B3C53; width:90%; max-width:300px; margin:150px auto;
padding:20px; border-radius:10px; text-align:center;">
<div id="notifyText" style="color:white; font-size:18px; margin-bottom:20px;"></div>
<button onclick="closeNotify()">OK</button>
</div>
</div>
<script>
let processRunning = false;
let countdownTimer = null;
let serial = 0;
let currentRow = null;
let remainingSeconds = 0;
function openProcessPopup() {
if (processRunning) return;
document.getElementById("popup").style.display = "block";
}
function closePopup() {
document.getElementById("popup").style.display = "none";
}
/* NOTIFICATION FUNCTIONS */
function showNotify(message) {
document.getElementById("notifyText").innerText = message;
document.getElementById("notifyPopup").style.display = "block";
}
function closeNotify() {
document.getElementById("notifyPopup").style.display = "none";
}
// START PROCESS
async function startNewProcess() {
let item = document.getElementById("itemName").value.trim();
let minutes = parseInt(document.getElementById("durationInput").value);
if (!item || minutes <= 0) {
alert("Enter valid item & duration");
return;
}
try {
let statusRes = await fetch('/status');
if (!statusRes.ok) {
showNotify("Error checking door status.");
return;
}
let status = await statusRes.text();
if (status === 'OPEN') {
showNotify("Door is not closed. Please close the door and try again.");
return;
}
let timeSec = minutes * 60;
let startRes = await fetch(`/start?time=${timeSec}`);
if (startRes.status === 403) {
showNotify("Door is open. Please close it.");
return;
}
if (!startRes.ok) {
showNotify("Failed to start process.");
return;
}
// Proceed if started successfully
closePopup();
processRunning = true;
document.getElementById("startBtn").disabled = true;
document.getElementById("cancelBtn").style.display = "inline-block";
serial++;
let now = new Date().toLocaleString();
currentRow = document.getElementById("logBody").insertRow(0);

currentRow.insertCell(0).innerText = serial;
currentRow.insertCell(1).innerText = now;
currentRow.insertCell(2).innerText = item;
currentRow.insertCell(3).innerText = formatTime(timeSec);
let statusCell = currentRow.insertCell(4);
statusCell.innerHTML = `<span class="status-badge running">On Process</span>`;
let sanitizationCell = currentRow.insertCell(5);
sanitizationCell.innerText = "---";
remainingSeconds = timeSec;
countdownTimer = setInterval(() => {
remainingSeconds--;
currentRow.cells[3].innerText = formatTime(remainingSeconds);
if (remainingSeconds <= 0) {
completeProcess(statusCell, sanitizationCell, minutes);
}
}, 1000);
} catch (e) {
showNotify("Error: " + e.message);
}
}
// CANCEL PROCESS
async function cancelProcess() {
try {
let cancelRes = await fetch('/cancel');
if (!cancelRes.ok) {
showNotify("Failed to cancel process.");
return;
}
clearInterval(countdownTimer);
currentRow.cells[4].innerHTML = `<span class="status-badge failed">Cancelled</span>`;
currentRow.cells[5].innerText = "Process was manually cancelled";
resetUI();
showNotify("❌ Process Cancelled");
} catch (e) {
showNotify("Error: " + e.message);
}
}
// COMPLETE PROCESS
function completeProcess(statusCell, sanitizationCell, minutes) {
clearInterval(countdownTimer);
statusCell.innerHTML = `<span class="status-badge completed">Completed</span>`;
if (minutes < 2) sanitizationCell.innerText = "Insufficient duration";
else if (minutes < 5) sanitizationCell.innerText = "Door not closed properly";
else if (minutes < 10) sanitizationCell.innerText = "Battery Low";
else sanitizationCell.innerText = "Fully Sanitized";
resetUI();
showNotify("✅ Process Completed Successfully");
}
function resetUI() {
processRunning = false;
document.getElementById("startBtn").disabled = false;
document.getElementById("cancelBtn").style.display = "none";
}
// FORMAT TIME
function formatTime(sec) {
return Math.floor(sec / 60) + ":" + String(sec % 60).padStart(2, "0");
}
</script>
</body>
</html>

/* ================= WIFI CONNECT ================= */
void connectWiFi() {
WiFi.mode(WIFI_STA);
for (int i = 0; i < wifiCount; i++) {
Serial.print("Connecting to ");
Serial.println(ssidList[i]);
WiFi.begin(ssidList[i], passList[i]);
int attempt = 0;
while (WiFi.status() != WL_CONNECTED && attempt < 15) {
delay(500);
Serial.print(".");
attempt++;
}
if (WiFi.status() == WL_CONNECTED) {
Serial.println("\nConnected!");
Serial.print("IP Address: ");
Serial.println(WiFi.localIP());
return;
}
}
Serial.println("WiFi Failed. Check credentials.");
}
/* ================= SETUP ================= */
void setup() {
Serial.begin(9600);
pinMode(RELAY_PIN, OUTPUT);
pinMode(BUZZER_PIN, OUTPUT);
pinMode(IR_PIN, INPUT);
digitalWrite(RELAY_PIN, LOW);
digitalWrite(BUZZER_PIN, LOW);
connectWiFi();
server.on("/", []() {
server.send_P(200, "text/html", MAIN_page);
});
server.on("/status", handleStatus);
server.on("/start", handleStart);
server.on("/cancel", handleCancel);
server.begin();
Serial.println("Web Server Started");
}
/* ================= LOOP ================= */
void loop() {
server.handleClient();
if (processRunning && millis() > processEndTime) {
Serial.println("Process finished");
digitalWrite(RELAY_PIN, LOW);
Serial.println("Relay OFF");
digitalWrite(BUZZER_PIN, HIGH);
Serial.println("Buzzer ON");
buzzerEndTime = millis() + 3000UL; // 3 seconds
processRunning = false;
}
if (buzzerEndTime && millis() > buzzerEndTime) {
digitalWrite(BUZZER_PIN, LOW);
Serial.println("Buzzer OFF");
buzzerEndTime = 0;
}
}
/* ================= HANDLERS ================= */
void handleStatus() {
int ir = digitalRead(IR_PIN);
Serial.print("IR Sensor: ");
Serial.println(ir);
if (ir == LOW)
server.send(200, "text/plain", "CLOSED");
else
server.send(200, "text/plain", "OPEN");
}
void handleStart() {
if (digitalRead(IR_PIN) == HIGH) {
Serial.println("Start blocked: Door open");
server.send(403, "text/plain", "OPEN");
return;
}
int timeSec = server.arg("time").toInt();
processEndTime = millis() + (timeSec * 1000UL);
processRunning = true;
digitalWrite(RELAY_PIN, HIGH);
Serial.println("Relay ON");
server.send(200, "text/plain", "STARTED");
}

void handleCancel() {
processRunning = false;
digitalWrite(RELAY_PIN, LOW);
Serial.println("Relay OFF - Cancelled");
if (buzzerEndTime) {
digitalWrite(BUZZER_PIN, LOW);
Serial.println("Buzzer OFF - Cancelled");
buzzerEndTime = 0;
}
server.send(200, "text/plain", "CANCELLED");
}