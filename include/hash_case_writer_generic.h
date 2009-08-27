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

#ifndef HASH_CASE_WRITER_GENERIC
#define HASH_CASE_WRITER_GENERIC

#include "hash_case_writer.h"

namespace tracking
{
class hash_case;
}


namespace utilities
{
class Generic_wrapper_base;


/**
   outputs the hash_case to a generic wrapper so the 
*/
class Hash_case_writer_generic: public Hash_case_writer{
public:
  Hash_case_writer_generic(Generic_wrapper_base * wo)
    :output_wrapper_(wo){};
  void write_hash_case( const tracking::hash_case* hc ) const;
  virtual ~Hash_case_writer_generic(){}
private:
  Generic_wrapper_base * output_wrapper_;
  
};
}

#endif


