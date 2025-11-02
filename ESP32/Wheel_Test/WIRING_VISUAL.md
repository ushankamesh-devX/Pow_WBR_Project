# 🔌 VISUAL WIRING DIAGRAM

## Complete Connection Diagram for WBR Robot

```
                                    ESP32 DevKit V1
                           ╔═══════════════════════════════╗
                           ║                               ║
        MPU6050            ║  3.3V ○  ────────────┐       ║
      ╔═══════╗            ║  GND  ○  ─────────┐  │       ║
      ║ VCC ──╫────────────╫─ 3.3V            │  │       ║
      ║ GND ──╫────────────╫─ GND             │  │       ║
      ║ SCL ──╫────────────╫─ GPIO22          │  │       ║
      ║ SDA ──╫────────────╫─ GPIO21          │  │       ║
      ╚═══════╝            ║                   │  │       ║
                           ║  GPIO13 ○ ────────┼──┼───┐   ║
                           ║  GPIO12 ○ ────────┼──┼───┼─┐ ║
                           ║                   │  │   │ │ ║
                           ║  GPIO16 ○ ────────┼──┼───┼─┼─┼─┐
                           ║  GPIO17 ○ ────────┼──┼───┼─┼─┼─┼─┐
                           ║  GPIO25 ○ ────────┼──┼───┼─┼─┼─┼─┼─┐
                           ║  GPIO26 ○ ────────┼──┼───┼─┼─┼─┼─┼─┼─┐
                           ║                   │  │   │ │ │ │ │ │ │
                           ║  GPIO27 ○ ────────┼──┼───┼─┼─┼─┼─┼─┼─┼───┐
                           ║  GPIO14 ○ ────────┼──┼───┼─┼─┼─┼─┼─┼─┼───┼──┐
                           ║  GPIO32 ○ ────────┼──┼───┼─┼─┼─┼─┼─┼─┼───┼──┼──┐
                           ║  GPIO33 ○ ────────┼──┼───┼─┼─┼─┼─┼─┼─┼───┼──┼──┼──┐
                           ║                   │  │   │ │ │ │ │ │ │   │  │  │  │
                           ╚═══════════════════╩══╩═══╩═╩═╩═╩═╩═╩═╩═══╩══╩══╩══╩═╗
                                               │  │   │ │ │ │ │ │ │   │  │  │  │ ║
                                               │  │   │ │ │ │ │ │ │   │  │  │  │ ║
                   ┌───────────────────────────┘  │   │ │ │ │ │ │ │   │  │  │  │ ║
                   │  ┌────────────────────────────┘   │ │ │ │ │ │ │   │  │  │  │ ║
                   │  │                                │ │ │ │ │ │ │   │  │  │  │ ║
    Left Hip       │  │      Right Hip                │ │ │ │ │ │ │   │  │  │  │ ║
    Servo          │  │      Servo                    │ │ │ │ │ │ │   │  │  │  │ ║
    ╔═══════╗      │  │      ╔═══════╗                │ │ │ │ │ │ │   │  │  │  │ ║
    ║ SIG ──╫──────┘  │      ║ SIG ──╫────────────────┘ │ │ │ │ │ │   │  │  │  │ ║
    ║ VCC ──╫─────────┼──────╫─ VCC ─╫──────────────────┼─┼─┼─┼─┼─┼───┼──┼──┼──┼─╫──┐
    ║ GND ──╫─────────┼──────╫─ GND ─╫──────────────────┼─┼─┼─┼─┼─┼───┼──┼──┼──┼─╫──┼───┐
    ╚═══════╝         │      ╚═══════╝                  │ │ │ │ │ │   │  │  │  │ ║  │   │
                      │                                  │ │ │ │ │ │   │  │  │  │ ║  │   │
                      │                                  │ │ │ │ │ │   │  │  │  │ ║  │   │
                      │     DRV8833 Motor Driver         │ │ │ │ │ │   │  │  │  │ ║  │   │
                      │     ╔════════════════════╗       │ │ │ │ │ │   │  │  │  │ ║  │   │
                      │     ║  AIN1 ──────────────────────┘ │ │ │ │ │   │  │  │  │ ║  │   │
                      │     ║  AIN2 ────────────────────────┘ │ │ │ │   │  │  │  │ ║  │   │
                      │     ║  BIN1 ──────────────────────────┘ │ │ │   │  │  │  │ ║  │   │
                      │     ║  BIN2 ────────────────────────────┘ │ │   │  │  │  │ ║  │   │
                      │     ║                    ║                 │ │   │  │  │  │ ║  │   │
                      │     ║  AOUT1 ────────┐   ║                 │ │   │  │  │  │ ║  │   │
                      │     ║  AOUT2 ────────┼─┐ ║                 │ │   │  │  │  │ ║  │   │
                      │     ║  BOUT1 ────────┼─┼─╫────┐            │ │   │  │  │  │ ║  │   │
                      │     ║  BOUT2 ────────┼─┼─╫────┼──┐         │ │   │  │  │  │ ║  │   │
                      │     ║                │ │ ║    │  │         │ │   │  │  │  │ ║  │   │
                      │     ║  VCC ──────────┼─┼─╫────┼──┼─────────┼─┼───┼──┼──┼──┼─╫──┼───┼──┐
                      │     ║  GND ──────────┼─┼─╫────┼──┼─────────┼─┼───┼──┼──┼──┼─╫──┼───┼──┼──┐
                      │     ╚════════════════╪═╪═╝    │  │         │ │   │  │  │  │ ║  │   │  │  │
                      │                      │ │      │  │         │ │   │  │  │  │ ║  │   │  │  │
                      │  Right Wheel Motor   │ │      │  │         │ │   │  │  │  │ ║  │   │  │  │
                      │  ╔═══════════════╗   │ │      │  │         │ │   │  │  │  │ ║  │   │  │  │
                      │  ║ Motor + ──────╫───┘ │      │  │         │ │   │  │  │  │ ║  │   │  │  │
                      │  ║ Motor - ──────╫─────┘      │  │         │ │   │  │  │  │ ║  │   │  │  │
                      │  ║               ║            │  │         │ │   │  │  │  │ ║  │   │  │  │
                      │  ║ Enc VCC ──────╫────────────┼──┼─────────┼─┼───┼──┼──┼──┘ ║  │   │  │  │
                      │  ║ Enc GND ──────╫────────────┼──┼─────────┼─┼───┼──┼──┼────╫──┼───┼──┼──┘
                      │  ║ Enc A ────────╫────────────┼──┼─────────┼─┼───┼──┼──┼────╫──┘   │  │
                      │  ║ Enc B ────────╫────────────┼──┼─────────┼─┼───┼──┼──┼────╫──────┘  │
                      │  ╚═══════════════╝            │  │         │ │   │  │  │    ║          │
                      │                               │  │         │ │   │  │  │    ║          │
                      │  Left Wheel Motor             │  │         │ │   │  │  │    ║          │
                      │  ╔═══════════════╗            │  │         │ │   │  │  │    ║          │
                      │  ║ Motor + ──────╫────────────┘  │         │ │   │  │  │    ║          │
                      │  ║ Motor - ──────╫───────────────┘         │ │   │  │  │    ║          │
                      │  ║               ║                         │ │   │  │  │    ║          │
                      │  ║ Enc VCC ──────╫─────────────────────────┼─┼───┼──┼──┘    ║          │
                      │  ║ Enc GND ──────╫─────────────────────────┼─┼───┼──┼───────╫──────────┘
                      │  ║ Enc A ────────╫─────────────────────────┼─┼───┼──┼───────╫──────────┐
                      │  ║ Enc B ────────╫─────────────────────────┼─┼───┼──┼───────╫──────────┼──┐
                      │  ╚═══════════════╝                         │ │   │  │       ║          │  │
                      │                                            │ │   │  │       ║          │  │
                      │                                            │ │   │  │       ║          │  │
                      │  5V BEC (Step-Down Regulator)              │ │   │  │       ║          │  │
                      │  ╔═══════════════╗                         │ │   │  │       ║          │  │
                      │  ║ Vin+ ─────────╫─────────────────────────┼─┼───┼──┘       ║          │  │
                      │  ║ Vin- ─────────╫─────────────────────────┼─┼───┼──────────╫──────────┼──┘
                      │  ║ Vout+ ────────╫─────────────────────────┘ │   │          ║          │
                      │  ║ Vout- ────────╫───────────────────────────┼───┼──────────╫──────────┘
                      │  ╚═══════════════╝                           │   │          ║
                      │                                              │   │          ║
                      │  7.4V LiPo Battery                           │   │          ║
                      │  ╔═══════════════╗                           │   │          ║
                      └──╫─ + (Positive)                             │   │          ║
                         ║                                           │   │          ║
                         ╠═ - (Negative) ────────────────────────────┴───┴──────────╝
                         ║                        COMMON GROUND
                         ╚═══════════════╝
                         
                         
    ═══════════════════════════════════════════════════════════════════
    LEGEND:
    ═══════════════════════════════════════════════════════════════════
    ──  Signal wire (3.3V logic)
    ══  Power wire (thick)
    ○   GPIO pin on ESP32
    ╔╗  Component boundary
    ╫   Connection point
    ───┐  Branch connection
    ───┘  Join connection
```

---

## 📊 CONNECTION TABLE (Simplified)

### **ESP32 → DRV8833:**
| ESP32 Pin | → | DRV8833 Pin |
|-----------|---|-------------|
| GPIO 16 | → | AIN1 |
| GPIO 17 | → | AIN2 |
| GPIO 25 | → | BIN1 |
| GPIO 26 | → | BIN2 |

### **ESP32 → Servos:**
| ESP32 Pin | → | Servo |
|-----------|---|-------|
| GPIO 13 | → | Left Hip Signal |
| GPIO 12 | → | Right Hip Signal |

### **ESP32 → Encoders:**
| ESP32 Pin | → | Encoder |
|-----------|---|---------|
| GPIO 27 | → | Right Wheel Encoder A |
| GPIO 14 | → | Right Wheel Encoder B |
| GPIO 32 | → | Left Wheel Encoder A |
| GPIO 33 | → | Left Wheel Encoder B |

### **ESP32 → MPU6050:**
| ESP32 Pin | → | MPU6050 |
|-----------|---|---------|
| GPIO 21 | → | SDA |
| GPIO 22 | → | SCL |

### **Power Distribution:**
```
Battery 7.4V ─┬→ DRV8833 VCC → Motors
              └→ 5V BEC → Servos
                       └→ (optional) ESP32 VIN

ESP32 USB ───→ ESP32 (for development)

3.3V (ESP32) ─→ MPU6050 VCC
              └→ Encoder VCC (both)

COMMON GROUND ← All components connected!
```

---

## 🎨 COLOR CODE (if using colored wires)

### **Recommended Wire Colors:**
- **Red:** Power (+) - Battery, BEC, Servo VCC
- **Black/Brown:** Ground (GND) - All grounds
- **Yellow/Orange:** Signal - GPIO to peripherals
- **Blue:** Motor wires
- **Green:** Encoder signals
- **White:** I2C (SDA/SCL)

---

## ⚡ POWER FLOW DIAGRAM

```
        ┌─────────────────────────────────────┐
        │   7.4V LiPo Battery (2S)            │
        │   Voltage: 6.4V - 8.4V              │
        │   Capacity: 2000-3000mAh            │
        └────────┬────────────────────┬────────┘
                 │                    │
                 │                    │
           ┌─────▼─────┐        ┌────▼────┐
           │ DRV8833   │        │ 5V BEC  │
           │ VCC       │        │ Input   │
           │ 7.4V      │        │ 7.4V    │
           └─────┬─────┘        └────┬────┘
                 │                   │
                 │                   │
        ┌────────▼────────┐     ┌───▼────┐
        │  N20 Motors     │     │ 5V Out │
        │  Right & Left   │     └───┬────┘
        │  6-12V          │         │
        └─────────────────┘    ┌────▼────────┐
                               │ Servos      │
                               │ 2× Hip      │
                               │ 5V, 1-2A    │
                               └─────────────┘
        
        ┌──────────────────────────────────┐
        │   USB 5V (During Development)    │
        │   For programming & testing      │
        └────────┬─────────────────────────┘
                 │
            ┌────▼────┐
            │  ESP32  │
            │  VIN/5V │
            └────┬────┘
                 │
            ┌────▼────┐
            │ 3.3V    │
            │ Regulator│
            └────┬────┘
                 │
        ┌────────┴────────┐
        │                 │
    ┌───▼───┐      ┌──────▼──────┐
    │MPU6050│      │  Encoders   │
    │ 3.3V  │      │  VCC 3.3V   │
    │ 3.5mA │      │  ~10mA each │
    └───────┘      └─────────────┘


    ═══════════════════════════════════════
          COMMON GROUND (ALL CONNECTED)
    ═══════════════════════════════════════
```

---

## 🔍 CRITICAL CONNECTIONS (DO NOT SKIP!)

### **1. Common Ground (MOST IMPORTANT!):**
```
Battery GND ─┬─ DRV8833 GND
             ├─ BEC GND
             ├─ ESP32 GND
             ├─ Servo 1 GND
             ├─ Servo 2 GND
             ├─ MPU6050 GND
             ├─ Right Encoder GND
             └─ Left Encoder GND
```
**Use a ground bus/rail on breadboard or star topology on PCB**

### **2. Power Isolation:**
```
✅ Motor Power (7.4V) → DRV8833 only
✅ Servo Power (5V) → From BEC only (NOT ESP32!)
✅ Logic Power (3.3V) → From ESP32 regulator
❌ Never connect motor power to ESP32!
❌ Never power servos from ESP32 pins!
```

### **3. Encoder Power:**
```
Encoder VCC → ESP32 3.3V pin
(Total current: ~20mA, safe for ESP32)
```

---

## 📏 WIRE LENGTH RECOMMENDATIONS

- **Power wires (Battery):** 18-20 AWG, keep short (<20cm)
- **Motor wires:** 22-24 AWG, keep short (<15cm)
- **Servo wires:** 24-26 AWG, can be longer (30cm ok)
- **Signal wires (GPIO):** 26-28 AWG, keep under 50cm
- **I2C wires:** 26-28 AWG, keep under 30cm (twisted pair)
- **Encoder wires:** 26-28 AWG, keep short (<20cm)

---

## 🛡️ PROTECTION COMPONENTS (Recommended)

### **Add These for Safety:**

1. **Battery Protection:**
   - Fuse: 5A inline with battery positive
   - Low voltage alarm: Set to 6.6V (for 2S LiPo)

2. **Power Supply Filtering:**
   - 100µF capacitor across DRV8833 VCC/GND
   - 100µF capacitor across BEC output
   - 10µF capacitor across ESP32 VIN/GND

3. **Motor Noise Suppression:**
   - 0.1µF ceramic capacitor across each motor terminal
   - Or use motors with built-in capacitors

4. **ESD Protection:**
   - Keep encoder wires away from motor wires
   - Use shielded cable for I2C if experiencing issues

---

## ✅ WIRING VERIFICATION CHECKLIST

### **Before Applying Power:**
- [ ] Battery polarity correct (+ and -)
- [ ] All grounds connected together (continuity test)
- [ ] No shorts between power rails (multimeter check)
- [ ] DRV8833 VCC connected to battery, NOT ESP32
- [ ] Servo power from BEC, NOT ESP32
- [ ] Encoder VCC is 3.3V, NOT battery voltage
- [ ] Motor wires secured (won't short during operation)
- [ ] All signal wires connected to correct GPIO
- [ ] I2C wires (SDA/SCL) not swapped
- [ ] Encoder A/B wires identified correctly

### **After Wiring, Before Code:**
- [ ] Battery voltage: 7.0V - 8.4V (for 2S LiPo)
- [ ] BEC output: 4.8V - 5.2V
- [ ] ESP32 3.3V pin: 3.2V - 3.4V
- [ ] No components getting hot
- [ ] ESP32 boots (power LED on)
- [ ] No magic smoke! 💨

---

**This completes the visual wiring guide!**
**See WIRING_GUIDE.md for detailed step-by-step instructions.**

🔌 **Happy Wiring!** 🤖
