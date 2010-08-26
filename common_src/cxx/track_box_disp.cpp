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
#include "track_box.h"
#include "particle_track.h"
#include "array.h"

#include "part_def.h"

#include "track_shelf.h"

#include "gnuplot_i.hpp"

#include <stdexcept>
using std::runtime_error;
using std::logic_error;


using gnuplot::Gnuplot;
using gnuplot::GnuplotException;
using gnuplot::wait_for_key;


using namespace tracking;
using utilities::Array;
using utilities::Tuplei;
using utilities::Tuplef;
using utilities::Tuple;


using std::vector;
using std::pair;

using std::cout;
using std::endl;


void Track_box::plot_intensity() const
{
  std::vector<float> inten_data(length_);
  std::vector<float> x_data(length_);
  std::vector<float> y_data(length_);
  std::vector<float> z_data(length_);
  const particle_track * cur_part = t_first_;
  float x_mean =0,y_mean=0;
  for(int j = 0; j< length_;++j)
  {
    cur_part->get_value(utilities::D_I,inten_data[j]);
    cur_part->get_value(utilities::D_ZPOS,z_data[j]);
    Tuplef pos = cur_part->get_position();
    cur_part = cur_part->get_next();
    x_data[j] = pos[0];
    y_data[j] = pos[1];

    
    y_mean +=y_data[j];
    x_mean +=x_data[j];
  }
  y_mean/=length_;
  x_mean/=length_;
  
    
  for(int j =0;j<length_;++j)
  {
    x_data[j] -= x_mean;
    y_data[j] -= y_mean;

  }
  
  
  Gnuplot g(z_data,inten_data,"intensity","linespoints","z","I");
  g.set_grid().replot();
  
  Gnuplot g2(x_data,y_data,"position", "linespoints");
  g2.set_grid().replot();
  
  wait_for_key();
  g.remove_tmpfiles();
  g2.remove_tmpfiles();
      

}
