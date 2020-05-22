#include "calc_util.h"
#include "stdio.h"

void func_fcn_process(void)
{
}
void func_cnn_process(void)
{
}
void func_pooling_process(void)
{

}

void func_conv2d_spike_pro(char *inX, float *ouX, float *p_wei,int Iy, int Kx, int Ky)
{
//	int coidx, cidx;
	int kxidx, kyidx;
	char *p_ix, *p_iy;
	float *p_kx, *p_ky;
	float tmp_sum0;
	tmp_sum0 = 0;
	p_kx = p_wei;
	p_ix = inX;
	for(kxidx=0; kxidx < Kx; kxidx++)
	{
		p_ky = p_kx;
		p_kx += Ky;
		p_iy = p_ix;
		p_ix += Iy;
		for(kyidx=0; kyidx < Ky; kyidx++)
		{
//			tmp_sum0 += p_iy[kyidx]*p_ky[kyidx];
			if (p_iy[kyidx] != 0)
				tmp_sum0 += p_ky[kyidx];
		}
	}
	*ouX = tmp_sum0;

}

void func_fcn_spike_pro(char *inX, float *ouX, str_calc_para *p_calc_para)
{
	int coidx, cidx;
	float *p_wei, *p_ko;
	float *p_bias;

	if ((p_calc_para->Kx != 1) || (p_calc_para->Ky != 1) || (p_calc_para->Ix != 1) \
		|| (p_calc_para->Iy != 1))
	{
		printf("Error in func_fcn_spike_pro,parametes not correct. Need debug! \n");
		return;
	}

	p_wei = p_calc_para->p_weight;
	for(cidx=0; cidx< p_calc_para->Ci; cidx++)
	{
		p_ko = p_wei;
		p_wei++;
		
		if (inX[cidx]!= 0)
		{
			for(coidx=0; coidx<p_calc_para->Co; coidx++)
			{
				ouX[coidx] += p_ko[0];
				p_ko += p_calc_para->Ci; // Next Ker point
			}
		}
	}
	if(p_calc_para->bias_en != 0)
	{	
		p_bias = p_calc_para->p_bias;
		for(coidx=0; coidx<p_calc_para->Co; coidx++)
		{
			ouX[coidx]+= p_bias[coidx];
		}
	}
}

void func_cnn_spike_pro(char *inX, float *ouX, str_calc_para *p_calc_para)
{
// in:Y*X*Ci, Ou:Y*X*Co, weight:Ky*Kx*Ci*Co
	int oyidx,oxidx,ocidx,icidx;
	uLint_t map_size_o,map_size_i, map_ker;
	float *p_co, *p_xo, *p_yo;
	char *p_ci, *p_xi;  //, *p_yi;
	float *p_ko, *p_ki, *p_kx; //, *p_ky;

	float tmp_sum0;

	map_size_o = p_calc_para->Ox*p_calc_para->Oy;
	map_size_i = p_calc_para->Ix*p_calc_para->Iy;
	map_ker    = p_calc_para->Kx*p_calc_para->Ky;
	p_co = ouX;
	p_ko  = p_calc_para->p_weight;
	
	for(ocidx=0; ocidx < p_calc_para->Co; ocidx++)
	{
		p_xo = p_co; 
		p_co += map_size_o;
		p_ki = p_ko;
		p_ko += map_ker;
		for(oxidx=0; oxidx < p_calc_para->Ox; oxidx++)
		{
			p_yo = p_xo;
			p_xo += p_calc_para->Oy;
			for(oyidx=0; oyidx < p_calc_para->Oy; oyidx++)
			{
				// each kernel
				tmp_sum0 = p_yo[oyidx];
				p_ci = inX;
				for(icidx = 0; icidx<p_calc_para->Ci; icidx++)
				{
					float sum0;
					p_xi = p_ci;
					p_ci += map_size_i;
					p_kx = p_ki;
					p_ki += map_ker;
					func_conv2d_spike_pro(p_xi, &sum0, p_kx, p_calc_para->Iy, p_calc_para->Kx, p_calc_para->Ky);
					
					tmp_sum0 += sum0;
				} // end icidx
				p_yo[oyidx] = tmp_sum0;
			} // end oyidx

		}// end oxidx
	}// end ocidx
}

void func_pooling_ave_pro(char *inX, float *ouX, str_calc_para *p_calc_para)
{

}
void func_pooling_max_pro(char *inX, float *ouX, str_calc_para *p_calc_para)
{

}
void func_concat_pro(char *inX, float *ouX, str_calc_para *p_calc_para)
{

}
void func_eltwise_pro(char *inX, float *ouX, str_calc_para *p_calc_para)
{

}


