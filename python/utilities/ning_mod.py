f_in = open("ning_base.txt",'r')
f_out = open("new_ning.txt",'w')
f_out_s = open("new_ning_small.txt",'w')
f_out_b = open("new_ning_big.txt",'w')
loop1 = range(0,100)
loop2 = range(0,1000)

z_range = .1
z_offset = .4
count = 0
for x in loop1:

    tmp = f_in.readline()
#    f_out.write(str(x) + '\n')
    for y in loop2:
        tmp = f_in.readline()
        b = tmp.split()
        b = float(b[2])
        #        if (b<(z_offset +z_range)) & (b>(z_offset-z_range)):
        #             f_out.write(str(y)+'\t'+tmp.rstrip()+'\t'+ str(x)+'\n' )
        f_out.write(tmp.rstrip()+'\t'+ str(x)+'\n' )
        if y<500:
            f_out_s.write(tmp.rstrip()+'\t'+ str(x)+'\n' )
        else:
            f_out_b.write(tmp.rstrip()+'\t'+ str(x)+'\n' )
        count +=1
 



f_in.close()
f_out.close()
f_out_s.close()
f_out_b.close()

print count
