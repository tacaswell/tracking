f_in = open("ning_base.txt",'r')
f_out = open("new_ning.txt",'w')
loop1 = range(0,100)
loop2 = range(0,1000)

z_range = .1

count = 0
for x in loop1:
    tmp = f_in.readline()
#    f_out.write(str(x) + '\n')
    for y in loop2:
        tmp = f_in.readline()
        b = tmp.split()
        b = float(b[2])
        if (b<z_range) & (b>-z_range):
            f_out.write(str(y)+'\t'+tmp.rstrip()+'\t'+ str(x)+'\n' )
            count +=1
 



f_in.close()
f_out.close()
print count
