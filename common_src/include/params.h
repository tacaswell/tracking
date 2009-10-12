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





#ifndef PARAMS
#define PARAMS
#include <string>
#include <map>
#include "wrapper_i.h"
#include "wrapper_o.h"

namespace utilities{
/**
    Abstract base Class to handle passing parameters around between
    the levels of the functions.  This will have lots of children.
 */

class params{

public:
  std::map<utilities::D_TYPE,int> contains;
  ///Default constructor
  params(std::map<utilities::D_TYPE,int> contents ):contains(contents){};
  ///hack to make stuff compile, kill this!
  //  params(){};
  ///virtual default destructor
  virtual ~params(){};
  /**
     Returns a pointer to a output wrapper_o object based on the
     parameter object.  This function instataites an object, but the
     paramter class does not clean it up.  This should only be called
     in cases where how it will be cleaned up is clear.  This should
     probably be moved in to protected and have every child class be
     friends with master_box.
   */
  virtual Wrapper_out* make_wrapper_out()=0;
  /**
     Returns a pointer to a wrapper_i object based on the parameter
     object.  This function instataites an object, but the paramter
     class does not clean it up.  This should only be called in cases
     where how it will be cleaned up is clear.  This should probably
     be moved in to protected and have every child class be friends
     with master_box.
   */
  virtual Wrapper_in* make_wrapper_in()=0;
protected:
};
}
#endif
