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
    int err_num;
    int err_rate;

}str_judge_data; 






#endif