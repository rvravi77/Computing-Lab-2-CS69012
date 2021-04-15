# Name:mapper2.py
# Ravi Pratap Singh (20CS60R60)
# Description: number of mutual friends for each pair of nodes in the chat network
import sys

def openfile():
    try:
        file=open("./network.txt",'r')
    except :
        print("File Network.txt does not exist")
        sys.exit()
    return file
def mapper():
    #opening file
    file=openfile()
    for line in file:
        #each value in adjacency matrix with edge has value 1
        value=1
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
            print("%s$%s$%s"%(key,col,str(value)))

        for k in range(0,300):
            key = col + "%" + str(k)
            print("%s$%s$%s"%(key,row,str(value)))

        #repeating again for A as we have to calculate A^2
        #printing matrix A` with <key, value> pair as <(i % k) $ (j $ a_ij)>  for all i in range(300)
        #for A`
        for i in range(0,300):
            key = str(i) + "%" + col 
            print("%s$%s$%s"%(key,row,str(value)))
        for i in range(0,300):
            key = str(i) + "%" + row
            print("%s$%s$%s"%(key,col,str(value)))
    #closing the file
    file.close()

if __name__ == '__main__':
    mapper()