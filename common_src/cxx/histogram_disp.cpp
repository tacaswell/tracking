//Copyright 2009,2010 Thomas A Caswell
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
#include "gnuplot_i.hpp"
#include "histogram.h"

using gnuplot::Gnuplot;


void Histogram::display() const
{

  vector<float> tmp = get_bin_edges();
  tmp.pop_back();
  
  vector<float> tmp2 = vector<float>(0);
  tmp2.reserve(hist_array_.size());
  
  for(vectori::const_iterator it = hist_array_.begin();
      it<hist_array_.end(); ++it)
    tmp2.push_back(*it);
  
  try
  {
    
    Gnuplot g(tmp,tmp2,"track length","histeps");
    g.set_grid();
    wait_for_key();
  }
  catch(GnuplotException & e)
  {
    cout<<e.what()<<endl;
  }
  
}


void Histogram::display(Gnuplot & g ) const
{

  vector<float> tmp = get_bin_edges();
  tmp.pop_back();
  
  vector<float> tmp2 = vector<float>(0);
  tmp2.reserve(hist_array_.size());
  
  for(vectori::const_iterator it = hist_array_.begin();
      it<hist_array_.end(); ++it)
    tmp2.push_back(*it);
  
  try
  {
    
    g.plot_xy(tmp,tmp2);
    g.set_style("histograms");
    
    g.set_grid();
    g.replot();
    
    wait_for_key();
  }
  catch(GnuplotException & e)
  {
    cout<<e.what()<<endl;
  }
  
}

sh
