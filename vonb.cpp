#include <admodel.h>
#include <contrib.h>

  extern "C"  {
    void ad_boundf(int i);
  }
#include <vonb.htp>

model_data::model_data(int argc,char * argv[]) : ad_comm(argc,argv)
{
  nobs.allocate("nobs");
  data.allocate(1,nobs,1,2,"data");
  ages.allocate(1,nobs);
  ages = column(data,1); //use !! because we are doing an assignment in the DATA_SECTION
  lengths.allocate(1,nobs);
  lengths = column(data,2); //use !! because we are doing an assignment in the DATA_SECTION
  cout << data << endl;
  eof.allocate("eof");
  if(eof != 999)  {cout << "Error Reading File. Application Aborted" << endl; exit(1);}
}

model_parameters::model_parameters(int sz,int argc,char * argv[]) : 
 model_data(argc,argv) , function_minimizer(sz)
{
  initializationfunction();
  Linf.allocate("Linf");
Linf = max(lengths); //initialize Linf, default is zero
  k.allocate("k");
k = 0.5; //initialize k, default is zero
  f.allocate("f");
  prior_function_value.allocate("prior_function_value");
  likelihood_function_value.allocate("likelihood_function_value");
  pred_lengths.allocate(1,nobs,"pred_lengths");
  #ifndef NO_AD_INITIALIZE
    pred_lengths.initialize();
  #endif
  epsilon.allocate(1,nobs,"epsilon");
  #ifndef NO_AD_INITIALIZE
    epsilon.initialize();
  #endif
}

void model_parameters::userfunction(void)
{
  f =0.0;
  pred_lengths = Linf*(1.0 - exp(-k*ages));
  epsilon = pred_lengths - lengths;
  cout << pred_lengths << endl;
  f = norm2(epsilon);
}

void model_parameters::report(const dvector& gradients)
{
 adstring ad_tmp=initial_params::get_reportfile_name();
  ofstream report((char*)(adprogram_name + ad_tmp));
  if (!report)
  {
    cerr << "error trying to open report file"  << adprogram_name << ".rep";
    return;
  }
}

void model_parameters::preliminary_calculations(void){
#if defined(USE_ADPVM)

  admaster_slave_variable_interface(*this);

#endif
}

model_data::~model_data()
{}

model_parameters::~model_parameters()
{}

void model_parameters::final_calcs(void){}

void model_parameters::set_runtime(void){}

#ifdef _BORLANDC_
  extern unsigned _stklen=10000U;
#endif


#ifdef __ZTC__
  extern unsigned int _stack=10000U;
#endif

  long int arrmblsize=0;

int main(int argc,char * argv[])
{
    ad_set_new_handler();
  ad_exit=&ad_boundf;
    gradient_structure::set_NO_DERIVATIVES();
    gradient_structure::set_YES_SAVE_VARIABLES_VALUES();
    if (!arrmblsize) arrmblsize=15000000;
    model_parameters mp(arrmblsize,argc,argv);
    mp.iprint=10;
    mp.preliminary_calculations();
    mp.computations(argc,argv);
    return 0;
}

extern "C"  {
  void ad_boundf(int i)
  {
    /* so we can stop here */
    exit(i);
  }
}
