# WiFi Remote Control Car — Full Beginner Build Guide

Built around: **ESP8266EX module + USB-to-TTL adapter**, **5V 4-channel relay board**, **2× 5V DC motors**

---

## Part 1 — What We're Actually Building (Plain Language)

Here's the whole idea in one paragraph:

Your phone will **not** connect to the internet for this. Instead, your ESP8266 will create its **own tiny WiFi hotspot**. Your phone connects to that hotspot (just like connecting to any home WiFi), then you open your phone's normal web browser and go to one address. A page with big **Forward / Backward / Left / Right / Stop** buttons shows up. When you tap a button, your phone tells the ESP8266 what to do, and the ESP8266 flips the relays on the relay board to send power to the two motors in the right direction. That's it — no app to install, no account, no internet needed.

**The one tricky part: a relay is just an on/off switch — it cannot make a motor spin backward by itself.** To make a motor go both forward AND backward, we need to flip the polarity (the + and −) of the wires going into it. We do that using **2 relays per motor** (4 relays total for your 2 motors) wired in a pattern called an **"H-bridge"**. Don't worry — you don't need to understand the theory. Just follow the wiring exactly as shown below and it will work. This guide is designed so your exact 4-relay board and 2 motors are enough — you don't need a 5th relay or a special motor driver chip.

**Important reality check about your ESP8266:** A bare ESP8266 module (the kind programmed through a USB-to-TTL adapter, usually called an "ESP-01") only has **2 easily-usable control pins**. This project needs **4** control pins (one per relay). Because of this, I strongly recommend also picking up a cheap **NodeMCU board** (~$3, also uses the exact same ESP8266EX chip inside, so nothing you own is wasted). It plugs straight into your computer over USB, has plenty of pins, and is dramatically easier and safer for a first project. I'll write the whole guide around the NodeMCU. At the end, I'll add a short note for the harder route if you insist on using the bare module.

---

## Part 2 — Full Parts List

### ✅ You already have:
- ESP8266EX module + USB-to-TTL adapter
- 5V 4-channel relay board
- 2× 5V DC motors

### 🛒 You need to buy (all cheap, available at any local electronics shop or online):

| # | Item | Why you need it | Notes |
|---|------|------------------|-------|
| 1 | **NodeMCU ESP8266 board** (e.g. "NodeMCU v3 CP2102") | Easiest brain for the car — has USB, enough pins, built-in voltage regulator | ~$3-4. Your bare ESP8266 chip + USB-TTL becomes optional/backup |
| 2 | **Small 2WD robot car chassis kit** (plastic/acrylic base + 2 wheels + caster/ball wheel) | A frame to mount everything on | Comes with mounting screws usually |
| 3 | **Battery holder for 4× AA batteries** (gives ~5–6V) OR a **small 5V/6V rechargeable battery pack** | Powers the motors + relay board | Do NOT use your laptop/PC USB to power motors |
| 4 | **4× AA batteries** (if using AA holder) | Power source | Rechargeable NiMH AA batteries work great too |
| 5 | **Male-to-female and male-to-male jumper wires** (a small pack of ~40) | All your wiring | Breadboard-style jumper wires |
| 6 | **Small breadboard** (optional but helpful) | Makes prototyping easier before permanent wiring | Not strictly required |
| 7 | **On/off power switch** (small toggle or slide switch) | Lets you turn the whole car on/off easily | Optional but very convenient |
| 8 | **Double-sided foam tape / small zip ties** | To mount the boards and battery onto the chassis | For final assembly |

That's it. No motor driver IC needed — your relay board replaces it.

---

## Part 3 — Understanding the "Relay Trick" (H-Bridge, Simplified)

Each of your 2 DC motors will use **2 of the 4 relay channels**:

- **Motor 1 (Left wheel):** uses Relay 1 (LEFT-FORWARD) + Relay 2 (LEFT-REVERSE)
- **Motor 2 (Right wheel):** uses Relay 3 (RIGHT-FORWARD) + Relay 4 (RIGHT-REVERSE)

For **each** relay, we wire it the exact same pattern:
- The relay's **COM** terminal → goes to one wire of the motor
- The relay's **NO** ("Normally Open") terminal → goes to Battery **+**
- The relay's **NC** ("Normally Closed") terminal → goes to Battery **−**

Here's why this is safe and simple — there are only 4 possible states per motor:

| Relay A (Fwd) | Relay B (Rev) | What happens |
|---|---|---|
| OFF | OFF | Both motor wires get Battery − → **motor stays still** ✅ safe |
| ON | ON | Both motor wires get Battery + → **motor stays still** ✅ safe |
| ON | OFF | One wire gets +, other gets − → **motor spins forward** |
| OFF | ON | Wires reversed → **motor spins backward** |

Notice: there is **no combination that ever short-circuits the battery**. The two "same" states just stop the motor. This is the standard, safe way electricians and hobbyists reverse small DC motors with relays, so you can wire this with confidence.

---

## Part 4 — Wiring Guide (Step by Step)

⚠️ Before touching anything: make sure batteries are **disconnected** while you wire things up. Only connect the battery at the very end, after double-checking everything.

### Step 1 — Power the relay board
- Connect the relay board's 5V/GND power input to the **NodeMCU's 5V (VU or VIN) and GND pins** — OR power it from the same battery pack you'll use for the motors (either works; sharing one battery for everything is fine for small motors).
- Connect relay board **GND** to NodeMCU **GND** no matter what (this "common ground" step is required for the signals to work — don't skip it).

### Step 2 — Connect the 4 signal wires (NodeMCU → Relay board)
Each relay channel on your board has its own small connector with 3 wires: **VCC, GND, Signal** (this matches the "in1/in2/in3/in4" labels on your board).

| Relay board channel | Connect Signal pin to NodeMCU pin | Purpose |
|---|---|---|
| IN1 | **D1** | Left motor – Forward |
| IN2 | **D2** | Left motor – Reverse |
| IN3 | **D5** | Right motor – Forward |
| IN4 | **D6** | Right motor – Reverse |

Also connect each channel's VCC to NodeMCU 3V3 (or 5V, per your board's label) and GND to NodeMCU GND.

### Step 3 — Wire the battery to the relay contacts (the blue screw terminals)
For **all four** relay channels, wire identically:
- Battery **+** → **NO** screw terminal
- Battery **−** → **NC** screw terminal
- **COM** screw terminal → goes to the motor (next step)

### Step 4 — Connect the motors
- **Left motor:** one wire → Relay 1's **COM**, other wire → Relay 2's **COM**
- **Right motor:** one wire → Relay 3's **COM**, other wire → Relay 4's **COM**

### Step 5 — Power switch (optional but recommended)
Put your on/off switch between the battery's + terminal and everything else, so you can kill power to the whole car in one flip.

### Text wiring summary (keep this next to you while wiring):

```
BATTERY (+) ──┬── Relay1-NO   BATTERY (−) ──┬── Relay1-NC
              ├── Relay2-NO                 ├── Relay2-NC
              ├── Relay3-NO                 ├── Relay3-NC
              └── Relay4-NO                 └── Relay4-NC

Relay1-COM ──── Left Motor wire A
Relay2-COM ──── Left Motor wire B
Relay3-COM ──── Right Motor wire A
Relay4-COM ──── Right Motor wire B

NodeMCU D1 ──── Relay board IN1 (signal)
NodeMCU D2 ──── Relay board IN2 (signal)
NodeMCU D5 ──── Relay board IN3 (signal)
NodeMCU D6 ──── Relay board IN4 (signal)
NodeMCU GND ─── Relay board GND (must connect!)
```

---

## Part 5 — Setting Up Arduino IDE for ESP8266

1. Download and install **Arduino IDE** (arduino.cc) if you don't have it.
2. Open Arduino IDE → **File → Preferences** → paste this into "Additional Boards Manager URLs":
   `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
3. Go to **Tools → Board → Boards Manager**, search "esp8266", install the package by ESP8266 Community.
4. Go to **Tools → Board**, select **"NodeMCU 1.0 (ESP-12E Module)"**.
5. Plug the NodeMCU into your computer with a USB cable, then select the correct **Port** under Tools → Port.

---

## Part 6 — The Arduino Code (copy, paste, upload)

Copy this whole block into a new sketch in Arduino IDE and click **Upload**. Nothing needs to be changed unless noted below.

```cpp
/*
  WiFi Relay-Controlled RC Car
  Board: NodeMCU (ESP8266)
  Creates its own WiFi hotspot; control it from a phone browser.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ---- WiFi hotspot settings (you can change the name/password) ----
const char* ssid     = "RC_Car";
const char* password = "12345678";   // must be at least 8 characters

// ---- Relay pins ----
#define LEFT_FWD   D1
#define LEFT_REV   D2
#define RIGHT_FWD  D5
#define RIGHT_REV  D6

// ---- Change this if your relays turn ON with HIGH instead of LOW ----
// Most cheap relay boards are "active LOW" (LOW = relay ON).
// If your motors behave backwards from what buttons say, flip these two lines.
#define RELAY_ON  LOW
#define RELAY_OFF HIGH

ESP8266WebServer server(80);

void allRelaysOff() {
  digitalWrite(LEFT_FWD,  RELAY_OFF);
  digitalWrite(LEFT_REV,  RELAY_OFF);
  digitalWrite(RIGHT_FWD, RELAY_OFF);
  digitalWrite(RIGHT_REV, RELAY_OFF);
}

void goForward() {
  digitalWrite(LEFT_REV,  RELAY_OFF);
  digitalWrite(RIGHT_REV, RELAY_OFF);
  digitalWrite(LEFT_FWD,  RELAY_ON);
  digitalWrite(RIGHT_FWD, RELAY_ON);
}

void goBackward() {
  digitalWrite(LEFT_FWD,  RELAY_OFF);
  digitalWrite(RIGHT_FWD, RELAY_OFF);
  digitalWrite(LEFT_REV,  RELAY_ON);
  digitalWrite(RIGHT_REV, RELAY_ON);
}

void turnLeft() {
  // left wheel stops, right wheel goes forward -> car pivots left
  digitalWrite(LEFT_FWD,  RELAY_OFF);
  digitalWrite(LEFT_REV,  RELAY_OFF);
  digitalWrite(RIGHT_REV, RELAY_OFF);
  digitalWrite(RIGHT_FWD, RELAY_ON);
}

void turnRight() {
  // right wheel stops, left wheel goes forward -> car pivots right
  digitalWrite(RIGHT_FWD, RELAY_OFF);
  digitalWrite(RIGHT_REV, RELAY_OFF);
  digitalWrite(LEFT_REV,  RELAY_OFF);
  digitalWrite(LEFT_FWD,  RELAY_ON);
}

// ---- The web page shown on the phone ----
String htmlPage() {
  String html = "<!DOCTYPE html><html><head><title>RC Car</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body{font-family:sans-serif;text-align:center;background:#111;color:#fff;}";
  html += "button{width:110px;height:110px;font-size:22px;margin:6px;border-radius:16px;border:none;background:#2196F3;color:white;}";
  html += "button:active{background:#0b5eab;}";
  html += ".stopbtn{background:#e53935;}";
  html += "</style></head><body>";
  html += "<h2>WiFi RC Car</h2>";
  html += "<div><button onclick=\"send('F')\">Forward</button></div>";
  html += "<div>";
  html += "<button onclick=\"send('L')\">Left</button>";
  html += "<button class='stopbtn' onclick=\"send('S')\">STOP</button>";
  html += "<button onclick=\"send('R')\">Right</button>";
  html += "</div>";
  html += "<div><button onclick=\"send('B')\">Backward</button></div>";
  html += "<script>";
  html += "function send(cmd){ fetch('/cmd?dir=' + cmd); }";
  html += "</script></body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleCommand() {
  String dir = server.arg("dir");
  if (dir == "F") goForward();
  else if (dir == "B") goBackward();
  else if (dir == "L") turnLeft();
  else if (dir == "R") turnRight();
  else if (dir == "S") allRelaysOff();
  server.send(200, "text/plain", "OK");
}

void setup() {
  pinMode(LEFT_FWD,  OUTPUT);
  pinMode(LEFT_REV,  OUTPUT);
  pinMode(RIGHT_FWD, OUTPUT);
  pinMode(RIGHT_REV, OUTPUT);
  allRelaysOff();

  WiFi.softAP(ssid, password);   // ESP8266 creates its own WiFi hotspot

  server.on("/", handleRoot);
  server.on("/cmd", handleCommand);
  server.begin();
}

void loop() {
  server.handleClient();
}
```

**No extra libraries need installing** — `ESP8266WiFi` and `ESP8266WebServer` come with the ESP8266 board package you installed in Part 5.

---

## Part 7 — Connecting Your Phone (Step by Step)

1. Make sure the car is powered on (battery connected, NodeMCU running your uploaded code).
2. On your phone, open **WiFi settings**.
3. Look for a network called **"RC_Car"** and connect to it using the password **`12345678`**.
   - Your phone may warn "no internet connection" — that's expected and fine, tap "stay connected" / "use anyway."
4. Open any web browser on your phone (Chrome, Safari, etc.).
5. In the address bar, type: **`http://192.168.4.1`** and hit go.
6. You should see the **WiFi RC Car** page with Forward / Backward / Left / Right / STOP buttons.
7. Tap and hold nowhere — just tap once for a command; tap **STOP** to stop it.
8. Add this page to your phone's home screen (browser menu → "Add to Home Screen") so it feels like an app icon next time.

---

## Part 8 — First Test & Troubleshooting

**Before mounting anything on the chassis**, test on your desk with wheels off the ground:

1. Upload the code, power everything on, connect your phone as above.
2. Tap **Forward** — both wheels should spin the same way. If a wheel spins the wrong way for "forward," just swap that motor's 2 wires on the relay COM terminals.
3. If **nothing** happens when you tap a button, add `#define RELAY_ON HIGH` and `#define RELAY_OFF LOW` (swap the two lines) — your specific relay board may be "active HIGH" instead of "active LOW."
4. If the ESP8266 **restarts/resets** every time a motor starts, your motors are pulling too much current for the battery to supply cleanly — use a separate, higher-capacity battery pack for the motors, or fresh/charged batteries.
5. Once wheel directions are all correct, mount everything on the chassis with tape/zip ties, keep wiring away from the wheels, and go test it on the floor.

---

## Safety Notes
- Keep this project to low-voltage batteries only (the 4×AA / 5-6V pack mentioned above). Do not use mains/wall AC power anywhere in this build.
- Double-check + and − on the battery before connecting it — reversed battery polarity can damage the ESP8266 and relay board.
- Always disconnect the battery before changing any wiring.

---

## Appendix — If You Insist on Using the Bare ESP8266 (ESP-01) Instead of a NodeMCU

This is harder and not recommended for a first project, but possible: the ESP-01 only exposes GPIO0 and GPIO2 easily as free pins; you can also reuse the TX (GPIO1) and RX (GPIO3) pins as your 3rd and 4th relay signals since you won't need the Serial Monitor while the car is running. You'll need a separate 3.3V regulator for the ESP8266 chip itself (it is **not 5V tolerant**), and you must program it the normal ESP-01 way (GPIO0 held to GND during upload, released afterward) using your USB-to-TTL adapter set to 3.3V. If this sounds confusing, that confusion is exactly why the NodeMCU is worth the few dollars.
