# Name:reducer1.py
# Ravi Pratap Singh (20CS60R60)
# Description:For users with at least 20 friends

import  sys

#Function to calculate nodes with atleast 20 freinds
def main():
    current_node=""
    current_count=0
    current_cost=0
    for line in sys.stdin:
        line=line.strip()
        #splitting string with comma separated nodes
        l_arr=line.split(",")
        node=l_arr[0]
        try:
            count=int(l_arr[1])
        except ValueError:
            continue
        #update current count of freinds
        if(current_node=="" or node==current_node):
            current_count+=count
        else:
            #update count of nodes with >=20 freinds
            if(current_count >= 20):
                current_cost+=count
            current_count=count
        current_node=node
    #output the last node if needed!
    if current_node == node:
        if(current_count >= 20):
            current_cost+=count

    print("Cost spent for ad campaign : ", current_cost*100)

if __name__ == "__main__":
    main()