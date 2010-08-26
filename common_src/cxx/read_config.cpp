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
#include "enum_utils.h"

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

using std::logic_error;


using xercesc::XMLPlatformUtils;
using xercesc::XMLString;
using xercesc::DOMElement;
using xercesc::DOMNode;
using xercesc::DOMDocument;
using xercesc::DOMNodeList;

using xercesc::XercesDOMParser;


Read_config::Read_config(std::string fname, string elm_str)
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
  XMLCh* key_str = XMLString::transcode("key");
  XMLCh* type_str = XMLString::transcode("type");
  XMLCh* value_str = XMLString::transcode("value");
  XMLCh* pram_str = XMLString::transcode("pram");
    
  
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
	  
	  
	  DOMNodeList* pram_elms = cur_elm->getChildNodes();
	  const XMLSize_t pram_count =  pram_elms->getLength();
	  
	  for(XMLSize_t k = 0; k<pram_count; ++k)
	  {
	    DOMNode* pram_node = pram_elms->item(k);
	    if(pram_node  && 	// make sure that current_node is non NULL
	       pram_node->getNodeType() == DOMNode::ELEMENT_NODE)
	    {
	      DOMElement* pram_elm = dynamic_cast<DOMElement*>(pram_node);
	      if(XMLString::equals(pram_elm->getTagName(),pram_str))
	      {

		
		if(!(pram_elm->hasAttribute(key_str) &&
		     pram_elm->hasAttribute(type_str) &&
		     pram_elm->hasAttribute(value_str)))
		  throw logic_error("read_config :: pram entry does not have proper attributes"); 
		
		char * key = XMLString::transcode(pram_elm->getAttribute(key_str));
		char * type = XMLString::transcode(pram_elm->getAttribute(type_str));
		char * value = XMLString::transcode(pram_elm->getAttribute(value_str));
		

		md_store_.add_element(key,type,value);

		XMLString::release(&key);
		XMLString::release(&type);
		XMLString::release(&value);
	      }
	    }
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
  
  
  XMLString::release(&key_str);
  XMLString::release(&type_str);
  XMLString::release(&value_str);
  XMLString::release(&pram_str);
  
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
  md_store_.print();
  
}

  
Read_config::~Read_config()
{
}

int Read_config::get_key_index(const string& key) const
{
  return md_store_.get_key_index(key);
}


bool Read_config::contains_key(const string& key) const
{
  return md_store_.contains_key(key);
}


template <class T>
T Read_config::get_value(int j,T & val)const
{
    return md_store_.get_value(j,val);
}


template <class T>
T Read_config::get_value(const string& key,T & val)const
{
  int j = md_store_.get_key_index(key);
  return md_store_.get_value(j,val);
  
}

template int Read_config::get_value(const string& key,int & val)const;
template unsigned Read_config::get_value(const string& key,unsigned & val)const;
template float Read_config::get_value(const string& key,float & val)const;
template string Read_config::get_value(const string& key,string & val)const;

template int Read_config::get_value(int j,int & val)const;
template unsigned Read_config::get_value(int j,unsigned & val)const;
template float Read_config::get_value(int j,float & val)const;
template string Read_config::get_value(int j,string & val)const;
