import sys, random, os

if sys.argv[1]=='--help':
	print 'Circullar patter matching patterns generator help.'
	print 'Generates r patterns of length m for the circular pattern matching (CPM) problem'
	print ''
	print 'Syntax:'
	print 'generatePatterns [--help] test_set r m'
	print ''
	print 'Parameters:'
	print '--help         Shows the help'
	print 'test_set       Test set(eg. english.100MB)'
	print 'r              Numer of generated patterns'
	print 'm              Pattern length(eg. 8)'
	print ''
	print 'Example:'
	print 'python generatePatterns.py dna.10MB 1 8,16,32,64,128'
	print ''
	exit()

t_args = sys.argv[1]
r      = int(sys.argv[2])	# no of patterns, must be an even number!
m_args = sys.argv[3]		# pattern length

t_args_list = t_args.split(',')
m_args_list = m_args.split(',')

for t_el in t_args_list:
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
