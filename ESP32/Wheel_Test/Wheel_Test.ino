#include "Params.h"
#include "Receiver.h"
#include "HRController.h"
#include "VYBController.h"
#include "IMU.h"
#include "POL.h"
#include "Logger.h"
#include "Timer.h"


// Properties와 Receiver, Controller 초기화
const Properties properties = createDefaultProperties();
POL Pol(properties);
Receiver receiver(Serial2);
IMU MPU6050;

HRController HR_controller;
VYBController VYB_controller;

Logger WIFI_Logger(ssid, password);

// Encoder variables
volatile long encL_count = 0;
volatile long encR_count = 0;
long prev_encL = 0;
long prev_encR = 0;
float speedL = 0.0f;
float speedR = 0.0f;
const float pulses_per_rev = 7.0f;  // Adjust based on your N20 encoder (typically 7-12)

Eigen::Matrix<float, 8, 1> z = Eigen::Matrix<float, 8, 1>::Zero();
Eigen::Matrix<int16_t, 2, 1> pwm_inputs = Eigen::Matrix<int16_t, 2, 1>::Zero();

int i = 0;
Timer log_timer(Timer::TimerType::Millis);
Timer sampling_timer(Timer::TimerType::Millis);
Timer temp_timer(Timer::TimerType::Micros);
float h_d = HEIGHT_MAX, phi_d = 0;

std::vector<int16_t> command_vec;  // PWM command vector
int16_t command_max = 255;
int16_t command_increment = 10;
int command_idx = 0;
int dt_command = 42;  // milli sec

// Encoder ISRs
void IRAM_ATTR encL_ISR() {
  if (digitalRead(ENC_L_A) == digitalRead(ENC_L_B)) {
    encL_count++;
  } else {
    encL_count--;
  }
}

void IRAM_ATTR encR_ISR() {
  if (digitalRead(ENC_R_A) == digitalRead(ENC_R_B)) {
    encR_count++;
  } else {
    encR_count--;
  }
}

void serialPrintStates();

void setup() {
  // ================================
  // Command 벡터 초기화
  // ================================
  for (int16_t command = command_increment; command < command_max; command += command_increment) {
    command_vec.push_back(command);   // 명령 벡터에 양수 값 추가
    command_vec.push_back(-command);  // 명령 벡터에 음수 값 추가
  }

  // ================================
  // 시리얼 통신, 리시버, 서보 컨트롤러 초기화
  // ================================
  Serial.begin(115200);                        // 시리얼 통신 시작
  receiver.begin();                            // 리시버 시작
  HR_controller.attachServos(LH_PIN, RH_PIN);  // 서보 핀 설정

  // ================================
  // IMU(MPU6050) 초기화
  // ================================
  if (!MPU6050.begin()) {
    Serial.println("[ERROR] Fail to initialize IMU.");
  }

  // ================================
  // PWM for DRV8833 초기화
  // ================================
  ledcSetup(0, 5000, 8);  // Channel 0 for left motor IN1
  ledcAttachPin(MOTOR_L_IN1, 0);
  ledcSetup(1, 5000, 8);  // Channel 1 for left motor IN2
  ledcAttachPin(MOTOR_L_IN2, 1);
  ledcSetup(2, 5000, 8);  // Channel 2 for right motor IN1
  ledcAttachPin(MOTOR_R_IN1, 2);
  ledcSetup(3, 5000, 8);  // Channel 3 for right motor IN2
  ledcAttachPin(MOTOR_R_IN2, 3);

  // ================================
  // Encoder 초기화
  // ================================
  pinMode(ENC_L_A, INPUT);
  pinMode(ENC_L_B, INPUT);
  pinMode(ENC_R_A, INPUT);
  pinMode(ENC_R_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_L_A), encL_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_R_A), encR_ISR, CHANGE);

  // ================================
  // WIFI 연결 설정
  // ================================
  WIFI_Logger.begin();  // WIFI 연결 초기화

  // ================================
  // PSRAM 상태 확인 및 초기화
  // ================================
  if (psramFound()) {
    Serial.println("PSRAM available.");
    if (!psramInit()) {
      Serial.println("PSRAM initialization failed!");
      while (1) {}  // 초기화 실패 시 무한 루프
    } else {
      Serial.println("PSRAM initialized successfully!");
    }
    Serial.printf("PSRAM size: %d bytes\n", ESP.getPsramSize());  // PSRAM 크기 출력
  } else {
    Serial.println("PSRAM not available.");
    while (1) {}  // PSRAM 미탐지 시 무한 루프
  }

  // ================================
  // SBUS 데이터 수신 대기 (타임아웃 처리)
  // ================================
  Timer receiver_timer(Timer::TimerType::Millis);
  receiver_timer.start();
  const unsigned long timeout = 5000;  // 타임아웃 5초 설정

  while (!receiver.readData()) {
    if (receiver_timer.getDuration() > timeout) {
      Serial.println("Timeout: No data received from SBUS.");
      receiver_timer.start();  // 타임아웃 초기화
    }
  }
  receiver.updateData();  // 데이터 업데이트

  // ================================
  // Logger pre-allocation
  // ================================
  WIFI_Logger.readyToLogValue("cal_time");
  WIFI_Logger.readyToLogTimeStamp();            // 시간 기록
  WIFI_Logger.readyToLogValue("h_d");           // (m)
  WIFI_Logger.readyToLogValue("pwm_RW");        // PWM duty
  WIFI_Logger.readyToLogValue("pwm_LW");        // PWM duty
  WIFI_Logger.readyToLogValue("acc_x");         // m/s^2
  WIFI_Logger.readyToLogValue("acc_y");         // m/s^2
  WIFI_Logger.readyToLogValue("acc_z");         // m/s^2
  WIFI_Logger.readyToLogValue("gyr_x");         // rad/s
  WIFI_Logger.readyToLogValue("gyr_y");         // rad/s
  WIFI_Logger.readyToLogValue("gyr_z");         // rad/s
  WIFI_Logger.readyToLogValue("speed_RW");      // rad/s
  WIFI_Logger.readyToLogValue("speed_LW");      // rad/s
  // WIFI_Logger.readyToLogValue("log_time");      // (us)


  // ================================
  // 시간 측정 시작
  // ================================
  log_timer.start();
  sampling_timer.start();
}


void loop() {
  // sampling time이 경과했을 때만 실행
  if (sampling_timer.getDuration() >= dt * 1000) {
    // 경과 시간 출력
    Serial.print("SamplingTime(ms):");
    Serial.print(sampling_timer.getDuration());
    Serial.print(" ");

    sampling_timer.start();  // sampling timer 초기화


    if (receiver.readData()) {
      receiver.updateData();
    }

    if (receiver.isRun()) {
      // Running Mode
      if (i >= dt_command / (dt * 1000)) {
        if (command_idx < command_vec.size()) {
          pwm_inputs << command_vec.at(command_idx), command_vec.at(command_idx);
          command_idx++;
          i = 0;
        } else {
          pwm_inputs << 0, 0;
        }
      }

      receiver.updateDesiredStates();
      h_d = receiver.getDesiredHeight();
      phi_d = 0;  // roll control diable

      Pol.setHR(h_d, phi_d);
      Pol.solve_inverse_kinematics();

      VYB_controller.sendDirectControlCommand(pwm_inputs);

      // Calculate speeds from encoders
      speedL = (encL_count - prev_encL) * 2 * PI / pulses_per_rev / dt;
      speedR = (encR_count - prev_encR) * 2 * PI / pulses_per_rev / dt;
      prev_encL = encL_count;
      prev_encR = encR_count;

      // measurement update
      MPU6050.readData();
      MPU6050.getIMUMeasurement(z);
      z(6) = speedR;  // Right wheel speed
      z(7) = speedL;  // Left wheel speed

      Serial.println(" Run Mode");

      ///// Logging /////
      WIFI_Logger.logValue("cal_time", sampling_timer.getDuration());
      WIFI_Logger.logTimeStamp(log_timer.getDuration());  // 시간 기록
      WIFI_Logger.logValue("h_d", h_d);  // (m)
      WIFI_Logger.logValue("pwm_RW", pwm_inputs(0));
      WIFI_Logger.logValue("pwm_LW", pwm_inputs(1));
      WIFI_Logger.logValue("acc_x", z(0));                         // m/s^2
      WIFI_Logger.logValue("acc_y", z(1));                         // m/s^2
      WIFI_Logger.logValue("acc_z", z(2));                         // m/s^2
      WIFI_Logger.logValue("gyr_x", z(3));                         // rad/s
      WIFI_Logger.logValue("gyr_y", z(4));                         // rad/s
      WIFI_Logger.logValue("gyr_z", z(5));                         // rad/s
      WIFI_Logger.logValue("speed_RW", z(6));                      // rad/s
      WIFI_Logger.logValue("speed_LW", z(7));                      // rad/s
      ////////////////////

      i++;
    } else if (receiver.isReset()) {
      // Estimator Reset
      pwm_inputs.setZero();
      WIFI_Logger.resetLogData();
      i = 0;
      command_idx = 0;
      log_timer.start();

    } else {
      // Off Mode
      ledcWrite(0, 0);  // Stop left motor
      ledcWrite(1, 0);
      ledcWrite(2, 0);  // Stop right motor
      ledcWrite(3, 0);

      // Calculate speeds
      speedL = (encL_count - prev_encL) * 2 * PI / pulses_per_rev / dt;
      speedR = (encR_count - prev_encR) * 2 * PI / pulses_per_rev / dt;
      prev_encL = encL_count;
      prev_encR = encR_count;

      // measurement update
      MPU6050.readData();
      MPU6050.getIMUMeasurement(z);
      z(6) = speedR;
      z(7) = speedL;

      WIFI_Logger.handleClientRequests();  // Log Data 전송

      Serial.println(" Off Mode");
    }
  }
}