
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_RATE 1000 // 采样频率 (Hz)
#define CUTOFF_FREQ 100  // 截止频率 (Hz)
#define PI 3.14159265358979323846

// 计算二阶巴特沃斯低通滤波器的系数
void calculate_butterworth_coefficients(double *b, double *a,
                                        double cutoff_freq,
                                        double sample_rate) {
  double Wn = 2 * PI * cutoff_freq / sample_rate; // 归一化截止频率
  double alpha = sin(Wn) / (2 * sqrt(2));

  b[0] = (1 - cos(Wn)) / 2;
  b[1] = 1 - cos(Wn);
  b[2] = b[0];

  a[0] = 1 + alpha;
  a[1] = -2 * cos(Wn);
  a[2] = 1 - alpha;
}

// 进行滤波
void butterworth_filter(double *input, double *output, int length, double *b,
                        double *a) {
  for (int n = 0; n < length; n++) {
    output[n] = (b[0] / a[0]) * input[n];
    if (n > 0) {
      output[n] += (b[1] / a[0]) * input[n - 1] - (a[1] / a[0]) * output[n - 1];
    }
    if (n > 1) {
      output[n] += (b[2] / a[0]) * input[n - 2] - (a[2] / a[0]) * output[n - 2];
    }
  }
}

int main() {
  // 输入信号
  int length = 1000;
  double *input = (double *)malloc(length * sizeof(double));
  double *output = (double *)malloc(length * sizeof(double));

  // 生成测试信号 (10Hz 正弦波 + 噪声)
  for (int i = 0; i < length; i++) {
    input[i] = sin(2 * PI * 10 * i / SAMPLE_RATE) +
               0.5 * ((double)rand() / RAND_MAX - 0.5);
  }

  // 计算滤波器系数
  double b[3], a[3];
  calculate_butterworth_coefficients(b, a, CUTOFF_FREQ, SAMPLE_RATE);

  // 应用滤波器
  butterworth_filter(input, output, length, b, a);

  // 打印输出信号的前 10 个值
  printf("Filtered Output (first 10 values):\n");
  for (int i = 0; i < 10; i++) {
    printf("%f\n", output[i]);
  }

  // 释放内存
  free(input);
  free(output);

  return 0;
}
