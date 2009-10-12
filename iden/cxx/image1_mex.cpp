//Peter J. Lu
//Copyright 2008 Peter J. Lu. 
//http://www.peterlu.org
//plu@fas.harvard.edu
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
//combining it with FreeImage (or a modified version of that library),
//containing parts covered by the terms of freeimage-license, the
//licensors of this Program grant you additional permission to convey
//the resulting work.  Corresponding Source for a non-source form of
//such a combination shall include the source code for the parts of
//FreeImage used as well as that of the covered work.
// 
//If you modify this Program, or any covered work, by linking or
//combining it with IPP (or a modified version of that library),
//containing parts covered by the terms of End User License Agreement
//for the Intel(R) Software Development Products, the licensors of
//this Program grant you additional permission to convey the resulting
//work.

//copied form https://plutarc.svn.sourceforge.net/svnroot/plutarc/trunk/matlab_wrapper rev9
// Modified by Thomas Caswell tcaswell@uchicago.edu 09/2009-


Image2D iden::mat_to_IPP(const mxArray *data)
{
  int j;
  double *mat_tmp;
  mat_tmp = mxGetPr(data);
  IppStatus status;

  int mat_width = mxGetM(data);
  int mat_height = mxGetN(data);
  

  IppiSize roi = {mat_width, mat_height};
  
  Image2D image_in(mat_height, mat_width);
  
  Ipp32f *image_tmp;
  image_tmp = image_in.get_image2D();
  for(j=0;j<mat_height*mat_width;j++)
    {
      image_tmp[j] = mat_tmp[j];
    } 

  return image_in;
}

IppStatus iden::IPP_to_mat(mxArray *data, Image2D &image_out)
{
  int j;
  double *mat_tmp;
  mat_tmp = mxGetPr(data);
  IppStatus status;
  Ipp32f *image_tmp;
  image_tmp = image_out.get_image2D();

  for (j = 0; j <image_out.get_width()*image_out.get_height() ; j++)
    {
      mat_tmp[j] = (double)image_tmp[j];
    }



  return status;
}


void Image2D::set_data(const mxArray *data)
{
  int mat_width = mxGetM(data);
  int mat_height = mxGetN(data);
  if(!(mat_width==width_ && mat_height == height_))
  {
    std::cerr<<"height: "<<height_<<'\t'<<"mat_height: "<<mat_height<<std::endl;
    std::cerr<<"width: "<<width_<<'\t'<<"mat_width: "<<mat_width<<std::endl;
    throw "Image2D: data is wrong size";
  }
  
  double *mat_tmp;
  mat_tmp = mxGetPr(data);


  for(int j=0;j<height_*width_;j++)
  {
    imagedata_[j] =(Ipp32f)mat_tmp[j];
  } 

}
