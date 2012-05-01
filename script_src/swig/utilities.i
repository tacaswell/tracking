//Copyright 2012 Thomas A Caswell
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



namespace utilities
{
  enum D_TYPE
  {
  D_INDEX               =0,	// index of particle in case,
				// (frame,indx) is unique and should
				// be persistent in wrappers (wrapper)
  D_XPOS  		=1,	// x-position (plu)
  D_YPOS  		=2,	// y-position (plu)
  D_FRAME  		=3,	// frame (wrapper)
  D_I  			=4,	// integrated intensity (plu)
  D_R2  		=5,	// radius of gyration (plu)
  D_E 			=6,	// eccentricity (plu)
  D_NEXT_INDX 		=7,	// index of the next particle in the
				// track (track)
  D_PREV_INDX  		=8,	// index of the previous particle in
				// the track (track)
  D_DX  		=9,	// x-shift (plu)
  D_DY  		=10,	// y-shift (plu)
  D_ORGX  		=11,	// original x-position (computed)
  D_ORGY  		=12,	// original y-position (computed)
  D_ZPOS 		=13,	// z-position (link or meta-data)
  D_TRACKID 		=14,	// track id (track)
  D_S_ORDER_PARAMETER 	=15,	// scalar order parameter (computed)
  D_MULT 		=16,	// multiplicity (plu)
  D_N_SIZE  		=17,	// neighborhood size (computed)
  D_SENTRY			// this entry must remain last, and I
				// am making assumptions about how the
				// numbering will work in the compiler
  
    };

typedef enum V_TYPE
  {
    V_ERROR = -1,		// error type
    V_UINT = 0,			// unsigned integer
    V_INT =1,			// integer
    V_FLOAT =2,			// float
    V_COMPLEX =3,		// complex
    V_STRING =4,		// string
    V_BOOL =5,			// bool
    V_TIME =6,			// date/time
    V_GUID =7,			// guid from MM meta-data
    

  }V_TYPE;
%include "tuple.i"
typedef Tuple<float,DIM_COUNT> Tuplef;
%template (Tuplef) Tuple<float,DIM_COUNT>;
   


class Md_store
{
public:
    
  template <class T>
  T get_value(const std::string & attr_name,T & val)const;
  
    
  template <class T>
  void set_value(const std::string & attr_name,const T & val);
  
  %template (set_value_i) set_value<int>;
  %template (set_value_ui) set_value<unsigned int>;
  %template (set_value_f) set_value<float>;

  %template (get_value_i) set_value<int>;
  %template (get_value_ui) set_value<unsigned int>;
  %template (get_value_f) set_value<float>;

  bool contains_key(const std::string& key)const;
  int get_key_index(const std::string& key)const;
  unsigned int size() const;
  std::string get_key(int j) const;
  
  utilities::V_TYPE get_type(int j)const;
  
  void add_elements(const Md_store * md_in);
  
  ~Md_store();
  
  Md_store();

 
};
 


class Wrapper_in
{
public:
  virtual int get_num_entries(int frame) =0;
  virtual int get_num_entries() =0;
  virtual int get_num_frames()  =0;
  virtual ~Wrapper_in();
};
 
class Wrapper_i_hdf: public Wrapper_in
{
public:
  int get_num_entries(int frame);
  int get_num_entries();
  int get_num_frames();

  Wrapper_i_hdf();
  ~Wrapper_i_hdf();
  
  bool set_file_name(const std::string & fname);
  bool add_dtype(utilities::D_TYPE dtype,int comp_key);
  bool set_twoD(bool twod_data);
  bool initialize(int f_count);

};
  


class Wrapper_i_generic:public Wrapper_in{
 public:
  Wrapper_i_generic();  
  ~Wrapper_i_generic();

  int get_num_entries(int frame);
  int get_num_entries();
  int get_num_frames();


  bool setup(std::set<D_TYPE>,int n,const utilities::Tuplef & d);

  bool open_frame(unsigned int frame,int N,float z);
  bool set_data_type(D_TYPE dtype);
%apply (int* IN_ARRAY1, int DIM1) {(int* data, int N)}  
  bool add_int_data(int * data,int N);
%clear (int* vector, int length);
%apply (float* IN_ARRAY1, int DIM1) {(float* data, int N)}
  bool add_float_data(float * data,int N);
%clear (float* vector, int length);
  //  bool set_meta_data(const Md_store & md_store);
  bool close_frame();
  bool finalize_wrapper();
  

};
 




class Generic_wrapper{
public:
  virtual void open_wrapper() =0;
  virtual void close_wrapper()=0;
  virtual bool is_open() const=0;
  virtual void open_group(const std::string & name)=0;
  virtual void close_group()=0;
  virtual void add_dset(int rank,const unsigned int * dims, V_TYPE vt , const void * data,const std::string & dset_name)=0;
  virtual void add_meta_data(const std::string & key, float val)=0;
  virtual void add_meta_data(const std::string & key, const Tuple<float,3> & val)=0;
  virtual void add_meta_data(const std::string & key, const Tuple<float,2>& val)=0;
  virtual void add_meta_data(const std::string & key,  const std::string & val)=0;
  virtual void add_meta_data(const std::string & key, int val)=0;
  virtual void add_meta_data(const std::string & key, unsigned int val)=0;
  virtual void add_meta_data(const Md_store * md_store)=0;
  virtual void add_meta_data(const std::string & key, float val,const std::string & dset_name)=0;
  virtual void add_meta_data(const std::string & key, const Tuple<float,3> & val,const std::string & dset_name)=0;
  virtual void add_meta_data(const std::string & key, const Tuple<float,2>& val,const std::string & dset_name)=0;
  virtual void add_meta_data(const std::string & key,  const std::string & val,const std::string & dset_name)=0;
  virtual void add_meta_data(const std::string & key, int val,const std::string & dset_name)=0;
  virtual void add_meta_data(const std::string & key, unsigned int val,const std::string & dset_name)=0;
  virtual void add_meta_data(const Md_store * md_store,const std::string & dset_name)=0;
  
  
 
};
class Generic_wrapper_read
{
public:
  virtual void open_wrapper() =0;
  virtual void close_wrapper()=0;
  virtual bool is_open() const=0;
  virtual void open_group(const std::string & name)=0;
  virtual void get_dset(std::vector<int> & data,std::vector<unsigned int> & dims, const std::string & dset_name) const=0;
  virtual void get_dset(std::vector<unsigned int> & data, std::vector<unsigned int> & dims,const std::string & dset_name) const=0;
  virtual void get_dset(std::vector<float> & data, std::vector<unsigned int> & dims, const std::string & dset_name) const=0;
  virtual void get_dset_info(std::vector<int> & dims,V_TYPE& vt ,const std::string & dset_name) const=0;
  virtual float get_meta_data(const std::string & key, float & val)=0;
  virtual Tuple<float,3> get_meta_data(const std::string & key,  Tuple<float,3> & val)=0;
  virtual Tuple<float,2> get_meta_data(const std::string & key,  Tuple<float,2>& val)=0;
  virtual std::string get_meta_data(const std::string & key,  std::string & val)=0;
  virtual int get_meta_data(const std::string & key, int & val)=0;
  virtual unsigned int get_meta_data(const std::string & key, unsigned int& val)=0;
  virtual Md_store& get_meta_data(Md_store & md_store)=0;
  virtual float get_meta_data(const std::string & key, float& val,const std::string & dset_name)=0;
  virtual Tuple<float,3>  get_meta_data(const std::string & key, Tuple<float,3> & val,const std::string & dset_name)=0;
  virtual Tuple<float,2> get_meta_data(const std::string & key, Tuple<float,2>& val,const std::string & dset_name)=0;
  virtual std::string get_meta_data(const std::string & key,  std::string & val,const std::string & dset_name)=0;
  virtual int get_meta_data(const std::string & key, int &val,const std::string & dset_name)=0;
  virtual Md_store& get_meta_data(Md_store & md_store,const std::string & dset_name)=0;

};

class Generic_wrapper_hdf{
public:
  typedef enum F_TYPE{ 
    /// open existing file read-write                   
    F_DISK_RDWR = 0,
    /// create a new file, must not previously exist             
    F_DISK_EXCL,
    /// creates new file, nukes existing file                    
    F_DISK_TRUNC,
    /// create file in memory                                    
    F_MEM,                     
  } F_TYPE;                                                                        

  Generic_wrapper_hdf(std::string fname, F_TYPE f_type = F_DISK_RDWR);

  void open_wrapper() ;
  void close_wrapper();
  bool is_open() const;
  void open_group(const std::string & name);
  void close_group();

  void add_dset(int rank,const unsigned int * dims, V_TYPE vt , const void * data,const std::string & dset_name);
  void add_meta_data(const std::string & key, float val);
  void add_meta_data(const std::string & key, const Tuple<float,3> & val);
  void add_meta_data(const std::string & key, const Tuple<float,2>& val);
  void add_meta_data(const std::string & key,  const std::string & val);
  void add_meta_data(const std::string & key, int val);
  void add_meta_data(const std::string & key, unsigned int val);
  void add_meta_data(const Md_store * md_store);
  void add_meta_data(const std::string & key, float val,const std::string & dset_name);
  void add_meta_data(const std::string & key, const Tuple<float,3> & val,const std::string & dset_name);
  void add_meta_data(const std::string & key, const Tuple<float,2>& val,const std::string & dset_name);
  void add_meta_data(const std::string & key,  const std::string & val,const std::string & dset_name);
  void add_meta_data(const std::string & key, int val,const std::string & dset_name);
  void add_meta_data(const std::string & key, unsigned int val,const std::string & dset_name);
  void add_meta_data(const Md_store * md_store,const std::string & dset_name);

  void get_dset(std::vector<int> & data,std::vector<unsigned int> & dims, const std::string & dset_name) const;
  void get_dset(std::vector<unsigned int> & data, std::vector<unsigned int> & dims,const std::string & dset_name) const;
  void get_dset(std::vector<float> & data, std::vector<unsigned int> & dims, const std::string & dset_name) const;
  void get_dset_info(std::vector<int> & dims,V_TYPE& vt ,const std::string & dset_name) const;
  float get_meta_data(const std::string & key, float & val);

  Tuple<float,3> get_meta_data(const std::string & key,  Tuple<float,3> & val);
  Tuple<float,2> get_meta_data(const std::string & key,  Tuple<float,2>& val);
  std::string get_meta_data(const std::string & key,  std::string & val);
  int get_meta_data(const std::string & key, int & val);
  unsigned int get_meta_data(const std::string & key, unsigned int& val);
  Md_store& get_meta_data(Md_store & md_store);
  float get_meta_data(const std::string & key, float& val,const std::string & dset_name);
  Tuple<float,3>  get_meta_data(const std::string & key, Tuple<float,3> & val,const std::string & dset_name);
  Tuple<float,2> get_meta_data(const std::string & key, Tuple<float,2>& val,const std::string & dset_name);
  std::string get_meta_data(const std::string & key,  std::string & val,const std::string & dset_name);
  int get_meta_data(const std::string & key, int &val,const std::string & dset_name);
  Md_store& get_meta_data(Md_store & md_store,const std::string & dset_name);

} ;
 


class Filter
{
public:
  virtual bool operator()(int index,int frame) const = 0;
  virtual void set_wrapper(const Wrapper_in * w_in)= 0;
  virtual ~Filter();
  
};


class Filter_trivial : public Filter
{
public:
  bool operator()(int index,int frame) const;
  void set_wrapper(const  Wrapper_in* w_in);
  Filter_trivial();
  ~Filter_trivial();

  
};


class Filter_basic : public Filter
{
public:
  bool operator()(int index,int frame) const;
  void set_wrapper(const Wrapper_in * w_i );
  Filter_basic();
  ~Filter_basic();

  void init(const std::string&,int);
  void init(float ecut,float rg_cut,float shift_cut);
  
    
  
 };


class Generic_wrapper{
public:
  void open_wrapper() ;
  void close_wrapper();
  bool is_open() const;
  void open_group(const std::string & name);
  void close_group();
  void add_dset(int rank,const unsigned int * dims, V_TYPE vt , const void * data,const std::string & dset_name);
  void add_meta_data(const std::string & key, float val);
  void add_meta_data(const std::string & key, const Tuple<float,3> & val);
  void add_meta_data(const std::string & key, const Tuple<float,2>& val);
  void add_meta_data(const std::string & key,  const std::string & val);
  void add_meta_data(const std::string & key, int val);
  void add_meta_data(const std::string & key, unsigned int val);
  void add_meta_data(const Md_store * md_store);
  void add_meta_data(const std::string & key, float val,const std::string & dset_name);
  void add_meta_data(const std::string & key, const Tuple<float,3> & val,const std::string & dset_name);
  void add_meta_data(const std::string & key, const Tuple<float,2>& val,const std::string & dset_name);
  void add_meta_data(const std::string & key,  const std::string & val,const std::string & dset_name);
  void add_meta_data(const std::string & key, int val,const std::string & dset_name);
  void add_meta_data(const std::string & key, unsigned int val,const std::string & dset_name);
  void add_meta_data(const Md_store * md_store,const std::string & dset_name);
  
  
 
};
class Generic_wrapper_read
{
public:
  void open_wrapper() ;
  void close_wrapper();
  bool is_open() const;
  void open_group(const std::string & name);
  void get_dset(std::vector<int> & data,std::vector<unsigned int> & dims, const std::string & dset_name) const;
  void get_dset(std::vector<unsigned int> & data, std::vector<unsigned int> & dims,const std::string & dset_name) const;
  void get_dset(std::vector<float> & data, std::vector<unsigned int> & dims, const std::string & dset_name) const;
  void get_dset_info(std::vector<int> & dims,V_TYPE& vt ,const std::string & dset_name) const;
  float get_meta_data(const std::string & key, float & val);
  Tuple<float,3> get_meta_data(const std::string & key,  Tuple<float,3> & val);
  Tuple<float,2> get_meta_data(const std::string & key,  Tuple<float,2>& val);
  std::string get_meta_data(const std::string & key,  std::string & val);
  int get_meta_data(const std::string & key, int & val);
  unsigned int get_meta_data(const std::string & key, unsigned int& val);
  Md_store& get_meta_data(Md_store & md_store);
  float get_meta_data(const std::string & key, float& val,const std::string & dset_name);
  Tuple<float,3>  get_meta_data(const std::string & key, Tuple<float,3> & val,const std::string & dset_name);
  Tuple<float,2> get_meta_data(const std::string & key, Tuple<float,2>& val,const std::string & dset_name);
  std::string get_meta_data(const std::string & key,  std::string & val,const std::string & dset_name);
  int get_meta_data(const std::string & key, int &val,const std::string & dset_name);
  Md_store& get_meta_data(Md_store & md_store,const std::string & dset_name);

} ;

class Generic_wrapper_hdf{
public:
  void open_wrapper() ;
  void close_wrapper();
  bool is_open() const;
  void open_group(const std::string & name);
  void close_group();

  void add_dset(int rank,const unsigned int * dims, V_TYPE vt , const void * data,const std::string & dset_name);
  void add_meta_data(const std::string & key, float val);
  void add_meta_data(const std::string & key, const Tuple<float,3> & val);
  void add_meta_data(const std::string & key, const Tuple<float,2>& val);
  void add_meta_data(const std::string & key,  const std::string & val);
  void add_meta_data(const std::string & key, int val);
  void add_meta_data(const std::string & key, unsigned int val);
  void add_meta_data(const Md_store * md_store);
  void add_meta_data(const std::string & key, float val,const std::string & dset_name);
  void add_meta_data(const std::string & key, const Tuple<float,3> & val,const std::string & dset_name);
  void add_meta_data(const std::string & key, const Tuple<float,2>& val,const std::string & dset_name);
  void add_meta_data(const std::string & key,  const std::string & val,const std::string & dset_name);
  void add_meta_data(const std::string & key, int val,const std::string & dset_name);
  void add_meta_data(const std::string & key, unsigned int val,const std::string & dset_name);
  void add_meta_data(const Md_store * md_store,const std::string & dset_name);

  void get_dset(std::vector<int> & data,std::vector<unsigned int> & dims, const std::string & dset_name) const;
  void get_dset(std::vector<unsigned int> & data, std::vector<unsigned int> & dims,const std::string & dset_name) const;
  void get_dset(std::vector<float> & data, std::vector<unsigned int> & dims, const std::string & dset_name) const;
  void get_dset_info(std::vector<int> & dims,V_TYPE& vt ,const std::string & dset_name) const;
  float get_meta_data(const std::string & key, float & val);

  Tuple<float,3> get_meta_data(const std::string & key,  Tuple<float,3> & val);
  Tuple<float,2> get_meta_data(const std::string & key,  Tuple<float,2>& val);
  std::string get_meta_data(const std::string & key,  std::string & val);
  int get_meta_data(const std::string & key, int & val);
  unsigned int get_meta_data(const std::string & key, unsigned int& val);
  Md_store& get_meta_data(Md_store & md_store);
  float get_meta_data(const std::string & key, float& val,const std::string & dset_name);
  Tuple<float,3>  get_meta_data(const std::string & key, Tuple<float,3> & val,const std::string & dset_name);
  Tuple<float,2> get_meta_data(const std::string & key, Tuple<float,2>& val,const std::string & dset_name);
  std::string get_meta_data(const std::string & key,  std::string & val,const std::string & dset_name);
  int get_meta_data(const std::string & key, int &val,const std::string & dset_name);
  Md_store& get_meta_data(Md_store & md_store,const std::string & dset_name);

} ;


}

