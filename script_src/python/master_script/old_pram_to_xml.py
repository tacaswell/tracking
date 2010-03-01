#Copyright 2010 Thomas A Caswell
#tcaswell@uchicago.edu
#http://jfi.uchicago.edu/~tcaswell
#
#This program is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 3 of the License, or (at
#your option) any later version.
#
#This program is distributed in the hope that it will be useful, but
#WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program; if not, see <http://www.gnu.org/licenses>.


from xml_data import xml_data
import sys
import os.path

def _parse_params_attr(xml_d,i_str):
    ssplit = i_str.strip().split(':')

    if len(ssplit)==3:
        xml_d.add_pram('date','date',i_str.strip())
        print ssplit
    elif ssplit[0].strip()=='p_rad' or ssplit[0].strip()=='d_rad'or ssplit[0].strip()=='mask_rad':
        xml_d.add_pram(ssplit[0].strip(),'int',int(float(ssplit[1].strip())))
        print ssplit
    elif ssplit[0].strip()=='threshold'or ssplit[0].strip()=='hwhm' or ssplit[0].strip()=='shift_cut' or ssplit[0].strip()=='rg_cut' or ssplit[0].strip()=='e_cut' or ssplit[0].strip()=='top_cut':
        xml_d.add_pram(ssplit[0].strip(),'float',ssplit[1].strip())
        print ssplit
    else:
        print ssplit[0].strip()



# eat file name
fin = sys.argv[1]

# format xml name
fout = '.'.join(fin.split('.')[:-1])+'.xml'

print fout

# copy data into xml
xml_d = xml_data()
xml_d.add_stanza("iden")
f = open(fin)
for line in f:
    _parse_params_attr(xml_d,line)
f.close()

# write xml back out to file
xml_d.write_to_disk(fout)






