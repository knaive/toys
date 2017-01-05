#!/usr/bin/python
import random

# randomly get k integers whose sum is equal to target
def int_split(target, k):
    l = [target]
    while len(l)!=k:
        point = random.randrange(1, target+1)
        while(point in l ):
            point = random.randrange(1, target+1)
        l.append(point)
    l.sort()
    parts = []
    lastone = 0
    for i in l:
        parts.append(i-lastone)
        lastone = i
    return parts

target_num = 5
beg = 20
end = 100
targets = []
for i in range(target_num):
    targets.append(random.randrange(beg, end))

ks = range(1, target_num+1)
matches = []
for k,target in zip(ks, targets):
    l = []
    l = int_split(target, k)
    matches.extend(l)

for i in range(0, len(targets)):
    print "%d %d" % (i, targets[i])
print ""
for i in range(0, len(matches)):
    print "%d %d" % (i, matches[i])

#  for k in ks:
    #  for target in targets:
        #  l = []
        #  l = int_split(target, k)
        #  print "target: %d, k: %d" %(target, k)
        #  print l
