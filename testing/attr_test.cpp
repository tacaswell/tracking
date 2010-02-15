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
//

#include <iostream>
#include <string>
#include <stdexcept>
#include "attr_list_hdf.h"
#include "H5Cpp.h"
#include "pair.h"
#include "triple.h"
using std::cout;
using std::cerr;
using std::endl;
using std::string;

using H5::H5File;
using H5::Group;

using utilities::Attr_list_hdf;
using utilities::Pair;
using utilities::Triple;


int main()
{
  
  try
  {
    
    cout<<"Hello World"<<endl;
    
    string file_name_ = "attr_test.h5";
    
    H5File file = H5File( file_name_, H5F_ACC_RDWR );  
    
    Group g = file.openGroup("/");
    Attr_list_hdf al(&g);
    al.print();
    
    cout<<al.contains_attr("test_attr")<<endl;
    cout<<al.contains_attr("test_attr2")<<endl;
   
    cout<<"end contain test"<<endl;
    
    int n = 0;
    cout<<"try 1:"<<endl;
    
    cout<<al.get_value("test_attr",n)<<endl;;
    int n4 = 13;
    
    al.set_value("attr3", n4);
    cout<<al.contains_attr("attr3")<<endl;
    cout<<al.get_value("attr3",n)<<endl;;
    
    n4 = 123;
    
    al.set_value("attr3", n4,true);
    cout<<al.get_value("attr3",n)<<endl;

    float f = 0;
    
    al.get_value("test_attrf",f);
    cout<<"float value "<<f<<endl;
    al.set_value("test_attrf",(float)654.321,true);
    
    al.get_value("test_attrf",f);
    cout<<"float value "<<f<<endl;
    
    al.set_value("test_attrf2",(float)3.14159);
    
    al.get_value("test_attrf2",f);
    cout<<"float value "<<f<<endl;

    std::string str,str2;
    str2 = al.get_value("test_str",str);
    cout<<"string attr: "<<str<<endl;
    cout<<"string attr return: "<<str2<<endl;
    
    al.set_value("test_str","over write test",true);



    cout<<"wrote"<<endl;
    str.clear();
    str2.clear();
    
    str2 = al.get_value("test_str",str);
    cout<<"string attr: "<<str<<endl;
    cout<<"string attr return: "<<str2<<endl;
    cout<<"---------"<<endl;
    
    
    string in("write test");
    al.set_value("test_str2",in);
    str.clear();
    str2.clear();
    cout<<"string attr: "<<str<<endl;
    cout<<"string attr return: "<<str2<<endl;
    str2 = al.get_value("test_str2",str);
    cout<<"expect: "<<in<<endl;
    cout<<"string attr: "<<str<<endl;
    cout<<"string attr return: "<<str2<<endl;


    cout<<"=== Pair Test ==="<<endl;
    Pair p,p2;
    cout<<"--- Read test"<<endl;
    cout<<"expect: (1.2,3.4)"<<endl;
    p2 = al.get_value("pair_test",p);
    cout<<"ref val: "<<p<<endl;
    cout<<"ret val: "<<p2<<endl;

    p.clear();
    p2.clear();

    cout<<"--- Write test"<<endl;
    al.set_value("pair_w_test", Pair(5.6,7.8));
    cout<<"expect: (5.6,7.8)"<<endl;
    p2 = al.get_value("pair_w_test",p);
    cout<<"ref val: "<<p<<endl;
    cout<<"ret val: "<<p2<<endl;
    
    p.clear();
    p2.clear();

    cout<<"--- Over write test"<<endl;
    al.set_value("pair_test", Pair(9.0,1.2),true);
    cout<<"expect: (9.0,1.2)"<<endl;
    p2 = al.get_value("pair_test",p);
    cout<<"ref val: "<<p<<endl;
    cout<<"ret val: "<<p2<<endl;


    cout<<"=== Triple Test ==="<<endl;
    Triple t,t2;
    cout<<"--- Read test"<<endl;
    cout<<"expect: (1.2,3.4,5.6)"<<endl;
    t2 = al.get_value("triple_test",t);
    cout<<"ref val: "<<t<<endl;
    cout<<"ret val: "<<t2<<endl;

    t.clear();
    t2.clear();

    cout<<"--- Write test"<<endl;
    al.set_value("triple_w_test", Triple(7.8,9.0,1.2));
    cout<<"expect: (7.8,9.0,1.2)"<<endl;
    t2 = al.get_value("triple_w_test",t);
    cout<<"ref val: "<<t<<endl;
    cout<<"ret val: "<<t2<<endl;
    
    t.clear();
    t2.clear();

    cout<<"--- Over write test"<<endl;
    al.set_value("triple_test", Triple(3.4,5.6,7.8),true);
    cout<<"expect: (3.4,5.6,7.8)"<<endl;
    t2 = al.get_value("triple_test",t);
    cout<<"ref val: "<<t<<endl;
    cout<<"ret val: "<<t2<<endl;

  }
  catch(H5::Exception & e)
  {

    
    e.printError();
    
  }
  catch(std::exception & e)
  {
    cerr<<e.what()<<endl;
  }
  
  

  return -1;
}
