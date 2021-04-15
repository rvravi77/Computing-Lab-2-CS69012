# Name:reducer3.py
# Ravi Pratap Singh (20CS60R60)
# Description:To find out the users who have friendship
#              with at least one out of the top-10 users.
import  sys
#Function to find out the users who have friendship with at least one out of the top-10 users.
def main():
    current_node=""
    current_count=0
    print("Users who have friendship with at least one out of the top-10 users:")
    for line in sys.stdin:
        line=line.strip()
        #splitting string with space separated nodes
        node=line.split()[0]
        try:
            count=int(line.split()[1])
        except ValueError:
            sys.exit(1)
        
        #update current count of freinds
        if(current_node=="" or node==current_node):
            current_count+=count
        else:
            #printing current node and updating count
            print(current_node)
            current_count=count
        current_node=node
    #printing last node if needed
    if current_node == node:
        print(current_node)

if __name__ == "__main__":
    main()