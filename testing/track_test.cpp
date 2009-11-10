//Copyright 2009 Thomas A Caswell
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

#include "master_box_t.h"

#include "particle_track.h"
#include "hash_case.h"
#include "pair.h"

#include "wrapper_i_dummy.h"
#include "filter.h"


#include "corr_gofr.h"


#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communication with Gnuplot

#include "track_shelf.h"
#include "track_box.h"

using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;



using utilities::Wrapper_i_dummy;

using utilities::Pair;
using utilities::Filter_trivial;
using utilities::D_TYPE;



using tracking::Master_box;
using tracking::particle;


using tracking::track_shelf;
using tracking::Track_box;


int main(int argc, const char * argv[])
{

  try
  {
      
    // set up data types to import form the input
    D_TYPE tmp[] = {utilities::D_XPOS,
		    utilities::D_YPOS,
		    utilities::D_DX,
		    utilities::D_DY,
		    utilities::D_I,
		    utilities::D_R2,
		    utilities::D_MULT,
		    utilities::D_E,
		    utilities::D_ZPOS
    };
    set<D_TYPE> data_types = set<D_TYPE>(tmp, tmp+9);
    
    
    Wrapper_i_dummy wrapper_in = Wrapper_i_dummy(data_types,20,1);
    Filter_trivial filt;
    Master_box box;
    box.init(wrapper_in,filt);
    cout<<box.size()<<endl;
    

    
    Track_box* track_box = new Track_box(box.get_particle(0));
    for(unsigned int j = 1; j<box.size();++j)
    {
      track_box->push_back(box.get_particle(j));
    }
    track_box->print();


//     Track_box * track_box_s = track_box->split_track(13);
//     Track_box * track_box_ss = track_box->split_track(7);
//     track_box->print();
//     track_box_s->print();
//     track_box_ss->print();
//     //track_box_s->trim_track(2,5);
//     track_box_s->print();
//     delete track_box_s;
//     delete track_box_ss;
       

    track_shelf shelf;
    track_box->split_to_parts(shelf);
    shelf.print();
    
    delete track_box;

    
 
  }
  catch(const char * err){
    std::cerr<<"caught on error: ";
    std::cerr<<err<<endl;
  } 

  

  return 0;

}
