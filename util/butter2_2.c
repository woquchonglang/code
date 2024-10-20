#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// 定义低通滤波器结构体
typedef struct {
  float a[3]; // 系数
  float b[3]; // 系数
  float x[3]; // 输入值
  float y[3]; // 输出值
} ButterworthLPF_t;

// 初始化滤波器
void ButterworthLPF_init(ButterworthLPF_t *filter, float cutoff_freq,
                         float sample_rate) {
  float omega = 2.0f * M_PI * cutoff_freq / sample_rate; // 归一化频率
  float omega2 = omega * omega;
  float sqrt2 = sqrt(2.0f); // 根号2

  // 计算系数
  float k = 1.0f / (1.0f + sqrt2 * omega + omega2);
  filter->a[0] = omega2 * k;
  filter->a[1] = 2.0f * filter->a[0];
  filter->a[2] = filter->a[0];
  filter->b[1] = 2.0f * (1.0f - omega2) * k;
  filter->b[2] = (1.0f - sqrt2 * omega + omega2) * k;

  // 初始化输入和输出值为0
  for (int i = 0; i < 3; i++) {
    filter->x[i] = 0.0f;
    filter->y[i] = 0.0f;
  }
}

// 更新滤波器
float ButterworthLPF_update(ButterworthLPF_t *filter, float new_value) {
  // 将输入值存储
  filter->x[0] = new_value;

  // 计算输出值
  filter->y[0] = filter->a[0] * filter->x[0] + filter->a[1] * filter->x[1] +
                 filter->a[2] * filter->x[2] - filter->b[1] * filter->y[1] -
                 filter->b[2] * filter->y[2];

  // 更新输入和输出历史值
  filter->x[2] = filter->x[1];
  filter->x[1] = filter->x[0];
  filter->y[2] = filter->y[1];
  filter->y[1] = filter->y[0];

  return filter->y[0]; // 返回输出值
}

// 示例主函数
int main() {
  ButterworthLPF_t filter;
  float cutoff_freq = 10.0f;  // 截止频率
  float sample_rate = 100.0f; // 采样频率

  ButterworthLPF_init(&filter, cutoff_freq, sample_rate);

  // 模拟输入信号
  float input_signal[] = {1.0f,  0.5f, 0.0f, -0.5f, -1.0f,
                          -0.5f, 0.0f, 0.5f, 1.0f};
  int signal_length = sizeof(input_signal) / sizeof(input_signal[0]);

  // 处理输入信号
  for (int i = 0; i < signal_length; i++) {
    float output = ButterworthLPF_update(&filter, input_signal[i]);
    printf("Input: %.2f, Output: %.2f\n", input_signal[i], output);
  }

  return 0;
}
