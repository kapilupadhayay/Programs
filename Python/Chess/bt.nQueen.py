#!/usr/bin/python
import sys

def get_queens_line_of_sight(Q, maxR, maxC):
    line_of_sight_list = [] # contains all the tuples that are in the line of sight 
    # first we get the straight line of sights, both horizontal and vertical.
    for r in range(0,maxR):
	line_of_sight_list.append((r, Q[1]))
    for c in range(0,maxC):
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

# Q : the tuple signifying the location of a queen
# b : list of tuples each represnting the location of Queens 
#      placed on previous columns
# maxR: highest numbered row
# maxC:	highest numbered column
#
# returns: 1 on safe, 0 otherwise
def is_this_move_safe(Q, b, maxR, maxC):
    # field_of_sight is the collective line of sights for all
    # the queens placed so far on the board.
    field_of_sight = []
    for t in b:
	field_of_sight = field_of_sight + get_queens_line_of_sight(t, maxR, maxC)
    #remove duplicates. As multiple intersecting line of sights 
    # are there that introduces some duplicates.
    field_of_sight = list(set(field_of_sight))
    #
    #print "Q" ,Q[1],":", Q
    #print "Board: " , b
    #print "FoS: " , field_of_sight
    return not (Q in field_of_sight)

#c is column number starting with 0. Incidently, the Queen number too.
#board : We use a list of tuples to represent the current state
#of the board with nth tupple specifying the coordinate
#of nth queen.
#We start with placing the 0th queen on 0th column, successively
#increasing n column wise. Obviously, each column 
#must have  one and only one queen and no column must have 
#0 queens.
def move_next_queen(c, board, maxR, maxC):
    # We will not change the state of board passed to us but
    # will take a copy of it and change that instead. Thus the
    # state of the board propagates under recursion keeping the
    # actual board intact. This will come in handy to start next
    # round of moves (backtracking) when forward path sees a dead end.
    #board_n = list(board)
    if c < maxC:
	for r in range(0, maxR):
	    if is_this_move_safe((r,c), board, maxR, maxC) == True:
		#print "Q", c,  "put on Board Position: (", r, ",", c,")" 
		board_n = list(board)
		board_n = board_n + list(((r,c),))
		if len(board_n) == maxR:
		    print "Solution:  ", board_n
	        move_next_queen (c+1, board_n , maxR, maxC)
	#print "Backtrack to Queen:", c - 1

if len(sys.argv) == 2:
    #print "Board Size:", sys.argv[1]
    move_next_queen(0,[],int(sys.argv[1]), int(sys.argv[1]))
else:
    print "Requires board size as a number"
    
