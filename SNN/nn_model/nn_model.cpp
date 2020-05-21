#include "nn_model.h"

//NN_layer_c g_Layers[LAYER_MAX_NUMBER];

NN_model_c::NN_model_c()
{

	nn_init_flag = 0;
	n_layer_tot  = LAYER_MAX_NUMBER;
	
	for(int idx=0; idx<n_layer_tot; idx++)
	{
		p_nn_layer[idx]= NULL;
		p_calc_para_buf[idx] = NULL;
	}
	p_layer_out_buf_max = NULL;
	p_inX_for_calc = NULL;

}

NN_model_c::~NN_model_c()
{
	printf("In ~NN_model_c, free memory, flag=%d\n",nn_init_flag);
	int idx;
	

}

void NN_model_c::nn_model_free(void)
{
	printf("In nn_model_free, free memory, flag=%d\n",nn_init_flag);

	for(int idx=0; idx<n_layer_tot; idx++)
	{
		if (NULL != p_nn_layer[idx])
		{
			p_nn_layer[idx]->nn_layer_free();
			p_nn_layer[idx] = NULL;
		}
		FREE_POINT(p_calc_para_buf[idx]);
	}
	FREE_POINT(p_layer_out_buf_max);
	FREE_POINT(p_inX_for_calc);
}


/* For XOR Example */
int xor_in_shape_buf[2][3] = {{1,1,2},{1,1,2}};
int xor_ou_shape_buf[2][3] = {{1,1,2},{1,1,1}};
int xor_ker_shape_buf[2][2] = {{1,1},{1,1}};
int xor_stride_buf[2][2] = {{1,1,},{1,1}};
float xor_wei0_0[2][2] = {{-0.5,0.5},{-0.75,0.75}};
float xor_wei0_1[2][2] = {{-4.4,4.4},{6,-6}};
float xor_bia0_0[2] = {0.5,0};
float xor_bia0_1[2] = {1,-1};
float xor_thed0[2] = [1,1];

/* Here a xor example, need amend codes for others */
void NN_model_c::NN_model_init(Simu_para_c *p_simu_para0)
{
	NN_model_init_xor(p_simu_para0);

}

int NN_model_c::NN_buffer_init(void)
{
	p_layer_out_buf_max = (float *)malloc(sizeof(float) * size_out_max_c);
	if (NULL == p_layer_out_buf_max)
	{
		printf("In NN_model_init_xor, memory is not enough for Layer data buffer, size if %ld, need debug \n",p_layer_out_buf_max);
		nn_model_free();
		return 0;
	}
	p_inX_for_calc = (char *)malloc(sizeof(char) * p_simu_para->in_size);
	if (NULL == p_inX_for_calc)
	{
		printf("In NN_model_init_xor, memory is not enough for First Input spike buffer, size if %ld, need debug \n",p_simu_para->in_size);
		nn_model_free();
		return 0;
	}

	return 1;
}

char NN_model_c::nn_get_layer_init(int idx)
{
	int fidx;
	p_nn_layer[idx] = (NN_layer_c *)malloc(sizeof(NN_layer_c));
	if (NULL == p_nn_layer[idx])
	{
		printf("In NN_model_c::nn_get_layer_init, Not Enough memory, need debug\n");
		if (idx > 0)
		{
			for(fidx=0; fidx<idx; fidx++)
			{
				free(p_nn_layer[fidx]);
				p_nn_layer[fidx] = NULL;
			}
		}
		n_layer_tot = 0;
		nn_init_flag = 0;
		return 0;
	}
	return 1;
}


void NN_model_c::NN_model_reset(void)
{
	int idx;
	for(idx=0; idx<n_layer_tot; idx++)
	{
		p_nn_layer[idx]->NN_layer_reset();
	}

//	p_in_spike = p_simu_para->p_simu_spike_init;

}

void NN_model_c::NN_data_input_trans(void *inX,char *outX,Simu_para_c *p_simu_para0)
{
	// Here for XOR, inX is type_char
	char *p_in;
	p_in = (char *)inX;
	for(uLint_t idx; idx<p_simu_para0->in_size)
	{
		outX[idx] = p_in[idx];
	}


}

void NN_model_c::NN_out_pro_one(void *outX)
{

}

void NN_model_c::Neuron_NN_pro(int tidx, void *inX, float *outX)
{
	char *inX2;
	int lay_idx;
	inX2 = p_inX_for_calc;

	NN_data_input_trans(inX,inX2,p_simu_para);

	for(lay_idx=1;lay_idx<n_layer_tot;lay_idx++)
	{
		p_nn_layer[lay_idx]->NN_layer_process(inX2);
		inX2 = p_nn_layer[lay_idx]->p_spikes;
	}
	// finally,
	NN_out_pro_one(outX);
}


/* Here a xor example, need amend codes for others */
void NN_model_c::NN_model_init_xor(Simu_para_c *p_simu_para0)
{
	char init_flg;
	uLint_t size_out_max;
	p_simu_para = p_simu_para0;
	p_in_spike = p_simu_para->p_simu_spike_init;

	// XOR Model
	n_layer_tot = 3;
	init_flg = nn_get_layer_init(0);
	if (0 == init_flg)
	{
		printf("In NN_model_init-0, Not Enough memory, need debug\n");
		nn_model_free();
		return;
	}


	int idx;
	#if 0
	{

		float		*p_weight;
		float		*p_bias;
		int Ix,Iy,Ci,Ox,Oy,Co,Kx,Ky,stride_x,stride_y;
		uLint_t size_out;
		char bias_en;
	} str_calc_para;
	#endif

	size_out_max = 0;
	for(idx=1; idx<n_layer_tot; idx++)
	{
		init_flg = nn_get_layer_init(idx);
		if (NULL == p_nn_layer[idx])
		{
			printf("In NN_model_init-%d, Not Enough memory, need debug\n",idx);
			return;
		}

		float *wei0;
		float *bia0;
		str_calc_para * p_calc_para0 = (str_calc_para*)malloc(sizeof(str_calc_para));
		if (NULL == p_calc_para0)
		{
			printf("In NN_model_init-%d, str_calc_para Not Enough memory, need debug\n",idx);
			
			nn_model_free();

			#if 0
			for(int fidx= 0; fidx<idx; fidx++)
			{
				free(p_nn_layer[fidx]);
				p_nn_layer[fidx] =NULL;
			}
			#endif
			
			return;
		}
		p_calc_para_buf[idx] = p_calc_para0;
		p_calc_para0->Ix = xor_in_shape_buf[idx-1][0];
		p_calc_para0->Iy = xor_in_shape_buf[idx-1][1];
		p_calc_para0->Ci = xor_in_shape_buf[idx-1][2];
		p_calc_para0->Ox = xor_ou_shape_buf[idx-1][0];
		p_calc_para0->Oy = xor_ou_shape_buf[idx-1][1];
		p_calc_para0->Co = xor_ou_shape_buf[idx-1][2];
		p_calc_para0->size_out = p_calc_para0->Ox *p_calc_para0->Oy * p_calc_para0->Co;
		p_calc_para0->Kx = xor_ker_shape_buf[idx-1][0];
		p_calc_para0->Ky = xor_ker_shape_buf[idx-1][1];
		p_calc_para0->stride_x = xor_stride_buf[idx-1][0];
		p_calc_para0->stride_y = xor_stride_buf[idx-1][1];
		p_calc_para0->bias_en = 1;
		p_nn_layer[idx]->threshold = xor_thed0[idx-1];

		if (size_out_max < p_calc_para0->size_out)
			size_out_max = p_calc_para0->size_out;
		
	}
	size_out_max_c = size_out_max;
	if (0 == NN_buffer_init())
		return;

	p_calc_para_buf[1]->p_weight = &xor_wei0_0[0][0];
	p_calc_para_buf[2]->p_weight = &xor_wei0_1[0][0];
	p_calc_para_buf[1]->bia0 = &xor_bia0_0[0];
	p_calc_para_buf[2]->bia0 = &xor_bia0_1[0];
	for(idx=1; idx<n_layer_tot; idx++)
	{
		p_nn_layer[idx]->NN_layer_init_id(idx);
		p_nn_layer[idx]->NN_layer_init_type(LAYER_FCN);
		p_nn_layer[idx]->NN_layer_init_calc_para(p_nn_layer[idx]);
	//	FREE_POINT(p_nn_layer[idx]);
	};

	nn_init_flag = 1;
	
}


