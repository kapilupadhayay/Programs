#!/usr/bin/python

# paradigm to solve this problem is to generate a covermap (a set of tuples)
# that represents the region(possibly discontiguous) on the board where earlier 
# queens can not see. Obviously, the next queen will be put at the square belonging 
# to cover map. If can successively put the queens in the regions formed by the 
# successively-shrinking covermap and reach a state where the nth queen is placed 
# and none of them sees each other, we have found a solution.
# 
# we place the 1st queen on 1st col, generate a covermap  and then work out the solutions recursively.
# Each time a new queen is placed its line of sight is subtracted from the covermap
# 

import sys
EMPTY_BOARD = []

def get_queens_line_of_sight(Q, maxR, maxC):
    line_of_sight_list = [] # contains all the tuples that are in the line of sight 
    # first we get the straight line of sights, both horizontal and vertical.
    for r in xrange(0,maxR):
	line_of_sight_list.append((r, Q[1]))
    for c in xrange(0,maxC):
    	line_of_sight_list.append((Q[0], c)) 
    # diagonals \ and /
    #
    # first \ diagonal
    if Q[0] == Q[1]:
	r,c = 0,0	# row column
    elif Q[0] < Q[1]:	# upper triangle w.r.t middle (n,n) diagonal
	r = 0		# here the r value in tuple is < c value
	c = Q[1] - Q[0]
    else:
	r = Q[0] - Q[1]
	c = 0
    while (r < maxR and c < maxC):
	line_of_sight_list.append((r,c))
	r =  r + 1
	c =  c + 1
    # Now / diagonal, board must be square
    if (Q[0] + Q [1]) <= maxR - 1:
	r = Q[0] + Q[1]
	c = 0
    else:
	rcSum = Q[0] + Q[1]
	r = maxR - 1
	c = rcSum - r
    while (r >= 0 and c < maxC):
	line_of_sight_list.append((r,c))
	r =  r - 1
	c =  c + 1
    # converting to set and back to list removes the
    # duplicates. But it also reshuffels the tupples
    # which makes it difficult to understand the sequence
    # of the tupples in the line of sight.
    #print "Q:", Q, "LoS:", line_of_sight_list
    return set(line_of_sight_list)


def make_empty_board(width):
    row = []
    for i in range(0,width):
        for j in range(0,width):
	    EMPTY_BOARD.append((i,j))

# cm: cover map tuple list
# b : We use a list of tuples to represent the current state
# of the board with nth tupple specifying the coordinate
# of nth queen.
# R : max row number
# C: max column number
def move_next_queen(cm, b, c, R, C):
    # get the the list of tupples belonging to col c
    # c_t means a tuple belonging to cm but has column c
    col_list	= [ c_t for c_t in cm if c_t[1] == c]
    # We need not to proceed further if a queen can not
    # be placed on nth column. Two queens can not share
    # a | -- \ or /, so this leg of recursion will not yield
    # a solution, abort from here.
    if not len(col_list):
	return
    for c_t in col_list:
	# b_next means the next board because of the move.
	b_next = list(b)		# copy a new board
	b_next = b_next + list(((c_t),))	# modify it
	# cm_next: cover map next, the cover map will reduce 
	# as we go on placing more queens because more and 
	# more places of attack becomes visible to subsiquent
	# queens.
	cm_next = cm - get_queens_line_of_sight(c_t, R,C)
	#print "cm_next", cm_next
	if cm_next:
	    move_next_queen(cm_next, b_next , c+1, R, C)
	elif len(b_next) == R:
	    print "Solution:  ", b_next


if len(sys.argv) == 2:
    # start with column number 0
    c = 0
    maxR = int(sys.argv[1])
    make_empty_board(maxR)
    # traverse the first column top-to-down
    for r in xrange(0, maxR):
	# cm means the cover map.
	cm = set(EMPTY_BOARD) - get_queens_line_of_sight((r,0), maxR, maxR)
	#print "Board Size:", sys.argv[1]
	move_next_queen(cm, [(r,0)],c + 1 ,maxR, maxR)
else:
    print "Requires board size as a number"
    
