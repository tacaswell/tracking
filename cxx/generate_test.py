f = open("new_dummy.txt",'w')
gsz = 100
dummy = range(0,gsz*gsz,1)
dummy2 = range(2)
count = 0


# for x in dummy:
#     f.write(str(x)+' ')
# #    f.write(str(x//gsz) + ' ')
# #    f.write(str(x//gsz) + ' ')
#     f.write(str((x//gsz))+' ')
#     f.write(str((x%gsz))+' ')
#     for y in dummy2:
#         f.write(str(count)+' ')
#         count = count +1
# #    f.write(str(x%gsz) + ' ')
# #    f.write(str(5*((x//gsz)//10) + x%gsz//10) +' ')
#     f.write(str(0))
#     f.write('\n')

    
for x in dummy:
    f.write(str((x//gsz))+' ')
    f.write(str((x%gsz))+' ')
    f.write('\n')

f.close()
                
