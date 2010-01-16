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
//Additional permission under GNU GPL version 3 section 7
//
//If you modify this Program, or any covered work, by linking or
//combining it with MATLAB (or a modified version of that library),
//containing parts covered by the terms of MATLAB User License, the
//licensors of this Program grant you additional permission to convey
//the resulting work.

// this file is closely related to http://www.yolinux.com/TUTORIALS/XML-Xerces-C.html

#include "read_config.h"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <string>
#include <stdexcept>

#include <iostream>
#include <sstream>


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


using utilities::Read_config;

using std::string;
using std::vector;

using std::cerr;
using std::cout;
using std::endl;

using xercesc::XMLPlatformUtils;
using xercesc::XMLString;
using xercesc::DOMElement;
using xercesc::DOMNode;
using xercesc::DOMDocument;
using xercesc::DOMNodeList;

using xercesc::XercesDOMParser;

//taken from
//http://www.codeguru.com/forum/showthread.php?t=231054
// the third parameter of from_string() should be 
// one of std::hex, std::dec or std::oct
template <class T>
bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}



Read_config::Read_config(std::string fname,vector<string> attr_names, string elm_str):
  attr_names_(attr_names),
  attr_values_(attr_names.size()),
  attr_found_ (attr_names.size())
{

  // Test to see if the file is ok.

  struct stat fileStatus;

  int iretStat = stat(fname.c_str(), &fileStatus);
  if( iretStat == ENOENT )
    throw ( std::runtime_error("Path file_name does not exist, or path is an empty string.") );
  else if( iretStat == ENOTDIR )
    throw ( std::runtime_error("A component of the path is not a directory."));
  else if( iretStat == ELOOP )
    throw ( std::runtime_error("Too many symbolic links encountered while traversing the path."));
  else if( iretStat == EACCES )
    throw ( std::runtime_error("Permission denied."));
  else if( iretStat == ENAMETOOLONG )
    throw ( std::runtime_error("File can not be read\n"));

  // set up xml parser stuff
  try
  {
    XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
  }
  catch( xercesc::XMLException& e )
  {
    char* message = XMLString::transcode( e.getMessage() );
    cerr << "XML toolkit initialization error: " << message << endl;
    XMLString::release( &message );
    // throw exception here to return ERROR_XERCES_INIT
  }
  

  XMLCh* elm_name = XMLString::transcode(elm_str.c_str());
  
  
  XercesDOMParser *config_parser = new XercesDOMParser;
  
  config_parser->setValidationScheme( XercesDOMParser::Val_Never );
  config_parser->setDoNamespaces( false );
  config_parser->setDoSchema( false );
  config_parser->setLoadExternalDTD( false );

  try
  {
    config_parser->parse(fname.c_str());
    
    DOMDocument * doc = config_parser->getDocument();
    DOMElement* root = doc->getDocumentElement();
    if(!root) throw(std::runtime_error("empty xml"));
    
    DOMNodeList* children = root->getChildNodes();
    const XMLSize_t node_count = children ->getLength();

    for( XMLSize_t j = 0;j<node_count;++j)
    {
      // get current node
      DOMNode* current_node = children->item(j);
      if(current_node  && 	// make sure that current_node is non NULL
	 current_node->getNodeType() == DOMNode::ELEMENT_NODE)
      {
	// cast to dom element
	DOMElement* cur_elm = dynamic_cast<DOMElement*>(current_node);
	// see if this is the element that we want
	if(XMLString::equals(cur_elm->getTagName(),elm_name))
	{
	  int max_name= attr_names_.size();
	  for(int j =0;j<max_name;++j)
	  {
	    XMLCh * attr_tmp = XMLString::transcode(attr_names_[j].c_str());
	    
	    if(cur_elm->hasAttribute(attr_tmp))
	    {

	      char * tmp = XMLString::transcode(cur_elm->getAttribute(attr_tmp));
	      
	      attr_values_.at(j) = string(tmp);
	      attr_found_.at(j) = true;
	      
	      XMLString::release(&tmp);
	      
	    }
	    else
	    {
	      attr_found_.at(j) = false;
	    }
	    XMLString::release(&attr_tmp);
	  }
	}
      }
      
    }
    
      
	   
  }
  catch( xercesc::XMLException& e )
  {
    char* message = xercesc::XMLString::transcode( e.getMessage() );
    std::ostringstream errBuf;
    errBuf << "Error parsing file: " << message << std::flush;
    XMLString::release( &message );
  }
  
  

  // clean up xml stuff
  XMLString::release(&elm_name);
  delete config_parser;

  try
  {
    XMLPlatformUtils::Terminate();  // Terminate Xerces
  }
  catch( xercesc::XMLException& e )
  {
    char* message = xercesc::XMLString::transcode( e.getMessage() );

    cerr << "XML ttolkit teardown error: " << message << endl;
    XMLString::release( &message );
  }



}


void Read_config::print() const
{
  for(unsigned int j = 0; j<attr_names_.size();++j)
  {
    cout<<attr_names_.at(j)<<'\t'<<attr_found_.at(j)<<'\t'<<attr_values_.at(j)<<endl;
    
  }
}

  
Read_config::~Read_config()
{
}

bool Read_config::get_val(string attr_name,float & val)const
{
  int max = attr_names_.size();
  for(int j = 0;j<max;++j)
  {
    if(attr_names_[j].compare(attr_name) == 0)
      if(attr_found_.at(j))
	return from_string<float> (val,attr_values_.at(j),std::dec);
      else
	return false;
  }
  return false;
}

bool Read_config::get_val(string attr_name,int & val)const
{
  int max = attr_names_.size();
  

  for(int j = 0;j<max;++j)
  {
    if(attr_names_[j].compare(attr_name) == 0)
      if(attr_found_.at(j))
	return from_string<int> (val,attr_values_.at(j),std::dec);
      else
	return false;
  }
  return false;
}

bool Read_config::get_val(string attr_name,string & val)const
{
  int max = attr_names_.size();
  

  for(int j = 0;j<max;++j)
  {
    if(attr_names_[j].compare(attr_name) == 0)
      if(attr_found_.at(j))
      {
	val = attr_values_.at(j);
	return true;
      }
      else
	return false;
  }
  return false;
}
