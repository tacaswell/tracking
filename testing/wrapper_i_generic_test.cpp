//Copyright 2012 Thomas A Caswell
//tcaswell@uchicago.edu
//http://jfi.uchicago.edu/~tcaswell
//
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 3 of the License, or (at
//your option) any later version.
//
//This program is distributed in the hope that it will be useful, but
//WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
//General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, see <http://www.gnu.org/licenses>.



#include <iostream>
#include <set>

#include "wrapper_i_generic.h"



using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;

using utilities::Wrapper_i_generic;
using utilities::D_TYPE;
using utilities::Tuplef;

int main(int argc, const char * argv[])
{

  cout<<"hello world"<<endl;
  
  // make fake data
  int N = 25;
  float X[N];
  float Y[N];
  for(int j=0;j<N;++j)
  {
    X[j] = j;
    Y[j] = j;
  }
  


    D_TYPE tmp[] = {utilities::D_XPOS,
        	    utilities::D_YPOS,
    };
    set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+2);
    

    Wrapper_i_generic wrapper_in = Wrapper_i_generic();

  try
  {
    
    wrapper_in.open_wrapper();
    wrapper_in.setup(data_types,1,Tuplef(0.0));
    wrapper_in.open_frame(0,N,0);

    wrapper_in.set_data_type(utilities::D_XPOS);
    wrapper_in.add_float_data(X,N);
    
    
    wrapper_in.set_data_type(utilities::D_YPOS);
    wrapper_in.add_float_data(Y,N);

    wrapper_in.close_frame();
    
    wrapper_in.finalize_wrapper();
    

      

  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
  } 
  float tempf;
  cout<<N<<endl;
  
  for(int j = 0; j <N; ++j)
  {
    cout<<j<<'\t';
    
    cout<<wrapper_in.get_value(tempf,j,utilities::D_XPOS,0)<<","
    	<<wrapper_in.get_value(tempf,j,utilities::D_YPOS,0)<<endl;
  }
  
  
  return 0;
  

}
