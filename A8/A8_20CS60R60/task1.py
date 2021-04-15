# CS60012 A8
# task1
# Ravi Pratap Singh (20CS60R60)

import urllib.request, urllib.error, urllib.parse
import os
import codecs
import re
import sys

def main():
	#getting links from text files
	if not os.path.exists("./HTML"): 
		os.mkdir("./HTML")
	inputFileName = "rotten tomatoes movie genre link.txt"
	with open(inputFileName) as f:
		content = f.readlines()
	content = [x.strip() for x in content] 
	genre_name=""
	genre_dict = {}
	for url in content:
		if url.endswith("/"):
			url.strip()
			genre_dict[genre_name] = url
		elif url[0].isnumeric():
				temp=url.split(".")
				genre_name=temp[1][:-1].strip()
		else:
			continue

	#prompt user to enter gere		
	while True:
		print("Genre list:")
		for g in genre_dict.keys():
			print(g)
		genre = str(input("Enter the genre(case sensitive): ") )
		genre_path = "./HTML/" + genre + ".html"
		if not os.path.isfile(genre_path):
			print(f'Fetching web page: {genre_dict[genre]}')
			response = urllib.request.urlopen(genre_dict[genre])
			webContent = response.read()
			f = open(genre_path, 'wb')
			f.write(webContent)
			f.close()
		
		f=codecs.open(genre_path, mode="r")
		s=f.read()
		all_urls={}

		#regex for getting top 100 movies
		regex = r"\ba\shref=\"\/m\/\b([a-zA-Z0-9\-_]+)\"\sclass=\"unstyled articleLink\">[^\\n*]\s*([a-zA-Z0-9.,:\-()ÄÖÜäöüâÂôÔêÊ[^\'\"\s+]+]*)"
		matches = re.finditer(regex, s, re.MULTILINE)

		#printing top 100 movies
		for matchNum, match in enumerate(matches, start=1):
			all_urls[match.group(2)] = match.group(1)
		for key in all_urls.keys():
			print(key)

		#prompt user to enter movie name
		while True:
			mov = input("SELECT MOVIE FROM THE ABOVE LIST(case sensitive):") 
			if not os.path.exists("./MOVIE"): 
				os.mkdir("./MOVIE")

			mov_url = "https://www.rottentomatoes.com/m/" + str(all_urls[mov])
			print(f'Fetching web page: {mov_url}')
			try:
				response_mov = urllib.request.urlopen(mov_url)
				webContent_mov = response_mov.read()
			except Exception as e:
				print(e)
				continue			
			mov_name = "./MOVIE/" + genre + ";" + mov + ".html"
			f = open(mov_name, 'wb')
			f.write(webContent_mov)
			f.close()
			print("Downloaded Movie ",mov," of ",genre)
			want = input("Want to continue on same genre (y/n)> ")
			if(want == 'y'):
				continue
			elif(want == 'n'):
				break
			else:
				print("Enter valid code")
				sys.exit(10)
		want2 = input("Want to continue more (y/n)> ")
		if(want2 == 'y'):
			continue
		elif(want2 == 'n'):
			break
		else:
			print("Enter valid code")
			sys.exit(10)

if __name__=="__main__":
	main()