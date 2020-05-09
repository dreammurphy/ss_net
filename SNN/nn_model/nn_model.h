#ifndef _NN_MODEL_H_
#define _NN_MODEL_H_


#include "common.h"
#include "nn_layers.h"

class NN_model_c
{
    public:
    int n_layer_tot;
NN_layer_c *p_nn_layer;


NN_model_c();
~NN_model_c();

void NN_model_init(str_simu_para *p_simu_para);
void Neuron_NN_pro(int tidx, float *inX, float *outX); // forward





};






#endif