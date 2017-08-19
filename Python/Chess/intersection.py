#!/usr/bin/python
from time import time
def line(p, q):
    A = (p[0][1] - p[1][1])
    B = (p[1][0] - p[0][0])
    C = (p[0][0]*p[1][1] - p[1][0]*p[0][1])

    D = (q[0][1] - q[1][1])
    E = (q[1][0] - q[0][0])
    F = (q[0][0]*q[1][1] - q[1][0]*q[0][1])

    L1 = (A, B, -C)
    L2 = (D, E, -F)
    return A, B, -C

def intersection(p, q):
    #print "check paper:", p, "against", q
    A = (p[0][1] - p[1][1])
    B = (p[1][0] - p[0][0])
    C = (p[0][0]*p[1][1] - p[1][0]*p[0][1])

    D = (q[0][1] - q[1][1])
    E = (q[1][0] - q[0][0])
    F = (q[0][0]*q[1][1] - q[1][0]*q[0][1])

    L1 = (A, B, -C)
    L2 = (D, E, -F)


    D  = L1[0] * L2[1] - L1[1] * L2[0]
    Dx = L1[2] * L2[1] - L1[1] * L2[2]
    Dy = L1[0] * L2[2] - L1[2] * L2[0]
    if D != 0:
        x = Dx / D
        y = Dy / D
	if p[0][0] <= x <= p[1][0] and p[0][1] <= y <= p[1][1]:
	    if q[0][0] <= x <= q[1][0] and q[0][1] <= y <= q[1][1]:
		#print "Intersects", x,y
		return x,y
    return False

#L1 = line([(0,0), (0,4)], [(-2,3), (4,3)])
#print L1
#L2 = line([-2,7], [3,7])
#L2 =  line([(-2,3), (4,3)])
#print L2

#R = intersection(L1, L2)
R = intersection([(0,0), (0,4)], [(0,2), (0,3)])
if R:
    print "Intersection detected:", R
else:
    print "No single intersection point detected"


def clip_square(paper_sq, scissor_sq):
    # n is north, e is east ...
    # _e is edge, h is horizontal, v is vertical
    # _p is paper, _s is scissor
    # do ee_s is eastedge_scissors
    #print "Paper, scissor", paper_sq, " ,", scissor_sq
    clipped_sq = []
    ne_p  = [paper_sq[0], paper_sq[1]]
    se_p  = [paper_sq[2], paper_sq[3]]
    ee_p  = [paper_sq[1], paper_sq[3]]
    we_p  = [paper_sq[0], paper_sq[2]]
    ne_s  = [scissor_sq[0], scissor_sq[1]]
    se_s  = [scissor_sq[2], scissor_sq[3]]
    ee_s  = [scissor_sq[1], scissor_sq[3]]
    we_s  = [scissor_sq[0], scissor_sq[2]]
    # First we find all the intersection points.
    clipped_sq = []
    for paper_edge in [ne_p, se_p]:
	for scissor_edge in [ee_s, we_s]:
	    common = intersection(paper_edge, scissor_edge)
	    if common:
		clipped_sq.append(common)

    for paper_edge in [ee_p, we_p]:
	for scissor_edge in [ne_s, se_s]:
	    common = intersection(paper_edge, scissor_edge)
	    if common:
		clipped_sq.append(common)
    #print "Intersection", clipped_sq

    for paper_vertex in paper_sq:
	if (paper_vertex[0] > scissor_sq[0][0] and 
	    paper_vertex[1] > scissor_sq[0][1] and 
	    paper_vertex[0] < scissor_sq[3][0] and 
	    paper_vertex[1] < scissor_sq[3][1]):
	    #print "Including PV", paper_vertex
	    clipped_sq.append(paper_vertex)

    for scissors_vertex in scissor_sq:
	if (scissors_vertex[0] > paper_sq[0][0] and 
	    scissors_vertex[1] > paper_sq[0][1] and 
	    scissors_vertex[0] < paper_sq[3][0] and 
	    scissors_vertex[1] < paper_sq[3][1]):
	    #print "Including SV", scissors_vertex
	    clipped_sq.append(scissors_vertex)
    
    return list(set(clipped_sq))

paper_list = [
	     [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
             [(0,0),(0,4),(3,0),(3,4)],
	     [(0, 0), (0, 8), (8, 0), (8, 8)],
	     [(0, 0), (0, 8), (8, 0), (8, 8)]
	     ]

scissors_list = [
		[(-1,2),(-1,6),(1,2),(1,6),],
		[(-1,-2),(-1,2),(1,-2),(1,2)],
		[(2,-2),(2,2),(4,-2),(4,2)],
		[(1,2),(1,6),(4,2),(4,6)],
		[(-1,1),(-1,3),(1,1),(1,3)],
		[(1,3),(1,5),(2,3),(2,5)],
		[(2,1),(2,3),(4,1),(4,3)],
		[(1,-1),(1,1),(2,-1),(2,1)],
		[(-1,-1),(-1,5),(1,-1),(1,5)],
		[(2,-1),(2,5),(4,-1),(4,5)],
		[(-1,-1),(-1,2),(4,-1),(4,2)],
		[(-1,2),(-1,5),(4,2),(4,5)],
		[(1,1),(1,3),(2,1),(2,3)],
		[(5,5),(5,6),(6,5),(6,6)],
		[(0, 0), (0, 10), (10, 0), (10, 10)],
		[(0, 0), (0, 8), (8, 0), (8, 8)]
		]

result_list = [
			[(0, 2), (1, 4), (0, 4), (1, 2)],
			[(0, 2), (1, 0), (0, 0), (1, 2)],
			[(3, 2), (2, 0), (3, 0), (2, 2)],
			[(3, 2), (1, 4), (3, 4), (1, 2)],
			[(0, 3), (0, 1), (1, 1), (1, 3)],
			[(1, 4), (2, 4), (1, 3), (2, 3)],
			[(3, 3), (3, 1), (2, 1), (2, 3)],
			[(1, 0), (2, 0), (1, 1), (2, 1)],
			[(1, 4), (1, 0), (0, 0), (0, 4)],
			[(2, 4), (2, 0), (3, 0), (3, 4)],
			[(0, 2), (3, 2), (0, 0), (3, 0)],
			[(0, 2), (3, 2), (0, 4), (3, 4)],
			[(1, 1), (1, 3), (2, 1), (2, 3)],
			[],
			[(0, 0), (8, 0), (0, 8), (0, 0), (8, 8)],
			[(0, 0), (0, 8), (8, 0), (8, 8)]
    
		]
print time()
for sq in xrange(len(paper_list)):
	l = clip_square(paper_list[sq], scissors_list[sq])
	if not l in result_list:
	    #print "Mismatch: ", paper_list[sq], scissors_list[sq]
	    #print " Expected: ", result_list[sq]
	    print " Got:", l 
	    #break
	else:
	    print "Matched:", paper_list[sq], scissors_list[sq]
	    print "Result:", result_list[sq]
	print

print time()

