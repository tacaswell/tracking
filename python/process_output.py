fname = "output_hd.txt"
f_in = open(fname)

source_file = f_in.readline()
total_particles = int(f_in.readline())
test = []

for x in range(0,1000):
    test.append([])

tracnk_num = 0
track_list = []
track_listp = []
count = 0
for line in f_in:
    tmp = line.split()
    if(tmp[0]=="track"):
        track_num = int(tmp[2])
        track_list.append([])
        track_listp.append(set([]))
    else:
        test[int(tmp[0])].append(track_num)
        track_list[track_num].append(line)
        count +=1
        track_listp[track_num].add(int(tmp[0]))

print count
print total_particles
f_in.close()

test2 = []
for x in test:
    test2.append(set(x))

#print test2

tmp_count = 0
for x in test2:
    if(len(x)>1):
        print tmp_count,x
    tmp_count+=1
tmp_count = 0
tmp_count2 = 0
for y in test:
    if(len(y)==100):
#        print tmp_count,y
        tmp_count2+=1
    tmp_count+=1
print tmp_count
print tmp_count2

print track_listp
tmp_count =0
for x in track_listp:
    if(len(x)>1):
        print x,tmp_count
    tmp_count+=1



resp = input( "what particle would you like to see?")
while(resp!=-1):
    print resp
    tmp_vector = []
    for y in test2[resp]:
        tmp_vector.append(y)
    tmp_vector.sort()
    for y in tmp_vector:
        print y
        for x in track_list[y]:
            print x.rstrip()
    resp = input( "what track would you like to see?")
