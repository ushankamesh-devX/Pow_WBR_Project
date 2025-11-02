# 🔌 COMPLETE WIRING GUIDE - WBR Robot
## ESP32 DevKit V1 + N20 Motors + DRV8833 + Servos + MPU6050

---

## 📋 **COMPONENT LIST**

### **Required Components:**
1. ✅ ESP32 DevKit V1 (30-pin)
2. ✅ 2× N20 Micro Gear Motor with Encoder
3. ✅ DRV8833 Dual H-Bridge Motor Driver
4. ✅ 2× RC Servo Motors (SG90/MG90S) for hip joints
5. ✅ MPU6050 IMU (6-axis accelerometer + gyroscope)
6. ✅ 7.4V 2S LiPo Battery (2000-3000mAh)
7. ✅ 5V BEC/Step-Down Regulator (2-3A rating)
8. ✅ Breadboard or PCB for connections
9. ✅ Jumper wires (Male-Male, Male-Female)
10. ✅ USB Cable for ESP32 programming

---

## ⚡ **POWER DISTRIBUTION SYSTEM**

```
7.4V LiPo Battery
    │
    ├──→ DRV8833 VCC (Motor Power) ──→ N20 Motors
    │
    └──→ 5V BEC Input
            │
            ├──→ Servo 1 VCC (Red wire)
            ├──→ Servo 2 VCC (Red wire)
            └──→ Optional: ESP32 VIN (if not using USB)

USB 5V (During development)
    │
    └──→ ESP32 USB Port (for programming + power)

Common Ground (CRITICAL!)
    │
    ├──→ Battery GND
    ├──→ DRV8833 GND
    ├──→ BEC GND
    ├──→ ESP32 GND
    ├──→ Servo 1 GND (Brown wire)
    ├──→ Servo 2 GND (Brown wire)
    └──→ MPU6050 GND
```

**⚠️ CRITICAL: All grounds MUST be connected together!**

---

## 🔌 **COMPLETE PIN CONNECTION TABLE**

### **ESP32 DevKit V1 Pinout:**
```
                    ┌─────────┐
              EN ──│1      30│── D23
             VP(36)│2      29│── D22 (SCL - MPU6050)
             VN(39)│3      28│── TX0 (USB)
             D34 ──│4      27│── RX0 (USB)
             D35 ──│5      26│── D21 (SDA - MPU6050)
             D32 ──│6      25│── D19
             D33 ──│7      24│── D18
             D25 ──│8      23│── D5
             D26 ──│9      22│── D17
             D27 ──│10     21│── D16
             D14 ──│11     20│── D4
             D12 ──│12     19│── D2
             D13 ──│13     18│── D15
             GND ──│14     17│── GND
             VIN ──│15     16│── 3V3
                    └─────────┘
```

---

### **1. DRV8833 MOTOR DRIVER CONNECTIONS**

| **DRV8833 Pin** | **ESP32 Pin** | **Function** | **Notes** |
|-----------------|---------------|--------------|-----------|
| **VCC** | 7.4V Battery + | Power input | Connect directly to battery |
| **GND** | Battery GND | Ground | Common ground |
| **VM** | (same as VCC) | Motor voltage | May be separate on some boards |
| **AIN1** | GPIO 16 | Right motor PWM | Motor A control |
| **AIN2** | GPIO 17 | Right motor DIR | Motor A direction |
| **BIN1** | GPIO 25 | Left motor PWM | Motor B control |
| **BIN2** | GPIO 26 | Left motor DIR | Motor B direction |
| **AOUT1** | Right Motor + | Motor A output | Red wire |
| **AOUT2** | Right Motor - | Motor A output | Black wire |
| **BOUT1** | Left Motor + | Motor B output | Red wire |
| **BOUT2** | Left Motor - | Motor B output | Black wire |

**Motor Connection Notes:**
- If motor spins backwards, swap AOUT1 ↔ AOUT2 (or BOUT1 ↔ BOUT2)
- N20 motors: Red = positive, Black = negative (usually)

---

### **2. N20 ENCODER CONNECTIONS**

**Right Wheel Motor Encoder:**
| **Encoder Wire** | **ESP32 Pin** | **Function** |
|------------------|---------------|--------------|
| VCC (Red) | 3.3V | Encoder power |
| GND (Black) | GND | Ground |
| A (Yellow/Green) | GPIO 27 | Phase A signal |
| B (White/Blue) | GPIO 14 | Phase B signal |

**Left Wheel Motor Encoder:**
| **Encoder Wire** | **ESP32 Pin** | **Function** |
|------------------|---------------|--------------|
| VCC (Red) | 3.3V | Encoder power |
| GND (Black) | GND | Ground |
| A (Yellow/Green) | GPIO 32 | Phase A signal |
| B (White/Blue) | GPIO 33 | Phase B signal |

**⚠️ Encoder Notes:**
- Encoder wires may vary by manufacturer (check datasheet)
- Most encoders have 4 or 6 wires
- Hall-effect encoders need 3.3V or 5V power
- Optical encoders may need 5V (use level shifter if needed)

---

### **3. RC SERVO CONNECTIONS**

**Left Hip Servo:**
| **Servo Wire** | **ESP32 Pin** | **Function** |
|----------------|---------------|--------------|
| Brown/Black | GND | Ground |
| Red | 5V (from BEC) | Power (5V) |
| Orange/Yellow | GPIO 13 | PWM signal |

**Right Hip Servo:**
| **Servo Wire** | **ESP32 Pin** | **Function** |
|----------------|---------------|--------------|
| Brown/Black | GND | Ground |
| Red | 5V (from BEC) | Power (5V) |
| Orange/Yellow | GPIO 12 | PWM signal |

**⚠️ Servo Power Warning:**
- **NEVER connect servo power to ESP32 3.3V or 5V pins!**
- Servos draw 300-600mA under load
- Use external 5V BEC (2-3A rating)
- Connect servo ground to ESP32 GND

---

### **4. MPU6050 IMU CONNECTIONS**

| **MPU6050 Pin** | **ESP32 Pin** | **Function** |
|-----------------|---------------|--------------|
| VCC | 3.3V (or 5V) | Power input |
| GND | GND | Ground |
| SCL | GPIO 22 | I2C Clock |
| SDA | GPIO 21 | I2C Data |
| INT | (not used) | Interrupt (optional) |
| AD0 | GND | I2C Address select |

**I2C Notes:**
- MPU6050 works with 3.3V or 5V
- I2C address: 0x68 (AD0=GND) or 0x69 (AD0=VCC)
- Built-in pull-up resistors on DevKit V1 (no external needed)

---

## 🔋 **POWER SUPPLY WIRING**

### **Option A: Development/Testing (USB Power)**
```
Computer USB ──→ ESP32 USB Port
                     │
                     └──→ 3.3V logic power

External 7.4V Battery ──→ DRV8833 VCC
                              │
                              └──→ Motors

External 7.4V Battery ──→ 5V BEC ──→ Servos VCC
```

**Use Case:** Programming, testing without motors running
**Limitation:** Motors won't work without external battery

---

### **Option B: Full Operation (External Battery)**
```
7.4V LiPo Battery (2S)
    │
    ├──→ DRV8833 VCC (motor power)
    │       └──→ N20 Motors
    │
    └──→ 5V BEC Input
            │
            ├──→ Servos VCC
            └──→ ESP32 VIN (optional - or keep USB)

All GNDs connected together ←── CRITICAL!
```

**Use Case:** Full robot operation
**Advantage:** All components powered, mobile robot

---

### **Battery Selection:**
- **Recommended:** 7.4V 2S LiPo (2000-3000mAh)
- **Voltage range:** 6.4V (discharged) to 8.4V (full)
- **C-rating:** 20C minimum (for motor current spikes)
- **Connector:** XT60 or JST recommended

### **BEC/Regulator Selection:**
- **Input:** 7.4V (2S LiPo)
- **Output:** 5V
- **Current rating:** 2-3A minimum
- **Type:** Switching regulator (more efficient than linear)
- **Example:** UBEC 5V 3A

---

## 🔧 **WIRING DIAGRAM (ASCII ART)**

```
                        ┌──────────────────────────────┐
                        │   ESP32 DevKit V1            │
                        │                              │
        ┌───────────────│  GPIO16 (RW_IN1)             │
        │   ┌───────────│  GPIO17 (RW_IN2)             │
        │   │   ┌───────│  GPIO25 (LW_IN1)             │
        │   │   │   ┌───│  GPIO26 (LW_IN2)             │
        │   │   │   │   │                              │
        │   │   │   │   │  GPIO27 (Enc_RW_A)───────────┼───┐
        │   │   │   │   │  GPIO14 (Enc_RW_B)───────────┼───┼───┐
        │   │   │   │   │  GPIO32 (Enc_LW_A)───────────┼───┼───┼───┐
        │   │   │   │   │  GPIO33 (Enc_LW_B)───────────┼───┼───┼───┼───┐
        │   │   │   │   │                              │   │   │   │   │
        │   │   │   │   │  GPIO21 (SDA)────────────────┼───┼───┼───┼───┼─── MPU6050
        │   │   │   │   │  GPIO22 (SCL)────────────────┼───┼───┼───┼───┼─── SDA/SCL
        │   │   │   │   │                              │   │   │   │   │
        │   │   │   │   │  GPIO13 (Left Servo)─────────┼───┼───┼───┼───┼───┐
        │   │   │   │   │  GPIO12 (Right Servo)────────┼───┼───┼───┼───┼───┼───┐
        │   │   │   │   │                              │   │   │   │   │   │   │
        │   │   │   │   │  GND ─────────────────────────── Common Ground ───┘   │
        │   │   │   │   │  3.3V ────────────────────────── Power Rail            │
        │   │   │   │   └──────────────────────────────┘                        │
        │   │   │   │                                                            │
        ↓   ↓   ↓   ↓                                                            │
    ┌───────────────────┐                                                        │
    │   DRV8833 Driver  │                                                        │
    │                   │                                                        │
    │  AIN1  AIN2       │         7.4V Battery                                  │
    │  BIN1  BIN2       │              │                                         │
    │                   │              ├─→ VCC ──→ Motors                        │
    │  AOUT1 AOUT2      │              │                                         │
    │  BOUT1 BOUT2      │              └─→ 5V BEC ──→ Servos + (optional ESP32) │
    │                   │                      │                                 │
    │  VCC    GND       │                      └─→ Common GND ←─────────────────┘
    └───┬───────┬───────┘
        │       │
     7.4V      GND
    Battery   Common
```

---

## 📐 **PHYSICAL LAYOUT RECOMMENDATIONS**

### **Component Placement:**
```
Top View:
┌──────────────────────────────────────┐
│                                      │
│  [Servo L]  [ESP32 DevKit]  [Servo R] │
│                                      │
│  [MPU6050]    [DRV8833]             │
│                                      │
│     [Motor L]        [Motor R]      │
│       🔵               🔵            │
│                                      │
│    [  7.4V LiPo Battery  ]          │
│    [     5V BEC         ]           │
│                                      │
└──────────────────────────────────────┘
```

### **Mounting Tips:**
1. **MPU6050:** Mount near robot center, aligned with robot axes
2. **ESP32:** Central location, easy USB access for programming
3. **DRV8833:** Near motors, short wire runs
4. **Battery:** Low in robot (stability), accessible for changes
5. **Servos:** At hip joints as per robot design

---

## ⚠️ **SAFETY CHECKLIST**

### **Before Powering On:**
- [ ] All grounds connected together
- [ ] No short circuits (check with multimeter)
- [ ] Battery polarity correct (+ and - not reversed)
- [ ] Servo power from BEC (NOT from ESP32 pins)
- [ ] Motor wires not touching each other
- [ ] Encoder wires correct polarity
- [ ] I2C wires correct (SDA ≠ SCL)
- [ ] Battery voltage 6.4V - 8.4V (for 2S LiPo)
- [ ] All solder joints solid (if using PCB)
- [ ] No loose wires that can short

### **First Power-On Sequence:**
1. **Connect ESP32 to USB** (without battery)
2. **Upload code** and verify no errors
3. **Disconnect USB**
4. **Connect battery** (motors should NOT move yet)
5. **Reconnect USB** (or power on via battery)
6. **Open Serial Monitor** (115200 baud)
7. **Check for "System ready" message**
8. **Connect to WiFi AP** "WBR_Robot" with password "12345678"
9. **Open web interface** http://192.168.4.1
10. **Test each component individually:**
    - Servos (adjust height slider)
    - Motors (low PWM first)
    - IMU (check readings)
    - Encoders (spin wheels by hand)

---

## 🔍 **TROUBLESHOOTING**

### **Problem: ESP32 won't boot**
- ✅ Check GPIO 12 is not pulled HIGH during boot
- ✅ Disconnect all peripherals, boot bare ESP32
- ✅ Check 3.3V power rail voltage
- ✅ Try different USB cable/port

### **Problem: Motors don't spin**
- ✅ Check DRV8833 VCC has 7.4V
- ✅ Check motor wires not swapped
- ✅ Test with simple PWM (full speed one motor)
- ✅ Check ground connections
- ✅ Measure voltage at DRV8833 outputs

### **Problem: Encoders not counting**
- ✅ Check encoder power (3.3V or 5V)
- ✅ Verify encoder wires (A, B, VCC, GND)
- ✅ Test with Serial.println in ISR
- ✅ Swap A/B if counting backwards

### **Problem: Servos jitter/don't work**
- ✅ Check 5V BEC output voltage
- ✅ Ensure servos powered from BEC (not ESP32)
- ✅ Check PWM frequency (50Hz for RC servos)
- ✅ Common ground ESP32 ↔ BEC

### **Problem: MPU6050 not detected**
- ✅ Check I2C address (0x68 or 0x69)
- ✅ Verify SDA/SCL not swapped
- ✅ Check 3.3V power to IMU
- ✅ Try I2C scanner sketch

### **Problem: WiFi won't connect**
- ✅ Check SSID/password in code
- ✅ Verify ESP32 creating Access Point (not client)
- ✅ Look for "WBR_Robot" network in WiFi list
- ✅ Check Serial Monitor for IP address
- ✅ Reduce WiFi distance if signal weak

### **Problem: Robot draws too much current**
- ✅ Measure battery voltage under load
- ✅ Check for motor stalls (mechanical binding)
- ✅ Reduce PWM duty cycle
- ✅ Use higher C-rating battery
- ✅ Check for short circuits

---

## 📊 **CURRENT CONSUMPTION TABLE**

| **Component** | **Idle** | **Active** | **Peak** |
|---------------|----------|------------|----------|
| ESP32 (WiFi) | 80mA | 150mA | 250mA |
| MPU6050 | 500µA | 3.5mA | 5mA |
| Servo (×2) | 50mA | 400mA | 1200mA |
| N20 Motor (×2) | 100mA | 600mA | 2000mA |
| DRV8833 Logic | 1mA | 2mA | 5mA |
| **TOTAL** | ~230mA | ~1.2A | ~3.5A |

**Battery Life Estimate:**
- 2000mAh battery @ 1.2A average = ~1.5 hours
- 3000mAh battery @ 1.2A average = ~2.5 hours

---

## 🎯 **TESTING PROCEDURE**

### **Step 1: Component Test (Individually)**
```cpp
// Test motors (no encoders)
motor_RW.setSpeed(100);  // Slow forward
delay(2000);
motor_RW.stop();

// Test servos
Pol.setHR(0.15, 0);  // 15cm height, 0° roll
Pol.solve_inverse_kinematics();
HR_controller.controlHipServos(Pol.get_theta_hips());

// Test IMU
MPU6050.readData();
MPU6050.printData();
```

### **Step 2: Encoder Test**
```cpp
// Spin motor and watch encoder count
motor_RW.setSpeed(150);
delay(5000);
Serial.print("Encoder count: ");
Serial.println(motor_RW.getEncoderCount());
```

### **Step 3: Web Interface Test**
1. Connect to WiFi "WBR_Robot"
2. Open http://192.168.4.1
3. Adjust sliders and verify Serial Monitor shows changes
4. Click START and verify motors respond

### **Step 4: Full System Test**
1. Place robot on blocks (wheels off ground)
2. Click START
3. Gradually increase velocity slider
4. Check both wheels spin together
5. Test yaw control (wheels spin opposite)
6. Check data logging (visit /logdata endpoint)

---

## 📝 **GPIO PIN SUMMARY**

| **GPIO** | **Function** | **Device** | **Notes** |
|----------|--------------|-----------|-----------|
| GPIO 12 | Right Hip Servo | PWM | Boot-sensitive (must be LOW) |
| GPIO 13 | Left Hip Servo | PWM | |
| GPIO 14 | Right Encoder B | Input | Interrupt |
| GPIO 16 | Right Motor IN1 | PWM | DRV8833 |
| GPIO 17 | Right Motor IN2 | Output | DRV8833 direction |
| GPIO 21 | I2C SDA | I2C | MPU6050 |
| GPIO 22 | I2C SCL | I2C | MPU6050 |
| GPIO 25 | Left Motor IN1 | PWM | DRV8833 |
| GPIO 26 | Left Motor IN2 | Output | DRV8833 direction |
| GPIO 27 | Right Encoder A | Input | Interrupt |
| GPIO 32 | Left Encoder A | Input | Interrupt |
| GPIO 33 | Left Encoder B | Input | Interrupt |

**Remaining Free GPIO:** 
- GPIO 4, 5, 15, 18, 19, 23, 34, 35, 36, 39 (input-only)

---

## ✅ **FINAL CHECKLIST**

### **Hardware:**
- [ ] All components connected as per diagram
- [ ] Common ground verified with multimeter
- [ ] Battery charged and voltage checked
- [ ] No loose connections
- [ ] BEC providing stable 5V
- [ ] Motors spin freely (no mechanical binding)

### **Software:**
- [ ] Code uploaded successfully
- [ ] Serial Monitor shows "System ready"
- [ ] WiFi AP "WBR_Robot" visible
- [ ] Web interface loads at 192.168.4.1
- [ ] All sensor readings valid

### **Testing:**
- [ ] Each component tested individually
- [ ] Encoders counting correctly
- [ ] Web controls responsive
- [ ] Data logging working
- [ ] Emergency stop tested

---

## 🆘 **EMERGENCY PROCEDURES**

### **If Something Goes Wrong:**
1. **Press ESP32 reset button** (or unplug USB)
2. **Disconnect battery immediately**
3. **Check for:**
   - Burning smell (component failure)
   - Hot components (short circuit)
   - Sparks (serious short)
4. **Measure voltages with multimeter:**
   - Battery: 6.4V - 8.4V
   - BEC output: 4.8V - 5.2V
   - ESP32 3.3V pin: 3.2V - 3.4V
5. **Review wiring diagram before reconnecting**

---

## 📞 **SUPPORT & RESOURCES**

- **ESP32 Pinout:** https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
- **DRV8833 Datasheet:** https://www.ti.com/product/DRV8833
- **MPU6050 Library:** https://github.com/jrowberg/i2cdevlib
- **ESP32 Arduino Core:** https://github.com/espressif/arduino-esp32

---

**Document Version:** 1.0
**Last Updated:** November 2, 2025
**Author:** WBR Robot Project Team

🤖 **Happy Building!** 🚀
