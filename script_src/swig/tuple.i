

template<class T,int length>
class Tuple{
public:
  /**
     Over loads for comparison	
  */
  bool operator==(const Tuple& b)const;
  
  /**
     Over loads for negation
  */
  Tuple operator- ( );

  /**
     Over loads for element-wise addition
  */
  Tuple operator+ (const Tuple& y) const;
  /**
     Over loads for constant addition
  */
  Tuple operator+ (const T y)const;
  /**
     adds Tuple to the current Tuple
  */
  void operator+= (const Tuple& y);
  /**
     adds constant to the current Tuple
  */
  void operator+= (const T y);



  /**
     Over loads for element-wise subtraction
  */
  Tuple operator- ( const Tuple& y)const;
  /**
     Over loads for constant subtraction
  */
  Tuple operator- ( const T y)const;
  /**
     subtracts Tuple to the current Tuple
  */
  void operator-= (const Tuple& y);
  /**
     subtracts constant to the current Tuple
  */
  void operator-= (const T y);



  /**
     Over loads for constant multilplication
  */
  Tuple operator* (const T y) const;
  /**
     Over loads for elementwise mustiplication
  */
  Tuple operator* (const Tuple& y) const;
  /**
     element wise multilication of the current Tuple
  */
  void operator*= (const Tuple& y);
  /**
     multiplies current Tuple by a constant
  */
  void operator*= (const T y);



  /**
     Over loads for element-wise division
  */
  Tuple operator/ ( const Tuple& y) const;
  /**
     element wise division of the current Tuple
  */
  void operator/= (const Tuple& y);
  /**
     devides current Tuple by a constant
  */
  void operator/= (const T y);



  /**
     square of the magnitude of the Tuple assuming euclidian space
  */
  T magnitude_sqr () const;

  /**
     magnitude of the Tuple assuming euclidian space
  */
  T magnitude () const;

  /**
     take the dot product between two tuple
  */
  T dot(const Tuple & y) const;
   
  /**
     Ruturns the product of the elements
  */
  T prod()const;
  
  /**
     Ruturns the sum of the elements
  */
  T sum()const;

  /**
     Returns the maximum element
   */
  T max()const;
  /**
     Returns the minimum element
   */
  T min()const;
  
  
  /**
     makes a tuple a unit vector
  */
  void make_unit();

  /**
     makes a tuple a unit vector
  */
  Tuple direction() const;

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
  Tuple();

  /**
     Copy constructor
  */
  Tuple(const Tuple & y);

  /**
     returns the distance squared between two points
  */
  T dist_sqr(const Tuple& y)const;
  

  /**
     Destructor
  */
  ~Tuple();
  /**
     Uniform constructor
   */
  Tuple(T x);

  /**
     hard coded 2-D constructor 
  */
  Tuple(T x,T y);
  /**
     hard coded 3-D constructor
   */
  Tuple(T x,T y,T z);
  
  /**
     Takes in an array, assumes it is the correct length
  */
  Tuple(const T * );

  /**
     returns a pointer to the internal data in a unsafe way.
     mostly to make it simpler to read out from hdf wrappers
  */
  T * get_ptr() ;      
  /**
     Returns the length
   */
  int get_len()const;
  %extend 
   {
     void set_val(T val,unsigned int ind)
     {
       if( ind < length)
       {
         T* tmp = self->get_ptr();
         tmp[ind] = val;
       }
       
     }
     T get_val(unsigned int ind)
     {
       if( ind < length)
       {
         T* tmp = self->get_ptr();
         return tmp[ind];
       }
       return 0;
       
     }
   }  ;
  
  };

