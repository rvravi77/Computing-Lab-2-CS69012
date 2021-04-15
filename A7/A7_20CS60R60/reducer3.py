# Name:reducer3.py
# Ravi Pratap Singh (20CS60R60)
# Description: Find out the total number of triangles passing through each node

import sys
#function to count number of mutual freind
def reducer():

    prev_index = ""     #stores previous index i.e the key
    prev_value= []      #to match two consequtive value which is in the form of (index,value), index is 'k' ,value is 'a_ij'
    result = 0          #stores result of index 'ij'
    curr_index=""
    for line in sys.stdin:
        #split the values from mapper
        curr_index, index, value = line.rstrip().split("$")
        #convert into int
        index = int(index)
        value = int(value)

        #if we are at same key
        if prev_index=="" or curr_index == prev_index:

            if  len(prev_value)<2:
                #storing only two consequtive value
                prev_value.append((index,value))

            elif prev_value[0][0] == prev_value[1][0]:
                #if two consequtive values are same
                #add the multiplication of those into result
                result+=prev_value[0][1] * prev_value[1][1]
                #remove those two values
                prev_value.pop(0)
                prev_value.pop(0)
                #append the current tuple
                prev_value.append((index,value))
                
            else:
                #if two consequtive values not are same
                #remove the first (index,value) tuple from list
                prev_value.pop(0)
                #append the current tuple
                prev_value.append((index,value))

        else:
            try:
                #calculate for last value if needed
                if prev_value[0][0] == prev_value[1][0]:
                    result+=prev_value[0][1] * prev_value[1][1]
            except Exception as e:
                #print(e)
                pass
            #printing out the result
            print_dat=prev_index.split("%")
            #print only diagonal values
            if(print_dat[0]==print_dat[1]):
                #get rid of duplicate calculation
                result=int(result/2)
                print(print_dat[0]+","+str(result))
            
            #setting up variable for next key
            result=0
            prev_value.clear()
            prev_value.append((index,value))
        prev_index = curr_index

    #output last result if needed
    if curr_index == prev_index:
        print_dat=prev_index.split("%")
        #print only diagonal values
        try:
            if(print_dat[0]==print_dat[1]):
                #get rid of duplicate calculation
                result=int(result/2)
                print(print_dat[0]+","+str(result))
        except:
            pass

if __name__ == '__main__':
    reducer()