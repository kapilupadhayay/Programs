Tree Topview
============

A small program that forms a biary search tree from a given input and
then displays the nodes which will be visible when the tree is viewed
from the top.

% ./topview 200 100 300 110 120 130 140 150 109 108


                                        (200)
  +---------------------------------------+----+
(100)                                        (300)
  +--------------+
               (110)
            +----+----+
          (109)     (120)
       +----+         +----+
     (108)               (130)
                           +----+
                              (140)
                                +----+
                                   (150)








Inorder             : 100 108 109 110 120 130 140 150 200 300
Top View            : 108 100 200 300 130 140 150



To see the working of the tree you can compile with DEBUG option.
Just uncomment the #DEBUG in the Makefile and recompile the code.

Thanks,
Kapil 
22 Aug, 2017
