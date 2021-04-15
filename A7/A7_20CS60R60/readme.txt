// Computing lab II ||  CS69012  ||   Assignment #7
//Submitted by - Ravi Pratap Singh (20CS60R60)

Running command for all queries: (on linux kernal) 
~$ make run

Note:
For Query1:
    1.Assumed "day<x>.txt" in the same directory as of mapper1.py,reducer1.py,combiner1.py.
    2.Ignored the values like (x,x).
    3.Considered (x,y) and (y,x) as same pair.
    4."network.txt" generated in same directory.
    5.each line in "network.txt" denotes <user_x , user_y> which are strongly connected.

For Query2:
    1.Accesses "network.txt" as "./network.txt".
    2."freinds.txt" generated in same directory.
    3.Each line in "freinds.txt"  denotes <user_x , user_y , mutual-freinds-count>.
    4.Reducer only uses list of size "2" having a tuple of size "2".
    
For query3:
    1.Accesses "network.txt"  as "./network.txt".
    2.Accesses "freinds.txt"  as "./freinds.txt".
    3."triangles.txt" generated in same directory.
    4.Each line in "freinds.txt"  denotes <user_x , triangle-count>.
    5.Reducer only uses list of size "2" having a tuple of size "2".
    6.If will take more time to run .