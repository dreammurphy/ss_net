#ifndef _COMMON_H_
#define _COMMON_H_


#include "stdio.h"

typedef enum
{
    LAYER_FCN = 0,
    LAYER_CNN,
    LAYER_POOLING,
    LAYER_DIVERSITY,
    LAYER_OTHERS


} LayerType;

/* str_data_para: Origin Data */
typedef struct str_data_para
{
    public:
    char *para;

}str_data_para; 

/* str_judge_data: Origin Data */
typedef struct str_judge_data
{
    public:
    float *judge_data;
    unsigned long int err_num=0;
    unsigned long int n_tot=0;
    float err_rate;

}str_judge_data; 






#endif