#include "stdio.h"

#if 0
#include "./utils/common.h"
#include "./nn_model/nn_model.h"
#include "./utils/snn_simu.h"
#include "./utils/pre_data.h"
#else
#include "common.h"
#include "nn_model.h"
#include "snn_simu.h"
#include "pre_data.h"
#include "calc_util.h"

#endif

float g_InBuffer[TEST_MAP_MAX_SIZE];
int g_IdeaBuffer[TEST_LABEL_OUT_MAX_SIZE]; // as Ideal res
float g_OuBuffer[TEST_LABEL_OUT_MAX_SIZE];
char g_InSpikeOne[TEST_MAP_MAX_SIZE];

str_data_para       g_pre_data;
str_judge_data 		g_final_res;
str_judge_data      g_res_judge;
Simu_para_c         g_simu_para;
NN_model_c          g_nn_mod;
Spike_generator 	g_spike_generator;

extern FILE * fp_deb_spike_fcn;
int main(void)
{
    float *test_x, *inX;;
    int *test_y;
//	float *train_;,
//	int   *train_y;
    uLint_t idx;
    float *res_data;

    str_data_para *p_pre_data;
    str_judge_data *p_res_judge;
    Simu_para_c  *p_simu_para;
    str_judge_data *p_final_res;

    NN_model_c *p_nn_mod;
	Spike_generator *p_spike_gen;
	
	fp_deb_spike_fcn = fopen("deb_spike_fcn.txt","w");
	
	p_spike_gen = &g_spike_generator;
	if(0 != p_spike_gen->spike_gen_init())
	{
		printf("Error in main, spike_gen_init, debug!\n");
		return 0;
	}
    
	test_rand();
	test_spike_gen();

    /* NN Process */
    /* First, init parameters and NN */
    /* then, do NN process */
    /* finally, do summary */

    /* First, init parameters and NN */
    p_pre_data = &g_pre_data; 
    // add codes to init pre_data
    func_pre_data_init(p_pre_data);
    // ...................

    p_res_judge = &g_res_judge;
    // add codes to init res_judge
    // ...................
    p_res_judge->n_tot   = p_pre_data->n_tot;
    p_res_judge->err_num = 0;

	p_final_res = &g_final_res;
	
    p_simu_para = &g_simu_para;
    if (0 != p_simu_para->Simu_para_init(p_pre_data))
    {
		printf("Error in main, Simu_para_init, debug!\n");
		return 0;
    }
	p_simu_para->p_simu_spike_init = g_InSpikeOne;


    p_nn_mod = &g_nn_mod;
    p_nn_mod->NN_model_init(p_simu_para);
    p_nn_mod->p_spike_gen = p_spike_gen;

    /* init buffers */
    test_x = &g_InBuffer[0];
    test_y = &g_IdeaBuffer[0];
    res_data = &g_OuBuffer[0];
    inX = test_x;

#if 1
    /* then, do NN process */
	printf("Total test case = %ld\n",p_simu_para->n_tot);
	p_res_judge->n_tot = 0;
    for(idx=0; idx<p_simu_para->n_tot; idx++)
    {
        p_simu_para->Get_test_data(idx, p_pre_data,inX,test_y);
        Neuron_sim_process(inX,p_nn_mod,res_data,p_res_judge);
        Judge_pro(idx,res_data,test_y,p_res_judge);
    }
	printf("Simulation End, the summary results:\n");

    /* finally, do summary */
    Analyze_process(p_res_judge,p_final_res);

#endif

	fclose(fp_deb_spike_fcn);
	
    return 0;
}
