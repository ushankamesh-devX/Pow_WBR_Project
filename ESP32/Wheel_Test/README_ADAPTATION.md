# 🎉 CODE ADAPTATION COMPLETE!

## ✅ **WHAT WAS CHANGED**

Your code has been successfully adapted for your actual hardware:
- **ESP32 DevKit V1** (NO PSRAM)
- **N20 DC Motors with Encoders** (instead of RS485 MG Servos)
- **DRV8833 PWM Driver** (instead of RS485 interface)
- **WiFi Web Interface** (instead of RC Receiver)
- **All other components kept:** MPU6050 IMU, RC Servos for hips

---

## 📁 **NEW & MODIFIED FILES**

### **NEW FILES CREATED:**
1. **`DRV8833_Motor.h`** - Motor driver for N20 motors with encoder support
2. **`WebController.h`** - WiFi web interface for robot control
3. **`WIRING_GUIDE.md`** - Complete wiring instructions (THIS IS IMPORTANT!)

### **MODIFIED FILES:**
1. **`Params.h`** - Updated GPIO pins for DevKit V1 compatibility
2. **`Logger.h`** - Changed from PSRAM to SRAM (reduced capacity)
3. **`Wheel_Test.ino`** - Complete rewrite of main code

### **REMOVED DEPENDENCIES:**
- ❌ `Receiver.h` - No longer needed (no RC receiver)
- ❌ `MGServo.h` - Not used (replaced with DRV8833_Motor.h)
- ❌ `VYBController.h` - Not used (wrong motor type)
- ❌ `ps_vector.h` - Still there but Logger uses std::vector now

---

## 🔌 **QUICK WIRING SUMMARY**

### **Power Connections:**
```
7.4V Battery → DRV8833 VCC → Motors
7.4V Battery → 5V BEC → Servos
USB Cable → ESP32 (for programming)

ALL GROUNDS CONNECTED TOGETHER!
```

### **ESP32 GPIO Assignments:**
```
GPIO 12  → Right Hip Servo
GPIO 13  → Left Hip Servo
GPIO 14  → Right Encoder B
GPIO 16  → Right Motor IN1 (PWM)
GPIO 17  → Right Motor IN2 (DIR)
GPIO 21  → MPU6050 SDA
GPIO 22  → MPU6050 SCL
GPIO 25  → Left Motor IN1 (PWM)
GPIO 26  → Left Motor IN2 (DIR)
GPIO 27  → Right Encoder A (interrupt)
GPIO 32  → Left Encoder A (interrupt)
GPIO 33  → Left Encoder B
```

**📖 See `WIRING_GUIDE.md` for complete details!**

---

## 🚀 **HOW TO USE**

### **Step 1: Upload Code**
1. Open `Wheel_Test.ino` in Arduino IDE
2. Select **Board:** "ESP32 Dev Module"
3. Select **Port:** Your ESP32 COM port
4. Click **Upload**

### **Step 2: Connect Hardware**
Follow the wiring guide in `WIRING_GUIDE.md` exactly!

### **Step 3: Power On**
1. Connect ESP32 to USB
2. Open Serial Monitor (115200 baud)
3. Wait for "System ready" message
4. Note the WiFi network name: **"WBR_Robot"**

### **Step 4: Connect & Control**
1. Connect your phone/laptop to WiFi: **"WBR_Robot"**
2. Password: **"12345678"**
3. Open browser: **http://192.168.4.1**
4. Use sliders to control:
   - Height (0.07 - 0.20 m)
   - Roll angle (-30° to +30°)
   - Velocity (-1.0 to +1.0 m/s)
   - Yaw rate (-1.5 to +1.5 rad/s)
5. Click **START** to run
6. Click **STOP** to stop motors
7. Click **RESET** to clear data

---

## 📊 **KEY CHANGES FROM ORIGINAL CODE**

| **Feature** | **Original Code** | **New Code** |
|-------------|-------------------|--------------|
| **Memory** | 50,000 samples (PSRAM) | 2,000 samples (SRAM) |
| **Logging Duration** | 150 seconds | 6 seconds |
| **Motor Control** | RS485 torque commands | PWM speed control |
| **Motor Feedback** | Current + speed from RS485 | Encoder pulses only |
| **User Input** | RC Receiver (SBUS) | WiFi web interface |
| **Sampling Rate** | 3ms (333Hz) | 5ms (200Hz) |
| **GPIO Pins** | ESP32-S3 (GPIO 40-42) | ESP32-WROOM (GPIO 0-39) |

---

## ⚠️ **IMPORTANT LIMITATIONS**

### **What You CAN Do:**
✅ Control motor speed (PWM)
✅ Read encoder pulses & calculate speed
✅ Control hip servos (height/roll)
✅ Read IMU data (acc + gyro)
✅ Log 6 seconds of data
✅ WiFi control from phone/PC
✅ Real-time status monitoring

### **What You CANNOT Do:**
❌ Torque control (N20 motors are open-loop)
❌ Current sensing (DRV8833 has no feedback)
❌ Long-term logging (limited to 6 seconds)
❌ Advanced LQR control (needs torque feedback)
❌ Use original EKF (wheel speed source changed)

---

## 🔧 **CONFIGURATION OPTIONS**

### **Adjust Logging Capacity** (if needed):
In `Logger.h`, change:
```cpp
const size_t LOG_INIT_CAP = 2000;  // Increase if you have free RAM
```

### **Change Sampling Rate:**
In `Params.h`, change:
```cpp
const float dt = 0.005;  // 5ms → Decrease for faster loop
```

### **Adjust Motor Speed:**
In `Wheel_Test.ino`, modify:
```cpp
int base_pwm = map(v_d * 1000, -1000, 1000, -200, 200);  // Max PWM ±200
```

### **Change WiFi Credentials:**
In `Params.h`, change:
```cpp
const char* ssid = "WBR_Robot";     // Your network name
const char* password = "12345678";  // Your password (min 8 chars)
```

### **Encoder PPR (Pulses Per Revolution):**
In `Params.h`, adjust for your motor:
```cpp
#define ENCODER_PPR 7        // Motor shaft pulses
#define GEAR_RATIO 100       // Your gear ratio
```

---

## 🐛 **TROUBLESHOOTING**

### **Code won't compile:**
- ✅ Install **ESP32 board support** in Arduino IDE
- ✅ Install **ArduinoEigenDense** library
- ✅ Install **ESP32Servo** library
- ✅ Select correct board: "ESP32 Dev Module"

### **Motors don't spin:**
- ✅ Check DRV8833 VCC connected to battery
- ✅ Verify motor wiring (red/black to AOUT/BOUT)
- ✅ Test with `motor_RW.setSpeed(100);` in Serial
- ✅ Check common ground connections

### **Encoders not counting:**
- ✅ Check encoder power (3.3V)
- ✅ Verify interrupt pins (GPIO 27, 32)
- ✅ Spin wheels by hand and watch Serial output
- ✅ Swap A/B wires if counting backwards

### **Web interface won't load:**
- ✅ Check Serial Monitor for IP address
- ✅ Ensure connected to "WBR_Robot" WiFi
- ✅ Try http://192.168.4.1 (default AP address)
- ✅ Restart ESP32 if WiFi won't start

### **ESP32 keeps resetting:**
- ✅ Power supply insufficient (add external battery)
- ✅ Motors drawing too much current (reduce PWM)
- ✅ Check for short circuits
- ✅ Add 100µF capacitor across ESP32 VIN/GND

---

## 📚 **REQUIRED LIBRARIES**

Install these in Arduino IDE (Tools → Manage Libraries):

1. **ESP32 Board Support** (via Board Manager)
   - File → Preferences → Additional Board URLs
   - Add: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`

2. **ArduinoEigenDense** (for matrix math)
   - Search: "Eigen" by Tom Backman

3. **ESP32Servo** (for servo control)
   - Search: "ESP32Servo" by Kevin Harrington

---

## 🎯 **NEXT STEPS**

### **Immediate Tasks:**
1. ✅ Wire hardware according to `WIRING_GUIDE.md`
2. ✅ Upload code to ESP32
3. ✅ Test each component individually
4. ✅ Verify web interface works
5. ✅ Test motor control with low PWM first

### **After Basic Testing Works:**
1. **Tune encoder parameters** (PPR, gear ratio)
2. **Calibrate IMU** (run calibration routine)
3. **Implement PID control** (for better speed control)
4. **Add balance controller** (use EKF for state estimation)
5. **Tune control parameters** (gains, limits)

### **Advanced Features (Optional):**
- Add PID controller for closed-loop speed control
- Implement complementary filter for IMU
- Add battery voltage monitoring
- Create mobile app interface
- Add data download to SD card

---

## 📞 **NEED HELP?**

### **Common Questions:**

**Q: Can I use 5V power supply instead of 7.4V battery?**
A: No, N20 motors need 6V minimum. Use 7.4V 2S LiPo or 6× AA batteries (9V).

**Q: My encoders are 3.3V but motors are 6V, is this okay?**
A: Yes! Encoder power is separate from motor power. Use 3.3V for encoder logic.

**Q: Can I power ESP32 from battery through BEC?**
A: Yes, connect BEC 5V output to ESP32 VIN pin. But keep USB for debugging.

**Q: How do I download logged data?**
A: Visit http://192.168.4.1/logdata in browser (downloads CSV file).

**Q: Motors spin but encoder doesn't count?**
A: Check encoder power and A/B wires. Test with Serial output in loop.

**Q: Web page is slow to load?**
A: Normal for ESP32 AP mode. Wait 5-10 seconds for full page load.

---

## ✅ **FINAL CHECKLIST**

Before powering on:
- [ ] All wiring matches `WIRING_GUIDE.md`
- [ ] Common ground verified
- [ ] Battery voltage checked (6.4V - 8.4V)
- [ ] No short circuits (multimeter continuity test)
- [ ] Code uploaded successfully
- [ ] Libraries installed
- [ ] Serial Monitor ready (115200 baud)

First power-on:
- [ ] ESP32 boots without errors
- [ ] WiFi AP "WBR_Robot" appears
- [ ] Web interface loads at 192.168.4.1
- [ ] Serial shows "System ready"
- [ ] IMU readings valid
- [ ] Encoders count when wheels spun by hand
- [ ] Motors respond to START command (low PWM!)

---

## 🎓 **UNDERSTANDING THE CODE**

### **Main Loop Flow:**
```
1. Handle web requests (continuously)
2. Every 5ms:
   a. Check for RESET command
   b. If RUN mode:
      - Read sensors (IMU + encoders)
      - Compute hip angles
      - Calculate motor PWM
      - Apply motor control
      - Log data
   c. If STOP mode:
      - Stop motors
      - Wait for commands
```

### **Control Strategy:**
Current code uses **simple open-loop control**:
```cpp
base_pwm = velocity command
yaw_pwm = yaw rate command
pwm_RW = base - yaw  (right wheel)
pwm_LW = base + yaw  (left wheel)
```

For better control, you should implement **PID controller**:
```
Error = desired_speed - actual_speed
PWM = Kp×Error + Ki×∫Error + Kd×dError/dt
```

---

## 📖 **CODE ARCHITECTURE**

```
Wheel_Test.ino (Main)
├── Params.h (Configuration)
├── WebController.h (WiFi interface)
├── DRV8833_Motor.h (Motor driver)
├── HRController.h (Hip servos)
├── IMU.h (MPU6050 sensor)
├── POL.h (Kinematics)
├── Logger.h (Data logging)
└── Timer.h (Timing utilities)
```

Each file is modular and can be tested independently!

---

## 🏆 **SUCCESS CRITERIA**

Your system is working correctly when:
1. ✅ ESP32 boots and shows "System ready"
2. ✅ WiFi AP appears in network list
3. ✅ Web interface loads and controls work
4. ✅ Motors spin when START clicked
5. ✅ Encoders count increases when wheels spin
6. ✅ IMU shows ~9.8 m/s² on Z-axis (gravity)
7. ✅ Hip servos move when height slider adjusted
8. ✅ Data logs successfully (check Serial output)
9. ✅ System runs for >10 seconds without crashing
10. ✅ STOP button immediately stops motors

---

## 🚀 **YOU'RE READY TO GO!**

Everything is prepared for your hardware. Follow these steps:

1. **Read `WIRING_GUIDE.md`** carefully
2. **Wire one section at a time** (test as you go)
3. **Upload code** and check Serial output
4. **Test incrementally** (don't run full speed immediately!)
5. **Monitor battery voltage** (don't discharge below 6.4V)
6. **Have fun and be safe!** 🤖

---

**Document Version:** 1.0
**Last Updated:** November 2, 2025
**Compatibility:** ESP32 DevKit V1 + N20 + DRV8833 + WiFi Control

🎉 **Good luck with your robot!** 🎉
