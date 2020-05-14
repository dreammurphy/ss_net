#ifndef _NN_LAYERS_H_
#define _NN_LAYERS_H_

#include "../utils/common.h"

class NN_layer_c
{
    public:
        int         layer_id;  
        LayerType   layer_type; 
        float       *p_weight;
        float       *p_bias;
        int Ix,Iy,Ci,Ox,Oy,Co,Kx,Ky,stride_x,stride_y;
        int         lastSimuTime;
        float       *p_out;
        float       *p_spikes;
        float       *sum_spikes;
        float       *mem;
        float       threshold;

    NN_layer_c();
    ~NN_layer_c();

    void NN_layer_init(int id0);
    void NN_layer_init_W(void);
    void NN_layer_reset(void);

    void NN_layer_process(float * inX);


};






#endif