# Name:mapper1.py
# Ravi Pratap Singh (20CS60R60)
# Description:For users with at least 20 friends

import sys
#Function for opening file
def openfile():
    try:
        file=open("network.txt",'r')    
    except:
        print("File Does not exist")
        sys.exit()
    return file
#Function to Print output nodes
def main():
    file=openfile()
    for line in file:
        line=line.strip()
        try:
            #splitting string with comma separated nodes
            l_arr=line.split(",")
            node1=l_arr[0]
            node2=l_arr[1]
            #printing <key,value> pair as <node_id,count>
            print(node1+","+str(1))
            print(node2+","+str(1))
        except Exception as e:
            #print(e)
            continue
    file.close()

if __name__ == "__main__":
    main()