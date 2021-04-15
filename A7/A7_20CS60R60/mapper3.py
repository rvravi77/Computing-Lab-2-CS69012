# Name:mapper3.py
# Ravi Pratap Singh (20CS60R60)
# Description:Find out the total number of triangles passing through each node

import sys

def openfile(filename,Dir):
    try:
        filename= "./"+ filename
        file=open(filename,'r')
    except :
        print(filename," does not exist in ",Dir)
        sys.exit()
    return file

def mapper():
    
    #opening file
    file_N=openfile("network.txt","Query1")
    for line in file_N:
        #each value in adjacency matrix with edge has value 1
        value_N=1
        #getting values of row and column from each line in file
        line = line.strip()
        try:
            row, col = line.split(",")
        except:
            continue
        #printing matrix A with <key, value> pair as <(i % k) $ (j $ a_ij)> for all k in range(300)
        #using special characters so that sort could function as per the need
        #for A
        for k in range(0,300):
            key = row + "%" + str(k)
            print("%s$%s$%s"%(key,col,str(value_N)))

        for k in range(0,300):
            key = col + "%" + str(k)
            print("%s$%s$%s"%(key,row,str(value_N)))
    #closing the file
    file_N.close()

    #opening file
    file_F=openfile("friends.txt","Query2")
    for line2 in file_F:
        #printing matrix A.A with <key, value> pair as <(i % k) $ (j $ a_jk)>  for all i in range(300)
        #for A.A
        line2 = line2.strip()
        try:
            row, col , value_F= line2.split(",")
        except:
            continue

        for i in range(0,300):
            key = str(i) + "%" + col 
            print("%s$%s$%s"%(key,row,str(value_F)))
        for i in range(0,300):
            key = str(i) + "%" + row 
            print("%s$%s$%s"%(key,col,str(value_F)))
    #closing the file
    file_F.close()

if __name__ == '__main__':
    mapper()