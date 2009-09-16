f = open("new_dummy.txt",'w')
gsz = 10
dummy = range(0,gsz*gsz,1)
dummy2 = range(2)
count = 0


for x in dummy:
    f.write(str(x)+' ')
    f.write(str(5)+' ')
    f.write(str(x)+' ')
    f.write('\n')
    
f.close()
                
