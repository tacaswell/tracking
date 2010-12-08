//Copyright 2010 Thomas A Caswell
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

#include "sql_handler.h"
#include "md_store.h"
#include <string>
#include <iostream>
using utilities::SQL_handler;
using utilities::Md_store;
using utilities::F_TYPE;

using std::string;
using std::cout;
using std::endl;
int test_fun(Md_store & md_store, utilities::F_TYPE f_type,string t_name,SQL_handler& test);


int main()
{ 
  // make object and add set up fake db in memory
  SQL_handler test;
  test.make_test_db();  

  
  // set up the md for the iden test
  Md_store iden_mdata;
  iden_mdata.add_element("dset_key"    ,(int) 1 );
  iden_mdata.add_element("threshold"   ,(float) .1);
  iden_mdata.add_element("hwhm"        ,(float) 1.5);
  iden_mdata.add_element("p_rad"       ,(int) 4  );
  iden_mdata.add_element("d_rad"       ,(int) 3  );
  iden_mdata.add_element("mask_rad"    ,(int) 5  );
  iden_mdata.add_element("top_cut"     ,(float) .001);
  iden_mdata.add_element("frames_avged",(int) 1  )  ;

  // get the iden comp_key
  int comp_key;
  test.start_comp(1,comp_key,utilities::F_IDEN);
  
  iden_mdata.add_element("comp_key",comp_key);
  // shove in md
  test.add_mdata(iden_mdata);
  test.commit();
  // pull it out
  Md_store mdata_out;
  test.get_comp_mdata(comp_key,mdata_out,"iden");
  // make sure they match
  
  iden_mdata.print();
  cout<<"--------------------------------------------------"<<endl;
  mdata_out.print();
  cout<<"=================================================="<<endl;
  
  
  // set up md for tracking test
  Md_store track_mdata;
  track_mdata.add_element("dset_key"    ,(int) 1 );
  track_mdata.add_element("iden_key"   ,(int) comp_key);
  track_mdata.add_element("search_range",(float) 3);
  track_mdata.add_element("shift_cut"       ,(float) 1.5  );
  track_mdata.add_element("rg_cut"       ,(float) 7.5  );
  track_mdata.add_element("e_cut"    ,(float) .5  );


  // get tracking comp_key
  int t_comp_key;
  
  test.start_comp(1,t_comp_key,utilities::F_TRACKING);
  track_mdata.add_element("comp_key",t_comp_key);
  // shove in md
  test.add_mdata(track_mdata);
  test.commit();
  Md_store t_md_out;
  test.get_comp_mdata(t_comp_key, t_md_out,"tracking");
  track_mdata.print();
  cout<<"--------------------------------------------------"<<endl;
  t_md_out.print();
  cout<<"=================================================="<<endl;
  

  // // set up md for msd_sweep test
  // Md_store msd_sweep_mdata;
  // msd_sweep_mdata.add_element("dset_key"    ,(int) 1 );
  // msd_sweep_mdata.add_element("iden_key"   ,(int) comp_key);
  // msd_sweep_mdata.add_element("track_key",(int) t_comp_key);
  // msd_sweep_mdata.add_element("trk_len_min"       ,(int) 2  );
  // msd_sweep_mdata.add_element("trk_len_step"       ,(int) 1  );
  // msd_sweep_mdata.add_element("steps"    ,(int) 15  );



  // int msd_sweep_comp_key;
  // msd_sweep_comp_key = test_fun(msd_sweep_mdata,utilities::F_MSD_SWEEP,"msd_sweep",test);
  
  
  Md_store msd_mdata;
  msd_mdata.add_element("dset_key"    ,(int) 1 );
  msd_mdata.add_element("iden_key"   ,(int) comp_key);
  msd_mdata.add_element("track_key",(int) t_comp_key);
  msd_mdata.add_element("msd_steps"       ,(int) 14  );
  msd_mdata.add_element("min_track_length"       ,(int) 15  );
  msd_mdata.add_element("steps"    ,(int) 15  );
  msd_mdata.add_element("fin"    ,"fin");
  msd_mdata.add_element("fout"    ,"fout");



  int msd_comp_key;
  
  msd_comp_key = test_fun(msd_mdata,utilities::F_MSD,"msd",test);
  


  test.close_connection();
  


}

  
int test_fun(Md_store & md_store, utilities::F_TYPE f_type,string t_name,SQL_handler& test)
{
  int comp_key;
  
  test.start_comp(1,comp_key,f_type);
  md_store.add_element("comp_key",comp_key);
  // shove in md
  test.add_mdata(md_store);
  test.commit();
  Md_store md_out;
  test.get_comp_mdata(comp_key, md_out,t_name);
  md_store.print();
  cout<<"--------------------------------------------------"<<endl;
  md_out.print();
  cout<<"=================================================="<<endl<<endl;
  
  return comp_key;
  
}

