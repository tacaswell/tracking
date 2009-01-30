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
#include "mex.h"
#include <vector>

#ifndef UTILS
#define UTILS
namespace utils{
/**
   Converts a std::vector an mxArray for output from the mexfunction.
   This assumes, but does not check that the contents of the vector
   are a number of some sort that can sensibly be cast to a double.
   This function justs copies the order, so if the 1-D array is
   secretly a 2D array, it may get transposed
*/
template<class T>
void vector_to_mat(mxArray** out, std::vector<T> &in){
  *out = mxCreateDoubleMatrix(1,in.size(), mxREAL);
   double*data = mxGetPr(*out);
   int j = 0;
   typename std::vector< T >::iterator it;
   for(it = in.begin(); it<in.end(); it++,j++)
     data[j] = (double)(*it);
};
/**
   Converts an 2 dimension array using c-style indexing to
   an mexArray.
*/
void array_to_mat(mxArray** out, double * in, int rows, int cols);

}


#endif
