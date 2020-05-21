#include "pre_data.h"

void func_pre_data_init(str_data_para *p_pre_data)
{
	// for XOR
	p_pre_data->nf = 1;
	p_pre_data->nx = 1;
	p_pre_data->ny = 2;  
	p_pre_data->outf  = 1;
	p_pre_data->n_tot = 4; 
}


Simu_para_c::Simu_para_c()
{

}

Simu_para_c::~Simu_para_c()
{

}

void Simu_para_c::Simu_para_init(str_data_para *p_data_para)
{
	p_in_data_para = p_data_para;
	in_size = p_in_data_para->nx * p_in_data_para->ny * p_in_data_para->nf;
	n_tot = p_data_para->n_tot;
	
	
}

// for XOR
float xor_buf_in[4][2] = {{0,0},{0,1},{1,0},{1,1}};
int   xor_ou[4] = {0,1,1,0};

// Get_test_data, get test_x,test_y
void Simu_para_c::Get_test_data(int idx, str_data_para *p_param, void *out_x, void *out_y)
{
	// for XOR
	float *in_x;
	int *ou_y;
	in_x = (float *)(out_x);
	ou_y = (int *)(out_y);
	in_x[0] = xor_buf_in[idx][0];
	in_x[1] = xor_buf_in[idx][1];
	ou_y[0] = xor_ou[idx];

}

// Get_train_data, get test_x,test_y
void Simu_para_c::Get_train_data(int idx, str_data_para *p_param, void *out_x, void *out_y)
{

}

