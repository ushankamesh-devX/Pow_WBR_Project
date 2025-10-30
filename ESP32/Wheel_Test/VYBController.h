#ifndef VYB_CONTROLLER_H
#define VYB_CONTROLLER_H

#include <Arduino.h>
#include <ArduinoEigenDense.h>
#include "Params.h"

/**
 * @class VYBController
 * @brief 제어 시스템의 LQR 기반 동작을 처리하는 클래스
 */
class VYBController {
private:
  std::vector<Eigen::Matrix<float, 2, 4>> Ks;  ///< LQR 게인 행렬들의 벡터
  Eigen::Matrix<float, 2, 4> K;                ///< 현재 사용 중인 LQR 게인
  Eigen::Matrix<float, 2, 1> u;                ///< 제어 입력 벡터

  float saturation;        ///< input saturation

public:
  /**
   * @brief 생성자: VYBController 초기화
   */
  VYBController() {
    saturation = 255.0f;  // PWM max


    // LQR 게인 초기화 (하드코딩된 데이터 삽입)
    Eigen::Matrix<float, 2, 4> mat;
    //////////////////////////////////////////////////////////
    mat << 1.08462239f, 0.12305272f, 0.19382449f, -0.19479993f,
      -1.10877978f, -0.12702366f, -0.19782372f, -0.19250106f;
    Ks.push_back(mat);


    // LQR 게인 초기화 (하드코딩된 데이터 삽입)
    Eigen::Matrix<float, 2, 4> mat;
    //////////////////////////////////////////////////////////
    mat << 1.08462239f, 0.12305272f, 0.19382449f, -0.19479993f,
      -1.10877978f, -0.12702366f, -0.19782372f, -0.19250106f;
    Ks.push_back(mat);

    mat << 1.13454252f, 0.12908051f, 0.19377222f, -0.19484792f,
      -1.15994509f, -0.13330291f, -0.19778948f, -0.19257405f;
    Ks.push_back(mat);

    mat << 1.18247271f, 0.13517704f, 0.19376463f, -0.19485655f,
      -1.20911107f, -0.13965707f, -0.19780903f, -0.19259824f;
    Ks.push_back(mat);

    mat << 1.22822571f, 0.14133210f, 0.19380100f, -0.19483537f,
      -1.25607926f, -0.14607383f, -0.19787917f, -0.19258459f;
    Ks.push_back(mat);

    mat << 1.27187628f, 0.14753870f, 0.19387452f, -0.19478663f,
      -1.30092440f, -0.15254568f, -0.19799221f, -0.19253632f;
    Ks.push_back(mat);

    mat << 1.31356399f, 0.15379065f, 0.19397781f, -0.19471129f,
      -1.34378880f, -0.15906634f, -0.19814025f, -0.19245517f;
    Ks.push_back(mat);

    mat << 1.35344225f, 0.16008252f, 0.19410421f, -0.19461047f,
      -1.38482905f, -0.16563050f, -0.19831622f, -0.19234296f;
    Ks.push_back(mat);

    mat << 1.39166084f, 0.16640989f, 0.19424814f, -0.19448583f,
      -1.42419769f, -0.17223381f, -0.19851408f, -0.19220205f;
    Ks.push_back(mat);

    mat << 1.42835960f, 0.17276955f, 0.19440513f, -0.19433973f,
      -1.46203648f, -0.17887311f, -0.19872882f, -0.19203546f;
    Ks.push_back(mat);

    mat << 1.46366649f, 0.17915983f, 0.19457175f, -0.19417517f,
      -1.49847430f, -0.18554664f, -0.19895636f, -0.19184681f;
    Ks.push_back(mat);

    mat << 1.49769806f, 0.18558107f, 0.19474555f, -0.19399568f,
      -1.53362759f, -0.19225457f, -0.19919351f, -0.19164030f;
    Ks.push_back(mat);

    mat << 1.53056218f, 0.19203646f, 0.19492502f, -0.19380546f,
      -1.56760289f, -0.19899973f, -0.19943784f, -0.19142077f;
    Ks.push_back(mat);

    mat << 1.56236614f, 0.19853400f, 0.19510961f, -0.19361074f,
      -1.60050406f, -0.20578947f, -0.19968755f, -0.19119521f;
    Ks.push_back(mat);

    mat << 1.59324034f, 0.20509105f, 0.19530021f, -0.19342572f,
      -1.63245203f, -0.21263951f, -0.19994099f, -0.19097907f;
    Ks.push_back(mat);
    /////////////////////////////////////////////////////////////////
  }

  /**
   * @brief 현재 제어 입력 벡터를 반환
   * @return Eigen::Matrix<float, 2, 1> u
   */
  Eigen::Matrix<float, 2, 1> getInputVector() {
    return u;
  }

  /**
   * @brief 상태 벡터를 기반으로 제어 입력 벡터를 계산
   * @param x_d 목표 상태 벡터
   * @param x 현재 상태 벡터
   */
  void computeInput(Eigen::Matrix<float, 4, 1>& x_d, Eigen::Matrix<float, 4, 1>& x) {
    u = K * (x_d - x) / 2;

    // Input saturation
    for (int j = 0; j < 2; j++) {
      if (u(j) > saturation) {
        u(j) = saturation;
      } else if (u(j) < -saturation) {
        u(j) = -saturation;
      }
    }
  }

  /**
 * @brief 직접 제어 명령을 모터에 전송 (PWM for DRV8833)
 * @param pwm_inputs 두 개의 바퀴에 대한 PWM 신호 (-255 to 255)
 */
  void sendDirectControlCommand(Eigen::Matrix<int16_t, 2, 1> pwm_inputs) {
    int16_t pwm_RW = pwm_inputs(0);
    int16_t pwm_LW = pwm_inputs(1);

    // Right motor
    if (pwm_RW > 0) {
      ledcWrite(2, pwm_RW);  // IN1
      ledcWrite(3, 0);       // IN2
    } else if (pwm_RW < 0) {
      ledcWrite(2, 0);
      ledcWrite(3, -pwm_RW);
    } else {
      ledcWrite(2, 0);
      ledcWrite(3, 0);
    }

    // Left motor
    if (pwm_LW > 0) {
      ledcWrite(0, pwm_LW);  // IN1
      ledcWrite(1, 0);       // IN2
    } else if (pwm_LW < 0) {
      ledcWrite(0, 0);
      ledcWrite(1, -pwm_LW);
    } else {
      ledcWrite(0, 0);
      ledcWrite(1, 0);
    }
  }
};

#endif  // VYB_CONTROLLER_H
