#include "nn_model.h"

NN_model_c::NN_model_c()
{
}

NN_model_c::~NN_model_c()
{

}

void NN_model_c::NN_model_init(Simu_para_c *p_simu_para0)
{
	p_simu_para = p_simu_para0;
	p_in_spike = p_simu_para->p_simu_spike_init;
}

void NN_model_c::NN_model_reset(void)
{
	
	p_in_spike = p_simu_para->p_simu_spike_init;
}

void NN_model_c::Neuron_NN_pro(int tidx, float *inX, float *outX)
{

}


