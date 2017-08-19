#!/usr/bin/python
####################################################################
# This module 'knightsmove' simulates a knight's move              #
# from point A -> B such that it takes minimum number of steps     #
# between A and B. It finds all the possible solutions.            #
# Here are the stats:                                              #
#                                                                  #
# BoardSize  #Sol    TimeTaken    A->B	    #StepCount             #
# =========  =====   =========  =========   =========              #
#   10x10        57  0m, 0.038s	 0,0->9,9	 6                 #
#   20x20    179763  0m, 5.301s  0,0->19,19	15                 #
#   30x30   2721049  1m,23.055s  0,0->29,29	21                 #
#   40x40  10400727  5m,44.751s  0,0->39,39	27                 #
####################################################################

import sys

# a list of lists, inteneded to be used as a C styled
# 2D array. The inner lists will have tupples representing
# a square.
board = []
# a 2D matrix of cost from each of the squares of the board to
# (dx,dy). This map is unique for each new value of (dx, dy).
# That means every time one invokes this program this cost matrix
# is generated again.
# It has the same dimensions as the board itself, only difference
# is that instead of tupple, it will store a number representing
# the number of knight's steps (cost) to reach to desitnation square.
costmap = []

# L1: the edge of the smallest square enclosing the square (dx,dy)
# the numbers represent the costs from the respective squares to (dx, dy)
l1cost  = [2, 3, 2]
# L2: edge next after L1
l2cost = [4, 1, 2 ,1, 4]

SAME_ROW= 0
SAME_COL = 1

# A line L or edge is represented as [(x1,y1), (x2, y2)]. Given
# two lines L1, L2 this function returns (x,y), the intersection.
# For more info read about crammers rule
#
# Note:
# p and q are line segments and not  the line, the approach works on 
# line and we pass a line segment. So after the function 
# calculates (x,y) the point of intersection, we need to additionally
# check that the point lies on the line. Remember lines are infinite and
# line segments are finite, thus, even if with this approach we get a
# (x,y) that may  possibly lie outside the line segments.
# For this matter this function will assumes the L1 is the credentials
# for the board lines and L2 always the enclosure squares. To be more clear,
# p should represnt the actual board edge and q as enclosing square edge.

def intersection(p, q):
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
	# The x,y represent the point where p, q should meet if they
	# are lines (infinite), which they are not. They are segments(finite)
	# so we need to validate x,y if it is present on both the edges
	if p[0][0] <= x <= p[1][0] and p[0][1] <= y <= p[1][1]:
	    if q[0][0] <= x <= q[1][0] and q[0][1] <= y <= q[1][1]:
		#print p, " and ", q, "intersect at", x,y
		return x,y
	else: 
	    return False
    else:
        return False


# 
# returns: how many iterations are to be done.
def get_num_iters(t, n):
    r, c = t[0], t[1]
    rn = n - 1 - r
    cn = n - 1 - c
    limit = max(r,c)
    limitn = max(rn, cn)
    return(max(limit, limitn))

# Based on tuple 't' and board size 'n', this
# returns a list of tuples each representing the 
# possible squares a knight can reach from t.
def make_knights_moves(t, n):
    D1, D2 = [1, -1], [2, -2]    #D1 is displacement1
    knight_t= []
    for d1 in D1:
        for d2 in D2:
            if ((t[0] + d1 >= 0) and (t[1] + d2) < n and
		(t[0] + d1 <  n) and (t[1] + d2) >= 0):    
                knight_t.append((t[0] + d1,t[1] + d2),)
            if ((t[0] + d2 >= 0) and (t[1] + d1) < n and
		(t[0] + d2 <  n) and (t[1] + d1) >= 0):
                knight_t.append((t[0] + d2,t[1] + d1),)
    #print "From Origin:", t, "Possible KM:", knight_t
    return knight_t


# Given two rectangles R1 = [ (r1,c1), (r1,c2), (r2,c1), (r2,c2) ]
# R2 = [ (r3,c3), (r3,c4), (r4,c3), (r4,c4) ]
# clips R1 w.r.t R2 and returns R3 such that R3 forms the intersection
# rectangle.
# The order of tupples in R is very important. The order is [1,2,3,4]
#	    1    2
#	    +----+
#	    |	 |
#	    |	 |   
#	    +----+
#	    3    4
# where 1,2,3,4 are the tuples representing the vertex of the rectangle.
# algo used:
# 1. form the set of all the intersection points, say s1
# 2. form the set of all the points of R1 inside R2, say s2
# 3. form the set of all the points of R2 inside R1, say s3
# 4. Finally, R = s1 U s2 U s3
#
# Returns: R
# Please also remember that the return value is again a R but the tupples 
# may not be in the same order. If no overlap, R is empty.

def clip_square(paper_sq, scissor_sq):
    # n is north, e is east ...
    # _e is edge, h is horizontal, v is vertical
    # _p is paper, _s is scissor
    # do ee_s is eastedge_scissors
    #print "Paper", paper_sq
    #print "scissor", scissor_sq
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
    # possible duplicates in case of overlapped edges. 
    # We remove them before returning.
    return list(set(clipped_sq))

def get_overlapping_edges(big_sq, lil_sq):
    #print "big sq", big_sq
    #print "lil_sq", lil_sq
    lil_h = []
    lil_v = []
    for i in xrange(4):
	if lil_sq[i][0][0] == lil_sq[i][1][0]:
	    lil_h.append(lil_sq[i])
	else:
	    lil_v.append(lil_sq[i])
    aligned_edge = []
    offset_list	 = []
    # hle : horizontal lil edge
    # vbe : vertical big edge
    for hle in lil_h:
	for hbe in [big_sq[0], big_sq[3]]:
	    # we compare horizontal edges on both the squares.
	    # the x coordinate is common if they overlap
	    #print "check h b/w", hle, hbe
	    if hbe[0][0] == hle[0][0]:
		#print "appending hle", hle
		aligned_edge.append(hle)
		hlil_min_x = abs(min(hle[0][1], hle[1][1]))
		hlil_max_x = abs(max(hle[0][1], hle[1][1]))
		hbig_min_x = abs(min(hbe[0][1], hbe[1][1]))
		hbig_max_x = abs(max(hbe[0][1], hbe[1][1]))
		#print "hlil_min_x:", hlil_min_x, "hlil_max_x:", hlil_max_x
		#print "hbig_min_x:", hbig_min_x, "hbig_max_x:", hbig_max_x
		up_offset  = abs(hlil_min_x - hbig_min_x)
		down_offset= abs(hbig_max_x - hlil_max_x)
		#print "up_offset:", up_offset, "down_offset:",down_offset
		offset_list.append((up_offset, down_offset),)
    for vle in lil_v:
	for vbe in [big_sq[1], big_sq[2]]:
	    #print "check v b/w", vle, vbe
	    if vle[0][1] == vbe[0][1]:
		#print "appending vle", vle
		aligned_edge.append(vle)
		vlil_min_y = abs(min(vle[0][0], vle[1][0]))
		vlil_max_y = abs(max(vle[0][0], vle[1][0]))
		vbig_min_y = abs(min(vbe[0][0], vbe[1][0]))
		vbig_max_y = abs(max(vbe[0][0], vbe[1][0]))
		#print "vlil_min_y:", vlil_min_y, "vlil_max_y:", vlil_max_y
		#print "vbig_min_y:", vbig_min_y, "vbig_max_y:", vbig_max_y
		left_offset = abs(vbig_min_y - vlil_min_y)
		right_offset= abs(vbig_max_y - vlil_max_y)
		#print "left_offset:", left_offset, "right_offset:", right_offset
		offset_list.append((left_offset,right_offset),)
    #print "aligned edge", aligned_edge
    #print "offset_list", offset_list
    return aligned_edge, offset_list
    
		
# Give it a tuple t, level l and board size n, it makes a enclosing square
# l level from t. Then determines which enclosing square edge is biggest 
# that lies in the board of size n. Also calcluates what part of the edge
# lies inside the board.
# returns:
# [(x1,y1),(x2,y2)], length, left_offset, right_offset
#    
def find_biggest_edge_inside_board(t, l, n):
    r, c = t[0], t[1]
    es = 2 * l + 1
    e_t = ()
    enclosure_sq = [(r - l, c - l), (r - l, c + l), 
		    (r + l, c - l), (r + l, c + l)]
    board_sq    =  [(0, 0), (0, n - 1), 
                    (n - 1, 0), (n - 1, n - 1)]
    #print "find_biggest_edge_inside_board:", t, "level", l
    #print "Enclosure Square: ", enclosure_sq
    #print "board_sq:         ", board_sq
    # clip the enclosing square w.r.t board
    clipped_tup_list = clip_square (enclosure_sq, board_sq)
    #print "Clipped tuple list:", clipped_tup_list
    # what clip_square() returned us is a list of 4 tupples that forms
    # the veertices of the 'intersecting' rectangle. We do not know the
    # order, so we now form the edges based on the x and y cordinates.
    clipped_edges = []
    for i in xrange(len(clipped_tup_list)):
	for j in xrange(i + 1, len(clipped_tup_list), 1):
	    if (clipped_tup_list[i][0] == clipped_tup_list[j][0] or 
		clipped_tup_list[i][1] == clipped_tup_list[j][1]):
		clipped_edges.append([clipped_tup_list[i],clipped_tup_list[j]])
    if len(clipped_edges) != 4:
	print "Something wrong with clipping, Aborting"
        print "clipped tupple List", clipped_tup_list	
	print "clipped_edges", clipped_edges
	sys.exit()
		#   order: N  W  E  S	
    enclosure_edges = [[enclosure_sq[0],enclosure_sq[1]], [enclosure_sq[0], enclosure_sq[2]],
		       [enclosure_sq[1], enclosure_sq[3]], [enclosure_sq[2], enclosure_sq[3]]]

    #print "Enclosure Edges:", enclosure_edges, "\nClipped edges:  ", clipped_edges
    # we have the enclosure_square edges and also the intersecting square edges. We need to 
    # see which edge can be used. Basically, the longes edge inside the intersecting square
    # that is aligned/overlapping the enclosure_sq edge is our candidate. We also need to see
    # the extent of alignment i.e offset calculations of the clipped edge w.r.t corresponding
    # enslosing square edge.
    encsq_clipped_edge, encsq_edge_offset = get_overlapping_edges(enclosure_edges, clipped_edges)
       
    #print "Enclosure Sq clipped edge", encsq_clipped_edge
    edge_len_list = []
    for i in xrange(len(encsq_clipped_edge)):
	a = 0
	if encsq_clipped_edge[i][0][0] != encsq_clipped_edge[i][1][0]:
	    edge_len_list.append(abs(encsq_clipped_edge[i][0][0] - encsq_clipped_edge[i][1][0]))
	else:
	    edge_len_list.append(abs(encsq_clipped_edge[i][0][1] - encsq_clipped_edge[i][1][1]))
    #print "Clipped Edges lenghts:", edge_len_list
    # Finally we have our eligible set of edges which form that part of enclosure
    # square  that is inside the board. Lets calculate the largest of such edge and
    # return that.
    max_len_index = edge_len_list.index(max(edge_len_list))
    #print "LONGEST EDGE INSIDE", encsq_clipped_edge[max_len_index]
    #print "WITH PRE-OFFSET:", encsq_edge_offset[max_len_index][0], " POST-OFFSET:", encsq_edge_offset[max_len_index][1] 
    return encsq_clipped_edge[max_len_index], max(edge_len_list),encsq_edge_offset[max_len_index][0], encsq_edge_offset[max_len_index][1]


def put_cost_list_on_egde(rc, costList, n, direction):
    r,c = rc[0], rc[1]
    #print "CL:", costList, "to be copied at", rc, "length: ", len(costList), "Direction", direction, "(0: SAME_ROW, 1: SAME_COL)"
    if direction == SAME_ROW:
	if r >= 0 and r < n:  # copy direction is left to right; SAME ROW
	    for i in xrange(len(costList)):
		if (c+i) >=0 and (c+i) < n:
		    #print "SAMEROW placing @ [",r,"][",c+i,"] copy: ", costList[i]
		    costmap[r][c+i] = costList[i]
    elif c >= 0 and c < n:   				# SAME COLUMN; direction is DOWN
        for i in xrange(len(costList)):
	    if (r+i) >=0 and (r+i) < n:
		#print "SAMECOL placing @ [",r+i,"][",c,"] copy: ", costList[i]
		costmap[r+i][c] = costList[i]
    #display_board(costmap, n)


# we make the cost list for l > 2 onwards.
# The aim is to pick up a corner square that is in the board.
# find all the location knight could possible be moved from "this"/present
# location. Get the cost of all the locations where the knight
# could be placed. The min of all those cost and we add one to it
# will be the min cost to (dx, dy) from present location.
#
# t: tupple i.e (dx,dy)
# l: level. Starting from the dx,dy which enslosing square it is.
#    edge size at each level is 2*l + 1        
# n: board size
def make_edge_cost_list(t, l, n):
    #es : edge size
    es = 2 * l + 1
    costList = []
    r, c = t[0], t[1]
    # need to find out which side is feasible
    # obviously we dont want to straddle across 
    # boundary during calculation.
    
    edge, length, l_off, r_off = find_biggest_edge_inside_board(t, l ,n)
    #print "Level ", l, "Max Edge:", edge, "Lenght:", length, "l_off:", l_off, "r_off:", r_off
    if length != 0:
	costList = [n+1 for x in xrange(es)]
	# this is a small hack, as we do not know
	# the edge tuples are properly ordered in
	# increasing order of their dimension.
	r, c = 0, 0
	if edge[0][0] == edge[1][0]:
	    loop_direction = SAME_ROW
	    r, c = edge[0][0], min(edge[0][1], edge[1][1])  
	else:
	    loop_direction = SAME_COL 
	    r, c = min(edge[0][0], edge[1][0]),  edge[0][1]
	for i in xrange(length + 1):
            cost = n + 1
	    if loop_direction == SAME_ROW:
		for krc in make_knights_moves((r , c + i),n):
		    if costmap[krc[0]][krc[1]] > n:
			continue
		    if costmap[krc[0]][krc[1]] < cost:
			cost = costmap[krc[0]][krc[1]] + 1
		costList[l_off + i] = cost
		#print "SR costList", costList
	    else:
		for krc in make_knights_moves((r + i , c ),n):
		    if costmap[krc[0]][krc[1]] > n:
			continue
		    if costmap[krc[0]][krc[1]] < cost:
			cost = costmap[krc[0]][krc[1]] + 1
		costList[l_off + i] = cost
		#print "SC costList", costList
	# using the symmetry to make costList complete at 
	# those points where it still contains n+1
	mid = es / 2
	for i in xrange(mid, es, 1):
	    if costList[i] < costList[-i-1]:
		costList[-i-1] = costList[i]
	    else:
		costList[i] = costList[-i-1]
    return costList
	
# t is (dx,dy) i.e dest tupple always
def make_next_cost_sq(t, l, n):
    costList = []
    if l == 1:
        costList = l1cost
    elif l == 2:
        costList = l2cost
    else:
        costList = make_edge_cost_list(t, l , n) 
    rc = (t[0] - l, t[1] - l)     # NW corner tupple
    put_cost_list_on_egde(rc, costList, n, SAME_ROW)
    rc = (t[0] - l, t[1] - l)     # NW corner tupple  
    put_cost_list_on_egde(rc, costList, n, SAME_COL)
    rc = (t[0] + l, t[1] - l)     # SW corner tupple
    put_cost_list_on_egde(rc, costList, n, SAME_ROW)
    rc = (t[0] - l, t[1] + l)     # NE corner tupple
    put_cost_list_on_egde(rc, costList, n, SAME_COL)

def fix_corner_cost(n):
    four_corners = [(0,0),(0,n-1),(n-1,0),(n-1,n-1)]
    for t in four_corners:
	r,c = t[0],t[1]
	for sq_r in range(r-1, r+2,1):
	    for sq_c in range(c-1, c+2, 1):
		if sq_r < 0 or sq_r >= n or sq_c < 0 or sq_c >=n:
		    continue	
		#print "Check corner:", (sq_r, sq_c)
		if costmap[sq_r][sq_c] == 0:
		    continue
		next_moves = make_knights_moves((sq_r,sq_c),n)
		cost_next_moves = [costmap[m[0]][m[1]] for m in next_moves]
		if min(cost_next_moves) >= costmap[sq_r][sq_c]:
		    print "Fixing: cost: ", costmap[sq_r][sq_c], "@(",sq_r,",",sq_c,")", " to ",
		    costmap[sq_r][sq_c] = min(cost_next_moves) + 1
		    print costmap[sq_r][sq_c]
   


def make_cost_board(t, n):
    # l, is level, d is direction
    l = get_num_iters(t, n)
    #print "Total Level", l
    for l in xrange(1, l + 1, 1):
	#print "Lvl: ", l
        make_next_cost_sq(t, l, n)
	#display_board(costmap, n)
    # There is a 1/1000 possibility that the square
    # be choosen is a corner on as source or desitination
    # and the other be its diagonally adjacent. The cost on
    # the source is fixed as 2 but its actually 4, we need
    # to check that condition and fix it. If we dont address
    # this chances are the solution may not converge at that
    # point.
    #fix_corner_cost(t,n)
    fix_corner_cost(n)

def display_board(board, n):
    print "Board:\n"
    for r in xrange(n):
        print board[r]
        print
        print

def initialise_board(n):
    b = []
    for r in xrange(n):
        rowlist = []
        for c in xrange(n):
            rowlist.append((r,c),)
        b.append(rowlist)
    return b

# given s tupple and d tupple on a chess board
# of size n x n. Finds all the minimum moves from s to d.
def find_knight_min_moves(s, path, n):
    sr, sc = s[0], s[1]
    next_moves = make_knights_moves((s),n)
    cost_next_moves = [costmap[m[0]][m[1]] for m in next_moves]
    min_indices = [i for i, v in enumerate(cost_next_moves) if v == min(cost_next_moves)]
    #print "Source", s
    #print "next_moves", next_moves
    #print "cost	     ", cost_next_moves
    #print "mins	     ", min_indices
    for i, v in enumerate(min_indices):
	next_path = list(path)
	if cost_next_moves[v] == 0:
	    next_path.append(next_moves[min_indices[i]])
	    print "MIN PATH", next_path
	    return
	else:
	    #print "= Existing Path:", path
	    #print "= Attempt with New Source:", next_moves[min_indices[i]]
	    next_path.append(next_moves[min_indices[i]])
	    find_knight_min_moves(next_moves[min_indices[i]], next_path, n)
    

def initialise_costmap(n):
    l = [[ n+1 for x in xrange(n)] for i in xrange(n)]
    return l

def usage():
    print "usage:\n", sys.argv[0], "<n> <dx> <dy> <sx> <sy> \n\n"
    print "To test all possible permutations: (debug)"
    print sys.argv[0], "<n> <dx> <dy> <sx> <sy> 1"
    sys.exit(0)

if len(sys.argv) < 6:
    usage()
check_all = 0
bsize   = int(sys.argv[1]) 
dx      = int(sys.argv[2]) 
dy      = int(sys.argv[3]) 
sx      = int(sys.argv[4]) 
sy      = int(sys.argv[5]) 
try:
    check_all = int(sys.argv[6])
except IndexError:
    pass

if sx < 0 or sy < 0 or sx >= bsize or sy >= bsize:
    print "Dont worry!, it works horizontally. Give me sane input\n"
    usage()

if dx < 0 or dy < 0 or dx >= bsize or dy >= bsize:
    print "Dont worry!, it works horizontally. Give me sane input\n"
    usage()

# this check_all is there to test the code for all
# possible costmaps for a borad size.
# Just post fix an extra 1 to trigger this code.
# please note that the rest of the arguments wont
# be considered now as it tests all permutations.
if check_all:
	error_rc = []
	for r in xrange(bsize):
	    print "Creating and checking",bsize," Costmaps at row:", r,
	    for c in xrange(bsize):
		#board = initialise_board(bsize)
		costmap = initialise_costmap(bsize)
		costmap[r][c] = 0
		make_cost_board((r,c), bsize)
		for er in xrange(bsize ):
		    if bsize + 1 in costmap[er]:
			error_rc.append((r,c))
			print "Error @", r,c
			display_board(costmap, bsize)
			break
	    print ".........[Done]"
	if not len(error_rc):
	    print "\n\nAll Clean :)\n"
	else:
	    print "Errors", set(error_rc)
else:
    #board = initialise_board(bsize)
    costmap = initialise_costmap(bsize)
    costmap[dx][dy] = 0
    make_cost_board((dx,dy), bsize)
    display_board(costmap, bsize)
    find_knight_min_moves((sx,sy), [(sx,sy)] ,bsize)


		    
