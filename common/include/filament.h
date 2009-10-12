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

#ifndef FILAMENT
#define FILAMENT
#include <vector>

namespace tracking
{
class hash_shelf;
}


namespace filament
{


class Puntca;


class Filament
{
public:
  
  /**
     connects this filament to those in the next frame
   */
  void connect_forward(tracking::hash_shelf*);
  /**
     returns a pointer to the front or back connected vector that
     corresponds to the end that end_puncta is on.
   */
  const vector<Filament*> * connected_filaments(Puncta* end_puncta) const;
  
  /*
    general analysis nonsense, length, curvature, etc
   */
  

protected:
  /**
     Pointers to puncta in the filament
   */
  vector<Puncta*> contents_;
  
  /**
     Pointers to filaments that also share the first puncta
   */
  vector<Filament*> front_connected_;
  /**
     Pointers to filaments that also share the last puncta
   */
  vector<Filament*> back_connected_;
  /**
     Pointers to the filaments that this filament came from
   */
  vector<Filament*> parents_;
  /**
     Pointers to the filaments that this filament goes to
   */
  vector<Filament*> children_;
  
  /**
     Pointer to hash shelf that this filament is in
   */
  tracking::hash_shelf* shelf_;
  
private:

};

  




}


#endif
