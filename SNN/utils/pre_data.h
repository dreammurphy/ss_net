#ifndef _PRE_DATA_H
#define _PRE_DATA_H

#include "common.h"

/* Simu_para_c, simulation parameters */
class Simu_para_c
{
    public:
    uLint_t n_tot;
	uLint_t t_idx, t_simu;
    float *test_x,*test_y,*train_x,*trian_y;
    float *this_x, *this_y; // for each time index, get one
    char * p_simu_spike_init;
	str_data_para *p_in_data_para;
	uLint_t in_size;
		
    Simu_para_c();
    ~Simu_para_c();

    void Simu_para_init(str_data_para *p_data_para);
    // Get_test_data, get test_x,test_y
    void Get_test_data(int idx, str_data_para *p_param, void *out_x, void *out_y);
    // Get_train_data, get test_x,test_y
    void Get_train_data(int idx, str_data_para *p_param, void *out_x, void *out_y);


};










#endif