#include "nn_model.h"

char g_inX_Buffer[TEST_MAP_MAX_SIZE];
NN_layer_c g_Layers[LAYER_MAX_NUMBER];

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

void NN_model_c::NN_data_input_trans(float *inX,char *outX,Simu_para_c *p_simu_para0)
{

}

void NN_out_pro_one(void *outX);
{

}

void NN_model_c::Neuron_NN_pro(int tidx, float *inX, float *outX)
{
	char *inX2;
	int lay_idx;
	inX2 = g_inX_Buffer;
	NN_data_input_trans(inX,inX2,p_simu_para);
	for(lay_idx=1;lay_idx<n_layer_tot;lay_idx++)
	{
		p_nn_layer[lay_idx]->NN_layer_process(inX2);
		inX2 = p_nn_layer[lay_idx]->p_spikes;
	}
	// finally,
	NN_out_pro_one(outX);
}


