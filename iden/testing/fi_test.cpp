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
#include <iostream>


#include <sstream>

#include <stdexcept>
#include <string>
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

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::logic_error;


using xercesc::XMLPlatformUtils;
using xercesc::XMLString;
using xercesc::DOMElement;
using xercesc::DOMNode;
using xercesc::DOMDocument;
using xercesc::DOMNodeList;
using xercesc::MemBufInputSource;

using xercesc::XercesDOMParser;


#include "FreeImagePlus.h"


struct Config_pram
{
  
    Config_pram(std::string key_i,std::string type_i,std::string value_i)
      :key(key_i),type(type_i),value(value_i){}
    Config_pram(const char * key_i,const char * type_i,const char * value_i)
      :key(key_i),type(type_i),value(value_i){}
    Config_pram();
    ~Config_pram(){};
    std::string key;
    std::string type;
    std::string value;
  

  };

void parse_description(string& des,vector<Config_pram> & prams_);
void parse_elements(DOMNode* pram_elm, vector<Config_pram> & prams_,XMLCh* pram_str,XMLCh* key_str,XMLCh* value_str,XMLCh* type_str);

static string fname = "/home/tcaswell/colloids/data/polyNIPAM_batch_12/20100524/4/exp2/27-7_27-1_27-6_vslow.tif";
static const char*  gMemBufId = "ImageDescription";
int main()
{
    
  FreeImage_Initialise();
  BOOL bMemoryCache = TRUE;
  fipMultiPage src(bMemoryCache);
  // Open src file (read-only, use memory cache)
  src.open(fname.c_str(), FALSE, TRUE);
  unsigned int img_frames = src.getPageCount();
  cout<<img_frames<<endl;
  
  fipImage image;

  image = src.lockPage(0);

  cout<<"meta data count: MAIN  "<<image.getMetadataCount(FIMD_EXIF_MAIN )<<endl;
  cout<<"meta data count: EXIF  "<<image.getMetadataCount(FIMD_EXIF_EXIF )<<endl;
  cout<<"meta data count: XMP   "<<image.getMetadataCount(FIMD_XMP  )<<endl;
  cout<<"meta data count: IPTC  "<<image.getMetadataCount(FIMD_IPTC )<<endl;
  cout<<"meta data count: GOETIFF  "<<image.getMetadataCount(FIMD_GEOTIFF )<<endl;


  fipTag tag;
  fipMetadataFind finder;
  if( finder.findFirstMetadata(FIMD_EXIF_MAIN, image, tag) ) {
    do {
      // process the tag
      cout << tag.getKey() << "\n";
    } while( finder.findNextMetadata(tag) );
  }
  cout<<"useful?"<<endl;
  
  // the class can be called again with another metadata model
  if( finder.findFirstMetadata(FIMD_EXIF_EXIF, image, tag) ) {
    do {
      // process the tag
      cout << tag.getKey() << "\n";
    } while( finder.findNextMetadata(tag) );
  }


  cout<<"try to get at the data"<<endl;
  
  image.getMetadata(FIMD_EXIF_MAIN,"ImageDescription",tag);
  cout<<(const char*)tag.getValue()<<endl;
  const char * xml_str = (const char*)tag.getValue();
  
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
  

  XMLCh* elm_name = XMLString::transcode("PlaneInfo");
  XMLCh* key_str = XMLString::transcode("id");
  XMLCh* type_str = XMLString::transcode("type");
  XMLCh* value_str = XMLString::transcode("value");
  XMLCh* pram_str = XMLString::transcode("prop");
    
    
  XercesDOMParser *config_parser = new XercesDOMParser;
  
  config_parser->setValidationScheme( XercesDOMParser::Val_Never );
  config_parser->setDoNamespaces( false );
  config_parser->setDoSchema( false );
  config_parser->setLoadExternalDTD( false );

  MemBufInputSource* memBuf = new MemBufInputSource((const XMLByte*)xml_str
					       , strlen(xml_str)
					       , gMemBufId
					       ,false
					       );
  
  
  vector<Config_pram> prams_;
  
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
	// look for top level groups
	parse_elements(cur_elm,prams_,pram_str,key_str,value_str,type_str);
	// see if this is the element that we want
	if(XMLString::equals(cur_elm->getTagName(),elm_name))
	{
	  DOMNodeList* pram_elms = cur_elm->getChildNodes();
	  const XMLSize_t pram_count =  pram_elms->getLength();
	  for(XMLSize_t k = 0; k<pram_count; ++k)
	  {
	    parse_elements(pram_elms->item(k),prams_,pram_str,key_str,value_str,type_str);
	    
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
  delete memBuf;
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

  
  for(unsigned int j = 0; j<prams_.size();++j)
    cout<<prams_[j].key<<": "
	<<'('<<prams_[j].type<<") "
	<<prams_[j].value<<endl;
  



  
  
  // clear the input data
  src.unlockPage(image,false);
  
  src.close(0);
  
  FreeImage_DeInitialise();

  return 0;
  
}

void parse_description(string& des,vector<Config_pram> & prams)
{
  size_t start_indx = 0;
  size_t colon_indx = 0;
  size_t end_indx = 0;
  
  int SW = 10;
  
  
  start_indx = des.find("&#13;&#10;",0)+SW;
  
  colon_indx = des.find(": ",start_indx);
  end_indx= des.find("&#13;&#10;",colon_indx);

  while(end_indx != string::npos)
  {
    prams.push_back(Config_pram(des.substr(start_indx,colon_indx-start_indx),"string",des.substr(colon_indx+2,end_indx-colon_indx-2)));
    start_indx = end_indx +SW;
    colon_indx = des.find(": ",start_indx);
    end_indx= des.find("&#13;&#10;",colon_indx);
  }
  
}


void parse_elements(DOMNode* pram_node, vector<Config_pram> & prams_,XMLCh* pram_str,XMLCh* key_str,XMLCh* value_str,XMLCh* type_str)
{
  XMLCh* des_str = XMLString::transcode("Description");

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

      
      if(XMLString::equals(pram_elm->getAttribute(key_str),des_str))
      {
	string v(value);
	parse_description(v,prams_);
      }
      else
	prams_.push_back(Config_pram(key,type,value));

      XMLString::release(&key);
      XMLString::release(&type);
      XMLString::release(&value);
    }
  }
  XMLString::release(&des_str);
  
}
