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
	nn_model_free();
	printf("In ~NN_model_c, free End\n");
}

void NN_model_c::nn_model_free(void)
{
	printf("In nn_model_free, free memory, flag=%d\n",nn_init_flag);

	for(int idx=0; idx<n_layer_tot; idx++)
	{
		if (NULL != p_nn_layer[idx])
		{
			printf("\nLayer:%d free . ",idx);
			p_nn_layer[idx]->nn_layer_free();
			FREE_POINT(p_nn_layer[idx]);
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
float xor_bia0_0[2] = {0.5,0};

#if 0
float xor_wei0_1[2][2] = {{-4.5,4.5},{6,-6}};
float xor_bia0_1[2] = {1,-1};
#else
float xor_wei0_1[1][2] = {{-4.5*2,6*2}};
float xor_bia0_1[2] = {2,0};
#endif
float xor_thed0[2] = {1*0.95,1*0.95};

#if (1 == CASE_TEST	)					// 0:XOR, 1:MNIST,FCN 2:MNIST CNN 3:MNIST CNN with Bias
float mnist_fcn_wei0_0[120][784];
float mnist_fcn_wei0_1[60][120];
float mnist_fcn_wei0_2[10][60];
float mnist_fcn_bia0_0[120];
float mnist_fcn_bia0_1[60];
float mnist_fcn_bia0_2[10];
#elif (2 == CASE_TEST	)
//float mnist_cnn_wei0_0[16][1*28*28];
//float mnist_cnn_wei0_1[16][16*12*12];
//float mnist_cnn_wei0_2[10][16*4*4];
float mnist_cnn_wei0_0[16][1*5*5];
float mnist_cnn_wei0_1[16][16*5*5];
float mnist_cnn_wei0_2[10][16*4*4];
float mnist_cnn_bia0_0[16];
float mnist_cnn_bia0_1[16];
float mnist_cnn_bia0_2[10];
#elif (3 == CASE_TEST	)
//float mnist_cnn_wei0_0[16][1*28*28];
//float mnist_cnn_wei0_1[32][16*12*12];
//float mnist_cnn_wei0_2[8][32*5*5];
float mnist_cnn_wei0_0[16][1*5*5];
float mnist_cnn_wei0_1[32][16*3*3];
float mnist_cnn_wei0_2[8][32*3*3];
float mnist_cnn_wei0_3[10][8*3*3];
float mnist_cnn_bia0_0[16];
float mnist_cnn_bia0_1[32];
float mnist_cnn_bia0_2[8];
float mnist_cnn_bia0_3[10];
#else

#endif



/* Here a xor example, need amend codes for others */
void NN_model_c::NN_model_init(Simu_para_c *p_simu_para0)
{
	printf("NN_model_init, start ...\n");

#if (0 == CASE_TEST)
	NN_model_init_xor(p_simu_para0);
#elif (1 == CASE_TEST)
	NN_model_init_fcn(p_simu_para0);
#elif ((2 == CASE_TEST)|| (3 == CASE_TEST))
	NN_model_init_cnn(p_simu_para0);
#else

#endif

	printf("NN_model_init, End\n");

}

int NN_model_c::NN_buffer_init(void)
{
	p_layer_out_buf_max = (float *)malloc(sizeof(float) * size_out_max_c);
	if (NULL == p_layer_out_buf_max)
	{
		printf("In NN_model_init_xor, memory is not enough for Layer data buffer, size if %ld, need debug \n",size_out_max_c);
		nn_model_free();
		return 1;
	}
	p_inX_for_calc = (char *)malloc(sizeof(char) * p_simu_para->in_size);
	if (NULL == p_inX_for_calc)
	{
		printf("In NN_model_init_xor, memory is not enough for First Input spike buffer, size if %ld, need debug \n",p_simu_para->in_size);
		nn_model_free();
		return 1;
	}

	return 0;
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
	p_nn_layer[idx]->NN_layer_init_base();
	if (idx == 0)
	{
		p_nn_layer[idx]->mem_ini_flag = 1;
	}

	return 1;
}


void NN_model_c::NN_model_reset(void)
{
	int idx;
	for(idx=1; idx<n_layer_tot; idx++)
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
	for(uLint_t idx=0; idx<p_simu_para0->in_size; idx++)
	{
		outX[idx] = p_in[idx];

		#if (1 == SPIKE_DEBUG) 
		// for debug
		printf("Spike to buffer, idx,va:%d,%d\n",idx,outX[idx]);
		#endif
	}


}

// here just copy ? 
void NN_model_c::NN_out_pro_one(void *outX)
{
	float *p_ou;
	p_ou = (float *)outX;
	//int *p_ou;
	//p_ou = (int *)outX;
	uLint_t size_out;
	char *spike_ou;
	// init sum
	size_out = p_calc_para_buf[n_layer_tot-1]->size_out;
	spike_ou = p_nn_layer[n_layer_tot-1]->p_spikes;
	for(uLint_t idx=0; idx<size_out; idx++)
	{
		p_ou[idx] = spike_ou[idx];
	}
}

void NN_model_c::Neuron_NN_pro(int tidx, void *inX, void *outX)
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

	NN_out_pro_one(outX); // outX as sum

	#if 0 // for debug
	char *pX;
	pX = (char *)inX;
	printf("In:%d,%d, out:%d, mem:%f\n",pX[0],pX[1],p_nn_layer[n_layer_tot-1]->p_spikes[0],p_nn_layer[n_layer_tot-1]->p_mem[0]);
	#endif
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
		printf("In NN_model_init_xor-0, Not Enough memory, need debug\n");
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

	//	float *wei0;
	//	float *bia0;
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
	if (0 != NN_buffer_init())
		return;

	p_calc_para_buf[1]->p_weight = &xor_wei0_0[0][0];
	p_calc_para_buf[2]->p_weight = &xor_wei0_1[0][0];
	p_calc_para_buf[1]->p_bias = &xor_bia0_0[0];
	p_calc_para_buf[2]->p_bias = &xor_bia0_1[0];
	for(idx=1; idx<n_layer_tot; idx++)
	{
		p_nn_layer[idx]->NN_layer_init_id(idx);
		p_nn_layer[idx]->NN_layer_init_type(LAYER_FCN);
		p_nn_layer[idx]->NN_layer_init_calc_para(p_calc_para_buf[idx]);
	//	FREE_POINT(p_nn_layer[idx]);
	};

	nn_init_flag = 1;

	// check layers, for debug
	printf("\nLayer 0 status, id=%d,type=%d,init_flg=%d\n",p_nn_layer[0]->layer_id,\
		p_nn_layer[0]->layer_type,p_nn_layer[0]->mem_ini_flag);
	for(idx=1; idx<n_layer_tot; idx++)
	{
		printf("Layer %d status, id=%d,type=%d,init_flg=%d,size=%ld\n",idx,p_nn_layer[idx]->layer_id,\
			p_nn_layer[idx]->layer_type,p_nn_layer[idx]->mem_ini_flag,p_nn_layer[idx]->p_calc_para->size_out);


	}
}


/* Here a fcn-for mnist example,784*120*60*10 need amend codes for others */
void NN_model_c::NN_model_init_fcn(Simu_para_c *p_simu_para0)
{
	p_simu_para = p_simu_para0;

	#if (1 == CASE_TEST)
	char init_flg;
	int idx;
	uLint_t size_out_max;
	p_in_spike = p_simu_para->p_simu_spike_init;
	int size0[4] = {784,120,60,10};
	char fn_str_0[] = "./data/ann.fcn1.weight.txt";
	char fn_str_1[] = "./data/ann.fcn2.weight.txt";
	char fn_str_2[] = "./data/ann.fcn3.weight.txt";
	int bias_en0;
	bias_en0 = 0; // this MNIST, bias set to zeros, disable
	
	// MNIST Model, FCN, 784-120-60-10
	n_layer_tot = 4;
	init_flg = nn_get_layer_init(0);
	if (0 == init_flg)
	{
		printf("In NN_model_init_fcn-0, Not Enough memory, need debug\n");
		nn_model_free();
		return;
	}
	if (0!= func_mnist_wei_init(fn_str_0, mnist_fcn_wei0_0[0], mnist_fcn_bia0_0, size0[0], size0[1], bias_en0))
	{
		printf("Error In NN_model_init_fcn-1, File could not open, need debug\n");
		nn_model_free();
		return;
	}
	if (0!= func_mnist_wei_init(fn_str_1, mnist_fcn_wei0_1[0], mnist_fcn_bia0_1, size0[1], size0[2], bias_en0))
	{
		printf("Error In NN_model_init_fcn-2, File could not open, need debug\n");
		nn_model_free();
		return;
	}
	if (0!= func_mnist_wei_init(fn_str_2, mnist_fcn_wei0_2[0], mnist_fcn_bia0_2, size0[2], size0[3], bias_en0))
	{
		printf("Error In NN_model_init_fcn-3, File could not open, need debug\n");
		nn_model_free();
		return;
	}
	// for debug
	FILE * fp_wei0;
	fp_wei0 = fopen("./data/de0.txt","w");
	for(idx=0; idx<120; idx++)
	{
		fprintf(fp_wei0,"%d",idx);
		for(int sidx=0; sidx<784; sidx++)
		{
			fprintf(fp_wei0,",%f",mnist_fcn_wei0_0[idx][sidx]);
		}
		fprintf(fp_wei0,"\n");
	}
	fclose(fp_wei0);

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

	//	float *wei0;
	//	float *bia0;
		str_calc_para * p_calc_para0 = (str_calc_para*)malloc(sizeof(str_calc_para));
		if (NULL == p_calc_para0)
		{
			printf("In NN_model_init-%d, str_calc_para Not Enough memory, need debug\n",idx);
			nn_model_free();
			return;
		}
		p_calc_para_buf[idx] = p_calc_para0;
		p_calc_para0->Ix = 1;
		p_calc_para0->Iy = 1;
		p_calc_para0->Ci = size0[idx-1];
		p_calc_para0->Ox = 1;
		p_calc_para0->Oy = 1;
		p_calc_para0->Co = size0[idx];
		p_calc_para0->size_out = p_calc_para0->Ox *p_calc_para0->Oy * p_calc_para0->Co;
		p_calc_para0->Kx = 1;
		p_calc_para0->Ky = 1;
		p_calc_para0->stride_x = 1;
		p_calc_para0->stride_y = 1;
		p_calc_para0->bias_en = bias_en0;  // this MNIST, bias set to zeros, disable
		p_nn_layer[idx]->threshold = 1;

		if (size_out_max < p_calc_para0->size_out)
			size_out_max = p_calc_para0->size_out;
		
	}
	size_out_max_c = size_out_max;
	if (0 != NN_buffer_init())
		return;

	p_calc_para_buf[1]->p_weight = &mnist_fcn_wei0_0[0][0];
	p_calc_para_buf[2]->p_weight = &mnist_fcn_wei0_1[0][0];
	p_calc_para_buf[3]->p_weight = &mnist_fcn_wei0_2[0][0];
	p_calc_para_buf[1]->p_bias = &mnist_fcn_bia0_0[0];
	p_calc_para_buf[2]->p_bias = &mnist_fcn_bia0_1[0];
	p_calc_para_buf[3]->p_bias = &mnist_fcn_bia0_2[0];
	for(idx=1; idx<n_layer_tot; idx++)
	{
		p_nn_layer[idx]->NN_layer_init_id(idx);
		p_nn_layer[idx]->NN_layer_init_type(LAYER_FCN);
		p_nn_layer[idx]->NN_layer_init_calc_para(p_calc_para_buf[idx]);
	//	FREE_POINT(p_nn_layer[idx])
	}

	nn_init_flag = 1;

	// check layers, for debug
	printf("\nLayer 0 status, id=%d,type=%d,init_flg=%d\n",p_nn_layer[0]->layer_id,\
		p_nn_layer[0]->layer_type,p_nn_layer[0]->mem_ini_flag);
	for(idx=1; idx<n_layer_tot; idx++)
	{
		printf("Layer %d status, id=%d,type=%d,init_flg=%d,size=%ld\n",idx,p_nn_layer[idx]->layer_id,\
			p_nn_layer[idx]->layer_type,p_nn_layer[idx]->mem_ini_flag,p_nn_layer[idx]->p_calc_para->size_out);
	}

	#else

	#endif
}

/* Here a cnn-for mnist example,784*16*16*10 need amend codes for others */
void NN_model_c::NN_model_init_cnn(Simu_para_c *p_simu_para0)
{
	p_simu_para = p_simu_para0;

#if (2 == CASE_TEST)
	
	char init_flg;
	int idx;
	uLint_t size_out_max;
	p_in_spike = p_simu_para->p_simu_spike_init;
	int size0[4] = {28,12,4,10};
	int ker_0[4] = {5,5,1,1};
	int ci_v0[4] = {1,16,16,10};
	int co_v0[4] = {16,16,10,1};
	int str_v0[]= {1,2,1,2,1};
	LayerType lay_type_v[6] = {LAYER_CNN, LAYER_POOLING_AVE, LAYER_CNN, LAYER_POOLING_AVE, LAYER_FCN};
	char fn_str_0[] = "./data/2/n_conv1.weight.txt";
	char fn_str_1[] = "./data/2/n_conv2.weight.txt";
	char fn_str_2[] = "./data/2/n_fc1.weight.txt";
	int bias_en0;
	bias_en0 = 0; // this MNIST, bias set to zeros, disable

	
	printf("NN_model_init_cnn - 0, start ...\n");
	// MNIST Model, CNN, 784X1-12*12X16-4*4*16-10
	n_layer_tot = 6;
	init_flg = nn_get_layer_init(0);
	printf("NN_model_init_cnn - 01, start ...\n");
	if (0 == init_flg)
	{
		printf("In NN_model_init_cnn-0, Not Enough memory, need debug\n");
		nn_model_free();
		return;
	}
	printf("NN_model_init_cnn - 02, start ...\n");
	if (0!= func_mnist_cnn_wei_init(fn_str_0,mnist_cnn_wei0_0[0],mnist_cnn_bia0_0,ci_v0[0],co_v0[0],ker_0[0]*ker_0[0],bias_en0))
	{
		printf("Error In NN_model_init_cnn-1, File could not open, need debug\n");
		nn_model_free();
		return;
	}
	printf("NN_model_init_cnn - 03, start ...\n");
	if (0!= func_mnist_cnn_wei_init(fn_str_1,mnist_cnn_wei0_1[0],mnist_cnn_bia0_1,ci_v0[1],co_v0[1],ker_0[1]*ker_0[1],bias_en0))
	{
		printf("Error In NN_model_init_cnn-2, File could not open, need debug\n");
		nn_model_free();
		return;
	}
	printf("NN_model_init_cnn - 04, start ...\n");
//	if (0!= func_mnist_cnn_wei_init(fn_str_2,mnist_cnn_wei0_2[0],mnist_cnn_bia0_2,ci_v0[2],co_v0[2],ker_0[2]*ker_0[2],0))
	if (0!= func_mnist_wei_init(fn_str_2,mnist_cnn_wei0_2[0],mnist_cnn_bia0_2,ci_v0[2]*size0[2]*size0[2],co_v0[2],bias_en0))
	{
		printf("Error In NN_model_init_cnn-3, File could not open, need debug\n");
		nn_model_free();
		return;
	}
	
	printf("NN_model_init_cnn - 05, start ...\n");

	// for debug
	FILE * fp_wei0;
	fp_wei0 = fopen("./data/de_cnn0.txt","w");
	for(idx=0; idx<16; idx++)
	{
		fprintf(fp_wei0,"%d",idx);
		for(int sidx=0; sidx<25; sidx++)
		{
			fprintf(fp_wei0,",%f",mnist_cnn_wei0_0[idx][sidx]);
		}
		fprintf(fp_wei0,"\n");
	}
	fclose(fp_wei0);

	#if 0
	{

		float		*p_weight;
		float		*p_bias;
		int Ix,Iy,Ci,Ox,Oy,Co,Kx,Ky,stride_x,stride_y;
		uLint_t size_out;
		char bias_en;
	} str_calc_para;
	#endif

	printf("NN_model_init_cnn - 1, start ...\n");

	for(idx=1; idx<n_layer_tot; idx++)
	{
		init_flg = nn_get_layer_init(idx);
		if (NULL == p_nn_layer[idx])
		{
			printf("In NN_model_init-%d, Not Enough memory, need debug\n",idx);
			return;
		}

	//	float *wei0;
	//	float *bia0;
		str_calc_para * p_calc_para0 = (str_calc_para*)malloc(sizeof(str_calc_para));
		if (NULL == p_calc_para0)
		{
			printf("In NN_model_init-%d, str_calc_para Not Enough memory, need debug\n",idx);
			nn_model_free();
			return;
		}
		p_calc_para_buf[idx] = p_calc_para0;
		
		p_nn_layer[idx]->NN_layer_init_id(idx);
		p_nn_layer[idx]->NN_layer_init_type(lay_type_v[idx-1]);
		
	}

	printf("NN_model_init_cnn - 2, start ...\n");
	
	size_out_max = 0;
	int lay_ccn_idx;
	lay_ccn_idx = 0;
	for(idx=1; idx<n_layer_tot; idx++)
	{
		str_calc_para * p_calc_para0;
		p_calc_para0 = p_calc_para_buf[idx];
		if (p_nn_layer[idx]->layer_type == LAYER_CNN)
		{
			p_calc_para0->Ix = size0[lay_ccn_idx];
			p_calc_para0->Iy = size0[lay_ccn_idx];
			p_calc_para0->Ci = ci_v0[lay_ccn_idx];
			p_calc_para0->Co = co_v0[lay_ccn_idx];
			p_calc_para0->Kx = ker_0[lay_ccn_idx];
			p_calc_para0->Ky = ker_0[lay_ccn_idx];
			p_calc_para0->stride_x = 1; // str_v0[idx];
			p_calc_para0->stride_y = 1; // str_v0[idx];
			p_calc_para0->Ox = p_calc_para0->Ix-p_calc_para0->Kx+1; // ker_x:5
			p_calc_para0->Oy = p_calc_para0->Iy-p_calc_para0->Ky+1; // ker_y:5
			p_calc_para0->size_out = p_calc_para0->Ox *p_calc_para0->Oy * p_calc_para0->Co;

			p_calc_para0->bias_en = bias_en0;  // this MNIST, bias set to zeros, disable
			p_nn_layer[idx]->threshold = 1;

			lay_ccn_idx++;
		}
		else if (p_nn_layer[idx]->layer_type == LAYER_FCN)
		{
			p_calc_para0->Ix = 1;
			p_calc_para0->Iy = 1;
			p_calc_para0->Ci = p_calc_para_buf[idx-1]->size_out;
			p_calc_para0->Co = co_v0[lay_ccn_idx];
			p_calc_para0->Kx = ker_0[lay_ccn_idx]; // should be 1
			p_calc_para0->Ky = ker_0[lay_ccn_idx]; // should be 1
			p_calc_para0->stride_x = 1; // str_v0[idx];
			p_calc_para0->stride_y = 1; // str_v0[idx];
			p_calc_para0->Ox = 1; // ker_x:5
			p_calc_para0->Oy = 1; // ker_y:5
			p_calc_para0->size_out = p_calc_para0->Ox *p_calc_para0->Oy * p_calc_para0->Co;

			p_calc_para0->bias_en = bias_en0;  // this MNIST, bias set to zeros, disable
			p_nn_layer[idx]->threshold = 1;
			lay_ccn_idx++; // fcn , cnn idx 
		}
		else if (p_nn_layer[idx]->layer_type == LAYER_POOLING_AVE)
		{
			p_calc_para0->Ix = p_calc_para_buf[idx-1]->Ox;
			p_calc_para0->Iy = p_calc_para_buf[idx-1]->Oy;
			p_calc_para0->Ci = p_calc_para_buf[idx-1]->Co;
			p_calc_para0->Co = p_calc_para_buf[idx-1]->Co;
			p_calc_para0->stride_x = str_v0[idx-1];
			p_calc_para0->stride_y = str_v0[idx-1];

			p_calc_para0->Kx = p_calc_para0->stride_x;
			p_calc_para0->Ky = p_calc_para0->stride_y;
			p_calc_para0->Ox = size0[lay_ccn_idx]; // (Ix+1-stride_x)/stride_x
			p_calc_para0->Oy = size0[lay_ccn_idx]; // (Iy+1-stride_y)/stride_y
			p_calc_para0->size_out = p_calc_para0->Ox *p_calc_para0->Oy * p_calc_para0->Co;

			p_calc_para0->bias_en = bias_en0;  // this MNIST, bias set to zeros, disable
			p_nn_layer[idx]->threshold = p_calc_para0->Kx*p_calc_para0->Ky*0.5-0.25; // for avoid 0.501,0.999?
		}
		if (size_out_max < p_calc_para0->size_out)
			size_out_max = p_calc_para0->size_out;
		
	}
	size_out_max_c = size_out_max;
	if (0 != NN_buffer_init())
		return;
		
	printf("NN_model_init_cnn - 3, start ...\n");

	p_calc_para_buf[1]->p_weight = &mnist_cnn_wei0_0[0][0];
	p_calc_para_buf[3]->p_weight = &mnist_cnn_wei0_1[0][0];
	p_calc_para_buf[5]->p_weight = &mnist_cnn_wei0_2[0][0];
	p_calc_para_buf[1]->p_bias = &mnist_cnn_bia0_0[0];
	p_calc_para_buf[3]->p_bias = &mnist_cnn_bia0_1[0];
	p_calc_para_buf[5]->p_bias = &mnist_cnn_bia0_2[0];
	for(idx=1; idx<n_layer_tot; idx++)
	{
		p_nn_layer[idx]->NN_layer_init_calc_para(p_calc_para_buf[idx]);
	//	FREE_POINT(p_nn_layer[idx])
	}

	nn_init_flag = 1;

	printf("NN_model_init_cnn - 4, start ...\n");

	// check layers, for debug
	printf("\nLayer 0 status, id=%d,type=%d,init_flg=%d\n",p_nn_layer[0]->layer_id,\
		p_nn_layer[0]->layer_type,p_nn_layer[0]->mem_ini_flag);
	for(idx=1; idx<n_layer_tot; idx++)
	{
		printf("Layer %d status, id=%d,type=%d,init_flg=%d,size=%ld\n",idx,p_nn_layer[idx]->layer_id,\
			p_nn_layer[idx]->layer_type,p_nn_layer[idx]->mem_ini_flag,p_nn_layer[idx]->p_calc_para->size_out);


	}

#elif (3 == CASE_TEST)

	char init_flg;
	int idx;
	uLint_t size_out_max;
	p_in_spike = p_simu_para->p_simu_spike_init;
	int size0[] = {28,12,5,3,10};
	int ker_0[] = {5,3,3,1,1};
	int ci_v0[] = {1,16,32,8,10};
	int co_v0[] = {16,32,8,10,1};
	int str_v0[]= {2,1,2,1,1,1};
	LayerType lay_type_v[] = {LAYER_CNN, LAYER_CNN, LAYER_POOLING_AVE, LAYER_CNN, LAYER_FCN};
	char fn_str_0[] = "./data/3/n_conv1.weight_b.txt";
	char fn_str_1[] = "./data/3/n_conv2.weight_b.txt";
	char fn_str_2[] = "./data/3/n_conv3.weight_b.txt";
	char fn_str_3[] = "./data/3/n_fc1.weight_b.txt";
	int bias_en0;
	bias_en0 = 1; // this MNIST, bias enable

	
	printf("NN_model_init_cnn - 0, start ...\n");
	// MNIST Model, CNN, 784X1-12*12X16-4*4*16-10
	n_layer_tot = 6;
	init_flg = nn_get_layer_init(0);
	printf("NN_model_init_cnn - 11, start ...\n");
	if (0 == init_flg)
	{
		printf("In NN_model_init_cnn-11, Not Enough memory, need debug\n");
		nn_model_free();
		return;
	}
	printf("NN_model_init_cnn - 12, start ...\n");
	if (0!= func_mnist_cnn_wei_init(fn_str_0,mnist_cnn_wei0_0[0],mnist_cnn_bia0_0,ci_v0[0],co_v0[0],ker_0[0]*ker_0[0],bias_en0))
	{
		printf("Error In NN_model_init_cnn-12, File could not open, need debug\n");
		nn_model_free();
		return;
	}
	printf("NN_model_init_cnn - 13, start ...\n");
	if (0!= func_mnist_cnn_wei_init(fn_str_1,mnist_cnn_wei0_1[0],mnist_cnn_bia0_1,ci_v0[1],co_v0[1],ker_0[1]*ker_0[1],bias_en0))
	{
		printf("Error In NN_model_init_cnn-13, File could not open, need debug\n");
		nn_model_free();
		return;
	}
	printf("NN_model_init_cnn - 14, start ...\n");
	if (0!= func_mnist_cnn_wei_init(fn_str_2,mnist_cnn_wei0_2[0],mnist_cnn_bia0_2,ci_v0[2],co_v0[2],ker_0[2]*ker_0[2],bias_en0))
	{
		printf("Error In NN_model_init_cnn-14, File could not open, need debug\n");
		nn_model_free();
		return;
	}
	printf("NN_model_init_cnn - 15, start ...\n");
//	if (0!= func_mnist_cnn_wei_init(fn_str_2,mnist_cnn_wei0_2[0],mnist_cnn_bia0_2,ci_v0[2],co_v0[2],ker_0[2]*ker_0[2],0))
	if (0!= func_mnist_wei_init(fn_str_3,mnist_cnn_wei0_3[0],mnist_cnn_bia0_3,ci_v0[3]*size0[3]*size0[3],co_v0[3],bias_en0))
	{
		printf("Error In NN_model_init_cnn-15, File could not open, need debug\n");
		nn_model_free();
		return;
	}
	
	printf("NN_model_init_cnn - 16, start ...\n");

	// for debug
	FILE * fp_wei0;
	fp_wei0 = fopen("./data/de_cnn_b0.txt","w");
	for(idx=0; idx<16; idx++)
	{
		fprintf(fp_wei0,"%d",idx);
		for(int sidx=0; sidx<25; sidx++)
		{
			fprintf(fp_wei0,",%f",mnist_cnn_wei0_0[idx][sidx]);
		}
		fprintf(fp_wei0,"\n");
	}
	fclose(fp_wei0);

	#if 0
	{

		float		*p_weight;
		float		*p_bias;
		int Ix,Iy,Ci,Ox,Oy,Co,Kx,Ky,stride_x,stride_y;
		uLint_t size_out;
		char bias_en;
	} str_calc_para;
	#endif

	printf("NN_model_init_cnn - 1, start ...\n");

	for(idx=1; idx<n_layer_tot; idx++)
	{
		init_flg = nn_get_layer_init(idx);
		if (NULL == p_nn_layer[idx])
		{
			printf("In NN_model_init-%d, Not Enough memory, need debug\n",idx);
			return;
		}

	//	float *wei0;
	//	float *bia0;
		str_calc_para * p_calc_para0 = (str_calc_para*)malloc(sizeof(str_calc_para));
		if (NULL == p_calc_para0)
		{
			printf("In NN_model_init-%d, str_calc_para Not Enough memory, need debug\n",idx);
			nn_model_free();
			return;
		}
		p_calc_para_buf[idx] = p_calc_para0;
		
		p_nn_layer[idx]->NN_layer_init_id(idx);
		p_nn_layer[idx]->NN_layer_init_type(lay_type_v[idx-1]);
		
	}

	printf("NN_model_init_cnn - 2, start ...\n");
	
	size_out_max = 0;
	int lay_ccn_idx;
	lay_ccn_idx = 0;
	for(idx=1; idx<n_layer_tot; idx++)
	{
		str_calc_para * p_calc_para0;
		p_calc_para0 = p_calc_para_buf[idx];
		if (p_nn_layer[idx]->layer_type == LAYER_CNN)
		{
			p_calc_para0->Ix = size0[lay_ccn_idx];
			p_calc_para0->Iy = size0[lay_ccn_idx];
			p_calc_para0->Ci = ci_v0[lay_ccn_idx];
			p_calc_para0->Co = co_v0[lay_ccn_idx];
			p_calc_para0->Kx = ker_0[lay_ccn_idx];
			p_calc_para0->Ky = ker_0[lay_ccn_idx];
			p_calc_para0->stride_x = str_v0[idx-1];
			p_calc_para0->stride_y = str_v0[idx-1];
			p_calc_para0->Ox = (p_calc_para0->Ix-p_calc_para0->Kx+1+p_calc_para0->stride_x-1)/p_calc_para0->stride_x; // ker_x:5
			p_calc_para0->Oy = (p_calc_para0->Iy-p_calc_para0->Ky+1+p_calc_para0->stride_y-1)/p_calc_para0->stride_y; // ker_y:5
			p_calc_para0->size_out = p_calc_para0->Ox *p_calc_para0->Oy * p_calc_para0->Co;

			p_calc_para0->bias_en = bias_en0;  // this MNIST, bias set to zeros, disable
			p_nn_layer[idx]->threshold = 1;

			lay_ccn_idx++;
		}
		else if (p_nn_layer[idx]->layer_type == LAYER_FCN)
		{
			p_calc_para0->Ix = 1;
			p_calc_para0->Iy = 1;
			p_calc_para0->Ci = p_calc_para_buf[idx-1]->size_out;
			p_calc_para0->Co = co_v0[lay_ccn_idx];
			p_calc_para0->Kx = ker_0[lay_ccn_idx]; // should be 1
			p_calc_para0->Ky = ker_0[lay_ccn_idx]; // should be 1
			p_calc_para0->stride_x = str_v0[idx-1];
			p_calc_para0->stride_y = str_v0[idx-1];
			p_calc_para0->Ox = 1; // ker_x:5
			p_calc_para0->Oy = 1; // ker_y:5
			p_calc_para0->size_out = p_calc_para0->Ox *p_calc_para0->Oy * p_calc_para0->Co;

			p_calc_para0->bias_en = bias_en0;  // this MNIST, bias set to zeros, disable
			p_nn_layer[idx]->threshold = 1;
			lay_ccn_idx++; // fcn , cnn idx 
		}
		else if (p_nn_layer[idx]->layer_type == LAYER_POOLING_AVE)
		{
			p_calc_para0->Ix = p_calc_para_buf[idx-1]->Ox;
			p_calc_para0->Iy = p_calc_para_buf[idx-1]->Oy;
			p_calc_para0->Ci = p_calc_para_buf[idx-1]->Co;
			p_calc_para0->Co = p_calc_para_buf[idx-1]->Co;
			p_calc_para0->stride_x = str_v0[idx-1];
			p_calc_para0->stride_y = str_v0[idx-1];

			p_calc_para0->Kx = p_calc_para0->stride_x;
			p_calc_para0->Ky = p_calc_para0->stride_y;
			p_calc_para0->Ox = size0[lay_ccn_idx]; // (Ix+1-stride_x)/stride_x
			p_calc_para0->Oy = size0[lay_ccn_idx]; // (Iy+1-stride_y)/stride_y
			p_calc_para0->size_out = p_calc_para0->Ox *p_calc_para0->Oy * p_calc_para0->Co;

			p_calc_para0->bias_en = bias_en0;  // this MNIST, bias set to zeros, disable
			p_nn_layer[idx]->threshold = p_calc_para0->Kx*p_calc_para0->Ky*0.5-0.25; // for avoid 0.501,0.999?
		}
		if (size_out_max < p_calc_para0->size_out)
			size_out_max = p_calc_para0->size_out;
		
	}
	size_out_max_c = size_out_max;
	if (0 != NN_buffer_init())
		return;
		
	printf("NN_model_init_cnn - 3, start ...\n");

	p_calc_para_buf[1]->p_weight = &mnist_cnn_wei0_0[0][0];
	p_calc_para_buf[2]->p_weight = &mnist_cnn_wei0_1[0][0];
	p_calc_para_buf[4]->p_weight = &mnist_cnn_wei0_2[0][0];
	p_calc_para_buf[5]->p_weight = &mnist_cnn_wei0_3[0][0];
	p_calc_para_buf[1]->p_bias = &mnist_cnn_bia0_0[0];
	p_calc_para_buf[2]->p_bias = &mnist_cnn_bia0_1[0];
	p_calc_para_buf[4]->p_bias = &mnist_cnn_bia0_2[0];
	p_calc_para_buf[5]->p_bias = &mnist_cnn_bia0_3[0];
	for(idx=1; idx<n_layer_tot; idx++)
	{
		p_nn_layer[idx]->NN_layer_init_calc_para(p_calc_para_buf[idx]);
	//	FREE_POINT(p_nn_layer[idx])
	}

	nn_init_flag = 1;

	printf("NN_model_init_cnn - 4, start ...\n");

	// check layers, for debug
	printf("\nLayer 0 status, id=%d,type=%d,init_flg=%d\n",p_nn_layer[0]->layer_id,\
		p_nn_layer[0]->layer_type,p_nn_layer[0]->mem_ini_flag);
	for(idx=1; idx<n_layer_tot; idx++)
	{
		printf("Layer %d status, id=%d,type=%d,init_flg=%d,size=%ld\n",idx,p_nn_layer[idx]->layer_id,\
			p_nn_layer[idx]->layer_type,p_nn_layer[idx]->mem_ini_flag,p_nn_layer[idx]->p_calc_para->size_out);


	}
	

#else


#endif
	
}


int func_mnist_wei_init(char *fn, float *p_wei_out, float *p_bia_out,int ci, int co, int bias_en)
{
	int idx;
	float tmp0;
	FILE *fp_wei;
	if (NULL == (fp_wei=fopen(fn,"r")))
	{
		printf("Error In func_mnist_wei_init, File:%s could not open, need debug\n",fn);
		return 1;
	}
	else
	{
		float * p_wei;
		for(idx=0; idx<co; idx++)
		{
			p_wei = p_wei_out;
			fscanf(fp_wei,"%f",&tmp0);
			fscanf(fp_wei,"%f",&tmp0);
			for(int cidx=0; cidx < ci; cidx++)
			{
				fscanf(fp_wei,"%f",p_wei);

				#if 1
				if ((idx < 2) && (cidx < 25)) // for debug
				{
					printf("[co,ci,value]=[%d,%d,%f]\n",idx,cidx,*p_wei);
				}
				#endif
				
				p_wei++;
			}
			p_wei_out += ci;
		}
	}
	if (bias_en != 0)
	{
		// process bias
	}
	
	fclose(fp_wei);
	return 0;
}

int func_mnist_cnn_wei_init(char *fn, float *p_wei_out, float *p_bia_out,int ci, int co, int ko,int bias_en)
{
	int idx;
	int size0;
	float tmp0;
	FILE *fp_wei;
	if (NULL == (fp_wei=fopen(fn,"r")))
	{
		printf("Error In func_mnist_wei_init, File:%s could not open, need debug\n",fn);
		return 1;
	}
	else
	{
		float *p_wxy, *p_wei;
		size0 = ci * ko;
		for(idx=0; idx<co; idx++)
		{
			p_wxy = p_wei_out;
			p_wei_out += size0;
			for(int cidx=0; cidx < ci; cidx++)
			{
				fscanf(fp_wei,"%f",&tmp0);
				fscanf(fp_wei,"%f",&tmp0);

				p_wei = p_wxy;
				p_wxy += ko;

				for(int xyidx=0;xyidx<ko; xyidx++)
				{
					fscanf(fp_wei,"%f",p_wei);

				#if 1
					if ((idx < 2) && (cidx < 25)) // for debug
					{
						printf("[co,ci,value]=[%d,%d,%f]\n",idx,cidx,*p_wei);
					}
				#endif
				
					p_wei++;
				}
				
			}
		}
		if (bias_en != 0)
		{
			// process bias
			p_wei = p_bia_out;
			fscanf(fp_wei,"%f",&tmp0);
			fscanf(fp_wei,"%f",&tmp0);
			for(idx=0; idx<co; idx++)
			{
				fscanf(fp_wei,"%f",&p_wei[idx]);
			}
		}
	
		fclose(fp_wei);

	}
	
	return 0;

}


