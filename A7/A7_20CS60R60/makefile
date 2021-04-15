NUMBERS = 1 2 3 4 5 6 7 8 9 10
run:
		($(foreach var,$(NUMBERS),(python3 ./mapper1.py ./day$(var).txt |sort |python3 ./combiner1.py);)) |sort -g|python3 ./reducer1.py >./network.txt
		python3 ./mapper2.py |sort -g|python3 ./reducer2.py >./friends.txt
		python3 ./mapper3.py |sort -g|python3 ./reducer3.py >./triangles.txt