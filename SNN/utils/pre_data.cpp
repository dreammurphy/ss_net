#include "pre_data.h"

void func_pre_data_init(str_data_para *p_pre_data)
{
	printf("func_pre_data_init, start ...\n");

	// for XOR
	//func_pre_data_XOR_init(p_pre_data);

	
	func_pre_data_MNIST_init(p_pre_data);
	
	printf("func_pre_data_init, End \n");
}

void func_pre_data_XOR_init(str_data_para *p_pre_data)
{
	// for XOR
	p_pre_data->nf = 2;
	p_pre_data->nx = 1;
	p_pre_data->ny = 1;  
	p_pre_data->outf  = 1;
	p_pre_data->n_tot = 4; 
}

void func_pre_data_MNIST_init(str_data_para *p_pre_data)
{
	// for XOR
	p_pre_data->nf = 28*28;
	p_pre_data->nx = 1;
	p_pre_data->ny = 1;  
	p_pre_data->outf  = 10;
	p_pre_data->n_tot = 4; //10000; 
}


Simu_para_c::Simu_para_c()
{
	t_idx   = 0;
	t_simu = SIMU_TIME_LEN;
}

Simu_para_c::~Simu_para_c()
{
	fclose(fp_data);
}

int Simu_para_c::Simu_para_init(str_data_para *p_data_para)
{
	t_idx   = 0;
	t_simu = SIMU_TIME_LEN;
	
	printf("Simu_para_init, start ...\n");

	p_in_data_para = p_data_para;
	in_size = p_in_data_para->nx * p_in_data_para->ny * p_in_data_para->nf;
	n_tot = p_data_para->n_tot;

	char data_fi_name[] = "./data/mnist_unit8_tx.txt";
	fp_data = fopen(data_fi_name,"r");
	if(NULL == fp_data)
	{
		printf("Error in Simu_para_init, file %s could not open\n",data_fi_name);
		return 1;
	}
	
	printf("Simu_para_init, End ...\n");

	return 0;
}

// for XOR
float xor_buf_in[4][2] = {{1,0},{0,0},{1,1},{0,1}};
int   xor_ou[4] = {1,0,0,1};

// Get_test_data, get test_x,test_y
void Simu_para_c::Get_test_data(int idx, str_data_para *p_param, void *out_x, void *out_y)
{
	// for XOR
	float *in_x;
	int *ou_y;
	in_x = (float *)(out_x);
	ou_y = (int *)(out_y);
	in_x[0] = xor_buf_in[idx][0];
	in_x[1] = xor_buf_in[idx][1];
	ou_y[0] = xor_ou[idx];

#if ((1 == CASE_TEST) || (2 == CASE_TEST))
	fscanf(fp_data,"%d", &ou_y[0]);
	float amp_div;
	amp_div = 1.0/255;
	int tmp_i;
	for(uLint_t idx=0; idx<in_size; idx++)
	{
		fscanf(fp_data,"%d", &tmp_i);
		in_x[idx] = tmp_i * amp_div ;
	//	printf("idx,in,float: %ld, %d, %f\n",idx,tmp_i,in_x[idx]); // for debug
	}
#endif

}

// Get_train_data, get test_x,test_y
void Simu_para_c::Get_train_data(int idx, str_data_para *p_param, void *out_x, void *out_y)
{

}

