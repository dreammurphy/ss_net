#include "nn_layers.h"
#include "calc_util.h"
#include "string.h"
#include "stdlib.h"

NN_layer_c::NN_layer_c()
{
	NN_layer_init_base();
}

NN_layer_c::~NN_layer_c()
{
	printf("In ~NN_layer_c, free memory, flag=%d\n",mem_ini_flag);

	nn_layer_free();
	printf("In ~NN_layer_c, free end\n");
}

void NN_layer_c::NN_layer_init_base(void) // could only be called before init others
{
	mem_ini_flag = 0;
	layer_id = 0;
	layer_type = LAYER_OTHERS;  // Not Init
	layer_lif_mod = NEURON_LIF_MODE;
	p_out = NULL;
	p_spikes = NULL;
	p_sum_spikes = NULL;
	p_mem = NULL;	
	p_calc_para = &layer_calc_para;
	p_calc_para->p_weight = NULL;
	p_calc_para->p_bias   = NULL;
	p_calc_para->size_out = 0;
	
}


void NN_layer_c::nn_layer_free(void)
{
	printf("In nn_layer_free, free memory, flag=%d,",mem_ini_flag);

	if (mem_ini_flag != 0)
	{
		FREE_POINT(p_mem);
		FREE_POINT(p_sum_spikes);
		FREE_POINT(p_spikes);
		p_out = NULL;
		
		mem_ini_flag = 0;

//		FREE_POINT(p_calc_para->p_weight); 
//		FREE_POINT(p_calc_para->p_bias);
	
		printf("Memory have been freed\n");
	}
	else
	{
		printf(" Not Need free memory\n");

	}
}


void NN_layer_c::NN_layer_init_id(int id0)
{
	layer_id = id0;

//	p_out = spike_buf;
//	p_spikes = spike_buf;
//	p_sum_spikes = spike_sum_buf;
//	p_mem = mem_buf;

}

void NN_layer_c::NN_layer_init_type(LayerType type0)
{
	layer_type = type0;

//	p_out = spike_buf;
//	p_spikes = spike_buf;
//	p_sum_spikes = spike_sum_buf;
//	p_mem = mem_buf;

}


void NN_layer_c::NN_layer_init_calc_para(str_calc_para *p_calc_para0)
{
	// copy
	p_calc_para = &layer_calc_para;
	memcpy(p_calc_para, p_calc_para0, sizeof(str_calc_para));
//	p_calc_para->size_out = p_calc_para->Ox*p_calc_para->Oy*p_calc_para->Co;
	p_mem = (float *)malloc(p_calc_para->size_out*sizeof(float));
	p_sum_spikes = (int *)malloc(p_calc_para->size_out*sizeof(int));
	p_spikes = (char *)malloc(p_calc_para->size_out*sizeof(char));
//	p_out = (char *)malloc(p_calc_para->size_out*sizeof(char));
	p_out = p_spikes; // same as spike ?

	mem_ini_flag = 1;
	if((p_mem==NULL) || (p_sum_spikes == NULL) || (p_spikes == NULL))
	{
		printf("in NN_layer_init_calc_para, Not enough memory, check it!\n");
		nn_layer_free();
		return;
	}

	NN_layer_reset();

}

void NN_layer_c::NN_layer_init_W(float *wei, float*bia, float thod)
{
	uLint_t idx;
	uLint_t size_tot;
	//str_calc_para *p_calc_para0;

	wei = wei;  // for compiler
	bia = bia;
	thod = thod;
	
	size_tot = p_calc_para->size_out;
	if (mem_ini_flag == 0)
	{
		printf("Error in NN_layer_init_W, should call NN_layer_init_calc_para before init weight.Debug\n");
		return;
	}
	for(idx=0; idx<size_tot; idx++)
	{
		p_out[idx]=0;
		p_spikes[idx]=0;
		p_sum_spikes[idx]=0;
		p_mem[idx] = 0; 
	}
//	str_calc_para *p_calc_para0;

}

void NN_layer_c::NN_layer_reset(void)
{
	uLint_t idx;
	uLint_t size_tot;
	if (mem_ini_flag == 0)
	{
		printf("In NN_layer_reset, before reset, should init memory pointers. \nDebug\n");
		return;
	}
	size_tot = p_calc_para->size_out;
	for(idx=0; idx<size_tot; idx++)
		p_out[idx]=0;
	for(idx=0; idx<size_tot; idx++)
		p_spikes[idx]=0;
	for(idx=0; idx<size_tot; idx++)
		p_sum_spikes[idx]=0;
	for(idx=0; idx<size_tot; idx++)
		p_mem[idx] = 0; 

}

void NN_layer_c::NN_layer_process(char * inX)
{
	uLint_t idx;
	uLint_t size_tot;
	size_tot = p_calc_para->size_out;

	switch(layer_type)
	{
		case LAYER_FCN:
			func_fcn_spike_pro(inX, p_mem, p_calc_para);
			break;
			
		case LAYER_CNN:
			func_cnn_spike_pro(inX, p_mem, p_calc_para);
			break;

		case LAYER_POOLING_AVE:
			func_pooling_ave_pro(inX, p_mem, p_calc_para);

			break;

		case LAYER_POOLING_MAX:
			func_pooling_max_pro(inX, p_mem, p_calc_para);

			break;

		case LAYER_DIVERSITY:

			break;

		case LAYER_OTHERS:

			break;

		default:
			printf("In NN_layer_process, layer_type not support! Debug!\n");
			break;
	}

	if ((LAYER_FCN == layer_type) || (LAYER_CNN == layer_type))
	{
		for(idx=0; idx<size_tot; idx++)
		{
			NN_neuron_out_pro(&p_mem[idx],&p_spikes[idx],threshold);
		}
	}
	else if((LAYER_POOLING_AVE == layer_type) || (LAYER_POOLING_MAX == layer_type))
	{
		// only do judge, do not need update memory
		for(idx=0; idx<size_tot; idx++)
		{
			p_spikes[idx] = (p_mem[idx]>=threshold);
		}
	}
	else //others, not support now
	{
		printf("In NN_layer_process-2, layer_type not support! Debug!\n");

	}
}

// just do one neuron out, LIF model
void NN_layer_c::NN_neuron_out_pro(float *in, char *out, float thresh)
{
	if (*in >= thresh)
	{
		*out = 1;
		if (0 == layer_lif_mod)
		{
			*in -= thresh;
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

