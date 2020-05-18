#ifndef _NN_LAYERS_H_
#define _NN_LAYERS_H_

#include "../utils/common.h"


#define NEURON_LIF_MODE		0

class NN_layer_c
{
    public:
		char		mem_ini_flag; // 1, means have init memory points
        int         layer_id;  
        LayerType   layer_type; 
		char		layer_lif_mod;
		str_calc_para *p_calc_para;
		str_calc_para layer_calc_para;

        int         lastSimuTime;
        char       *p_out;
        char       *p_spikes;
        int       	*p_sum_spikes;
        float       *p_mem;
        float       threshold;

//		char	spike_buf[TEST_MAP_MAX_SIZE];
//		int     spike_sum_buf[TEST_MAP_MAX_SIZE];
//		float   mem_buf[TEST_MAP_MAX_SIZE];



    NN_layer_c();
    ~NN_layer_c();

    void NN_layer_init(int id0);
    void NN_layer_init_calc_para(str_calc_para *p_calc_para0);
    void NN_layer_init_W(void);
    void NN_layer_reset(void);

    void NN_layer_process(char * inX);
	void NN_neuron_out_pro(float *in, char *out); // just do one neuron out. LIF model


};






#endif
