DATA_SECTION

  init_int nobs; //number of observations
  init_matrix data(1,nobs,1,2); //age-length array
  //init_matrix name(1,nobs,1,2)
  //matrices are row major

  vector ages(1,nobs);
  !!  ages = column(data,1); //use !! because we are doing an assignment in the DATA_SECTION
  vector lengths(1,nobs); 
  !!  lengths = column(data,2); //use !! because we are doing an assignment in the DATA_SECTION

  !!  cout << data << endl;
  
  // check to read all data
  init_int eof
  !!  if(eof != 999)  {cout << "Error Reading File. Application Aborted" << endl; exit(1);}
  
PARAMETER_SECTION
  init_number Linf; //can't initialize an integer, must be a number
  !!Linf = max(lengths); //initialize Linf, default is zero

  init_number k; //can't initialize an integer, must be a number
  !!k = 0.5; //initialize k, default is zero
  
  objective_function_value f;
		
  vector pred_lengths(1,nobs); //predicted lengths 
  vector epsilon(1,nobs); //epsilon 
  
PROCEDURE_SECTION
  pred_lengths = Linf*(1.0 - exp(-k*ages));
  epsilon = pred_lengths - lengths;
  cout << pred_lengths << endl;
  
  f = norm2(epsilon);
  
REPORT_SECTION
  report << "linf\n" << Linf << endl;
  REPORT(Linf);
  
GLOBALS_SECTION
  #undef REPORT
  #define REPORT(object) report << #object "\n" << object << endl;
