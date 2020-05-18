#include "pre_data.h"

void func_pre_data_init(str_data_para *p_pre_data)
{
	// for XOR
	p_pre_data->nf = 1;
	p_pre_data->nx = 1;
	p_pre_data->ny = 2;  
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

	
}

// Get_test_data, get test_x,test_y
void Simu_para_c::Get_test_data(int idx, str_data_para *p_param, void *out_x, void *out_y)
{

}

// Get_train_data, get test_x,test_y
void Simu_para_c::Get_train_data(int idx, str_data_para *p_param, void *out_x, void *out_y)
{

}

