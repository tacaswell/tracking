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

#include <iostream>
#include <sstream>
namespace utilities{
/**
   Class for carrying around the posistion and velocity vectors.
   The logic for making this class is a memory concern that there
   will be millions of these objects.  Also to make some specfic
   matlab like functions simpler to implement
 */
class Triple{
public:
  /**
     Over loads for comparison	
  */
  bool operator==(const Triple& b)const;

  /**
     Over loads for copying
   */
  void operator= (const Triple& y);
  /**
     Over loads for negation
   */
  Triple operator- ( );

  /**
     Overides random access
   */
  float& operator[] (int i);	
  /**
     Overides random access
   */
  const float& operator[] (int i) const;

  /**
     Over loads for element-wise addition
   */
  Triple operator+ (const Triple& y) const;
  /**
     Over loads for constant addition
   */
  Triple operator+ (const float y)const;
  /**
     adds Triple to the current Triple
   */
  void operator+= (const Triple& y);
  /**
     adds constant to the current Triple
   */
  void operator+= (const float y);



  /**
     Over loads for element-wise subtraction
   */
  Triple operator- ( const Triple& y)const;
  /**
     Over loads for constant subtraction
   */
  Triple operator- ( const float y)const;
  /**
     subtracts Triple to the current Triple
   */
  void operator-= (const Triple& y);
  /**
     subtracts constant to the current Triple
   */
  void operator-= (const float y);



  /**
     Over loads for constant multilplication
   */
  Triple operator* (const float y) const;
  /**
     Over loads for elementwise mustiplication
   */
  Triple operator* (const Triple& y) const;
  /**
     element wise multilication of the current Triple
   */
  void operator*= (const Triple& y);
  /**
     multiplies current Triple by a constant
   */
  void operator*= (const float y);



  /**
     Over loads for element-wise division
   */
  Triple operator/ ( const Triple& y) const;
  /**
     element wise division of the current Triple
   */
  void operator/= (const Triple& y);
  /**
     devides current Triple by a constant
   */
  void operator/= (const float y);



  /**
     square of the magnitude of the Triple assuming euclidian space
   */
  float magnitude_sqr () const;

  /**
     magnitude of the Triple assuming euclidian space
   */
  float magnitude () const;

  /**
     take the dot product between two tuple
   */
  float dot(const Triple & y) const;
   
  /**
     Ruturns the product of the elements
   */
  float prod()const;
  
  /**
     Ruturns the sum of the elements
   */
  float sum()const;
  

  /**
     makes a tuple a unit vector
   */
  void make_unit();

  /**
     makes a tuple a unit vector
   */
  Triple direction() const;

  /**
     prints the data
   */
  void print() const;

  /**
     sets tuple to (0,0)
   */

  void clear();

  /**
     empty constructor
   */
  Triple();

  /**
     Copy constructor
   */
  Triple(const Triple & y);

  /**
     returns the distance squared between two points
   */
  float dist_sqr(const Triple& y)const;
  

  /**
     Destructor
   */
  ~Triple();

  /**
     The length of tuple.  This should match the dimenion of the tracking.
     Exactly how this will all fit together will be seen.
  */
  const static int length_ =3;
  
  /**
     hard coded 3-D constructor 
   */
  Triple(float x,float y,float z );
  /**
     Takes at an array, assumes it is the length_ length_
   */
  Triple(const float * );
  
  
  /**
     returns a pointer to the internal data in a safe way.
     mostly to make it simpler to write out to hdf wrappers
   */
  const float * get_ptr() const;
  float * get_ptr() ;
  
  
protected:
  /**
     pointer to data
   */
  float data_[length_];

private:
};

  
  /**
     prints out the tuple to cout
     lifted almost whole cloth from c++/4.3/complex
  */
  template<typename _CharT, class _Traits>
  std::basic_ostream<_CharT, _Traits>&
  operator<<(std::basic_ostream<_CharT, _Traits>& __os, const Triple& in)
  {

    
    std::basic_ostringstream<_CharT, _Traits> s;
    s.flags(__os.flags());
    s.imbue(__os.getloc());
    s.precision(__os.precision());

    
    s << '(' ;
    s<< in[0]  ;
    for(int j = 1;j<(Triple::length_);++j)
      s<< ','<<in[j]  ;
    s << ')';

    return __os << s.str();
  }



}

#endif
