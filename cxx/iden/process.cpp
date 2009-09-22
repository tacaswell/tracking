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

//#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot

using std::cout;
using std::endl;
using utilities::Wrapper_i_plu;

using iden::Iden;
using iden::Params;

int main()
{
  try
  {
    
    Params p(4,1.3,3,1,3);
    p.PrintOutParameters(std::cout);
  
    Iden iden(p);
    iden.set_fname("30um_27-2_0.tif");
    Wrapper_i_plu wp(1);
    iden.fill_wrapper(wp,5,5);
    cout<<wp.get_num_entries(-1)<<endl;
    
  }
  catch(const char * err){
    std::cout<<err<<endl;
  } 
  
  return 0;
}

