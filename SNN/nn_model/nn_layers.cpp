#include "nn_layers.h"



NN_layer_c::NN_layer_c()
{
	layer_id = 0;
	layer_type = LAYER_OTHERS;  // Not Init
	layer_lif_mod = NEURON_LIF_MODE;
}

NN_layer_c::~NN_layer_c()
{

}

void NN_layer_c::NN_layer_init(int id0)
{
	layer_id = id0;

	p_out = spike_buf;
	p_spikes = spike_buf;
	p_sum_spikes = spike_sum_buf;
	p_mem = mem_buf;

}

void NN_layer_c::NN_layer_init_W(void)
{
	uLint_t idx;
	uLint_t size_tot;
	size_tot = p_calc_para->size_out;
	for(idx=0; idx<size_tot; idx++)
	{
		p_out[idx]=0;
		p_spikes[idx]=0;
		p_sum_spikes[idx]=0;
		p_mem[idx] = 0; 
	}

}

void NN_layer_c::NN_layer_reset(void)
{
	uLint_t idx;
	uLint_t size_tot;
	size_tot = p_calc_para->size_out;
	for(idx=0; idx<size_tot; idx++)
	{
		p_out[idx]=0;
		p_spikes[idx]=0;
		p_sum_spikes[idx]=0;
		p_mem[idx] = 0; 
	}


}

void NN_layer_c::NN_layer_process(char * inX)
{
	uLint_t idx;
	uLint_t size_tot;
	size_tot = p_calc_para->size_out;

	switch(layer_type):
	{
		case LAYER_FCN:
			func_fcn_spike_pro(inX, p_mem, p_calc_para);
			break;
			
		case LAYER_CNN:
			func_cnn_spike_pro(inX, p_mem, p_calc_para);
			break;

		case LAYER_POOLING:
			func_pooling_ave_pro(inX, p_mem, p_calc_para);

			break;

		case LAYER_DIVERSITY:

			break;

		case LAYER_OTHERS:

			break;

		default:
			printf("In NN_layer_process, layer_type not support! Debug!\n");
			break;
	}
	for(idx=0; idx<size_tot; idx++)
	{
		NN_neuron_out_pro(&p_mem[idx],&p_spikes[idx]);
	}
}

// just do one neuron out, LIF model
void NN_layer_c::NN_neuron_out_pro(float *in, char *out)
{
	if (*in >= threshold)
	{
		*out = 1;
		if (0 == layer_lif_mod)
		{
			*in -= threshold;
		}
		else
		{
			*in = 0;
		}
	}
	else
	{
		*out = 0;
	}
		
}

