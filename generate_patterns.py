import os, sys, random, argparse

pattern_loc = "."
corpus_log = "./sets"

parser = argparse.ArgumentParser(description='Generates r patterns of length m for the circular pattern matching (CPM) problem',
                epilog="Example:\npython3 generate_patterns.py -c english.100MB -r 1,10 -m 8,16,32,64")
parser.add_argument("-c", "--corpus", dest='c', type=str, default='english.100MB', help="corpus")
parser.add_argument("-r", "--npatterns", dest='r', type=str, default='1', help="number of patterns")
parser.add_argument("-m", "--length", dest='m', type=str, default='8,16,32,64', help="pattern length[s] (e.g. 8,16,32)")

args = parser.parse_args()

c_args_list     = args.c.split(',')
r_args_list     = args.r.split(',')
m_args_list     = args.m.split(',')


for c in c_args_list:
	for m_el in m_args_list:
		for r in r_args_list:
			m = int(m_el)
			set_filename = "{}/{}".format(corpus_log, c)
			
			if not os.path.isfile(set_filename):
				print("Error: The set file not found ({})".format(set_filename))
				exit()

			with open(set_filename, "rb") as corpus:
				t = corpus.read()
				pattern_filename = "{}/patterns.r{}/patterns.{}.{}.bin".format(pattern_loc, r, c, m)

				os.makedirs(os.path.dirname(pattern_filename), exist_ok=True)
				outFile = open(pattern_filename, "wb")

				for i in range(int(r)):
					startPos = random.randint(0, len(t) - m)
					rotation = random.randint(0, m - 1)
					patt = t[startPos:startPos + m]
					patt = patt[rotation:] + patt[:rotation]
					outFile.write(patt)
				outFile.close()
