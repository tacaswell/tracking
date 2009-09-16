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


/** @mainpage Particle Identification and Tracking
   
@authors Thomas A Caswell
    
This is an implementation in c++ of the particle tracking algorithms
developed by Croker and Grier.  This algorithm has been implemented a
number of times.

<TABLE>
<TR>
<TD>
Crocker and Grier
</TD>
<TD>
<a href="http://physics.nyu.edu/grierlab/software.html">
http://physics.nyu.edu/grierlab/software.html
</a>
</TD>
<TD>
IDL
</TD>
</TR>

<TR>
<TD>
Crocker and Weeks
</TD>
<TD>
<a href = "http://www.physics.emory.edu/~weeks/idl/">
http://www.physics.emory.edu/~weeks/idl/
</a>
</TD>
<TD>
IDL
</TD>
</TR>

<TR>
<TD>
Blair and Dufresne
</TD>
<TD>
<a herf="http://physics.georgetown.edu/matlab/">
http://physics.georgetown.edu/matlab/
</a>
</TD>
<TD>
MATLAB
</TD>
</TR>


<TR>
<TD>
Maria Kilfoil
</TD>
<TD>
<a herf="http://www.physics.mcgill.ca/~kilfoil/downloads.html">
http://www.physics.mcgill.ca/~kilfoil/downloads.html
</a>
</TD>
<TD>
MATLAB
</TD>
</TR>


<TR>
<TD>
Grahm Milne
</TD>
<TD>
<a herf="http://faculty.washington.edu/gmilne/tracker.htm">
http://faculty.washington.edu/gmilne/tracker.htm
</a>
</TD>
<TD>
LabVIEW
</TD>
</TR>

<TR>
<TD>
Ryan Smith
</TD>
<TD>
<a herf="http://titan.iwu.edu/~gspaldin/rytrack.html">
http://titan.iwu.edu/~gspaldin/rytrack.html
</a>
</TD>
<TD>
stand alone/IDL GUI
</TD>
</TR>


</TABLE>

The primary advantage of this implementation is that it is
significantly faster than implantation in IDL or MATLAB simply because
they are interpreted languages (~20x against the Blair code but I think
that can be improved).  This implementation also provides
structural advantages and allows information about the particle's
spatial and temporal neighborhood to be quickly extracted.

The code is very flexible and can be readily adapted to N-dimensions,
but currently only 2 and 3 dimensions have been implemented.  The
format of the input and output data is extremely flexible.  There are
wrapper classes that can be quickly subclassed to allow the program to
take input/give output to basically anything.  Currently there are
implementations for input/output from/to text files and MATLAB.

Currently track linking and a few basic static measurements
 (g(r),\f$\Phi_6\f$) have been implemented, but the long term goal is
 to move all static and dynamic measurements in to this frame work.
 The frame work allows any measurement that requires information about
 a particles neighborhood can be efficiently calculated.

 The identification code is a modified version of Peter Lu's particle
 identification code.  The most current version of PLuTARC, as well as
 a MATLAB wrapper for it, is available at <a
 href="http://plutarc.sourceforge.net/">http://plutarc.sourceforge.net/</a>
 



 
The most recent code is revision TREV avaialble at
<KBD>svn://innoue.uchicago.edu/tracking/trunk</KBD> or as a <a
href="http://jfi.uchicago.edu/~tcaswell/track_doc/tars/tarball_rev_TTAR.tar">
tarball</a>.  This code should be reasonably portable to other *nix systems,
but the library paths will need to be fixed.  As it is the code will not
work with matlab on a windows machine, but might work as a stand alone. (There
are lots of couts in the code which matlab in linux can cope with, but matlab
on windows apparently can't (according to matlab documentation)).

The current 'stable' version is 0.1 and is available at 
<KBD>svn://innoue.uchicago.edu/tracking/tags/tracking-0.1</KBD>
or as a <a
href="http://jfi.uchicago.edu/~tcaswell/track_doc/tars/tracking-0.1.tar">
tarball</a>.

Funded by a Keck Grant to Heinrich Jaeger, Sidney Nagel, Wendy Zhang 
and Margaret Gardel


&copy; 2008,2009  <a href="http://jfi.uchicago.edu/~tcaswell"> Thomas A Caswell</a>


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



/**
   Namespace for tracking and identifying filaments
*/
namespace filament{}
