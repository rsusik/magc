#ifndef cpm_MAG_DYN_H
#define cpm_MAG_DYN_H

#ifndef Q_GRAM_UTIL_V
	#define Q_GRAM_UTIL_V q_gram_utilities_dna_opt_l3
#endif

#ifndef Q_GRAM_UTIL_NEW_L
	#define Q_GRAM_UTIL_NEW_L 3
#endif

#include "libraries/mag_dyn.h"
#include <unordered_map>
#include <set>

#define log2cache 70000
word log2v[log2cache];


//#define SEARCH_FUNCTION_NAME mag_dyn
//#define MAP_CREATEION_FUNCTION CreateMapByHistogramBinPack

using namespace Q_GRAM_UTIL_V; // defines which q-gram creation should be used
using namespace mag_dyn;

namespace cpm_mag_dyn {

	/*
Checks how many chars on the left are matched 
Parameters:
	P		- pattern pointer
	p_size	- size of the pattern
	T		- text pointer
	t_size	- size of the text
	t_index	- index (position) in text where the matching will begin
	p_index	- index (position) in pattern where the matching will begin
	number	- number of founded chars (should be zero)
*/
unsigned int chcpml( const uchar *P, unsigned int p_size, const uchar *T, unsigned int t_size, int t_index, int p_index, unsigned int number ) {
	if ( p_index < 0) p_index = p_size-1;
	if ( t_index < 0) return number;
	//if ( P[p_index--] != T[t_index--] ) return number;
	if ( memcmp(P + p_index--, T + t_index--, 1) != 0 ) return number;
	return chcpml( P, p_size, T, t_size, t_index, p_index, ++number );
}

/* 
Checks how many chars on the right are matched
Parameters - look at the chcpml function description
*/
unsigned int chcpmr( const uchar *P, unsigned int p_size, const uchar *T, unsigned int t_size, int t_index, int p_index, unsigned int number ) {
	if ( p_index > (int)p_size-1) p_index = 0;
	if ( t_index > (int)t_size-1) return number;
	//if ( P[p_index++] != T[t_index++] ) return number;
	if ( memcmp(P + p_index++, T + t_index++, 1) != 0 ) return number;
	return chcpmr( P, p_size, T, t_size, t_index, p_index, ++number );
}

	void print_result(double data_acc_time, double pre_time, double search_time,
		long ver_count, long while_loops, long matches, long sigma, long super_sigma,
		long text_file_size, long pattern_size
	) {
		double inmb = text_file_size / 1000.0 / 1000.0;
		double pre_search_time = pre_time + search_time;										// pre+search time
		printf("%ld\t%ld\t", sigma, super_sigma);												// sigmas
		printf("%ld\t", pattern_size);															// pattern size
		printf("%ld\t", text_file_size);														// data information
		printf("%ld\t%ld\t%ld\t", ver_count, while_loops, matches);								// search information
		printf("%f\t%f\t%f\t%f\t", data_acc_time, pre_time, search_time, pre_search_time);		// times
		printf("%f\t%f", inmb/search_time, inmb/pre_search_time);								// speed
		printf("\n");
	}

	/* BSEARCH variant */

	// data structures
	typedef struct pelem {
		const uchar * pattern;	// shifed pattern
		const uchar * op;		// pattern
		int offset;				// offset of shifted pattern
	} pelem;

	typedef struct plist {
		pelem * list;    // list of patterns that have the same index (first q-gram)
		int count;       // number of patterns
		static int BYTES_TO_COMPARE;
	} plist;
	int plist::BYTES_TO_COMPARE = 0;

	// patterns indexed by first qgram
	plist *indexed_patterns = NULL;

	// pelem comparator
	int pelem_compar (const pelem* a, const pelem* b) {
		//printf("___________\n%.*s\nVS\n%.*s\n___________\n", plist::BYTES_TO_COMPARE, a->pattern, plist::BYTES_TO_COMPARE, b->pattern);
		return memcmp(a->pattern, b->pattern, plist::BYTES_TO_COMPARE);
	}

	typedef struct {
		long pattern_size;
		long part_size;
		long text_size;
	} cpm_mag_dyn_verification_args;

	// fill indexed_patterns structure (this is needed for verification)
	template<class T> long cpm_mag_dyn_build_indexed_patterns(uchar *patterns_text, int pattern_text_size, int nparts, int part_size, int pattern_size) {
		uchar *pattern = 0;
		indexed_patterns = (plist *)calloc(T::super_sigma, sizeof(plist));
		long patterns_number = 0;
		for ( long l = 0; l < pattern_text_size; l += pattern_size) {
			for ( long a = 0; a < nparts; a++ ) {
				pattern = patterns_text + l + a * part_size;
				memcpy(mag_dyn::patterns[patterns_number], pattern, pattern_size);
				for(int i =0; i < T::q_size; i++) {
					unsigned long long int index = T::get_q_gram(mag_dyn::patterns[patterns_number] + i);
					indexed_patterns[index].list = (pelem*)realloc(indexed_patterns[index].list, sizeof(pelem) * (indexed_patterns[index].count+1));
					indexed_patterns[index].list[indexed_patterns[index].count].pattern = pattern + i ;
					indexed_patterns[index].list[indexed_patterns[index].count].op = patterns_text + l;
					indexed_patterns[index].list[indexed_patterns[index].count].offset = a * part_size + i;
					indexed_patterns[index].count++;
				}
				++patterns_number;
			}
		}

		for(int i = 0; i < T::super_sigma; ++i) {
			if(indexed_patterns[i].count > 0) {
				qsort(indexed_patterns[i].list, indexed_patterns[i].count, sizeof(pelem), (int(*)(const void*,const void*)) pelem_compar);
			}
		}

		return patterns_number;

	}

	std::set<word> tmp;

	// verification
	template <class T> static void cpm_mag_dyn_verification( const uchar *text, word d, word mm, int pos, int *vt, void *args ) {
		int s, c;
		d = ( d & mm ) ^ mm;
		int index1;
		pelem el;
		el.offset = 0;
		pelem *el2;
		pelem* ref = NULL;

		while( d ) {
			s = d>=log2cache?ilog2(d):log2v[d];	// max(d) = 255 << ( ~ max(b) >> 1) <== is that the number that says how many values should be cached?
			c = vt[ s ] * T::q_size + pos;
			el.pattern = text + c;
			index1 = T::get_q_gram(el.pattern);
			
			//if(c >= 147091360 - 16 && c <= 147091360 + 16) {
			//	printf("=========================\n");
			//	printf("FOUND(c=%d, index=%d, BYTES_TO_COMPARE=%d): %.*s\n", c, index1, plist::BYTES_TO_COMPARE, plist::BYTES_TO_COMPARE, text + c);
			//	printf("=========================\n");
			//}
			

			el2 = (pelem*)bsearch(&el, indexed_patterns[index1].list, indexed_patterns[index1].count, sizeof(pelem), (int (*)(const void *, const void *))pelem_compar);
			if(el2) {
				//printf(">>>> IN <<<<\n");
				ref = el2;
				while( el2 - indexed_patterns[index1].list >= 0) {
						long m = ((cpm_mag_dyn_verification_args*)(args))->pattern_size;
						long part_size = ((cpm_mag_dyn_verification_args*)(args))->part_size;
						long n = ((cpm_mag_dyn_verification_args*)(args))->text_size;
						int l_number = 0;
						int r_number = 0;
						if(el2->offset>=part_size) { // SECOND HALF
							l_number = chcpml(el2->op, m, text, n, c - 1                          , el2->offset - 1                          , 0);
							r_number = chcpmr(el2->op, m, text, n, c + plist::BYTES_TO_COMPARE, el2->offset + plist::BYTES_TO_COMPARE, 0);
						} else { // FIRST HALF
							l_number = chcpml(el2->op, m, text, n, c - 1                          , el2->offset - 1                          , 0);
							r_number = chcpmr(el2->op, m, text, n, c + plist::BYTES_TO_COMPARE, el2->offset + plist::BYTES_TO_COMPARE, 0);
						}
						//printf("%.*s\n-----------------\n\n", m, text + c);
						if (l_number + r_number + (int)plist::BYTES_TO_COMPARE >= m) {
							for (int a = 0; a <= l_number + r_number + (int)plist::BYTES_TO_COMPARE - m;  a++) {
								long my_pos = c - l_number + a;
								indexes.insert(my_pos);
							}
						}
						
					
					el2--;
				}
				el2 = ++ref;
				while( el2 - indexed_patterns[index1].list < indexed_patterns[index1].count) {
						long m = ((cpm_mag_dyn_verification_args*)(args))->pattern_size;
						long part_size = ((cpm_mag_dyn_verification_args*)(args))->part_size;
						long n = ((cpm_mag_dyn_verification_args*)(args))->text_size;
						int l_number = 0;
						int r_number = 0;
						if(el2->offset>=part_size) { // SECOND HALF
							l_number = chcpml(el2->op, m, text, n, c - 1                          , el2->offset - 1                          , 0);
							r_number = chcpmr(el2->op, m, text, n, c + plist::BYTES_TO_COMPARE, el2->offset + plist::BYTES_TO_COMPARE, 0);
						} else { // FIRST HALF
							l_number = chcpml(el2->op, m, text, n, c - 1                          , el2->offset - 1                          , 0);
							r_number = chcpmr(el2->op, m, text, n, c + plist::BYTES_TO_COMPARE, el2->offset + plist::BYTES_TO_COMPARE, 0);
						}
						//printf("%.*s\n-----------------\n\n", m, text + c);
						if (l_number + r_number + (int)plist::BYTES_TO_COMPARE >= m) {
							for (int a = 0; a <= l_number + r_number + (int)plist::BYTES_TO_COMPARE - m;  a++) {
								long my_pos = c - l_number + a;
								indexes.insert(my_pos);
							}
						}

					el2++;
				}
			}

			d &= ~(( word )1 << s );
		}
	}


}

#endif
