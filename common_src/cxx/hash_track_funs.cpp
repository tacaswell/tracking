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
//see website for additional permissions under GNU GPL version 3 section 7


void hash_case::D_lots(utilities::Coarse_grain_array & Duu,
		       utilities::Coarse_grain_array & DuuL,
		       utilities::Coarse_grain_array & DuuT,
		       utilities::Coarse_grain_array & Ddrdr,
// 		       utilities::Coarse_grain_array & Dxx,
// 		       utilities::Coarse_grain_array & Dyy,
		       utilities::Coarse_grain_array & Ddudu,
		       utilities::Counted_vector const& md 
		       )const
{
  for(vector<hash_shelf*>::const_iterator shelf_it = h_case_.begin();
      shelf_it!= h_case_.end();++shelf_it)
  {
//     (*shelf_it)->D_lots(Duu,DuuL,DuuT,Ddrdr,Dxx,Dyy,Ddudu,md);
    (*shelf_it)->D_lots(Duu,DuuL,DuuT,Ddrdr,Ddudu,md);
  }

}

void hash_shelf::D_lots(utilities::Coarse_grain_array & Duu,
			utilities::Coarse_grain_array & DuuL,
			utilities::Coarse_grain_array & DuuT,
			utilities::Coarse_grain_array & Ddrdr,
// 			utilities::Coarse_grain_array & Dxx,
// 			utilities::Coarse_grain_array & Dyy,
			utilities::Coarse_grain_array & Ddudu,
			utilities::Counted_vector const& md 
			)const
{

  if(!(md.averaged()))
  {
    throw "need an averaged md";
  }
    
  list<particle_track*> current_box;
  list<particle_track*> current_region;
  
  int max_r_int = (int) ceil(Duu.get_r_max());
  int buffer = (max_r_int%ppb == 0)?(max_r_int/ppb):(max_r_int/ppb + 1);
  int max_tau = Duu.get_d_bins();
  float max_sep = Duu.get_r_max();
  float min_sep = Duu.get_r_min();
  
  

  for(int j = 0; j<(int)hash_.size(); ++j)
  {

    get_region(j,current_region,buffer);
    hash_[j]->box_to_list(current_box);
    


    // remove particles with out tracks as they are
    // useless for this
    for(list<particle_track*>::iterator it = current_region.begin();
	it != current_region.end();)
    {
      if((*it)->has_track())
      {
	++it;
      }
      else
      {
	current_region.erase(it++);
      }
      
    }
    for(list<particle_track*>::iterator it = current_box.begin();
	it != current_box.end();)
    {
      if((*it)->has_track())
      {
	track_box* cur_track = (*it)->get_track();
	if ((*it) == (cur_track->get_first()))
	{
	  ++it;
	}
	else
	{
	  current_box.erase(it++);
	}
      }
      else
      {
	current_region.erase(it++);
      }
    }


    for(list<particle_track*>::const_iterator box_part = current_box.begin();
	box_part != current_box.end();++box_part)
    {
      for(list<particle_track*>::const_iterator region_part = current_region.begin();
	  region_part!= current_region.end();++region_part)
      {
	
	const particle_track* box_part_ptr = *box_part;
	const particle_track* region_part_ptr = *region_part;
	float sep_r;
	

	// only compute the correlations once for each pair that
	// starts in this frame
	if((region_part_ptr == ((region_part_ptr)->get_track())->get_first()) &&  
	   (((region_part_ptr)->get_position())[0] < ((box_part_ptr)->get_position())[0]))
	{
	  continue;
	}

	
	// ignore self
	if((region_part_ptr)== (box_part_ptr) )
	{
	  continue;
	}
	
	 sep_r = sqrt(box_part_ptr->distancesq(region_part_ptr));
	if((sep_r>max_sep) || (sep_r<min_sep))
	{
	  continue;
	}


	// the positions with any drift removed
	Tuple pos_box_i    = (box_part_ptr)->get_position() - cumulative_disp_;;
 	Tuple pos_region_i = (region_part_ptr)->get_position() - cumulative_disp_;;
	Tuple Rij          = pos_region_i - pos_box_i;
	Rij.make_unit();
	

	// 	// the center with no drift
	// 	Tuple center = pos_box_i +pos_region_i;
	// 	center/=2;
	
	// 	// need to add this frames drift back to the center to find the radius in this plane
	// 	float r_box_i = (box_part_ptr)->get_r(center + cumulative_disp_);
	// 	float r_region_i = (region_part_ptr)->get_r(center + cumulative_disp_);
	
	// 	// need to add this frames drift back to the center to find the angle in this plane
	// 	float theta_box_i = (box_part_ptr)->get_theta(center + cumulative_disp_);
	// 	float theta_region_i = (region_part_ptr)->get_theta(center + cumulative_disp_);

	// figure out the maximum step that can be taken
	int box_part_trk_len = ((box_part_ptr)->get_track())->get_length();
	int region_part_trk_len = ((region_part_ptr)->get_track())->get_length();
	int max_step = (box_part_trk_len < region_part_trk_len)? box_part_trk_len : region_part_trk_len;
	max_step = (max_step<max_tau)?max_step:max_tau;
	

	for(int tau = 1; tau<=max_step;++tau)
	{
	  // the pointers that will walk along the tracks
	  const particle_track * tmp_box_part = *box_part;
	  const particle_track * tmp_region_part = *region_part;

	  // take the first step
	  bool more_box_track = tmp_region_part->step_forwards(tau,tmp_region_part);
	  bool more_region_track = tmp_box_part->step_forwards(tau,tmp_box_part);
	  float u_bar = md.get_val(tau-1);
	  

	  while (more_region_track && more_box_track)
 	  {
	    // get the amount of drift in the frame we have stepped to
	    const Tuple displacement_correction= ((tmp_box_part->get_shelf())->get_cum_forward_disp());
	    //  // and find the center of the pair in this frame

	    
	    
	    Tuple pos_box_tau    = ((tmp_box_part)->get_position())   -displacement_correction;
	    Tuple pos_region_tau = ((tmp_region_part)->get_position())-displacement_correction;
	    
	    Tuple u_box_tau =  (pos_box_tau - pos_box_i);
	    Tuple u_region_tau = (pos_region_tau - pos_region_i);
	    
	      
	

// 	    Duu.add_to_element(sep_r,tau-1,(u_box_tau.dot(u_region_tau))/(u_box_tau.magnitude()*u_region_tau.magnitude()));
	    // tac 2009-06-29
	    // removed the normalization
	    Duu.add_to_element(sep_r,tau-1,(u_box_tau.dot(u_region_tau)));
	    float u_box_T= (u_box_tau.dot(Rij));
	    float u_region_T = (u_region_tau.dot(Rij));

	    DuuT.add_to_element(sep_r,tau-1, (u_box_T) * (u_region_T));
	    DuuL.add_to_element(sep_r,tau-1, (u_box_tau-(Rij*u_box_T)).dot((u_region_tau-(Rij*u_region_T))));

	    // mobility correlation
	    Ddrdr.add_to_element(sep_r,tau-1,(u_box_tau.magnitude())*(u_region_tau.magnitude()));
	    Ddudu.add_to_element(sep_r,tau-1,(u_box_tau.magnitude() - u_bar)*(u_region_tau.magnitude() - u_bar));
	      
	      
	    more_region_track = tmp_region_part->step_forwards(tau,tmp_region_part);
	    more_box_track    = tmp_box_part   ->step_forwards(tau,tmp_box_part);
	      
	    
	  }
	  
	  
	}

      }
    }
    
  } // loop over boxes
    

}
void hash_shelf::D_rr(utilities::Coarse_grain_array & Drr)const
{
  list<particle_track*> current_box;
  list<particle_track*> current_region;
  Tuple center;
  int max_r_int = (int) ceil(Drr.get_r_max());
  int buffer = (max_r_int%ppb == 0)?(max_r_int/ppb):(max_r_int/ppb + 1);
  int max_tau = Drr.get_d_bins();
  float max_sep = Drr.get_r_max();
  float min_sep = Drr.get_r_min();
  
  

  for(int j = 0; j<(int)hash_.size(); ++j)
  {
    get_region(j,current_region,buffer);
    hash_[j]->box_to_list(current_box);

    // remove particles with out tracks as they are
    // useless for this
    for(list<particle_track*>::iterator it = current_region.begin();
	it != current_region.end();)
    {
      if((*it)->has_track())
      {
	++it;
      }
      else
      {
	current_region.erase(it++);
      }
      
    }
    for(list<particle_track*>::iterator it = current_box.begin();
	it != current_box.end();)
    {
      if((*it)->has_track())
      {
	track_box* cur_track = (*it)->get_track();
	if ((*it) == (cur_track->get_first()))
	{
	  ++it;
	}
	else
	{
	  current_box.erase(it++);
	}
      }
      else
      {
	current_region.erase(it++);
      }
    }
    
    for(list<particle_track*>::const_iterator box_part = current_box.begin();
	box_part != current_box.end();++box_part)
    {
      for(list<particle_track*>::const_iterator region_part = current_region.begin();
	  region_part!= current_region.end();++region_part)
      {
	
	const particle_track* box_part_ptr = *box_part;
	const particle_track* region_part_ptr = *region_part;

	// only compute the correlations once for each pair that
	// starts in this frame
	if((region_part_ptr == ((region_part_ptr)->get_track())->get_first()) &&  
	   (((region_part_ptr)->get_position())[0] < ((box_part_ptr)->get_position())[0]))
	{
	  continue;
	}
	// ignore self
	if((region_part_ptr)== (box_part_ptr) )
	{
	  continue;
	}
	
	float sep_r = sqrt(box_part_ptr->distancesq(region_part_ptr));
	if((sep_r>max_sep) || (sep_r<min_sep))
	{
	  continue;
	}
	center = ((box_part_ptr)->get_position ()  + (region_part_ptr)->get_position ());
	center/=2;
	
	
	float box_r_i = (box_part_ptr)->get_r(center);
	float region_r_i = (region_part_ptr)->get_r(center);
		
	int box_part_trk_len = ((box_part_ptr)->get_track())->get_length();
	int region_part_trk_len = ((region_part_ptr)->get_track())->get_length();
	int max_step = (box_part_trk_len < region_part_trk_len)? box_part_trk_len : region_part_trk_len;
	
	max_step = (max_step<max_tau)?max_step:max_tau;
	

	for(int tau = 1; tau<max_step;++tau)
	{
	  const particle_track * tmp_box_part = *box_part;
	  const particle_track * tmp_region_part = *region_part;
	  while (true)
	  {
	    try
	    {
	      tmp_region_part = tmp_region_part->step_forwards(tau);
	      tmp_box_part    = tmp_box_part   ->step_forwards(tau);
	      const Tuple displacement_correction = 
		(tmp_box_part->get_shelf())->get_cum_forward_disp();
	      float box_r_tau = (tmp_box_part)->
		get_r(center + (displacement_correction - cumulative_disp_));
	      float region_r_tau = (tmp_region_part)->get_r(center);
	      Drr.add_to_element(sep_r,tau-1,
				 (box_r_tau - box_r_i) * (region_r_tau - region_r_i));
	      
	    }
	    catch(Ll_range_error& e)
	    {
	      break;
	    }
	  }
	}
      }
    }
  } // loop over boxes
}


void hash_case::D_rr(utilities::Coarse_grain_array& D)const
{
  for(vector<hash_shelf*>::const_iterator shelf_it = h_case_.begin();
      shelf_it!= h_case_.end();++shelf_it)
  {
    (*shelf_it)->D_rr(D);
  }
}



void hash_case::link(float max_range, track_shelf& tracks){

  //doing this rehash takes a long time for large data sets, better to just
  //get it right begin with
  //this->rehash((int)(1.2*max_range));

  //  cout<<"rehashed"<<endl;

  vector<hash_shelf*>::iterator it = h_case_.begin();

  //generate first list
  list<particle_track*>* t_list = (*it)->shelf_to_list();

  //  cout<<"generated list 1"<<endl;
  //fill first track pos_link_next
  fill_pos_link_next(t_list,++it,max_range);
  //  cout<<"get links "<<endl;

  for(list<particle_track*>::iterator it2 = t_list ->begin();
      it2!=t_list->end(); it2++)
    tracks.add_new_track(*it2);
  
  //  cout<<"populated initial tracks"<<endl;
  
  //  tracks.print();
  
  //make local track_list object
  track_list tracking(t_list, max_range,&tracks);
  
  int stupid_counter = 0;
  //cout<<"here"<<endl;
  //loop over shelves
  while( it<(h_case_.end()-1))
  {
    ++stupid_counter;
    if(stupid_counter%50==0)
      cout<<stupid_counter<<endl;;
    //generate next list
    t_list = (*it)->shelf_to_list();
    //    cout<<": "<<t_list->size()<<endl;
    
    //fill next list's pos_link_next
    fill_pos_link_next(t_list,++it,max_range);
    


    //shove in to track_list
    tracking.link_next(t_list);
  
  }
  //make last list
  t_list = (*it)->shelf_to_list();
  tracking.link_next(t_list);
  //  cout<<"here"<<endl;
}
