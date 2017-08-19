#!/usr/bin/python
import sys

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
    return list(set(line_of_sight_list))


#c is column number starting with 0. Incidently, the Queen number too.
#board : We use a list of tuples to represent the current state
#of the board with nth tupple specifying the coordinate
#of nth queen.
#We start with placing the 0th queen on 0th column, successively
#increasing n column wise. Obviously, each column 
#must have  one and only one queen and no column must have 
#0 queens.
# Now we add another list los i.e Line of sight. Instead of 
# every time calculating the los for any change in queens position 
# we calculate it at the time of queen's placement and add it to
# the field of sight(fos). This fos is next passed to the subsequent 
# recusrsions as at n column all the queens < n are going to stay the
# same only the n+1th and subsequent queens change their locations.
# This yields in significant increase in the speed
# Following are the results:
# #Board Squares    #sol    bt		    dp
#  --------------------------------------------------    
#   10		     724    0m26.981s	    0m1.545s
#   11		    2680    2m48.230s	    0m6.468s
#   12		   14200   18m31.642s	   0m37.687s
#   13		   73712   Not Executed	   4m05.344s 
#   14		  365596   Not Executed	  30m15.249s 
def move_next_queen(c, b, R, C, los):
    # We will not change the state of board passed to us but
    # will take a copy of it and change that instead. Thus the
    # state of the board propagates under recursion keeping the
    # actual board intact. This will come in handy to start next
    # round of moves (backtracking) when forward path sees a dead end.
    # b_next = list(b)
    if c < C:
	for r in xrange(0, R):
	    if  not((r,c) in los) :
		#print "Q", c,  "put on Board Position: (", r, ",", c,")" 
		b_next = list(b)
		b_next = b_next + list(((r,c),))
		if len(b_next) == R:
		    print "Solution:  ", b_next
		los_next = los + get_queens_line_of_sight((r,c), R,C)
	        move_next_queen (c+1, b_next , R, C, los_next)
	#print "Backtrack to Queen:", c - 1

if len(sys.argv) == 2:
    #print "Board Size:", sys.argv[1]
    move_next_queen(0,[],int(sys.argv[1]), int(sys.argv[1]), [])
else:
    print "Requires board size as a number"
    
