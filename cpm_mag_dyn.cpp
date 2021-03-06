#ifndef Q_GRAM_UTIL_V
#define Q_GRAM_UTIL_V q_gram_utilities_dna_l_bits
#endif

#ifndef Q_GRAM_UTIL_NEW_L
#define Q_GRAM_UTIL_NEW_L 3
#endif

#include "cpm_mag_dyn.h"
#include "cpm_mag_dyn_wrappers.h"

/* MAIN */
int main(int argc, char *argv[]) {
	if ( argc < 4 ) return EXIT_FAILURE;
	t_timemes t_data_acc = {0}, t_pre = {0}, t_run = {0};
	t_init(t_data_acc); t_init(t_pre); t_init(t_run);
	t_start(t_pre);

	const char *patterns_filename = argv[1], *pattern_size_pch = argv[2], *text_filename = argv[3];
	const unsigned int U = argc>4 ? atoi(argv[4]) : 4;
	const unsigned int k = argc>5 ? atoi(argv[5]) : 2;	/* q */
	const int q_size = argc>6 ? atoi(argv[6]) : 2;
	long sigma = argc>7 ? atoi(argv[7]) : 5;

	long pattern_size = atoi(pattern_size_pch);
	uchar *text = NULL, *patterns_txt = NULL;
	FILE * file = NULL;

	if(get_super_sigma(q_size) > (size_t)1000000000) {
		report_critical_error("Too big super_sigma (%lld), RAM issue.\n", get_super_sigma(q_size));
	}

	long part_size  = pattern_size/2;
	int nparts = 2;
	if(part_size - q_size < 2) report_critical_error("part_size (%d) - q_size (%d) < 2", part_size, q_size);
	cpm_mag_dyn::plist::BYTES_TO_COMPARE = part_size - q_size;
	
	for(unsigned int l = 0; l < log2cache; ++l) log2v[l] = ilog2((ldouble)l);
	
	t_stop(t_pre);

	/* read text from disk */
	t_start(t_data_acc);
	long text_size = read_file_content<uchar>(&text, text_filename);
	if (text == NULL) { report_critical_error("text==NULL\n"); }
	long pattern_txt_size = read_file_content<uchar>(&patterns_txt, patterns_filename);
	t_stop(t_data_acc);
	
	t_start(t_pre);

	long p_number = build_indexed_patterns_wrapper(patterns_txt, pattern_txt_size, nparts, part_size, pattern_size, q_size);

	t_stop(t_pre);
	cpm_mag_dyn::cpm_mag_dyn_verification_args *args = new cpm_mag_dyn::cpm_mag_dyn_verification_args();
	args->pattern_size = pattern_size;
	args->part_size = part_size;
	args->text_size = text_size;
	
	t_start(t_run);

	search_wrapper(U, k, q_size, (const uchar*)text, text_size, part_size, p_number, (void*)(args) );

	t_stop(t_run);
	long int matches = indexes.size();
	
	for ( std::set<word>::iterator it = cpm_mag_dyn::tmp.begin(); it != cpm_mag_dyn::tmp.end(); it++) printf("%lu\n", *it);

	cpm_mag_dyn::print_result(t_get_seconds(t_data_acc), t_get_seconds(t_pre), t_get_seconds(t_run), matches, sigma, get_super_sigma(q_size), text_size, part_size);


	free(patterns_txt);
	free(text);
	return EXIT_SUCCESS;
}
