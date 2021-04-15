# Name:mapper3.py
# Ravi Pratap Singh (20CS60R60)
# Description:To find out the users who have friendship
#              with at least one out of the top-10 users. 

import sys
#Function for opening file
def openfile():
    try:
        file=open("network.txt",'r')  
    except:
        print("File Does not exist")
        sys.exit()
    return file

def get_top_10():
    try:
        file2=open("../Query2/result.txt",'r')
    except:
        print("query2 result file does not exist")
        sys.exit()
    top_10=[]
    for line in file2:
        line=line.strip()
        top_10.append(int(line))
    file2.close()
    return top_10

#Function to Print output nodes
def main():
    file = openfile()
    top_10 = get_top_10()
    node1=""
    node2=""
    for line in file:
        try:
            #splitting string with comma separated nodes
            line=line.strip()
            l_arr=line.split(",")
            node1=l_arr[0]
            node2=l_arr[1]
        except Exception as e:
            print(e)
            continue
       
        #printing <key,value> pair as <node_id,count>
        #which are freinds with alteast one of top-10 users
        for line2 in top_10:
            if (str(node1)==str(line2)) and (int(node2)not in top_10):
                print(node2+" "+str(1))
                break
            if (str(node2)==str(line2)) and (int(node1)not in top_10) :
                print(node1+" "+str(1))
                break
    file.close()

if __name__ == "__main__":
    main()