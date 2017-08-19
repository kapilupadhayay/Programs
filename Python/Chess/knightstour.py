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

from copy import deepcopy

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
# costmap = []

def initialise_costmap(n):
    l = [[ -1 for x in xrange(n)] for i in xrange(n)]
    return l

def usage():
    print "usage:\n", sys.argv[0], "<n> <sx> <sy> \n\n"
    sys.exit(0)


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

# Based on tuple 't' and board size 'n', this
# returns a list of tuples each representing the 
# possible squares a knight can reach from t.
def make_knights_moves(t, costmap, n):
    D1, D2 = [1, -1], [2, -2]    #D1 is displacement1
    knight_t= []
    for d1 in D1:
        for d2 in D2:
            if ((t[0] + d1 >= 0) and (t[1] + d2) < n and
                (t[0] + d1 <  n) and (t[1] + d2) >= 0):    
                if costmap[t[0] + d1][t[1] + d2] < 0:
                    knight_t.append((t[0] + d1,t[1] + d2),)
            if ((t[0] + d2 >= 0) and (t[1] + d1) < n and
                (t[0] + d2 <  n) and (t[1] + d1) >= 0):
                if costmap[t[0] + d2][t[1] + d1] < 0:
                    knight_t.append((t[0] + d2,t[1] + d1),)
    #print "From Origin:", t, "Possible KM:", knight_t
    return knight_t


def kt_next_move_from(t, costmap, n):
    next_moves = make_knights_moves(t, costmap, n)
    #print "Src: ",t
    #print "\tnext moves    :", next_moves
    # accessibility, an array that contains numbers. Each number represents
    # the number of locations that can lead to the tuple in focus.
    accessibility = []
    for nm in next_moves:
        #print "Check accessibility for ", nm
        length = len(make_knights_moves(nm, costmap, n))
        #print "length:", length
        accessibility.append(length)
    if not len(accessibility):
        # 9, our way to tell that from this tuple in next_moves list
        # there is no accessibility to t. At most there can be 8 locations
        # that can reach to t at any point. We set it to one more than that.
        accessibility.append(9)
    min_indexes = [i for i,v in enumerate(accessibility) if (v == min(accessibility) and v != 9)]
    #print "\taccessibility:", accessibility
    #print "min_indexes", min_indexes, "next_moves:", next_moves
    return [next_moves[i] for i in min_indexes]

def make_knights_open_tour(s, costmap, moves_left, n):
    next_moves = kt_next_move_from(s, costmap, n)
    #print "Next Set", next_moves, "And Moves Left", moves_left
    for s in next_moves:
        print "Executing",s , "Out of ", next_moves, "And moves left:", moves_left
        print "\n================"
        next_costmap = deepcopy(costmap)
        next_costmap[s[0]][s[1]] = (n * n) - moves_left
        moves_left -= 1
        print "Displaying current board:",display_board(next_costmap, n)
        print "================"
        print "->[",next_costmap[s[0]][s[1]],"]: ",s
        make_knights_open_tour(s, next_costmap, moves_left, n)
    if len(next_moves) == 0:
        print "moves_left", moves_left, "Solution", display_board(costmap, n)
            
if len(sys.argv) < 4:
    usage()
check_all = 0
bsize   = int(sys.argv[1]) 
sx      = int(sys.argv[2]) 
sy      = int(sys.argv[3]) 
try:
    check_all = int(sys.argv[6])
except IndexError:
    pass

if sx < 0 or sy < 0 or sx >= bsize or sy >= bsize:
    print "Dont worry!, it works. Give me sane input\n"
    usage()

costmap = initialise_costmap(bsize)
display_board(costmap, bsize)
costmap[sx][sy] = 0
make_knights_open_tour((sx,sy,), costmap, bsize*bsize - 1, bsize)
#display_board(costmap, bsize)
