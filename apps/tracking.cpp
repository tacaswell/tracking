//Copyright 2008-2010 Thomas A Caswell
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



#include "master_box_t.h"

#include "particle_base.h"
#include "hash_case.h"

#include "wrapper_o_hdf.h"
#include "wrapper_i_hdf.h"
#include "filter.h"

#include "generic_wrapper_hdf.h"

#include <unistd.h>
#include <vector>

#include "read_config.h"
#include "cl_parse.h"
#include "md_store.h"
#include "track_shelf.h"
#include "sql_handler.h"


using std::string;
using std::vector;


using std::cout;
using std::endl;
using std::set;
using std::string;
using std::cerr;
using std::vector;
using std::map;

using std::logic_error;
using std::exception;


using utilities::Wrapper_o_hdf;
using utilities::Wrapper_i_hdf;
using utilities::Md_store;
using utilities::SQL_handler;


using utilities::Filter_basic;
using utilities::Filter_trivial;
using utilities::D_TYPE;
using utilities::Generic_wrapper_hdf;

using utilities::Read_config;
using utilities::CL_parse;



using namespace tracking;


static string APP_NAME = "tracking :: ";

int main(int argc, char * const argv[])
{
  
  string pram_file ;

    
  // Extract the input and output files from the command line arguements
  try
  {
    CL_parse cl(argc,argv);
    cl.parse();
    cl.get_fpram(pram_file);
  }
  catch(std::invalid_argument &e )
  {
    cout<<e.what()<<endl;
    return -1;
  }
  

  // parse out the standard logistic parameters
  int read_comp_num, write_comp_num,dset_key;
  Read_config comp_prams(pram_file,"comps");
  try
  {
    comp_prams.get_value("iden_key",read_comp_num);
    comp_prams.get_value("dset_key",dset_key);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the computation numbers"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }

  
  // parse out the parameters that this application needs
  float search_range;
  Read_config app_prams(pram_file,"tracking");
  try
  {
    app_prams.get_value("search_range",search_range);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the apps parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }
  
  // parse out the files
  string db_path;
  string in_file ;
  string out_file ;
  Read_config files(pram_file,"files");
  try
  {
    files.get_value("db_path",db_path);
    files.get_value("fin",in_file);
    files.get_value("fout",out_file);
  }
  catch(logic_error & e)
  {
    cerr<<"error parsing the file parameters"<<endl;
    cerr<<e.what()<<endl;
    return -1;
  }


  



    
    

  //nonsense to get the map set up
  map<utilities::D_TYPE, int> contents;
  utilities::D_TYPE tmp[] = {utilities::D_XPOS,
			     utilities::D_YPOS,
			     utilities::D_DX,
			     utilities::D_DY,
			     utilities::D_R2,
			     utilities::D_E};
  set<D_TYPE> data_types = set<D_TYPE>(tmp,tmp+6);
    
    

  
  // set up sql stuff
  SQL_handler sql;
  sql.open_connection(db_path);


  // claim the entry in the DB and lock it before doing anything

  sql.start_comp(dset_key,write_comp_num,utilities::F_TRACKING);


  
  // print what will be done
  cout<<"file to read in: "<<in_file<<endl;
  cout<<"file that will be written to: "<<out_file<<endl;
  cout<<"Parameters: "<<endl;
  cout<<"  search_range: "<<search_range<<endl;
  cout<<"comps: "<<endl;
  cout<<"  read_comp_num "<<read_comp_num<<endl;
  cout<<"  write_comp_num "<<write_comp_num<<endl;

    
    
  // set up the input wrapper
  Wrapper_i_hdf wh(in_file,data_types,read_comp_num);
    
    
  Master_box box;
  // filter based on the values stored with the initial computation
  

  Filter_basic filt;
  if(app_prams.contains_key("e_cut") &&
     app_prams.contains_key("rg_cut") &&
     app_prams.contains_key("shift_cut"))
  {
    float e_cut,rg_cut,shift_cut;
    app_prams.get_value("e_cut",e_cut);
    app_prams.get_value("rg_cut",rg_cut);
    app_prams.get_value("shift_cut",shift_cut);
    filt.init(e_cut,rg_cut,shift_cut);
  }
  else
    filt.init(in_file,read_comp_num);

  Md_store filt_md = filt.get_parameters();
  
      
  


  // build the md_store for handing in to the hdf file and the db
  Md_store md_store;
  md_store.add_elements(app_prams.get_store());    
  md_store.add_elements(comp_prams.get_store());    
  md_store.add_elements(files.get_store());    
  md_store.add_elements(&filt_md);
  md_store.add_element("comp_key",write_comp_num);
  cout<<"set up md_store"<<endl;
  md_store.print();



  // fill the master_box
  box.init(wh,filt);

  
  
    
  // fill the hash case
  hash_case hcase;
  hcase.init(box,wh.get_dims(),search_range,wh.get_num_frames());
  int dtime = hcase.get_avg_dtime();
  float temperature = hcase.get_avg_temp();
  md_store.add_element("dtime",dtime);
  md_store.add_element("temperature",temperature);

  cout<<"hash case filled"<<endl;

    
  Track_shelf tracks;
  // link tracks together
  hcase.link(search_range,tracks);
  cout<<"after searching found "<<tracks.get_track_count()<<" tracks"<<endl;
  // remove tracks with one element
  tracks.remove_short_tracks(2);
  // renumber to get rid of gaps
  tracks.renumber();
    
    
    
  set<D_TYPE> d2;
  d2.insert(utilities::D_NEXT_INDX);
  d2.insert(utilities::D_PREV_INDX);
  d2.insert(utilities::D_TRACKID);
    
  // set up output wrapper
  Wrapper_o_hdf particle_data_wrapper(out_file,d2,write_comp_num,
				      Wrapper_o_hdf::APPEND_FILE,
				      "frame");
    
    

  cout<<"made output wrapper"<<endl;
  try
  {
    particle_data_wrapper.initialize_wrapper();
    cout<<"inited wrapper"<<endl;
    hcase.output_to_wrapper(particle_data_wrapper);
    particle_data_wrapper.add_meta_data_list(app_prams,d2);
    particle_data_wrapper.finalize_wrapper();
  }
  catch(exception & err)
  {
    std::cerr<<"caught on error outputting to particle data: "<<err.what()<<endl;
    return -1;
      
  }
    
  cout<<"finished outputting to particle data"<<endl;
  
  
  
  
    
  // shove in to hdf
  try
  {
    Generic_wrapper_hdf track_data_wrapper(out_file,true);
    tracks.output_to_wrapper(track_data_wrapper,&md_store);
  }
  catch(exception & err)
  {
    std::cerr<<"caught on error outputting track data: "<<err.what()<<endl;
    return -1;
      
  }
  // shove in to db
  sql.add_mdata(md_store);
  sql.commit();
  
  // clean up the data base
  sql.close_connection();
  
  
    
    
  
  return 0;
}
