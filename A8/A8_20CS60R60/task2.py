# CS60012 A8
# task2
# Ravi Pratap Singh (20CS60R60)
import urllib.request, urllib.error, urllib.parse
import ply.lex as lex
import ply.yacc as yacc
import re
import codecs
import glob
import os
import unicodedata
import sys

#global variables to store required fields
Movie_Name = []
Director = []
Writers = []
Producer = []
Original_Language = []
Cast_Character = []
Synopsys = []
Box_Office = []
Runtime = []
movie_dict = []
genre = []
#functions for lexical analyzer starts here
tokens = (
    'S_STAR',
    'S_CHAR',
    'S_GENRE',
    'E_BR',
	'S_MOVIE',
	'E_MOVIE',
	'S_DIREC',
	'S_WRITE',
    'S_NAME',
	'E_DIV',
	'S_PRODU',
    'S_LANGU',   
    'S_SYNOP',
    'S_BOXOF',
    'S_RUNTI',
    'E_RUNTI',
    'ALL',
    'COLLECTION',
    'E_ANKOR',
    'E_SPAN'
)

#function to convert latin characters
def strip_accents(text):
    return ''.join(char for char in unicodedata.normalize('NFKD', text) if unicodedata.category(char) != 'LC')
 
#fucntions for lexer starts here   
def t_S_GENRE(t):
    r'Genre:</div>\s*<div\sclass=\"meta-value\sgenre\"\s.*\s*'
    return(t)

def t_S_STAR(t):
    r'<a\shref=\"\s/celebrity/.*\s\"\sclass=\"unstyled\sarticleLink\"\sdata-qa=\"cast-crew-item-link\">\s*<span.*\s*'
    return(t)

def t_S_CHAR(t):
    r'\s*</a>\s*<span\sclass=\"characters\ssubtle\ssmaller\"\stitle=.*\s*.*\s*'
    return(t)

def t_E_BR(t):
    r'\s*<br\/>'
    return(t)

def t_S_WRITE(t):
    r'Writer:</div>\n*\s*<div.*[^<]+'
    return(t)

def t_S_PRODU(t):
    r'Producer:</div>\n*\s*<div.*[^<]+'
    return(t)

def t_S_NAME(t):
    r'<a\s*href=\"/celebrity/[a-zA-Z0-9_\-\":]+>'
    return(t)

def t_E_SPAN(t):
    r'\s*</span>'
    return(t)

def t_error(t):
#   print("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)

t_S_MOVIE    = r'<title>'
t_E_MOVIE    = r'</title>'
t_S_DIREC    = r'\s*<a.*\s*\n*data-qa=\"movie-info-director\">\s*'
t_E_ANKOR    = r'\n*\s*<\/a>'
t_E_DIV      = r'\n*\s*<\/div>\s*\n*'
t_S_LANGU    = r'Original\sLanguage:</div.*[^>]+>'
t_S_SYNOP    = r'<div\sid=\"movieSynopsis\".*\s*\n*'
t_S_BOXOF    = r'Box\sOffice\s[a-zA-Z\s\(\)]+:</div>\n*\s*<div\sclass=.*\">'
t_S_RUNTI    = r'Runtime:.*\s*<div.*\s*<time.*\s*'
t_E_RUNTI    = r'\s*<\/time>'
t_COLLECTION = r'[\$|\€|\£|\₹][0-9]+\.?[0-9]*[a-zA-Z]+'
t_ALL        = r'[a-zA-Z0-9|\(\)\.\-\s\"\'\,\-:]+'
    
#fucntions for parser starts here
#parser function for genre
def p_genre(t):
    'start : S_GENRE ALL E_DIV'
    gen = "".join(t[2].split()) 
    genre.append(gen)

#parser function for starcast
def p_star(t):
    '''start : S_STAR ALL E_SPAN S_CHAR ALL E_BR
             | S_STAR ALL E_SPAN S_CHAR ALL E_SPAN'''
    print(t[2])
    characters = t[5].split(",")
    res = ""
    for c in range(len(characters)):
        res = res + str(characters[c]).strip().replace("\\n","") + "|"
    entry  = str(t[2]).strip() + ";" + str(res)[:-1]
    #not storing the fields belonging to crew
    if entry.find("Director")<0 and entry.find("Producer")<0 and entry.find("Writer")<0 and entry.find("Screenwriter")<0 and entry.find("Music")<0 and entry.find("Editor")<0 and entry.find("Cinematographer")<0 and entry.find("Casting")<0 and entry.find("Production") and entry.find("Editor")<0 and entry.find("Researcher") and entry.find("Direction")<0:     
        Cast_Character.append(entry)

#parser function for writer
def p_write(t):
    '''start : S_WRITE termw E_DIV
       termw : termw endw
             | S_NAME ALL E_ANKOR ALL
       endw  : S_NAME ALL E_ANKOR ALL'''
    if len(t)==5:
        Writers.append(t[2])

#parser function for producer
def p_produ(t):
    '''start : S_PRODU termp E_DIV
       termp : termp endp
             | S_NAME ALL E_ANKOR ALL
       endp  : S_NAME ALL E_ANKOR ALL'''
    if len(t)==5:
        Producer.append(t[2])

#parser function for movie name
def p_movie(t): #done
    'start : S_MOVIE ALL E_MOVIE'
    Movie_Name.append(str(t[2].split("-")[0].strip()))

#parser function for director
def p_direc(t): #done
    'start : S_DIREC ALL E_ANKOR'
    Director.append(t[2])

#parser function for language
def p_langu(t): #done
    'start : S_LANGU ALL E_DIV'
    temp_l = str(t[2]).strip()
    Original_Language.append(temp_l)

#parser function for synopsys
def p_synop(t): #done
    'start : S_SYNOP ALL E_DIV'
    temp_s = str(t[2]).strip()
    Synopsys.append(temp_s)

#parser function for boxoffice
def p_boxof(t): #done
    'start : S_BOXOF COLLECTION E_DIV'
    Box_Office.append(t[2])

#parser function for runtime
def p_runti(t): 
    'start : S_RUNTI ALL E_RUNTI'
    temp = str(t[1]).strip()[-15:]
    time = re.findall(r'P([a-z0-9\s]+)M', temp)[0]
    Runtime.append(time)

def p_error(t):
	pass

#main starts here
def main():
                                                                        #parsing every movie file in the MOVIE directory
    for filename in glob.glob("./MOVIE/*.html"):
        with open(os.path.join(os.getcwd(), filename), 'r') as myfile:  #opening the file
            print("Parsing -->" , filename)             
            data = strip_accents(myfile.read())                         #convert all accents letters to english letter
            data = data.replace('&#39;','\'')                           #replace the apostrophie denotion
            lexer = lex.lex()
            lexer.input(data)
            parser = yacc.yacc()
            parser.parse(data)

                                                                        #handling fields which may have zero values
            if len(Writers)== 0 or len(Writers[-1]) == 0: 
                Writers.append("NOT AVAILABLE")
            if len(Producer)== 0 or len(Producer[-1]) == 0: 
                Producer.append("NOT AVAILABLE")
            if (len(Box_Office)<len(Movie_Name)):
                Box_Office.append("NOT AVAILABLE")
            if (len(Runtime)<len(Movie_Name)):
                Runtime.append("NOT AVAILABLE")
                                                                        #adding delimeter to list which might have multiple vales
            Writers.append(";")
            Producer.append(";")
            Director.append(";")
            Cast_Character.append(";")
                                                                        #opening log file
    of = open("20CS60R60_Log_file.txt","w")
                                                                        #get user input
    while True:
        idx = 0
                                                                        #print("Giving movie name choice to user")
        for i in Movie_Name:
            idx = idx + 1
            print(str(idx) + ".",i)
        user_movie = input("Enter the movie no.(1 - N) from above list\n>")
        user_choice = int(user_movie) - 1
                                                                        #exit on invalid choice
        if int(user_movie) not in range(1,idx+1):
            print("Enter Val in range !!!")
            sys.exit(0)
        field_requested = input("Enter the field no.(1-8) \n1.Director\n2.Writer\n3.Producer\n4.Language\n5.Cast and character\n6.Synopsys\n7.Box Office\n8.Runtime\n>")
        #getting required value according to user input
                                                                        #exit on invalid field range
        if int(field_requested) not in range(1,9):
            print("Enter Val in range !!!")
            sys.exit(0)
                                                                        #nested if-else on different fields
        if(field_requested  == "1"):
            tt = str(Director)[1:][:-6]                                 #Removing delimeter for field which have multiple entries
            if tt.count(';')>0:
                tp = tt.split("';'")
                field_value = tp[user_choice]
            else:
                field_value = tt
            field_req = "Director"
        elif(field_requested == "2"):
            tt = str(Writers)[1:][:-6]                                  #Removing delimeter for field which have multiple entries
            if tt.count(';')>0:
                tp = tt.split("';'")
                field_value = tp[user_choice]
            else:
                field_value = tt
            field_req = "Writer"
        elif(field_requested == "3"):
            tt = str(Producer)[1:][:-6]                                 #Removing delimeter for field which have multiple entries
            if tt.count(';')>0:
                tp = tt.split("';'")
                field_value = tp[user_choice]
            else:
                field_value = tt
            field_req = "Producer"
        elif(field_requested == "4"):
            field_value = Original_Language[user_choice]
            field_req = "Language"
        elif(field_requested =="5"):
            tt = str(Cast_Character)[1:][:-6]                           #Removing delimeter for field which have multiple entries
            if tt.count(';')>0:
                tp = tt.split("';'")
                field_value = tp[user_choice]
            else:
                field_value = tt
            field_req = "Cast;Character"
        elif(field_requested == "6"):
            field_value = Synopsys[user_choice]
            field_req = "Synopsys"
        elif(field_requested == "7"):
            field_value = Box_Office[user_choice]
            field_req = "Box Office"
        elif(field_requested == "8"):
            field_req = "Runtime"
            field_value = Runtime[user_choice]
        
        #printing to file
        file_data = ""
        #for multiple entry field
        if((len(field_value.split(","))>1) and (field_requested == "1" or field_requested == "2" or field_requested == "3" or field_requested == "5")):
            for i in field_value.split(","):
                if len(i.strip(" "))>0:
                    file_data = str('\n') + "<" + str(genre[user_choice]) + "><" + str(Movie_Name[user_choice]) +  "><" + str(field_req) +  "><"  + str(i).strip(" ") + '>' + str('\n') + str('\n')
                    print(file_data)
                    of.write(file_data)
        else:                                                          #for single entry field     
            file_data = str('\n') + "<" + str(genre[user_choice]) + "><" + str(Movie_Name[user_choice]) +  "><" + str(field_req) +  "><"  + str(field_value).strip(" ") + '>' + str('\n') + str('\n')
            print(file_data)
            of.write(file_data)

        if len(file_data) == 0:
            file_data = str('\n') + "<" + str(genre[user_choice]) + "><" + str(Movie_Name[user_choice]) +  "><" + str(field_req) +  "><"  + "NOT AVAILABLE" + '>' + str('\n') + str('\n')
            print(file_data)
        
        choice = input("Want to continue on movie list(y/n)")
        if choice.casefold() == 'n':
            break
        elif choice.casefold() == 'y':
            continue
        else:
            print("Enter valid choice")
            pass
    of.close()

if __name__ == "__main__":
    main()