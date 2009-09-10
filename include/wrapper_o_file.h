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
#include "wrapper_o.h"
#include <fstream>

#ifndef WRAPPER_O_FILE
#define WRAPPER_O_FILE
namespace tracking{
//set up to use stl
using std::string;
using std::vector;
using std::map;
using std::cout;
using std::pair;
using std::endl;
using std::ofstream;
//forward declerations
class params_file;
/**
   wrapper_o class for dealing with  writing data to a file.  The
   data is stored in a vector of vectors.  The wrapper must be
   explicitly told to write the data to disk
 */

class wrapper_o_file :public Wrapper_out{
private:

protected:
  ///File name to write data to
  string fname;

  ///Temperoray data structure for holding current 
  vector<double > data;

  ///File stream to write data into
  ofstream f_out;

  ///number of fields per particle
  int cols;

  ///initializes the wrapper_i class
  void initialize(params_file* param);
  
  ///takes care of ensuring everything is in range for writing stuff
  ///into the data structure.
  void store_data(int index, int pos, double val);
  
  ///Map that stores what values go where in what columns
  map<utilities::D_TYPE, int> contents;



public:
  ///adds another particle to the data table and fills it with
  ///zero values.  Returns the index of the new particle
  //  int add_particle();

  ///print out a representation of the data
  void print() const;
  
  void print(int ind);
  
  ///default constructor
  //nuke this eventully
  wrapper_o_file(params_file* params);
  

  int num_entries() const{return data.size();}

  //  virtual void set_value(int ind, utilities::D_TYPE type,double val);



  void set_new_value(utilities::D_TYPE type, double val);
  void end_new_particle();
  void finalize_wrapper();
  void initialize_wrapper();
  void reset_wrapper(params * param);
  void start_new_particle();
  
  
  ///writes data structure out to disk
  //  void finalize();
};
}

#endif
