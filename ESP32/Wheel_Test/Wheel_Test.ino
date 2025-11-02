#include "Params.h"
#include "WebController.h"     // WiFi web interface (replaces Receiver)
#include "HRController.h"      // Hip servo controller
#include "DRV8833_Motor.h"     // N20 motor driver (replaces MGServo)
#include "IMU.h"
#include "POL.h"
#include "Logger.h"
#include "Timer.h"

// ============================================
// Global Objects
// ============================================
const Properties properties = createDefaultProperties();
POL Pol(properties);

// Motor objects (N20 with DRV8833)
DRV8833_Motor motor_RW(MOTOR_RW_IN1, MOTOR_RW_IN2, ENCODER_RW_A, ENCODER_RW_B, 0);  // Right wheel, PWM channel 0
DRV8833_Motor motor_LW(MOTOR_LW_IN1, MOTOR_LW_IN2, ENCODER_LW_A, ENCODER_LW_B, 1);  // Left wheel, PWM channel 1

// Web controller (replaces Receiver)
WebController webControl;

// Hip servos
HRController HR_controller;

// IMU
IMU MPU6050;

// Logger
Logger WIFI_Logger(ssid, password);

// State variables
Eigen::Matrix<float, 8, 1> z = Eigen::Matrix<float, 8, 1>::Zero();
int16_t pwm_RW = 0;  // PWM for right wheel
int16_t pwm_LW = 0;  // PWM for left wheel

// Timers
Timer log_timer(Timer::TimerType::Millis);
Timer sampling_timer(Timer::TimerType::Millis);

// Control parameters
float h_d = HEIGHT_MAX, phi_d = 0;

// ============================================
// Encoder Interrupt Service Routines
// ============================================
void IRAM_ATTR encoder_RW_ISR() {
  // Read encoder B to determine direction
  if (digitalRead(ENCODER_RW_B) == HIGH) {
    motor_RW.incrementEncoder();
  } else {
    motor_RW.decrementEncoder();
  }
}

void IRAM_ATTR encoder_LW_ISR() {
  // Read encoder B to determine direction
  if (digitalRead(ENCODER_LW_B) == HIGH) {
    motor_LW.incrementEncoder();
  } else {
    motor_LW.decrementEncoder();
  }
}

void setup() {
  // ================================
  // Serial Communication
  // ================================
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=================================");
  Serial.println("WBR Robot - N20 Motor Test");
  Serial.println("=================================");

  // ================================
  // Initialize Motors
  // ================================
  Serial.println("Initializing motors...");
  motor_RW.begin();
  motor_LW.begin();

  // Attach encoder interrupts
  attachInterrupt(digitalPinToInterrupt(ENCODER_RW_A), encoder_RW_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_LW_A), encoder_LW_ISR, RISING);
  Serial.println("Encoders attached");

  // ================================
  // Initialize Hip Servos
  // ================================
  Serial.println("Initializing hip servos...");
  HR_controller.attachServos(LH_PIN, RH_PIN);
  Serial.println("Hip servos ready");

  // ================================
  // Initialize IMU (MPU6050)
  // ================================
  Serial.println("Initializing IMU...");
  if (!MPU6050.begin()) {
    Serial.println("[ERROR] Failed to initialize IMU!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("IMU ready");

  // ================================
  // Initialize WiFi Web Controller
  // ================================
  Serial.println("Starting WiFi Access Point...");
  webControl.begin();
  Serial.println("WiFi ready - Connect to: " + String(ssid));
  Serial.println("Visit: http://192.168.4.1");

  // ================================
  // Initialize Logger
  // ================================
  Serial.println("Initializing data logger...");
  WIFI_Logger.readyToLogValue("loop_time");
  WIFI_Logger.readyToLogTimeStamp();
  WIFI_Logger.readyToLogValue("h_d");           // Desired height (m)
  WIFI_Logger.readyToLogValue("pwm_RW");        // Right wheel PWM
  WIFI_Logger.readyToLogValue("pwm_LW");        // Left wheel PWM
  WIFI_Logger.readyToLogValue("speed_RW");      // Right wheel speed (rad/s)
  WIFI_Logger.readyToLogValue("speed_LW");      // Left wheel speed (rad/s)
  WIFI_Logger.readyToLogValue("enc_RW");        // Right wheel encoder count
  WIFI_Logger.readyToLogValue("enc_LW");        // Left wheel encoder count
  WIFI_Logger.readyToLogValue("acc_x");         // Acceleration X (m/s²)
  WIFI_Logger.readyToLogValue("acc_y");         // Acceleration Y (m/s²)
  WIFI_Logger.readyToLogValue("acc_z");         // Acceleration Z (m/s²)
  WIFI_Logger.readyToLogValue("gyr_x");         // Gyro X (rad/s)
  WIFI_Logger.readyToLogValue("gyr_y");         // Gyro Y (rad/s)
  WIFI_Logger.readyToLogValue("gyr_z");         // Gyro Z (rad/s)
  Serial.println("Logger ready (Capacity: " + String(LOG_INIT_CAP) + " samples)");

  // ================================
  // Start Timers
  // ================================
  log_timer.start();
  sampling_timer.start();

  Serial.println("=================================");
  Serial.println("Setup complete! System ready.");
  Serial.println("=================================\n");
}


void loop() {
  // Handle web client requests continuously
  webControl.handleClient();

  // Run control loop at fixed sampling rate
  if (sampling_timer.getDuration() >= dt * 1000) {
    unsigned long loop_start = micros();
    sampling_timer.start();

    // ================================
    // Check control state
    // ================================
    if (webControl.isReset()) {
      // Reset command received
      Serial.println("RESET command received");
      motor_RW.stop();
      motor_LW.stop();
      motor_RW.resetEncoder();
      motor_LW.resetEncoder();
      WIFI_Logger.resetLogData();
      log_timer.start();
      return;
    }

    if (webControl.isRun()) {
      // ================================
      // RUN MODE - Active Control
      // ================================
      
      // Get desired states from web interface
      h_d = webControl.getDesiredHeight();
      phi_d = webControl.getDesiredRoll();
      float v_d = webControl.getDesiredVel();
      float psi_d = webControl.getDesiredYawVel();

      // ================================
      // Read Sensors
      // ================================
      // Read IMU
      MPU6050.readData();
      MPU6050.getIMUMeasurement(z);

      // Update motor speeds from encoders
      motor_RW.updateSpeed();
      motor_LW.updateSpeed();

      // ================================
      // Compute Hip Joint Angles
      // ================================
      Pol.setHR(h_d, phi_d);
      Pol.solve_inverse_kinematics();
      HR_controller.controlHipServos(Pol.get_theta_hips());

      // ================================
      // Simple Motor Control
      // (Replace with your control algorithm)
      // ================================
      // Example: Simple velocity control
      int base_pwm = map(v_d * 1000, -1000, 1000, -200, 200);
      int yaw_pwm = map(psi_d * 100, -150, 150, -100, 100);
      
      pwm_RW = constrain(base_pwm - yaw_pwm, -PWM_MAX, PWM_MAX);
      pwm_LW = constrain(base_pwm + yaw_pwm, -PWM_MAX, PWM_MAX);

      motor_RW.setSpeed(pwm_RW);
      motor_LW.setSpeed(pwm_LW);

      // ================================
      // Data Logging (TEMPORARILY DISABLED FOR TESTING)
      // ================================
      unsigned long loop_time = micros() - loop_start;
      
      // COMMENTED OUT TO TEST MEMORY ISSUE
      /*
      WIFI_Logger.logValue("loop_time", loop_time / 1000.0);  // Convert to ms
      WIFI_Logger.logTimeStamp(log_timer.getDuration());
      WIFI_Logger.logValue("h_d", h_d);
      WIFI_Logger.logValue("pwm_RW", pwm_RW);
      WIFI_Logger.logValue("pwm_LW", pwm_LW);
      WIFI_Logger.logValue("speed_RW", motor_RW.getSpeedRadS());
      WIFI_Logger.logValue("speed_LW", motor_LW.getSpeedRadS());
      WIFI_Logger.logValue("enc_RW", motor_RW.getEncoderCount());
      WIFI_Logger.logValue("enc_LW", motor_LW.getEncoderCount());
      WIFI_Logger.logValue("acc_x", z(0));
      WIFI_Logger.logValue("acc_y", z(1));
      WIFI_Logger.logValue("acc_z", z(2));
      WIFI_Logger.logValue("gyr_x", z(3));
      WIFI_Logger.logValue("gyr_y", z(4));
      WIFI_Logger.logValue("gyr_z", z(5));
      */

      // Update web status
      webControl.setStatus(h_d, v_d, psi_d, z(0), z(1), z(2));

      // Print status every 200ms
      static unsigned long last_print = 0;
      if (millis() - last_print >= 200) {
        Serial.print("PWM: R=");
        Serial.print(pwm_RW);
        Serial.print(" L=");
        Serial.print(pwm_LW);
        Serial.print(" | Speed: R=");
        Serial.print(motor_RW.getSpeedRadS(), 2);
        Serial.print(" L=");
        Serial.print(motor_LW.getSpeedRadS(), 2);
        Serial.print(" | Loop:");
        Serial.print(loop_time);
        Serial.println("us");
        last_print = millis();
      }

    } else {
      // ================================
      // STOP MODE - Motors Off
      // ================================
      motor_RW.stop();
      motor_LW.stop();

      // Still read IMU for monitoring
      MPU6050.readData();
      MPU6050.getIMUMeasurement(z);

      // Handle data download requests
      WIFI_Logger.handleClientRequests();

      // Print status
      static unsigned long last_print_stop = 0;
      if (millis() - last_print_stop >= 1000) {
        Serial.println("STOPPED - Waiting for START command");
        last_print_stop = millis();
      }
    }
  }
}