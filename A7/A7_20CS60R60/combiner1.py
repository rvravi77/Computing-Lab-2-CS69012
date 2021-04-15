# Name:combiner1.py
# Ravi Pratap Singh (20CS60R60)
# Description:Finding all such strongly connected pairs of nodes to create the chat network.

import  sys

def convert_to(x):
    #makeing nodes like (x,y) (y,x) to consistent form
    if(int(x[0]) > int(x[1])):
        node = x[1] + "," + x[0]
    else:
        node = x[0] + "," + x[1]
    return node

#Function to combine nodes with <key,value> as <nodes,count>
def combiner():
    current_node=""
    current_count=0
    for line in sys.stdin:
        if line=="":
            print("NO input from mapper")
            sys.exit(0)     
        line=line.strip()
        #splitting string with ';' separated nodes and count
        try:
            l_arr = line.split(";")
            nodes = l_arr[0]
            x = nodes.split(",")
        except:
            continue
        
        try:
            node = convert_to(x)
            count = int(l_arr[1])
        except ValueError:
            continue

        #update current count of chats
        if (current_node=="" or node==current_node):
            current_count+=count
        else:
            #update count of chats
            print(current_node+"$"+str(current_count))
            current_count=count
        current_node=node

    try:
        #output the chat count if needed!
        if current_node == node:
            print(current_node+"$"+str(current_count))
    except:
        pass

if __name__ == "__main__":
    combiner()