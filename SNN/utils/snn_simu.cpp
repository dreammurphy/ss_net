#include "common.h"
#include "snn_simu.h"
#include "calc_util.h"


// Generator spike sequences, input:in_data, size:n_in, output:spike_buf,size:n_in; do n_tot get full spikes
void Spike_input_gen_one(char *spike_out, int tidx, int t_sim, void *in_data, uLint_t n_in, int mod_se,NN_model_c *p_nn)
{
	float *inX;

	inX = (float*)in_data;
	if (mod_se == 0) // Here for XOR, ignore tidx, t_sim
	{
		uLint_t idx;
		for(idx=0; idx<n_in; idx++)
		{
			if (inX[idx] > 0.5)
				spike_out[idx] = 1;
			else
				spike_out[idx] = 0;
		}
	}
	else if(mod_se == 1) // for others
	{
		p_nn->p_spike_gen->spike_gen_one(inX,spike_out, tidx,n_in);
	}
	else // ahead
	{

		uLint_t idx;
		float tho;
		for(idx=0; idx<n_in; idx++)
		{
			tho = t_sim * inX[idx];
			if (tidx < tho)
				spike_out[idx] = 1;
			else
				spike_out[idx] = 0;

		#if (0 !=  CASE_DEBUG)
			printf("Spike, idx, tidx, t_sim, out: %ld, %d, %d, %f, %d\n",idx,tidx,t_sim,tho,spike_out[idx]);
		#endif
		}
	}
}

// Generator spike sequences, input:in_data, size:n_in, output:spike_buf,size:t_sim*n_in
void Spike_input_generator(char *spike_buf, int t_sim, void *in_data, uLint_t n_in, int mod_se,NN_model_c *p_nn)
{


}

// Neuron_NN_pro, suing nn_model process function, results in res_data
void Neuron_NN_pro(int tidx, void *inX, NN_model_c *p_nn, void*res_data)
{


}

// Neuron_out_pro, do summary
void Neuron_out_pro(int tidx, int t_sim, uLint_t n, void* inX, void *outY, void* deb_info)
{
	float *p_in;
	float *p_ou;
	uLint_t idx;
	p_in = (float *)inX;
	p_ou = (float *)outY;

	if (tidx == 0)
	{
		for(idx=0; idx< n; idx++)
		{
			p_ou[idx] = p_in[idx];
		}

	}
	else
	{
		for(idx=0; idx< n; idx++)
		{
			p_ou[idx] += p_in[idx];
		}
		
	}

}

void Neuron_get_res(int tidx, void *outY, void *res_data)
{

}


// Neuron_sim_one,using NN process, simulate one case, results in res_data
void Neuron_sim_one(void *in_data, NN_model_c *p_nn, void *res_data, str_judge_data*p_res_judge)
{
	uLint_t n_in = p_nn->p_simu_para->in_size;
	char *spike_buf = p_nn->p_in_spike;
	uint_t tidx,t_sim;
	uLint_t size_out;
	uLint_t jud_idx;
	int mod_se;

//	char *inX;
	float *outX;
	float *outY;
	float deb_info[1]; // for compiler


	mod_se = SPIKE_MODE_SEL;
	t_sim = p_nn->p_simu_para->t_simu;
	outX = p_nn->p_layer_out_buf_max;
	size_out = p_nn->p_calc_para_buf[p_nn->n_layer_tot-1]->size_out;
	
#if (0 == CASE_TEST)
	printf("In data is for XOR, %f,%f\n",*((float *)in_data),*((float *)in_data+1));
#endif
	for(tidx=0; tidx<t_sim; tidx++)
	{
		Spike_input_gen_one(spike_buf, tidx, t_sim, in_data, n_in, mod_se, p_nn);

		p_nn->Neuron_NN_pro(tidx, spike_buf, outX);
		
		outY = (float *)res_data; // -- should be delete ?, only for compiler debug
		Neuron_out_pro(tidx, t_sim, size_out, outX, outY, deb_info);

#if (0 != CASE_DEBUG)
		printf("In Neuron_sim_one:\n");
	#if (0 == CASE_TEST)
		printf("XOR, tidx:%d,res:%f\n",tidx,outY[0]/(tidx+1));
	#elif (1 == CASE_TEST)
		printf("tidx:%d,res:%.2f",tidx,outY[0]);
		for(uLint_t sidx=1; sidx<size_out; sidx++)
		{
			printf(",%.2f",outY[sidx]);
		}
		printf("\n");
	#else

	#endif
#endif

	//	Neuron_get_res(tidx, outY, res_data);
	}

	#if (0 == CASE_TEST)
	jud_idx = ((2*outY[0])>=t_sim); // for XOR
	#elif (1 == CASE_TEST)
	float   jud_va;
	func_find_max(outY, size_out, &jud_idx, &jud_va);
	#endif
	
	p_res_judge->judge_data = jud_idx+1; // for classes-index, start from 1

	// other process
	float amp_div;
	amp_div = 1.0/t_sim;
	for(uLint_t sidx=0; sidx<size_out;sidx++)
	{
		outY[sidx] *= amp_div;
	}

}


// Judge_pro, do judgement
void Judge_pro(uLint_t idx, void *outY, void *ouIdeal, str_judge_data *p_judgeRes)
{
#if 0
    int *p_out, *p_ideal;

    p_out = (int *)outY;
    p_ideal = (int *)ouIdeal;
#endif

	if (idx==0)
		p_judgeRes->err_num = 0;
		
    p_judgeRes->n_tot++;

    /* judge error */
    #if 0
    if ((idx % 16) ==3)
    {

        p_judgeRes->err_num++;
    }
	#endif
    int *ideal;
    ideal = (int *)ouIdeal;
	#if (0 == CASE_TEST) 
    /* for XOR */
//    if (((ou[0]>0.5) ^ ideal[0]) != 0)
    float *ou;
    ou = (float *)outY;
	if (p_judgeRes->judge_data != ideal[0])
    {
    	//	for debug
    	printf("case:%ld, out is %f, logic is %d, expected is %d\n",idx,ou[0],(ou[0]>0.5),ideal[0]);
    	p_judgeRes->err_num++;
    }
    #elif (1 == CASE_TEST)
	if (p_judgeRes->judge_data != (ideal[0]))
    {
    	//	for debug
    	printf("case:%ld, judge is %d, expected is %d\n",idx,p_judgeRes->judge_data,ideal[0]);
    	p_judgeRes->err_num++;
    }    
    #else


    #endif
}

// Neuron_sim_process, using NN process, get the results
void Neuron_sim_process(void *test_data, NN_model_c *p_nn, void *res_data,str_judge_data *p_res_judge)
{
#if 0
	char spike_buf[2];
	int n_tot = 10;
	int *in_data = (int *)test_data;
	int n_in = 0;
	int mod_se = SPIKE_MODE_SEL;
#endif

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
    printf("Simulation Results, the accuracy is %.3f%%, Total Data is %ld !\n",100*(1-p_fin_res->err_rate),p_judgeRes->n_tot);

}



