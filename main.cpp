/*
Sistema de 2 dimensiones de un ferroelectrico relaxor

TO DO
Unidades del sistema
umbtener valor de mu según datos de l a PNR
J intercambio debe contener valores del mu
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <gsl/gsl_rng.h>
#include "sistema.h"
#include "impresor.h"

using namespace std;

int main(int argc, char **argv) {
  //iniciar sistema
  time_t start, end;
  time(&start);
  
//  vaciar archivo de datos en cada ejecución
  system("rm *.dat");
  
  gsl_rng * rng = gsl_rng_alloc (gsl_rng_taus);
  
  unsigned int L=16, numexps = 10, Equi_iter=300, Exp_iter= 3000;
  double T=16,dT = 0.2, DeltaJ = 1;
  
  gsl_rng_set(rng, time(NULL) );
  
  clock_t cl_start = clock();
  Sistema relaxor(L, rng, DeltaJ);
  vector<double> temperaturas, campos, tau;
  ostringstream frec, fieldamp, temp;
  string id_proc;
  temperaturas = step2vec(DeltaJ, T, dT, false);
  clock_t cl_stop = clock();
  cout<<"Iniciar sistema "<<cl_stop-cl_start<<"\n";
  // weak field
  cl_start = clock();
  campos = str2vec(DeltaJ, "0.1");
  tau = str2vec(1,"100 50 20 10");
  for(unsigned int t=0;t<tau.size();t++){
    frec.str(""); fieldamp.str("");
    frec<<tau[t]; fieldamp<<campos[0];
    id_proc="cool_E"+fieldamp.str()+"_t"+frec.str();
    for(unsigned int n=0;n<numexps;n++){
      relaxor.init(rng,DeltaJ,false);
      for(unsigned int T=0; T<temperaturas.size(); T++){
	relaxor.experimento(temperaturas[T],campos[0],tau[t], Equi_iter,false,rng, id_proc);
	relaxor.experimento(temperaturas[T],campos[0],tau[t], Exp_iter,true,rng, id_proc);
      }
    }
    eval_pol(Exp_iter,numexps, DeltaJ, temperaturas, id_proc );
    vector<double> intfield (1,campos[0]);
    calc_sus(numexps,tau[t],Exp_iter,DeltaJ, temperaturas,intfield,id_proc);
  }
  
  //strong fields
  campos = str2vec(DeltaJ, "0.5 1 1.5 2");
  tau = str2vec(1,"50 10");
  for(unsigned int E=0;E<campos.size();E++){
    for(unsigned int t=0;t<tau.size();t++){
      frec.str(""); fieldamp.str("");
      frec<<tau[t]; fieldamp<<campos[E];
      id_proc="cool_E"+fieldamp.str()+"_t"+frec.str();
      for(unsigned int n=0;n<numexps;n++){
	relaxor.init(rng,DeltaJ,false);
	for(unsigned int T=0; T<temperaturas.size(); T++){
	  relaxor.experimento(temperaturas[T],campos[E],tau[t], Equi_iter,false,rng, id_proc);
	  relaxor.experimento(temperaturas[T],campos[E],tau[t], Exp_iter,true,rng, id_proc);
	}
      }
      eval_pol(Exp_iter,numexps, DeltaJ, temperaturas, id_proc );
      vector<double> intfield (1,campos[E]);
      calc_sus(numexps,tau[t],Exp_iter,DeltaJ, temperaturas,intfield,id_proc);
    }
  }
  //Multifield temperature steps
  cl_start = clock();
  temperaturas = step2vec(DeltaJ, 16, 1, true);
  campos = step2vec(DeltaJ, 16, 0.2, true);
  tau = str2vec(1,"10");
  for(unsigned int T=0;T<temperaturas.size(); T++){
    temp.str("");
    temp<<temperaturas[T];
    id_proc="fixT"+temp.str()+"_riseE_t10";
    for(unsigned int n=0;n<numexps;n++){
      relaxor.init(rng,DeltaJ,false);
      for(unsigned int E=0;E<campos.size();E++){
	relaxor.experimento(temperaturas[T],campos[E],tau[0], Equi_iter,false,rng, id_proc);
	relaxor.experimento(temperaturas[T],campos[E],tau[0], Exp_iter,true,rng, id_proc);
      }
    }
    eval_pol(Exp_iter,numexps,DeltaJ,campos,id_proc);
    calc_sus(numexps,tau[0], Exp_iter, DeltaJ,campos,campos,id_proc);
  }
  //Multifield, single temp, various frec
  cl_start = clock();
  temperaturas = str2vec(DeltaJ, "2.5");
  campos = step2vec(DeltaJ, 16, 0.4, false);
  tau = str2vec(1,"10 30 100");
  temp.str(""); temp<<temperaturas[0];
  for(unsigned int t=0; t<tau.size(); t++){
    frec.str(""); frec<<tau[t];
    id_proc="fixT"+temp.str()+"_riseE_t"+frec.str();
    for(unsigned int n=0;n<numexps;n++){
      relaxor.init(rng,DeltaJ,false);
      for(unsigned int E=0;E<campos.size();E++){
	relaxor.experimento(temperaturas[0],campos[E],tau[t], Equi_iter,false,rng, id_proc);
	relaxor.experimento(temperaturas[0],campos[E],tau[t], Exp_iter,true,rng, id_proc);
      }
    }
    eval_pol(Exp_iter,numexps,DeltaJ,campos,id_proc);
    calc_sus(numexps,tau[t],Exp_iter,DeltaJ,campos,campos,id_proc);
  }

  cl_stop = clock();
  cout<<"Experimeto "<<cl_stop-cl_start<<"\n";
  
  
//   system("gnuplot ../plots.p");
  
  time(&end);
  cout<<difftime(end,start)<<endl;
  return 0;
}

