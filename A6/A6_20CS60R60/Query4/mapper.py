# Name:mapper4.py
# Ravi Pratap Singh (20CS60R60)
# Description:To find cost of ad for top-10 US users with  
#             the highest number of friends in the US
   
import sys
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
        try:
            #splitting string with comma separated nodes
            line=line.strip()
            l_arr=line.split(",")
            node1=l_arr[0]
            node2=l_arr[1]
        except Exception as e:
            #print(e)
            continue
        #printing <key,value> pair as <node_id,<edge>>
        print(node1+","+node1+","+node2)
        print(node2+","+node1+","+node2)
    file.close()

if __name__ == "__main__":
    main()