#ifndef _CALC_UTIL_H_
#define _CALC_UTIL_H_

#include "common.h"

void func_fcn_process();
void func_cnn_process();
void func_pooling_process();

void func_conv2d_spike_pro(char *inX, float *ouX, float *p_wei,int Iy, int Kx, int Ky);
void func_fcn_spike_pro(char *inX, float *ouX, str_calc_para *p_calc_para);
void func_cnn_spike_pro(char *inX, float *ouX, str_calc_para *p_calc_para);
void func_pooling_ave_pro(char *inX, float *ouX, str_calc_para *p_calc_para);
void func_pooling_max_pro(char *inX, float *ouX, str_calc_para *p_calc_para);
void func_concat_pro(char *inX, float *ouX, str_calc_para *p_calc_para);
void func_eltwise_pro(char *inX, float *ouX, str_calc_para *p_calc_para);



#endif
