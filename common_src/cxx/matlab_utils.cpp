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
#include "matlab_utils.h"

using namespace utilities;

void array_to_mat(mxArray** out, double * in, int rows, int cols) {
  *out = mxCreateDoubleMatrix(rows,cols, mxREAL);
  double*data = mxGetPr(*out);
  for(int j = 0; j<rows;j++)
    for(int k = 0; k<cols;k++)
      //takes transpose
      *(data+rows*k + j  ) = *(in+cols*j + k );
  return;
}
