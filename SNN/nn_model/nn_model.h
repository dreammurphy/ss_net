#ifndef _NN_MODEL_H_
#define _NN_MODEL_H_


#include "../utils/common.h"
#include "../utils/pre_data.h"
#include "nn_layers.h"



class NN_model_c
{
    public:
    int n_layer_tot;
NN_layer_c *p_nn_layer;
	Simu_para_c *p_simu_para;

	char *p_in_spike;

NN_model_c();
~NN_model_c();

void NN_model_init(Simu_para_c *p_simu_para0);
void NN_model_reset(void);
void Neuron_NN_pro(int tidx, float *inX, float *outX); // forward





};






#endif