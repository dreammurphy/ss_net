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

typedef unsigned long int uLint_t;
typedef unsigned int uint_t;


/* str_data_para: Origin Data */
typedef struct str_data_para
{
    public:
    char *para;
	uint_t nx,ny,nf; // input map Size, x, y and features(CI)
	uint_t outf;     // output features(Co)

}str_data_para; 

/* str_judge_data: Origin Data */
typedef struct str_judge_data
{
    public:
    float *judge_data;
    uLint_t err_num=0;
    uLint_t n_tot=0;
    float err_rate;

}str_judge_data; 






#endif
