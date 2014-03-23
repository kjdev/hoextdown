#include "html.h"

#include "common.h"
//#include <time.h>

#define DEF_IUNIT 1024
#define DEF_OUNIT 64
#define DEF_MAX_NESTING 16


/* PRINT HELP */

void
print_help(const char *basename) {
	/* usage */
	printf("Usage: %s [OPTION]... [FILE]\n\n", basename);

	/* description */
	printf("Apply SmartyPants smart punctuation to the HTML in FILE (or standard input), and output the resulting HTML to standard output.\n\n");

	/* main options */
	printf("Main options:\n");
	print_option('T', "time", "Show time spent in SmartyPants processing.");
	print_option('i', "input-unit=N", "Reading block size. Default is " str(DEF_IUNIT) ".");
	print_option('o', "output-unit=N", "Writing block size. Default is " str(DEF_OUNIT) ".");
	print_option('h', "help", "Print this help text.");
	print_option('v', "version", "Print Hoedown version.");
	printf("\n");

	/* ending */
	printf("Options are processed in order, so in case of contradictory options the last specified stands.\n\n");

	printf("When FILE is '-', read standard input. If no FILE was given, read standard input. Use '--' to signal end of option parsing. "
	       "Exit status is 0 if no errors occured, 1 with option parsing errors, 4 with memory allocation errors or 5 with I/O errors.\n\n");
}


/* MAIN LOGIC */

int
main(int argc, char **argv)
{
	int show_time = 0;
	//struct timespec start, end;

	/* buffers */
	hoedown_buffer *ib, *ob;
	size_t iunit = DEF_IUNIT, ounit = DEF_OUNIT;

	/* files */
	FILE *in = NULL;


	/* option parsing */
	int just_args = 0;
	int i, j;
	for (i = 1; i < argc; i++) {
		char *arg = argv[i];
		if (!arg[0]) continue;

		if (just_args || arg[0] != '-') {
			/* regular argument */
			in = fopen(arg, "r");
			if (!in) {
				fprintf(stderr, "Unable to open input file \"%s\": %s\n", arg, strerror(errno));
				return 5;
			}
			continue;
		}

		if (!arg[1]) {
			/* arg is "-" */
			in = stdin;
			continue;
		}

		if (arg[1] != '-') {
			/* parse short options */
			char opt;
			const char *val;
			for (j = 1; (opt = arg[j]); j++) {
				if (opt == 'h') {
					print_help(argv[0]);
					return 1;
				}

				if (opt == 'v') {
					print_version();
					return 1;
				}

				if (opt == 'T') {
					show_time = 1;
					continue;
				}

				/* options requiring value */
				if (arg[++j]) val = arg+j;
				else if (argv[++i]) val = argv[i];
				else {
					fprintf(stderr, "Wrong option '-%c' found.\n", opt);
					return 1;
				}

				long int num;
				int isNum = parseint(val, &num);

				if (opt == 'i' && isNum) {
					iunit = num;
					break;
				}

				if (opt == 'o' && isNum) {
					ounit = num;
					break;
				}

				fprintf(stderr, "Wrong option '-%c' found.\n", opt);
				return 1;
			}
			continue;
		}

		if (!arg[2]) {
			/* arg is "--" */
			just_args = 1;
			continue;
		}

		/* parse long option */
		char opt [100];
		strncpy(opt, arg+2, 100);
		opt[99] = 0;

		char *val = strchr(opt, '=');

		long int num = 0;
		int isNum = 0;

		if (val) {
			*val = 0;
			val++;

			if (*val)
				isNum = parseint(val, &num);
		}

		int opt_parsed = 0;

		if (strcmp(opt, "help")==0) {
			print_help(argv[0]);
			return 1;
		}

		if (strcmp(opt, "version")==0) {
			print_version();
			return 1;
		}

		if (strcmp(opt, "input-unit")==0 && isNum) {
			opt_parsed = 1;
			iunit = num;
		}
		if (strcmp(opt, "output-unit")==0 && isNum) {
			opt_parsed = 1;
			ounit = num;
		}

		if (strcmp(opt, "time")==0) {
			opt_parsed = 1;
			show_time = 1;
		}

		if (!opt_parsed) {
			fprintf(stderr, "Wrong option '%s' found.\n", arg);
			return 1;
		}
	}

	if (!in)
		in = stdin;


	/* reading everything */
	ib = hoedown_buffer_new(iunit);
	if (!ib) {
		fprintf(stderr, "Couldn't allocate input buffer.\n");
		return 4;
	}

	while (!feof(in)) {
		if (ferror(in)) {
			fprintf(stderr, "I/O errors found while reading input.\n");
			return 5;
		}
		if (hoedown_buffer_grow(ib, ib->size + iunit) != HOEDOWN_BUF_OK) {
			fprintf(stderr, "Couldn't grow input buffer.\n");
			return 4;
		}
		ib->size += fread(ib->data + ib->size, 1, iunit, in);
	}

	if (in != stdin)
		fclose(in);


	/* performing SmartyPants processing */
	ob = hoedown_buffer_new(ounit);
	if (!ob) {
		fprintf(stderr, "Couldn't allocate output buffer.\n");
		return 4;
	}

	//clock_gettime(CLOCK_MONOTONIC, &start);
	hoedown_html_smartypants(ob, ib->data, ib->size);
	//clock_gettime(CLOCK_MONOTONIC, &end);


	/* writing the result to stdout */
	(void)fwrite(ob->data, 1, ob->size, stdout);


	/* showing rendering time */
	if (show_time) {
		//TODO: enable this
		//long long elapsed = (  end.tv_sec*1000000000 +   end.tv_nsec)
		//                  - (start.tv_sec*1000000000 + start.tv_nsec);
		//if (elapsed < 1000000000)
		//	fprintf(stderr, "Time spent on rendering: %.2f ms.\n", ((double)elapsed)/1000000);
		//else
		//	fprintf(stderr, "Time spent on rendering: %.3f s.\n", ((double)elapsed)/1000000000);
	}


	/* cleanup */
	hoedown_buffer_free(ib);
	hoedown_buffer_free(ob);

	if (ferror(stdout)) {
		fprintf(stderr, "I/O errors found while writing output.\n");
		return 5;
	}

	return 0;
}
