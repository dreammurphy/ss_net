#include "common.h"
#include "snn_simu.h"



// Generator spike sequences, input:in_data, size:n_in, output:spike_buf,size:n_in; do n_tot get full spikes
void Spike_input_gen_one(char *spike_buf, int tidx, int t_sim, void *in_data, uLint_t n_in, int mod_se)
{

}

// Generator spike sequences, input:in_data, size:n_in, output:spike_buf,size:t_sim*n_in
void Spike_input_generator(char *spike_buf, int t_sim, void *in_data, uLint_t n_in, int mod_se)
{


}

// Neuron_NN_pro, suing nn_model process function, results in res_data
void Neuron_NN_pro(int tidx, void *inX, NN_model_c *p_nn, void*res_data)
{


}

// Neuron_out_pro, do summary
void Neuron_out_pro(int tidx, int t_sim, void* outX, void *outY, void* deb_info)
{


}

void Neuron_get_res(int tidx, void *outY, void *res_data)
{

}


// Neuron_sim_one,using NN process, simulate one case, results in res_data
void Neuron_sim_one(void *test_data, NN_model_c *p_nn, void *res_data, str_judge_data*p_res_judge)
{
	uLint_t n_tot = p_nn->p_simu_para->n_tot;
	int t_sim = p_nn->p_simu_para->t_simu;
	uLint_t n_in = p_nn->p_simu_para->in_size;
	char *spike_buf = p_nn->p_in_spike;
	int tidx;
	int mod_se;

	float *inX, *outX, *outY;
	float *deb_info;

	mod_se = 0;
	
	for(tidx=0; tidx<n_tot; tidx++)
	{
		Spike_input_gen_one(spike_buf, tidx, t_sim, test_data, n_in, mod_se);
		p_nn->Neuron_NN_pro(tidx,float * inX,float * outX);
		Neuron_out_pro(tidx, t_sim,void * outX,void * outY,void * deb_info);
		Neuron_get_res(tidx, outY, res_data);


	}

	// other process
	

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
void Neuron_sim_process(void *test_data, NN_model_c *p_nn, void *res_data,str_judge_data *p_res_judge)
{
	char spike_buf[2];
	int n_tot = 10;
	int *in_data = (int *)test_data;
	int n_in = 0;
	int mod_se = 0;

	p_nn->NN_model_reset();
	Neuron_sim_one(test_data, p_nn, res_data,p_res_judge);
	Debug_analyze(p_res_judge);

}

// for debug
void Debug_analyze(str_judge_data *p_judgeRes)
{

}

// for analyze
void Analyze_process(str_judge_data *p_judgeRes, str_judge_data *p_fin_res)
{
    p_fin_res->err_rate = float(p_judgeRes->err_num) /p_judgeRes->n_tot;
    printf("Simulation Results, the accuracy is %.3f%%, Total Data is %ld !\n",100*p_fin_res->err_rate,p_judgeRes->n_tot);

}



