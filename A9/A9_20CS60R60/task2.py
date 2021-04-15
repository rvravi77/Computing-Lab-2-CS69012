# CS60012 A9
# task1
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
also_like_name = []
also_like_link = []
where_to_watch = []
Cast_Character_link = []
high_rate = []
low_rate = []
bi_day = []
o_movie = []
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
    'E_SPAN',
    'S_LNAME',
    'E_LNAME',
    'LLINK',
    'S_WTW',
    'H_RATE',
    'L_RATE',
    'S_BIDAY',
    'E_BIDAY',
    'S_OMT',
    'E_OMT',
    'TROW'
)

#function to convert latin characters to english
def strip_accents(text):
    return ''.join(char for char in unicodedata.normalize('NFKD', text) if unicodedata.category(char) != 'Mn')
 
#fucntions for lexer starts here
def t_S_BIDAY(t):
    r'\s*</p>\s*<p\s*class=\"celebrity-bio__item\"\s*data-qa=\"celebrity-bio-bday\">\s*Birthday:'
    return(t)

def t_H_RATE(t):
    r'\s*Highest\s*Rated:.*[^%]*%\s*.*\s*'
    return(t)

def t_L_RATE(t):
    r'\s*Lowest\sRated:.*[^%]*%\s*.*\s*'
    return(t)

def t_E_BIDAY(t):
    r'\s*</p>\s*<p\sclass=\"c'
    return(t)

def t_TROW(t):
    r'\s*<tr\s*'
    return(t)

def t_S_OMT(t):
    r'data-title=.*\s*data-boxoffice=.*\s*data-year=.*'
    return(t)

def t_E_OMT(t):
    r'\s*data-tomatometer=.*\s*data-audiencescore=.*\s*data-qa=\"celebrity-filmography-movies-trow\">'
    return(t)

def t_S_WTW(t):
    r'link--veneer\s*js-affiliate-link\"\s*data-affiliate=\"'
    return(t)

def t_LLINK(t):
    r'\s*<a\shref=\"/m/([a-zA-Z0-9\-_]+)\"\s*class=\"recommendations-panel__poster-link\">\s*'
    t.value = str(t.value).strip()
    return(t)

def t_S_LNAME(t):
    r'<span\sslot=\"title\".*[^\>]\">'
    return(t)

def t_E_LNAME(t):
    r'\s*</span>\s*</tile-poster-meta>\s*'
    return(t)

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

def t_S_LANGU(t):
    r'Original\sLanguage:</div.*[^>]+>'
    return(t)

def t_S_RUNTI(t):
    r'Runtime:.*\s*<div.*\s*<time.*\s*'
    return(t)

def t_error(t):
#    print("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)

t_S_MOVIE    = r'<title>'
t_E_MOVIE    = r'</title>'
t_S_DIREC    = r'\s*<a.*\s*\n*data-qa=\"movie-info-director\">\s*'
t_E_ANKOR    = r'\n*\s*<\/a>'
t_E_DIV      = r'\n*\s*<\/div>\s*\n*'
t_S_SYNOP    = r'<div\sid=\"movieSynopsis\".*\s*\n*'
t_S_BOXOF    = r'Box\sOffice\s[a-zA-Z\s\(\)]+:</div>\n*\s*<div\sclass=.*\">'
t_E_RUNTI    = r'\s*<\/time>'
t_COLLECTION = r'[\$|\€|\£|\₹][0-9]+\.?[0-9]*[a-zA-Z]+'
t_ALL        = r'[a-zA-Z0-9|\(\)\.\-\s\"\'\,\-:\!\?]+'

#fucntions for parser starts here
#parser function for start of other movie name of cast
def p_o_movie(t):
    'start : TROW S_OMT E_OMT'
    string = re.split('\s\s+',t[2])
    o_movie.append(string)

#parser function for birthday of cast
def p_biday(t):
    'start : S_BIDAY ALL E_BIDAY'
    bi_day.append(t[2].strip())

#parser function for highest rated movie of cast
def p_hrate(t):
    'start : H_RATE ALL E_ANKOR'
    high_rate.append(t[2].strip())

#parser function for lowest rated movie of cast
def p_lrate(t):
    'start : L_RATE ALL E_ANKOR'
    low_rate.append(t[2].strip())

#parser function for where to watch
def p_wtw(t):
    'start : S_WTW ALL'
    wtw = t[2][:-8]
    where_to_watch.append(wtw)

#parser function for might also like name
def p_lname(t):
    'start : S_LNAME ALL E_LNAME'
    name = str(t[2]).replace(","," ")
    also_like_name.append(name)

#parser function for might also like link
def p_llink(t):
    'start : LLINK ALL'
    link = re.findall(r'=\"(\/m\/[a-zA-Z0-9\-_]+)\"', t[1])[0]
    also_like_link.append(link)

#parser function for genre
def p_genre(t):
    'start : S_GENRE ALL E_DIV'
    gen = "".join(t[2].split()) 
    genre.append(gen)

#parser function for cast and character
def p_star(t):
    '''start : S_STAR ALL E_SPAN S_CHAR ALL E_BR
             | S_STAR ALL E_SPAN S_CHAR ALL E_SPAN'''
    characters = t[5].split(",")                                        #all different character in movie of a cast
    char_link = re.findall(r'\"\s/celebrity/(.*)\s\"',t[1])[0]          #link of the page for that cast
    res = ""
    for c in range(len(characters)):
        res = res + str(characters[c]).strip().replace("\\n","") + "|"  #storing all character with delimeter -->'|'
    entry  = str(t[2]).strip() + ";" + str(res)[:-1]
                                                                        #append in list if it only cast and not crew
    if entry.find("Director")<0 and entry.find("Producer")<0 and entry.find("Writer")<0 and entry.find("Screenwriter")<0 and entry.find("Music")<0 and entry.find("Editor")<0 and entry.find("Cinematographer")<0 and entry.find("Casting")<0 and entry.find("Production") and entry.find("Editor")<0 and entry.find("Researcher") and entry.find("Direction")<0:     
        Cast_Character.append(entry)
        Cast_Character_link.append(char_link)

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

#function to get HTML Data for the page
def get_HTML_DATA(link_value,choice_val):
    link_val = str(link_value[int(choice_val)-1]).strip()[1:][:-1]
    new_link = "https://www.rottentomatoes.com" + link_val
    response_mov = urllib.request.urlopen(new_link)
    webContent_mov = response_mov.read()
    webContent_mov = webContent_mov.decode("utf-8")
    new_data = strip_accents(webContent_mov)
    new_data = new_data.replace('&#39;','\'')
    return new_data

#function to add delimeter for multiple value fields
def add_DELIM():
    Writers.append(";")
    Producer.append(";")
    Director.append(";")
    Cast_Character.append(";")
    Cast_Character_link.append(";")
    also_like_link.append(";")
    also_like_name.append(";")
    where_to_watch.append(";")

#function to add "Not available" for fields with empty values
def not_AVAIL_FIELDS():
    if len(Writers)== 0 or len(Writers[-1]) == 0: 
        Writers.append("Not Available")
    if len(Producer)== 0 or len(Producer[-1]) == 0: 
        Producer.append("Not Available")
    if (len(Box_Office)<len(Movie_Name)):
        Box_Office.append("Not Available")
    if (len(Runtime)<len(Movie_Name)):
        Runtime.append("Not Available")

#recursive function for you might also like
def ymal(name, link, choice):

    #getting the desired fields value corresponding to choice 
    name = str(name)[1:][:-6]
    link = str(link)[1:][:-6]
    t_name_new = name.split("';'")
    t_link_new = link.split("';'")
    name_value = [ele for ele in t_name_new[choice].split(",") if len(ele.strip()) != 0]
    link_value = [ele for ele in t_link_new[choice].split(",") if len(ele.strip()) != 0]

    count = 0
    print("You might also like -- >")
    for i in name_value:
        count += 1
        print(str(count)+"."+str(i))
    more_val = input("Want more alike?(y/n)>")
    #if user want to view more alike movies name
    if more_val == "y":
        choice_val = input("Enter choice (1-N) from above>")

        if int(choice_val) not in range(1,count+1):
            print("Enter Val in Range !!!!")
            sys.exit(choice_val)

        new_data = get_HTML_DATA(link_value,choice_val)                 #getting html data
        parser2 = yacc.yacc()
        parser2.parse(new_data)                                         #parsing new data
        #handling fields which may have zero values
        not_AVAIL_FIELDS()
        #adding delimeter to list which might have multiple vales
        Movie_Name.pop()
        add_DELIM()
        #recursive call
        ymal(also_like_name, also_like_link , -1)
        return
    else:
        return

#expanding cast member functionality
def STAR_CAST_FILTER(field_value,field_value_link,star_no):
    
    #getting webpage link corresponding to cast member
    link_val = str(field_value_link[int(star_no)-1]).strip()[1:][:-1]
    new_link = "https://www.rottentomatoes.com/celebrity/" + link_val

    #downloading web page
    response_mov = urllib.request.urlopen(new_link)
    webContent_mov = response_mov.read()

    #saving web_page
    if not os.path.exists("./CAST_PROFILE"): 
        os.mkdir("./CAST_PROFILE")
    new_mov_path = "./CAST_PROFILE/" +str(field_value[int(star_no)-1]).strip()[1:][:-1] + ".html"
    f = open(new_mov_path, 'wb')
    f.write(webContent_mov)
    f.close()

    #reading,converting latin alphabets and aphostrophie of the web page content
    webContent_mov = webContent_mov.decode("utf-8")
    new_data = strip_accents(webContent_mov)
    new_data = new_data.replace('&#39;','\'')

    #lexing and parsing new web page
    lexer2 = lex.lex()
    lexer2.input(new_data)
    parser3 = yacc.yacc()
    parser3.parse(new_data)

    #Giving choice to user
    while True:
        options = input("Choose options(1-4) \n1.Highest rated film\n2.Lowest rated film\n3.Birthday\n4.Other Movies\n>")

        #Exit if value not in range
        if int(options) not in range(1,5):
            print("Enter VAL in range !!!")
            sys.exit(0)
        
        if options == "1":
            if len(high_rate)>0:
                print("Highest Rated Movie:",high_rate[0])
            else:
                print("Highest Rated Movie: NOT AVAILABLE")
        elif options == "2":
            if len(low_rate)>0:
                print("LOwest Rated Movie:",low_rate[0])
            else:
                print("Lowest Rated Movie: NOT AVAILABLE")
        elif options == "3":
            if len(bi_day)>0:
                print("Birthday:",bi_day[0])
            else:
                print("Brthday: NOT AVAILABLE")
        elif options == "4":
            new_m = []
            #Getting only movie title and year out of the 3 fields extracted
            for i in o_movie:
                t_new_m=[]
                for j in i:
                    new_x = j.split("=")
                    if new_x[0] == "data-title":
                        t_new_m.append(new_x[1][1:][:-1])
                    if new_x[0] == "data-year":
                        t_new_m.append(int(new_x[1][1:][:-1]))
                new_m.append(t_new_m) 
            #sorting accotding to year   
            new_m.sort(key=lambda x: x[1])
            year = input("Enter year to filter> ")
            #Filtering movie list according to year
            count_list = 0
            for y in new_m:
                if(y[1] > int(year)):
                    count_list +=1
                    print(str(count_list)+"."+ y[0])
        want_info = input("Want more info on this cast?(y/n)>")
        if want_info.casefold() == "y":
            continue
        else:
            break
    #Clearing up the temporary variables
    Movie_Name.pop()
    high_rate.clear()
    low_rate.clear()
    bi_day.clear()
    o_movie.clear()
    return

#main starts here
def main():
    
    if not os.path.exists("./MOVIE"):
        print("ENSURE ALL MOVIE FILES ARE IN ./MOVIE Directory!!")
        sys.exit(0)
    
    #parsing every movie file in the MOVIE directory
    for filename in glob.glob("./MOVIE/*.html"):
        with open(os.path.join(os.getcwd(), filename), 'r') as myfile:
            print("Parsing -->" , filename)
            data = strip_accents(myfile.read())
            data = data.replace('&#39;','\'')
            lexer = lex.lex()
            parser = yacc.yacc()
            parser.parse(data)
            #handling fields which may have zero values
            not_AVAIL_FIELDS()
            #adding delimeter to list which might have multiple vales
            add_DELIM()
    of = open("2C0S60R60_log_file_task2.txt","w")
    #get user input
    while True:
        idx = 0
        print("Giving movie name choice to user")
        for i in Movie_Name:
            idx = idx + 1
            print(str(idx) + ".",i)
        user_movie = input("Enter the movie no.(1 - N) from above list\n>")
        user_choice = int(user_movie) - 1
        
        #exit if movie choice not in range
        if int(user_movie) not in range(1,idx+1):
            print("Enter Val in Range !!!!")
            sys.exit(0)
        
        field_requested = input("Enter the field no.(1-10) \n1.Director\n2.Writer\n3.Producer\n4.Language\n5.Cast and character\n6.Synopsys\n7.Box Office\n8.Runtime\n9.You might also like\n10.Where to watch\n>")
        
        #exit if field requested not in range
        if int(field_requested ) not in range(1,11):
            print("Enter Val in Range !!!!")
            sys.exit(field_requested)
        
        #nested if-else according to field requested
        if(field_requested  == "1"):
            tt = str(Director)[1:][:-6]                         #Getting field for requied movie if more than 1 possible values
            if tt.count(';')>0:
                tp = tt.split("';'")
                field_value = tp[user_choice]
            else:
                field_value = tt
            field_req = "Director"
        elif(field_requested == "2"):                           #Getting field for requied movie if more than 1 possible values
            tt = str(Writers)[1:][:-6]
            if tt.count(';')>0:
                tp = tt.split("';'")
                field_value = tp[user_choice]
            else:
                field_value = tt
            field_req = "Writer"
        elif(field_requested == "3"):                           #Getting field for requied movie if more than 1 possible values
            tt = str(Producer)[1:][:-6]
            if tt.count(';')>0:
                tp = tt.split("';'")
                field_value = tp[user_choice]
            else:
                field_value = tt
            field_req = "Producer"
        elif(field_requested == "4"):
            field_value = Original_Language[user_choice]
            field_req = "Language"
        elif(field_requested =="5"):                            #Getting field for requied movie if more than 1 possible values
            
            #getting cast name and corresponding web page link for it
            tt = str(Cast_Character)[1:][:-6]
            tt1 = str(Cast_Character_link)[1:][:-6]
            #if multiple movies are parsed
            if tt.count(';')>0 and tt1.count(';')>0:
                tp = tt.split("';'")
                tp1 = tt1.split("';'")
                field_value = tp[user_choice]
                field_value_link = tp1[user_choice]
            #if single movie is parsed
            else:
                field_value = tt
                field_value_link = tt1
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
        elif(field_requested == "9"):
                                                                    #Function  call for you might also like
            ymal(also_like_name , also_like_link ,user_choice)
            field_value = ""
            pass
        elif(field_requested == "10"):
            tt = str(where_to_watch)[1:][:-6]
            if tt.count(';')>0:
                tp = tt.split("';'")
                field_value = tp[user_choice].strip(",")
            else:
                field_value = tt
            field_req = "Where to watch"
        
        file_data = ""
        #If output fiels is having multiple value
        if( (len(field_value.split(","))>1) and (field_requested == "1" or field_requested == "2" or field_requested == "3" or field_requested == "10" )):
            for i in field_value.split(","):
                if len(i.strip(" "))>0:
                    file_data = str('\n') + "<" + str(genre[user_choice]) + "><" + str(Movie_Name[user_choice]) +  "><" + str(field_req) +  "><"  + str(i).strip(" ") + '>' + str('\n') + str('\n')
                    print(file_data)
                    of.write(file_data)

        #If output fiels is having single value exept you might also like and cast and character
        elif(field_requested != "9" and field_requested != "5"):
            if len(field_value.strip(" "))>0:
                file_data = str('\n') + "<" + str(genre[user_choice]) + "><" + str(Movie_Name[user_choice]) +  "><" + str(field_req) +  "><"  + str(field_value).strip(" ") + '>' + str('\n') + str('\n')
                print(file_data)
                of.write(file_data)

        #if cast character field requested
        elif(field_requested == "5"):

            #strip out the extra newlines and white spaces
            field_value = [ele for ele in field_value.split(",") if len(ele.strip(" ")) != 0]
            field_value_link  = [ele for ele in field_value_link.split(",") if len(ele.strip(" ")) != 0]

            #giving user to choose from the cast;character list
            star_count = 0
            for i in field_value:
                star_count +=1
                file_data = str(i).strip(" ")
                print(str(star_count)+". "+ file_data)
            
            #if more than one cast;character available
            if len(file_data) != 0:
                star_no = input("Select Cast No. (1-N) from above list\n>")
                #when user enters out of range value
                if int(star_no) not in range(1,star_count+1):
                    print("Enter Val in Range !!!!")
                    sys.exit(0)
                #calling the function for expanded functionality of cast and character field
                STAR_CAST_FILTER(field_value,field_value_link,star_no)
            else:
                file_data = str('\n') + "<" + str(genre[user_choice]) + "><" + str(Movie_Name[user_choice]) +  "><" + str(field_req) +  "><"  + "NOT AVAILABLE" + '>' + str('\n') + str('\n')
                print(file_data) 
        
        #If no output to the file then field is not available
        if len(file_data) == 0 and field_requested != "9" and field_requested != "5":
            file_data = str('\n') + "<" + str(genre[user_choice]) + "><" + str(Movie_Name[user_choice]) +  "><" + str(field_req) +  "><"  + "NOT AVAILABLE" + '>' + str('\n') + str('\n')
            print(file_data)

        choice = input("Want to continue on movie list(y/n)>")
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