import os, sys, random, argparse

parser = argparse.ArgumentParser(description='Generates r patterns of length m for the circular pattern matching (CPM) problem',
                epilog="Example:\npython3 generate_patterns.py -c english.100MB -r 100,1000 -m 8,16,32,64")
parser.add_argument("-c", "--corpus", dest='c', type=str, default='english.100MB', help="corpus")
parser.add_argument("-m", "--length", dest='m', type=str, default='8,16,32,64', help="pattern length[s] (e.g. 8,16,32)")

args = parser.parse_args()

c_args_list     = args.c.split(',')
m_args_list     = args.m.split(',')


for t_el in c_args_list:
	for m_el in m_args_list:
		m = int(m_el)
		set_filename = "./sets/" + t_el
		
		if not os.path.isfile(set_filename):
			print "Error: The set file not found ( "+set_filename+")"
			exit()

		t = open(set_filename, "rb").read()
		outFile = open("./patterns.r" + str(r) + "/patterns." + t_el + "." + str(m) + ".bin", "wb")

		for i in xrange(r):
			startPos = random.randint(0, len(t) - m)
			rotation = random.randint(0, m - 1)
			patt = t[startPos:startPos + m]
			patt = patt[rotation:] + patt[:rotation]
			outFile.write(patt)
		outFile.close()
