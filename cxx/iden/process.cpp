//Copyright 2008,2009 Thomas A Caswell
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
//
//Additional permission under GNU GPL version 3 section 7
//
//If you modify this Program, or any covered work, by linking or
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for FreeImage Public License, the licensors of
//this Program grant you additional permission to convey the resulting
#include <iostream>

#include "iden/wrapper_i_plu.h"
#include "iden/iden.h"
#include "iden/params1.h"

#include "master_box_t.h"
#include "particle_base.h"
#include "hash_case.h"
#include "tuple.h"
#include "wrapper_o_hdf.h"
//#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot

using std::cout;
using std::endl;

using utilities::Wrapper_i_plu;
using utilities::Wrapper_o_hdf;
using utilities::Tuple;

using tracking::master_box_t;
using tracking::particle_base;
using tracking::hash_case;

using iden::Iden;
using iden::Params;

int main()
{
  try
  {
    
    Params p(4,1.3,3,1,4);
    p.PrintOutParameters(std::cout);
  
    Iden iden(p);
    iden.set_fname("25-0_mid_0.tif");
    Wrapper_i_plu wp(1);
    int frame_c = 1200;
    
    iden.fill_wrapper(wp,frame_c,0);
    cout<<wp.get_num_entries(-1)<<endl;
    master_box_t<particle_base> box;
    box.init(wp);
    
    cout<<"total number of particles is: "<<box.size()<<endl;;
    
    Tuple dims(1392,520);
    particle_base* pa;

//     for(int j =0;j<box.size();++j)
//     {
//       particle_base* p = box.get_particle(j);
//       const Tuple pos = p->get_position();
      
//       if(pos[1]>dims[1]||pos[0]>dims[0])
//       {
// 	cout<<"-------------------------"<<endl;
// 	for(int k = -5;k<6;++k)
// 	{
// 	  p = box.get_particle(j+k);
// 	  cout<<k<<" ";
// 	  p->print();
// 	}
	
//       }
      
//     }
    
 
    
    hash_case hcase(box,dims,20,frame_c);
    //    hcase.print();
    
    Wrapper_o_hdf hdf_w("25-0_mid_0.h5",wp.get_data_types());
    
    try
    {
      hcase.output_to_wrapper(hdf_w);
    }
    catch(const char * err)
    {
      std::cerr<<"caught on error: ";
      std::cerr<<err<<endl;
    }
    catch(...)
    {
      std::cerr<<"not right type"<<endl;
    }
    

    
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
  } 
  
  return 0;
}

