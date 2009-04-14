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
#include "hash_box.h"
#include "hash_shelf.h"
#include "hash_case.h"
#include <algorithm>
#include "math.h"
#include <list>


using namespace tracking;
using std::list;

void vec_print(vector<double> in){
  for(unsigned int j = 0 ; j<in.size(); j++)
    cout<<in.at(j)<<"\t";
  cout<<endl;
}

namespace{
///Local struct of structs for use with the transform operators
struct mymath{
  ///Local struct for use with the transform operators
  struct mysub{
    double sub;
    double operator()(double in){
      return in-sub;
    }
    double operator()(double a, double b){
      
      return sqrt(pow(a,2)+pow(b,2));
    }
    mysub(){sub = 0;}
  }myobject;
  

  ///Local struct for use with the transform operators
  struct myarea{
    //figure out how to make this a static
    double pi;
    double operator()(double a, double b){
      
      //      return (pow(b,2)-pow(a,2))*pi;
      return (b*b-a*a)*pi;
    }
    myarea(){
      pi= 4*atan(1.0);
    }
  }myaob;

  

///Local struct for use with the transform operators
  struct mydiv{
    double div;
    double operator()(double a){return a/div;}
    double operator()(double a, double b){
      return a/b;
    }
    mydiv(){div = 1;};
  }mydob;
  

///Local struct for use with the transform operators
  struct myadd{
    double add;
    double operator()(double a){return a+add;}
    double operator()(double a, double b){
      return a+b;
    }
    myadd(){add = 0;};
  }myaddob;
  mymath(){
    
  };
}mymathobs;
}
//}
// int hash_box::gofr(double max_d, int nbins, hash_box* points,
// 		   vector<double>& bin_count,vector<double>& bin_r){
//   bin_count.clear();
//   bin_count.resize(nbins);
//   bin_r.clear();
//   bin_r.resize(nbins);
//   for(int j = 0; j<(int)bin_r.size();j++)
//     bin_r.at(j) = j*max_d/nbins;

//   cout<<"shouldn't be using this one"<<endl;
//   return gofr( max_d,  nbins,  points, bin_count);

// }

// int hash_box::gofr_norm_a(double max_d, unsigned int nbins, hash_box* points,
// 			  vector<double>& bin_count,vector<double>& bin_r){
//   cout<<"shouldn't be using this one"<<endl;
//   int count = gofr(max_d, nbins, points, bin_count, bin_r);
//   vector<double>area;
//   area.resize(bin_r.size());

//   //vec_print(bin_r);
//   bin_r.push_back(max_d);
//   //vec_print(bin_r);
//   transform(bin_r.begin(),bin_r.end()-1,bin_r.begin()+1,area.begin(),mymathobs.myaob);
//   //vec_print(bin_r);
  
  
//   transform(bin_count.begin(), bin_count.end(), area.begin(),
// 	    bin_count.begin(),mymathobs.mydob);
//   //vec_print(bin_r);
//   bin_r.pop_back();
//   //vec_print(bin_r);
//   return count;
  
// }

// int hash_shelf::gofr(double max_d, int nbins, vector<double>& bin_count,
// 		      vector<double>& bin_r) const{
//   cout<<"shouldn't be using this one"<<endl;
//   int count = 0;

//   bin_count.clear();
//   bin_count.resize(nbins);

//   bin_r.clear();
//   bin_r.resize(nbins);

  

//   for(int j = 0; j<(int)bin_r.size();j++)
//     bin_r.at(j) = j*max_d
//       /nbins;

//   gofr(max_d, nbins, bin_count, bin_r,count);
//   return count;
  
// }


// void hash_shelf::gofr_norm(double max_d, int nbins, vector<double>& bin_count,
// 		      vector<double>& bin_r) const{
//   cout<<"shouldn't be using this one"<<endl;
//   int count =   gofr(max_d, nbins,bin_count,bin_r);

//   mymathobs.mydob.div = count ;
				      
//   //average over number of particles
//   transform(bin_count.begin(), bin_count.end(), bin_count.begin(),mymathobs.mydob);

// }




void hash_case::gofr_norm(double max_d, int nbins,
			  vector<double>& bin_count,vector<double>& bin_r) const{
  
  int count = 0;
  double dens = 0;
  bin_count.clear();
  bin_count.resize(nbins);

  bin_r.clear();
  bin_r.resize(nbins);
  for(int j = 0; j<(int)bin_r.size();j++)
    bin_r.at(j) = j*max_d /nbins;
  for(int j = 0; j<(int)bin_r.size();j++)
    bin_count.at(j) = 0;
  
  //compute the area of each ring, this requires a point at
  //max_d which is added and removed
  vector<double>area(bin_r.size());
  bin_r.push_back(max_d);
  transform(bin_r.begin(),bin_r.end()-1,
	    bin_r.begin()+1,
	    area.begin(),
	    mymathobs.myaob);
  bin_r.pop_back();
  

  
  for(vector<hash_shelf*>::const_iterator it = h_case_.begin();
      it != h_case_.end(); ++it)
    {
      dens += ((*it)->gofr(max_d, nbins, bin_count,count));
     
    }

  
				      

  cout<<"total count "<<count<<endl;
  cout<<"average density "<<dens/h_case_.size()<<endl;

    transform(bin_count.begin(), bin_count.end(), 
   	    area.begin(),
    	    bin_count.begin(),
   	    mymathobs.mydob);
  
    //average over number of particles
    mymathobs.mydob.div = count ;
    transform(bin_count.begin(), bin_count.end(), bin_count.begin(),mymathobs.mydob);
    // //normalize by density
    mymathobs.mydob.div = dens/h_case_.size() ;
    transform(bin_count.begin(), bin_count.end(), bin_count.begin(),mymathobs.mydob);
  
}


double hash_shelf::gofr(double max_d, int nbins, vector<double>& bin_count,
			int & count) const{
  //this needs some extra code to cope with edge cases between this
  // and making the hash table something better needs to be done about
  // how the size mismatch for the image and the hash table are handled
  unsigned int buffer = (unsigned int)((int)max_d%(int)ppb==0?max_d/ppb:(1+max_d/ppb));
  int local_count = 0;

  hash_box*  working_box;
  hash_box working_region = hash_box();

  


  

  // the hack to have asymmetric buffers is here
  for(unsigned int x = buffer; x<(hash_dims_[0]-buffer-1);++x){
    for(unsigned int y = buffer; y<(hash_dims_[1]-buffer-1);++y){
      working_box = get_box(x,y);
      get_region(x,y,&working_region,buffer);

      local_count += working_box->gofr(max_d,nbins,&working_region,bin_count);
      working_region.clear();
    }
  }




  // devide by the
  //average density.  The average density used is the density with in
  // the non-exlcuded area.  If you have nice uniform samples then
  //this should be fine.
  //div = 1 * count/((number of boxes)(area of a box))
  //div = 1 * (count /((hash_dims[0]-buffer)(hash_dims[1]-buffer)(area of box)))

  count += local_count;
  //  cout<<count<<endl;
  return local_count /(double)(ppb*ppb*(hash_dims_[0]-2*buffer -1)*(hash_dims_[1]-2*buffer-1));



}


int hash_box::gofr(double max_d, int nbins, hash_box* points,
		   vector<double>& bin_count){



  double max_d_sqr = max_d * max_d;
  double tmp_dist = 0;

  vector<particle_base*>::iterator box_it, points_it;

  for(box_it = contents_.begin(); box_it!=contents_.end(); ++box_it)
    {
      
      for(points_it = (points->contents_).begin(); 
	points_it !=(points->contents_).end();++points_it )
	{
   	  tmp_dist = (*box_it)->distancesq(*points_it);
//   	  (*box_it)->print();
//   	  (*points_it)->print();
//   	  cout<<"distance sqr: "<<tmp_dist<<endl;
// 	  cout<<"max distance sqr: "<<max_d_sqr<<endl;
	  if((tmp_dist < max_d_sqr) && (tmp_dist !=0))
	    {
	      bin_count.at((int)(((sqrt(tmp_dist))/max_d)*nbins))++;
	
	    }
	  else
	    {

	    }
	}

    }

  return contents_.size(); 
}
	     
