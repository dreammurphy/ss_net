#include "stdio.h"

#include "common.h"
#include "nn_model.h"
#include "snn_simu.h"
#include "pre_data.h"


#define TEST_MAP_MAX_SIZE       (32*32)
#define TEST_LABOuLES_MAX_SIZE    (10)
float g_InBuffer[TEST_MAP_MAX_SIZE];
int g_IdeaBuffer[TEST_LABOuLES_MAX_SIZE]; // as Ideal res
int g_OuBuffer[TEST_LABOuLES_MAX_SIZE];

str_data_para       g_pre_data;
str_judge_data      g_res_judge;
Simu_para_c         g_simu_para;
NN_model_c          g_nn_mod;

void main(void)
{
    float *test_x, *train_x, *inX;;
    int *test_y, *train_y;
    int idx;
    int *res_data;

    str_data_para *p_pre_data;
    str_judge_data *p_res_judge;
    Simu_para_c  *p_simu_para;

    NN_model_c *p_nn_mod;

    

    /* NN Process */
    /* First, init parameters and NN */
    /* then, do NN process */
    /* finally, do summary */

    /* First, init parameters and NN */
    p_pre_data = &g_pre_data; 
    // add codes to init pre_data
    // ...................

    p_res_judge = &g_res_judge;
    // add codes to init res_judge
    // ...................
    p_res_judge->n_tot   = 0;
    p_res_judge->err_num = 0;

    p_simu_para = &g_simu_para;
    p_simu_para->Simu_para_init(p_pre_data);


    p_nn_mod = &g_nn_mod;
    p_nn_mod->NN_model_init(p_simu_para);

    /* init buffers */
    test_x = &g_InBuffer[0];
    test_y = &g_IdeaBuffer[0];
    res_data = &g_OuBuffer[0];
    inX = test_x;

    /* then, do NN process */
    for(idx=0; idx<p_simu_para->n_tot; idx++)
    {
        p_simu_para->Get_test_data(idx, p_pre_data,inX,test_y);
        Neuron_sim_process(inX,p_nn_mod,res_data);
        Judge_pro(idx,res_data,test_y,p_res_judge);
    }

    /* finally, do summary */
    Debug_analyze(p_res_judge);


}