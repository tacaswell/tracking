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
#include "particle_base.h"

#ifndef PUNCTA
#define PUNCTA
namespace filament{

/**
   
 */
class Puncta::public tracking::particle_base{

public:

protected:
  /**
     is this puncta at the end of a filament, if so then it must be handled
     differently.
   */
  bool end_;
  /**
     Pointer to the filament that this puncta is part of.  If end_ is
     true this points to the first filament that the puncta was
     identified as part of, consult the 
   */
  Filament * filament_;
  


};


#endif
