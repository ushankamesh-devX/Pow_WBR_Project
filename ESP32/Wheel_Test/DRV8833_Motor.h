#ifndef DRV8833_MOTOR_H
#define DRV8833_MOTOR_H

#include <Arduino.h>
#include "Params.h"

/**
 * @class DRV8833_Motor
 * @brief Controls N20 DC motor via DRV8833 H-Bridge with encoder feedback
 */
class DRV8833_Motor {
private:
  // Motor control pins
  uint8_t pin_IN1;  // PWM pin
  uint8_t pin_IN2;  // Direction pin
  
  // Encoder pins
  uint8_t pin_encoder_A;
  uint8_t pin_encoder_B;
  
  // Encoder state
  volatile long encoder_count;
  volatile long last_encoder_count;
  
  // Speed calculation
  float speed_rpm;
  float speed_rad_s;
  unsigned long last_speed_calc_time;
  
  // PWM channel (ESP32 specific)
  uint8_t pwm_channel;
  
  // Motor direction
  int8_t direction;  // 1 = forward, -1 = reverse, 0 = stop

public:
  /**
   * @brief Constructor
   * @param IN1 PWM control pin
   * @param IN2 Direction control pin
   * @param ENC_A Encoder phase A pin
   * @param ENC_B Encoder phase B pin
   * @param channel PWM channel (0-15 on ESP32)
   */
  DRV8833_Motor(uint8_t IN1, uint8_t IN2, uint8_t ENC_A, uint8_t ENC_B, uint8_t channel)
    : pin_IN1(IN1), pin_IN2(IN2), pin_encoder_A(ENC_A), pin_encoder_B(ENC_B), 
      pwm_channel(channel), encoder_count(0), last_encoder_count(0),
      speed_rpm(0), speed_rad_s(0), direction(0) {
    last_speed_calc_time = millis();
  }

  /**
   * @brief Initialize motor pins and PWM
   */
  void begin() {
    // Configure motor control pins
    pinMode(pin_IN1, OUTPUT);
    pinMode(pin_IN2, OUTPUT);
    
    // Configure PWM on IN1 pin (ESP32 Arduino Core 3.x)
    ledcAttach(pin_IN1, PWM_FREQUENCY, PWM_RESOLUTION);
    
    // Stop motor initially
    stop();
    
    // Configure encoder pins (interrupts attached externally)
    pinMode(pin_encoder_A, INPUT_PULLUP);
    pinMode(pin_encoder_B, INPUT_PULLUP);
    
    Serial.print("Motor initialized on channel ");
    Serial.print(pwm_channel);
    Serial.print(", IN1=");
    Serial.print(pin_IN1);
    Serial.print(", IN2=");
    Serial.println(pin_IN2);
  }

  /**
   * @brief Set motor speed with PWM
   * @param pwm_value -255 to 255 (negative = reverse, positive = forward)
   */
  void setSpeed(int pwm_value) {
    // Constrain PWM value
    pwm_value = constrain(pwm_value, -PWM_MAX, PWM_MAX);
    
    // Determine direction and magnitude
    if (pwm_value > 0) {
      // Forward
      direction = 1;
      digitalWrite(pin_IN2, LOW);
      ledcWrite(pin_IN1, pwm_value);
    } else if (pwm_value < 0) {
      // Reverse
      direction = -1;
      digitalWrite(pin_IN2, HIGH);
      ledcWrite(pin_IN1, PWM_MAX + pwm_value);  // PWM is inverted when IN2=HIGH
    } else {
      // Stop
      stop();
    }
  }

  /**
   * @brief Stop motor (coast mode)
   */
  void stop() {
    direction = 0;
    ledcWrite(pin_IN1, 0);
    digitalWrite(pin_IN2, LOW);
  }

  /**
   * @brief Brake motor (short brake)
   */
  void brake() {
    direction = 0;
    ledcWrite(pin_IN1, PWM_MAX);
    digitalWrite(pin_IN2, HIGH);
  }

  /**
   * @brief Increment encoder count (call from ISR)
   */
  void incrementEncoder() {
    encoder_count++;
  }

  /**
   * @brief Decrement encoder count (call from ISR)
   */
  void decrementEncoder() {
    encoder_count--;
  }

  /**
   * @brief Update speed calculation (call periodically, not in ISR)
   */
  void updateSpeed() {
    unsigned long current_time = millis();
    unsigned long dt_ms = current_time - last_speed_calc_time;
    
    if (dt_ms >= 50) {  // Update every 50ms
      long delta_count = encoder_count - last_encoder_count;
      
      // Calculate RPM
      float revolutions = (float)delta_count / WHEEL_PPR;
      float dt_min = dt_ms / 60000.0;  // Convert to minutes
      speed_rpm = revolutions / dt_min;
      
      // Convert to rad/s
      speed_rad_s = speed_rpm * 2.0 * PI / 60.0;
      
      // Update for next calculation
      last_encoder_count = encoder_count;
      last_speed_calc_time = current_time;
    }
  }

  /**
   * @brief Get current encoder count
   * @return Encoder pulse count
   */
  long getEncoderCount() const {
    return encoder_count;
  }

  /**
   * @brief Get motor speed in RPM
   * @return Speed in revolutions per minute
   */
  float getSpeedRPM() const {
    return speed_rpm;
  }

  /**
   * @brief Get motor speed in rad/s
   * @return Angular velocity in radians per second
   */
  float getSpeedRadS() const {
    return speed_rad_s;
  }

  /**
   * @brief Reset encoder count
   */
  void resetEncoder() {
    encoder_count = 0;
    last_encoder_count = 0;
  }

  /**
   * @brief Get current direction
   * @return 1=forward, -1=reverse, 0=stopped
   */
  int8_t getDirection() const {
    return direction;
  }

  /**
   * @brief Print motor status for debugging
   */
  void printStatus() const {
    Serial.print("Motor[CH");
    Serial.print(pwm_channel);
    Serial.print("] Dir:");
    Serial.print(direction);
    Serial.print(" Enc:");
    Serial.print(encoder_count);
    Serial.print(" RPM:");
    Serial.print(speed_rpm, 2);
    Serial.print(" rad/s:");
    Serial.println(speed_rad_s, 3);
  }
};

#endif  // DRV8833_MOTOR_H
