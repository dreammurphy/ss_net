#ifndef _NN_MODEL_H_
#define _NN_MODEL_H_


#include "../utils/common.h"
#include "../utils/pre_data.h"
#include "nn_layers.h"



class NN_model_c
{
    public:
	char nn_init_flag; // when init layers, this is set to 1
    int n_layer_tot;
	NN_layer_c *p_nn_layer[LAYER_MAX_NUMBER];	
	str_calc_para *p_calc_para_buf[LAYER_MAX_NUMBER];
	Simu_para_c *p_simu_para;
	uLint_t size_out_max_c;
	float * p_layer_out_buf_max;
	char *p_in_spike;
	char *p_inX_for_calc;
	
NN_model_c();
~NN_model_c();

char nn_get_layer_init(int idx);
void nn_model_free(void);

int NN_buffer_init(void); // 0:Error, 1:Success, for p_inX_for_calc and p_layer_out_buf_max

void NN_model_init(Simu_para_c *p_simu_para0);
void NN_model_reset(void);
void Neuron_NN_pro(int tidx, void *inX, void *outX); // forward


void NN_data_input_trans(void *inX,char *outX, Simu_para_c *p_simu_para0);
void NN_out_pro_one(void *outX);


void NN_model_init_xor(Simu_para_c *p_simu_para0);

};






#endif
