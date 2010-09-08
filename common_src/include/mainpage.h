//Copyright 2008,2009,2010 Thomas A Caswell
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


/** @mainpage Particle Identification and Tracking
   
@authors Thomas A Caswell
    
This is an implementation in c++ of the particle tracking algorithms
developed by Croker and Grier (<a
href="http://dx.doi.org/10.1006/jcis.1996.0217">
http://dx.doi.org/10.1006/jcis.1996.0217</a>).  This algorithm has
been implemented a number of times (see table).  The primary advantage
of the C++ implementation over IDL or MATLAB implementations is that
C++ is significantly faster.  On identical data the C++ code is at
least ~20x than Dan Blair's MATLAB code.

<table>
<tr>
<td>
Crocker and Grier
</td>
<td>
<a href="http://physics.nyu.edu/grierlab/software.html">
http://physics.nyu.edu/grierlab/software.html
</a>
</td>
<td>
IDL
</td>
</tr>

<tr>
<td>
Crocker and Weeks
</td>
<td>
<a href = "http://www.physics.emory.edu/~weeks/idl/">
http://www.physics.emory.edu/~weeks/idl/
</a>
</td>
<td>
IDL
</td>
</tr>

<tr>
<td>
Blair and Dufresne
</td>
<td>
<a herf="http://physics.georgetown.edu/matlab/">
http://physics.georgetown.edu/matlab/
</a>
</td>
<td>
MATLAB
</td>
</tr>


<tr>
<td>
Maria Kilfoil
</td>
<td>
<a herf="http://www.physics.mcgill.ca/~kilfoil/downloads.html">
http://www.physics.mcgill.ca/~kilfoil/downloads.html
</a>
</td>
<td>
MATLAB
</td>
</tr>


<tr>
<td>
Grahm Milne
</td>
<td>
<a herf="http://faculty.washington.edu/gmilne/tracker.htm">
http://faculty.washington.edu/gmilne/tracker.htm
</a>
</td>
<td>
LabVIEW
</td>
</tr>

<tr>
<td>
Ryan Smith
</td>
<td>
<a herf="http://titan.iwu.edu/~gspaldin/rytrack.html">
http://titan.iwu.edu/~gspaldin/rytrack.html
</a>
</td>
<td>
stand alone/IDL GUI
</td>
</tr>


</table>


<h2>Getting Source</h2>
The code is also being tracked on 
<a href="http://github.com/tacaswell/tracking">github</a>.
There is currently no 'stable' version of the code as I am still
actively developing it.


<h2>Functionality</h2>

Currently, 
<ul>
<li>2D particle location
<li>3D particle location
<li>tracking in time
<li>mean squared displacement
<li>van Hove distributions
<li>\f$g(r)\f$ (2D and 3D) (\ref gofr_doc "doc")
</ul>
are written and fully tested and
<ul>
<li>\f$\Phi_6\f$
<li>\f$g(\theta)\f$
<li>\f$S(\vec{q})\f$
</ul>
are partially written or tested.

<h2>Code Features</h2>


The code for particle location and further processing of the locations are
almost completely separate.   The identification code
is a modified version of Peter Lu's particle identification code.  The most current version of PLuTARC,
as well as a MATLAB wrapper for it, is available at <a
href="http://plutarc.sourceforge.net/">http://plutarc.sourceforge.net/</a>.
The analysis code is newly developed.


Parameters are passed into the analysis programs via XML files.  There is
python code included to generate appropriate XML files.

The code can easily be extended for N-dimensions, but only 2 and 3 are
currently implemented.  

Input and output is done through a family of
wrapper classes.  With minimal new code basically any input/output
format can be dealt with.  There are wrappers written for 
HDF5 files and interacting with MATLAB.  

The code is very modular and for the most part adding new computations
to the framework only requires writing a single new subclass.
 
This code should be reasonably portable to *nix systems, but the
library paths will need to be fixed, see INSTALL for directions and a
list of required libraries.  The some of the MATLAB related code will
not work on a windows machine (there are lots of couts in the code
which matlab in linux can cope with, but matlab on windows apparently
can't (according to matlab documentation)), but I expect that the rest
of the code (if it will compile) will work fine.  To my knowledge this
has not been tested in practice.



<em>
Funded by a Keck Grant to Heinrich Jaeger, Sidney Nagel, Wendy Zhang 
and Margaret Gardel
</em>

<h2>License</h2>

The following is the license for code written by me and supersedes
what individual files may say at the top with regards to the
Additional Permissions.  Files not written by copyright Thomas A
Caswell have their copyright/license at the top.


&copy; 2008-2010  <a href="http://jfi.uchicago.edu/~tcaswell"> Thomas A Caswell</a>


This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses>.

Additional permission under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with MATLAB (or a modified version of that library),
containing parts covered by the terms of MATLAB User License, the
licensors of this Program grant you additional permission to convey
the resulting work.

If you modify this Program, or any covered work, by linking or
combining it with IPP (or a modified version of that library),
containing parts covered by the terms of End User License Agreement
for the Intel(R) Software Development Products, the licensors of
this Program grant you additional permission to convey the resulting
work.

If you modify this Program, or any covered work, by linking or
combining it with FreeImage (or a modified version of that library),
containing parts covered by the terms of End User License Agreement
for FreeImage Public License, the licensors of
this Program grant you additional permission to convey the resulting


*/
 
/**
   Name space for all of the tracking code.
*/
namespace tracking{}


/**
   Name space for all of the particle identification code.
*/
namespace iden{}

/**
   Namespace for utilities and helper functions
*/
namespace utilities{}



/*
   Namespace for tracking and identifying filaments
*/
//namespace filament{}
