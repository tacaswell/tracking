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
#ifndef TOUPLE
#define TOUPLE

namespace utilities{
/**
   Class for carrying around the posistion and velocity vectors.
   The logic for making this class is a memory concern that there
   will be millions of these objects.  Also to make some specfic
   matlab like functions simpler to implement
 */
class Touple{
public:
 /**
     Over loads for copying
   */
  void operator= (const Touple& y);
  /**
     Over loads for element-wise addition
   */
  Touple operator+ (const Touple& y);
  /**
     Over loads for element-wise subtraction
   */
  Touple operator- ( const Touple& y);
  /**
     Over loads for negation
   */
  Touple operator- ( );
  /**
     Over loads for element-wise division
   */
  Touple operator/ ( const Touple& y);

  /**
     Overides random access
   */
  double& operator[] (int i);	
  /**
     adds Touple to the current Touple
   */
  void operator+= (const Touple& y);
  /**
     adds constant to the current Touple
   */
  void operator+= (const double y);
  /**
     subtracts Touple to the current Touple
   */
  void operator-= (const Touple& y);
  /**
     subtracts constant to the current Touple
   */
  void operator-= (const double y);
  /**
     element wise division of the current Touple
   */
  void operator/= (const Touple& y);
  /**
     devides current Touple by a constant
   */
  void operator/= (const double y);
  /**
     element wise multilication of the current Touple
   */
  void operator*= (const Touple& y);
  /**
     multiplies current Touple by a constant
   */
  void operator*= (const double y);

  /**
     square of the magnitude of the Touple assuming euclidian space
   */
  double magnitude_sqr ();


  /**
     magnitude of the Touple assuming euclidian space
   */
  double magnitude ();

  /**
     prints the data
   */
  void print();
  /**
     empty constructor
   */
  Touple();
  /**
     Destructor
   */
  ~Touple();
protected:
  /**
     The length of touple.  This should match the dimenion of the tracking.
     Exactly how this will all fit together will be seen.
  */
  static int length_;
  /**
     pointer to data
   */
  double* data_;

  /**
     if length_ can be changed
   */
  static bool allow_dim_change_;
private:
};


}

#endif
