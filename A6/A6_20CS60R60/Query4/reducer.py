# Name:reducer4.py
# Ravi Pratap Singh (20CS60R60)
# Description:To find cost of ad for top-10 US users with  
#             the highest number of friends in the US
import  sys
#Function To find cost of ad for top-10 US users with the highest number of friends in the US
def main():

    current_node=""
    current_count=0
    us_count=0
    #Will be maintained in descending order of counts
    # <0,0,0> ==> <node_id,US_degree_count,overall_degree>
    user_count=["0,0,0","0,0,0","0,0,0","0,0,0","0,0,0","0,0,0","0,0,0","0,0,0","0,0,0","0,0,0"]
    
    for line in sys.stdin:
        line=line.strip()
        #splitting string with space separated nodes
        l_arr=line.split(",")
        node=l_arr[0]
        #if the node is non_US , just ignore!
        if (int(node) not in range(0,1000)):
            continue
        #getting the edge
        try:
            count1=l_arr[1]
            count2=l_arr[2]
        except ValueError:
            continue
        #update current count of freinds
        if(current_node=="" or node==current_node):
            current_count+=1
            #if edge between US_nodes
            if((int(count1) in range(0,1000)) and (int(count2) in range(0,1000))):
                us_count+=1
        # if current node is US_node
        elif (int(current_node) in range(0,1000)):
            # split the list element
            rett = list(user_count[9].split(","))
            if us_count>int(rett[1]):
                #if element is greater than the last then add to list
                user_count[9]=str(current_node) + "," + str(us_count) + "," + str(current_count)
                #bubble sort in descending order
                for i in range(8,-1,-1):
                    ret = list(user_count[i].split(","))
                    if(int(ret[1]) < us_count):
                        #swap logic
                        t_node=int(ret[0])
                        t_uscount=int(ret[1])
                        t_cc=int(ret[2])
                        user_count[i]=user_count[i+1]
                        user_count[i+1]= str(t_node) + "," + str(t_uscount) + "," + str(t_cc)
            #update value for next node
            if(int(node) in range(0,1000)):
                us_count=1
            else:
                us_count=0
            current_count=1
        current_node=node
    #check the last node if needed
    if (current_node==node):
        if (int(current_node) in range(0,1000)):
            # split the list element
            rett = list(user_count[9].split(","))
            if us_count>int(rett[1]):
                #if element is greater than the last then add to list
                user_count[9]=str(current_node) + "," + str(us_count) + "," + str(current_count)
                #bubble sort in descending order
                for i in range(8,-1,-1):
                    ret = list(user_count[i].split(","))
                    if(int(ret[1]) < us_count):
                        #swap logic
                        t_node=int(ret[0])
                        t_uscount=int(ret[1])
                        t_cc=int(ret[2])
                        user_count[i]=user_count[i+1]
                        user_count[i+1]= str(t_node) + "," + str(t_uscount) + "," + str(t_cc)
    #printing the top-10 nodes
    print_cost(user_count)

#Function to multiply with 10 and print cost
def print_cost(user_count):
    cost=0
    for element in range(0,10):
        rett = list(user_count[element].split(","))
        cost+=int(rett[2])*10
    print("Cost = ",cost)

if __name__ == "__main__":
    main()