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
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.
//
//If you modify this Program, or any covered work, by linking or
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for FreeImage Public License, the licensors of
//this Program grant you additional permission to convey the resulting
//work.



#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>


#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include "mm_md_parser.h"
#include "FreeImagePlus.h"
#include "md_store.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::logic_error;
using std::runtime_error;


using xercesc::XMLPlatformUtils;
using xercesc::XMLString;
using xercesc::DOMElement;
using xercesc::DOMNode;
using xercesc::DOMDocument;
using xercesc::DOMNodeList;
using xercesc::MemBufInputSource;

using xercesc::XercesDOMParser;

using utilities::Mm_md_parser;
using utilities::Md_store;

void parse_description(string& des,Md_store *store);

// string to make MemBufInputSource happy
static const char*  gMemBufId = "ImageDescription";


Mm_md_parser::Mm_md_parser()
{
    try
  {
    XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
  }
  catch( xercesc::XMLException& e )
  {
    char* message = XMLString::transcode( e.getMessage() );
    cerr << "XML toolkit initialization error: " << message << endl;
    XMLString::release( &message );
    // throw exception here to return ERROR_XERCES_INTI
  }

    
  elm_name_ = XMLString::transcode("PlaneInfo");
  key_str_  = XMLString::transcode("id");
  type_str_ = XMLString::transcode("type");
  value_str_= XMLString::transcode("value");
  pram_str_ = XMLString::transcode("prop");

}



Mm_md_parser::~Mm_md_parser()
{
  XMLString::release(&  elm_name_);
  XMLString::release(&key_str_   );
  XMLString::release(&type_str_  );
  XMLString::release(&value_str_ );
  XMLString::release(&pram_str_  );



  
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



Md_store * Mm_md_parser::parse_md(fipImage & image) const 
{

  Md_store * md_store = new Md_store();
  
  
  fipTag tag;
  image.getMetadata(FIMD_EXIF_MAIN,"ImageDescription",tag);
  

  // this test should make sure the below casts all work as intended
  // as my interpretation of 
  // 	FIDT_ASCII		= 2,	// 8-bit bytes w/ last byte null 
  // from FreeImage.h is that ascii is an unsigned char restricted to 
  // 0-128, which can be safely cast to an unsigned char
  //     typedef unsigned char       XMLByte;
  // from XercesDefs.hpp
  // which is what XMLByte is
  if(tag.getType() != FIDT_ASCII)
    throw runtime_error("The data type of the meta data is not correct (expect ascii" );
  
  const XMLByte*  xml_str = (const XMLByte*)tag.getValue();
  
  MemBufInputSource* memBuf = new MemBufInputSource(xml_str
						    , tag.getLength()
						    , gMemBufId
						    ,false
						    );
  
  
  XercesDOMParser *config_parser = new XercesDOMParser;
  
  try
  {
    
    config_parser->parse(*memBuf);
    
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
	// look for prop elements in the top group
	parse_elements(cur_elm,md_store);
	// see if this is the element that we want
	if(XMLString::equals(cur_elm->getTagName(),elm_name_))
	{
	  DOMNodeList* pram_elms = cur_elm->getChildNodes();
	  const XMLSize_t pram_count =  pram_elms->getLength();
	  for(XMLSize_t k = 0; k<pram_count; ++k)
	  {
	    // parse the prop elements
	    parse_elements(pram_elms->item(k),md_store);
	    
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
  
  delete config_parser;
  config_parser = NULL;
  delete memBuf;
  memBuf = NULL;
  
  return md_store;
  

}





void parse_description(string& des,Md_store* md_store)
{

  string strstr = "string";
  
  size_t start_indx = 0;
  size_t colon_indx = 0;
  size_t end_indx = 0;
  
  int SW = 10;
  
  
  start_indx = des.find("&#13;&#10;",0)+SW;
  
  colon_indx = des.find(": ",start_indx);
  end_indx= des.find("&#13;&#10;",colon_indx);

  while(end_indx != string::npos)
  {

    string key = des.substr(start_indx,colon_indx-start_indx);
    
    string val = des.substr(colon_indx+2,end_indx-colon_indx-2);
    
    // add special handling
    if(false)
    {
    }
    else
    {
      md_store->add_element(key,strstr,val);
      
    }
    
    // step pointers in string
    start_indx = end_indx +SW;
    colon_indx = des.find(": ",start_indx);
    end_indx= des.find("&#13;&#10;",colon_indx);
  }
  
}


void Mm_md_parser::parse_elements(DOMNode* pram_node, Md_store* md_store)const
{
  XMLCh* des_str = XMLString::transcode("Description");

  if(pram_node  && 	// make sure that current_node is non NULL
     pram_node->getNodeType() == DOMNode::ELEMENT_NODE)
  {
    DOMElement* pram_elm = dynamic_cast<DOMElement*>(pram_node);
    if(XMLString::equals(pram_elm->getTagName(),pram_str_))
    {

		
      if(!(pram_elm->hasAttribute(key_str_) &&
	   pram_elm->hasAttribute(type_str_) &&
	   pram_elm->hasAttribute(value_str_)))
	throw logic_error("read_config :: pram entry does not have proper attributes"); 
		
      char * key = XMLString::transcode(pram_elm->getAttribute(key_str_));
      char * type = XMLString::transcode(pram_elm->getAttribute(type_str_));
      char * value = XMLString::transcode(pram_elm->getAttribute(value_str_));

      
      if(XMLString::equals(pram_elm->getAttribute(key_str_),des_str))
      {
	string v(value);
	parse_description(v,md_store);
      }
      else
	md_store->add_element(key,type,value);

      XMLString::release(&key);
      XMLString::release(&type);
      XMLString::release(&value);
    }
  }
  XMLString::release(&des_str);
   
  
}
