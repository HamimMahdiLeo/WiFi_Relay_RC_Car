# WiFi Remote Control Car — Full Beginner Build Guide

Built around: **ESP-32 module**, **Batteries or power bank for powering the relay board and ESP**, **5V 4-channel relay board**, **2 motor drivers**, **2× 5V DC motors**

---

## Part 1 — What We're Actually Building (Plain Language)

Your phone will **not** connect to the internet for this. Instead, your ESP-32 will create its **own tiny WiFi hotspot**. Your phone connects to that hotspot (just like connecting to any home WiFi), then you open your phone's normal web browser and go to one address. A page with big **Forward / Backward / Left / Right / Stop** buttons shows up. When you tap a button, your phone tells the ESP-32 what to do, and the ESP-32 flips the relays on the relay board to send power to the two motors in the right direction. That's it — no app to install, no account, no internet needed.

**The one tricky part: a relay is just an on/off switch — it cannot make a motor spin backward by itself.** To make a motor go both forward AND backward, we need to flip the polarity (the + and −) of the wires going into it. We do that using **2 relays per motor** (4 relays total for your 2 motors) wired in a pattern called an **"H-bridge"**. Don't worry — you don't need to understand the theory. Just follow the wiring exactly as shown below and it will work. This guide is designed so your exact 4-relay board and 2 motors are enough — you don't need a 5th relay or a special motor driver chip.

---

## Part 2 — Full Parts List

### ✅ We need:
- ESP-32
- 5V 4-channel relay board
- 2 motor drivers
- 2× 5V DC motors
- Breadboard
- Bunch of wires
- External power source

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
- Connect the relay board's 5V/GND power input to the **ESP-32's 5V (VU or VIN) and GND pins** — OR power it from the same battery pack you'll use for the motors (either works; sharing one battery for everything is fine for small motors).
- Connect relay board **GND** to ESP-32 **GND** no matter what (this "common ground" step is required for the signals to work — don't skip it).

### Step 2 — Connect the 4 signal wires (NodeMCU → Relay board)
Each relay channel on your board has its own small connector with 3 wires: **VCC, GND, Signal** (this matches the "in1/in2/in3/in4" labels on your board).

| Relay board channel | Connect Signal pin to ESP-32 pin | Purpose |
|---|---|---|
| IN1 | **D1** | Left motor – Forward |
| IN2 | **D2** | Left motor – Reverse |
| IN3 | **D5** | Right motor – Forward |
| IN4 | **D6** | Right motor – Reverse |

Also connect each channel's VCC to ESP-32 3V3 (or 5V, per your board's label) and GND to ESP-32 GND.

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

## Part 6 — The Arduino Code

Check the "src" folder inside "RC Car" folder for the main.cpp 
---

## Part 7 — Connecting Your Phone (Step by Step)

1. Make sure the car is powered on (battery connected, ESP-32 running your uploaded code).
2. On your phone, open **WiFi settings**.
3. Look for a network called **"RC_Car"** and connect to it using the password **`12345678`**. (You can always change the network SSID and Password from the code)
   - Your phone may warn "no internet connection" — that's expected and fine, tap "stay connected" / "use anyway."
4. Open any web browser on your phone.
5. In the address bar, type: **`http://192.168.4.1`** and hit go.
6. You should see the **WiFi RC Car** page with Forward / Backward / Left / Right / STOP buttons.
7. Tap and hold nowhere — just tap once for a command; tap **STOP** to stop it.

---

## Part 8 — First Test & Troubleshooting

**Before mounting anything on the chassis**, test on your desk with wheels off the ground:

1. Upload the code, power everything on, connect your phone as above.
2. Tap **Forward** — both wheels should spin the same way. If a wheel spins the wrong way for "forward," just swap that motor's 2 wires on the relay COM terminals.
3. If **nothing** happens when you tap a button, add `#define RELAY_ON HIGH` and `#define RELAY_OFF LOW` (swap the two lines) — your specific relay board may be "active HIGH" instead of "active LOW."
4. If the ESP-32 **restarts/resets** every time a motor starts, your motors are pulling too much current for the battery to supply cleanly — use a separate, higher-capacity battery pack for the motors, or fresh/charged batteries.
5. Once wheel directions are all correct, mount everything on the chassis with tape/zip ties, keep wiring away from the wheels, and go test it on the floor.

---

## Safety Notes
- Keep this project to low-voltage batteries only (the 4×AA / 5-6V pack mentioned above). Do not use mains/wall AC power anywhere in this build.
- Double-check + and − on the battery before connecting it — reversed battery polarity can damage the ESP-32 and relay board.
- For any kind of testing, it is recommended to use the Type C port of ESP-32 with computer or laptop.
- Always disconnect the battery before changing any wiring.

---
