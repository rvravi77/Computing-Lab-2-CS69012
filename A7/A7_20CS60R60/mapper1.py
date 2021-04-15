# Name:mapper1.py
# Ravi Pratap Singh (20CS60R60)
# Description:Finding all such strongly connected pairs of nodes to create the chat network.

import sys

def openfile(inputFileName):
    try:
        file=open(inputFileName,'r')    
    except:
        print("File Does not exist")
        sys.exit()
    return file

#Function to Print output nodes
def mapper():
    #get filename from command line
    inputFileName = sys.argv[1]
    file=openfile(inputFileName)
    for line in file:
        #removing trailing endline and white spaces
        line=line.strip()
        try:
            #split the pair of nodes
            n_arr=line.split(",")
            node0=n_arr[0]
            node1=n_arr[1]
            #if nodes are not of form (x,x) , then send to combiner
            if int(node0)!=int(node1):
                print(node0+","+node1+";"+str(1))
        except Exception as e:
            #print(e)
            pass
    file.close()

if __name__ == "__main__":
    mapper()