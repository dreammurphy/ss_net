#include "common.h"
#include "snn_simu.h"

// Generator spike sequences, input:in_data, size:n_in, output:spike_buf,size:n_tot*n_in
void Spike_input_generator(char *spike_buf, int n_tot, void *in_data, int n_in, int mod_se)
{


}

// Neuron_NN_pro, suing nn_model process function, results in res_data
void Neuron_NN_pro(int tidx, void *inX, NN_model_c *p_nn, void*res_data)
{


}

// Neuron_out_pro, do summary
void Neuron_out_pro(int tidx, int n_tot, void* outX, void *outY, void* deb_info)
{


}

// Neuron_sim_one,using NN process, simulate one case, results in res_data
void Neuron_sim_one(void *test_data, NN_model_c *p_nn, void *res_data)
{


}


// Judge_pro, do judgement
void Judge_pro(int idx, void *outY, void *ouIdeal, str_judge_data *p_judgeRes)
{
    int *p_out, *p_ideal;

    p_out = (int *)outY;
    p_ideal = (int *)ouIdeal;

    /* judge error */
    if ((idx % 16) ==3)
    {

        p_judgeRes->err_num++;
    }
    p_judgeRes->n_tot++;
}

// Neuron_sim_process, using NN process, get the results
void Neuron_sim_process(void *test_data, NN_model_c *p_nn, void *res_data)
{


}

// for debug
void Debug_analyze(str_judge_data *p_judgeRes)
{
    p_judgeRes->err_rate = float(p_judgeRes->err_num) /p_judgeRes->n_tot;
    printf("Simulation Results, the accuracy is %.3f, Total Data is %ld !\n",100*p_judgeRes->err_rate,p_judgeRes->n_tot);
}