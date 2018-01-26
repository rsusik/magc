
cpm_mag_dna_opt_l3: cpm_mag_dyn.cpp
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_dna_opt_l3 -std=c++11 -Wno-aggressive-loop-optimizations -O3 -o cpm_mag_dna_opt_l3 cpm_mag_dyn.cpp
	
cpm_mag_dna_opt_l2: cpm_mag_dyn.cpp
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_dna_opt_l2 -std=c++11 -Wno-aggressive-loop-optimizations -O3 -o cpm_mag_dna_opt_l2 cpm_mag_dyn.cpp
	
cpm_mag_dna_l4: cpm_mag_dyn.cpp
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_dna_l_bits -D Q_GRAM_UTIL_NEW_L=4 -std=c++11 -Wno-aggressive-loop-optimizations -O3 -o cpm_mag_dna_l4 cpm_mag_dyn.cpp

cpm_mag_dna_l3: cpm_mag_dyn.cpp
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_dna_l_bits -D Q_GRAM_UTIL_NEW_L=3 -std=c++11 -Wno-aggressive-loop-optimizations -O3 -o cpm_mag_dna_l3 cpm_mag_dyn.cpp

cpm_mag_dna_l2: cpm_mag_dyn.cpp
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_dna_l_bits -D Q_GRAM_UTIL_NEW_L=2 -std=c++11 -Wno-aggressive-loop-optimizations -O3 -o cpm_mag_dna_l2 cpm_mag_dyn.cpp

cpm_mag_l4: cpm_mag_dyn.cpp
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_l_bits -D Q_GRAM_UTIL_NEW_L=4 -std=c++11 -Wno-aggressive-loop-optimizations -O3 -o cpm_mag_l4 cpm_mag_dyn.cpp
	
cpm_mag_l3: cpm_mag_dyn.cpp
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_l_bits -D Q_GRAM_UTIL_NEW_L=3 -std=c++11 -Wno-aggressive-loop-optimizations -O3 -o cpm_mag_l3 cpm_mag_dyn.cpp
	
cpm_mag_l2: cpm_mag_dyn.cpp
	g++ -D Q_GRAM_UTIL_V=q_gram_utilities_l_bits -D Q_GRAM_UTIL_NEW_L=2 -std=c++11 -Wno-aggressive-loop-optimizations -O3 -o cpm_mag_l2 cpm_mag_dyn.cpp
	
cpm_mag_dna_opt: cpm_mag_dna_opt_l2 cpm_mag_dna_opt_l3 

cpm_mag_dna: cpm_mag_dna_l2 cpm_mag_dna_l3 cpm_mag_dna_l4

cpm_mag: cpm_mag_l2 cpm_mag_l3 cpm_mag_l4
	
all: cpm_mag cpm_mag_dna cpm_mag_dna_opt 

clean:
	rm -f cpm_mag_dna_opt_l3 cpm_mag_dna_opt_l2 cpm_mag_dna_l4 cpm_mag_dna_l3 cpm_mag_dna_l2 cpm_mag_l4 cpm_mag_l3 cpm_mag_l2 *.exe *.pdb