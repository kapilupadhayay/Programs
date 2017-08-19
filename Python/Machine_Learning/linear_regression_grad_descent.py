#!/usr/bin/python

# This is based on Andrew Ng's course on Coursera
# The dataset has been taken from there and I have
# attempted the implement the linear regression of 
# one variable using gradient descent alogrithm in 
# python. 
# Additionally, to provide a view of how the algorithm is
# working, a graph continuously updates itself while
# algorithm calculateas the best fitting regression line 
# in each iteration.
# Note: that makes the program run slowly, so the graphing
# capabilities have been commented out. uncomment lines below
# followin comment:
#	# XXXXXXXXXXXX  For Graph XXXXXXXXXXX
import numpy as np
import matplotlib.pyplot as plt

num_iter = 1500
# Change the learning rate and see the effect
# The fastest convergence is observed when set to
# 0.01. Visually, best convergence is on 0.01.
learning_rate = 0.01

def computeCost(X, Y, theta):
    #print X.shape,"*", theta.shape," - ", Y.shape
    #print "X*theta - Y\n", X, "*", theta, "-", Y
    sumsq = np.sum(np.square(np.dot(X,theta) - Y))
    J = 1.0/(2 * len(Y)) * sumsq
    return J

def plot_graph(X, theta, Y):
	y = []
	x = np.array([0, max(X)])
	plt.cla()
	y.append(theta[0] + x[0] * theta[1])
	y.append(theta[0] + x[1] * theta[1])
	m, c = np.polyfit(x, y, 1)
	ax.scatter(X, Y)
	ax.plot(x,x*m + c, 'r')
	fig.canvas.draw()


def gradient_descent(X, Y, theta, learning_rate, num_iter):
    m = len(Y)
    delta_theta = np.array([0,0])
    for iter in xrange(num_iter):
	error_vector = np.dot(X, theta) - Y
	#print "Error_vector", error_vector, error_vector.shape
	sum_ev = np.dot(error_vector, X)
	delta_theta = (learning_rate/m) * sum_ev
	#print "sum_ev:", sum_ev, sum_ev.shape
	theta = theta - delta_theta
	# see how this affects cost
	#print "Cost:", computeCost(X, Y, theta)
	# XXXXXXXXXXXX  For Graph XXXXXXXXXXX
	#print "Iter: ", iter, "graph:" , plot_graph(X[:,1],theta, Y)
	
    return theta

#XXXXXXXXXXXX  For Graph XXXXXXXXXXX
#plt.ion()
#fig, ax = plt.subplots()

X = np.genfromtxt('ex1data1.txt', delimiter=",", usecols=(0), comments='#')
Y = np.genfromtxt('ex1data1.txt', delimiter=",", usecols=(1), comments='#')
theta = np.array([0,0])
X = np.c_[ np.ones(len(Y)),X ]

#computeCost(X, Y, theta)

grad_theta = gradient_descent(X, Y, theta, learning_rate, num_iter)
print "Geadient'd theta:", grad_theta

# Caution:
# note how the 'X[i]' values in dataset are normalised. Look at ex1.pdf fig 1,
# the X axis represents the city population in 10000s. If we use 35000 directly 
# in matrix multiplication the result differs. 
# Nutshell, be aware of the intitial trteatment to be done to the X. We need to 
# feed the asking value treated in the same way as rest of the sample values in 
# the data set.  Rather if we directly do np.dot(np.array([1,35000]), grad_theta). 
# Result is somewhat different. Its easy to every thing correct and still get a 
# wrong answer.
print "For population of 35000, profit predicted:", np.dot(np.array([1,3.5]), grad_theta) * 10000
print "For population of 70000, profit predicted:", np.dot(np.array([1,7.0]), grad_theta) * 10000

# Last but not the leaset, may thanks to Prof. Andrew Ng.
# Kapil
