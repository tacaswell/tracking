//Copyright 2010 Thomas A Caswell
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
#include <stdexcept>
#include <string>

#include "FreeImagePlus.h"

#include "mm_md_parser.h"
#include "md_store.h"
#include "enum_utils.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::logic_error;
using utilities::Mm_md_parser;
using utilities::Md_store;

using utilities::VT2str_s;





static string fname = "/home/tcaswell/colloids/data/polyNIPAM_batch_12/20100524/4/exp2/27-7_27-1_27-6_vslow.tif";

int main()
{
    
  FreeImage_Initialise();
  BOOL bMemoryCache = TRUE;
  fipMultiPage src(bMemoryCache);
  // Open src file (read-only, use memory cache)
  src.open(fname.c_str(), FALSE, TRUE);
  unsigned int img_frames = src.getPageCount();
  cout<<img_frames<<endl;
  Mm_md_parser mm_md_p;

  fipImage image;
  image = src.lockPage(0);
  Md_store * md_store = mm_md_p.parse_md(image);
  // clear the input data
  src.unlockPage(image,false);


  image = src.lockPage(1);
  Md_store * md_store2 = mm_md_p.parse_md(image);
  // clear the input data
  src.unlockPage(image,false);


  src.close(0);
  
  FreeImage_DeInitialise();

  cout<<"Frame 0"<<endl;
  cout<<"==============="<<endl;
  
  for(unsigned int j = 0; j<md_store->size();++j)
    cout<<md_store->get_key(j)<<" ("
	<<VT2str_s(md_store->get_type(j))<<") "
	<<md_store->get_val(j)<<'|'<<endl;
  

  float exposure;
  int key_ind = md_store->get_key_index("Exposure");
  
  key_ind = md_store->get_key_index("Exposure");
  cout<<"The exposure is "<<md_store->get_value("Exposure",exposure);
  
  cout<<endl
      <<"    (string)"<<md_store->get_val(key_ind)<<endl
      <<"     (float)"<<exposure<<endl;
  
  


  cout<<"Frame 1"<<endl;
  cout<<"==============="<<endl;
  for(unsigned int j = 0; j<md_store2->size();++j)
    cout<<md_store2->get_key(j)<<" ("
	<<VT2str_s(md_store2->get_type(j))<<") "
	<<md_store2->get_val(j)<<'|'<<endl;
  
  exposure = 3.14159;
  
  
  
  key_ind = md_store2->get_key_index("Exposure");

  
  cout<<"The exposure is "<<md_store2->get_value("Exposure",exposure);
  
  cout<<endl
      <<"    (string) "<<md_store2->get_val(key_ind)<<endl
      <<"     (float) "<<exposure<<endl;
  

  float stupid = 374.9899;
  cout<<stupid<<endl;


  float a,b;
  
  a = md_store2->get_value("Exposure",b);
  cout<<a<<"  "<<b<<endl;
  

  delete md_store;
  md_store=NULL;
  delete md_store2;
  md_store2=NULL;
  
      

  return 0;
}
