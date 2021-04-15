# Name:reducer2.py
# Ravi Pratap Singh (20CS60R60)
# Description:To find the top-10 users (node-ids) 
#             with the highest number of friends 

import  sys
#Function to find the top-10 users (node-ids) with the highest number of friends 
def main():

    current_node=""
    current_count=0
    user_count=["0,0","0,0","0,0","0,0","0,0","0,0","0,0","0,0","0,0","0,0"]#To maintain descending order of degree

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
            #selection sort to keep node'id and their neighbours
            rett = list(user_count[9].split(","))
            if(current_count>int(rett[1])):
                user_count[9]=str(current_node) + "," + str(current_count)
                for i in range(8,-1,-1):
                    ret = list(user_count[i].split(","))
                    if(int(ret[1]) < current_count):
                        t_node=int(ret[0])
                        t_count=int(ret[1])
                        user_count[i]=user_count[i+1]
                        user_count[i+1]= str(t_node) + "," + str(t_count)

            current_count=count
        current_node=node
    #output the last node if needed!
    if current_node == node:
        rett = list(user_count[9].split(","))
        if(current_count>int(rett[1])):
            user_count[9]=str(current_node) + "," + str(current_count)
            for i in range(8,-1,-1):
                ret = list(user_count[i].split(","))
                if(int(ret[1]) < current_count):
                    t_node=int(ret[0])
                    t_count=int(ret[1])
                    user_count[i]=user_count[i+1]
                    user_count[i+1]= str(t_node) + "," + str(t_count)

    for element in range(0,10):
        rett = list(user_count[element].split(","))
        print(rett[0])

if __name__ == "__main__":
    main()