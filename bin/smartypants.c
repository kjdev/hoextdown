#include "html.h"

#include "common.h"
/*#include <time.h>*/


/* FEATURES INFO / DEFAULTS */

#define DEF_IUNIT 1024
#define DEF_OUNIT 64


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
	       "Exit status is 0 if no errors occurred, 1 with option parsing errors, 4 with memory allocation errors or 5 with I/O errors.\n\n");
}


/* MAIN LOGIC */

int
main(int argc, char **argv)
{
	/*struct timespec start, end;*/
	hoedown_buffer *ib, *ob;
	FILE *in = NULL;
	ib = hoedown_buffer_new(iunit);

	/* Parse options */
	/*TODO*/

	/* Read everything */

	while (!feof(in)) {
		if (ferror(in)) {
			fprintf(stderr, "I/O errors found while reading input.\n");
			return 5;
		}
		hoedown_buffer_grow(ib, ib->size + iunit);
		ib->size += fread(ib->data + ib->size, 1, iunit, in);
	}

	if (in != stdin) fclose(in);

	/* Perform SmartyPants processing */
	ob = hoedown_buffer_new(ounit);

	/*clock_gettime(CLOCK_MONOTONIC, &start);*/
	hoedown_html_smartypants(ob, ib->data, ib->size);
	/*clock_gettime(CLOCK_MONOTONIC, &end);*/

	/* Write the result to stdout */
	(void)fwrite(ob->data, 1, ob->size, stdout);

	/* Show rendering time */
	if (show_time) {
		/*TODO: enable this
		long long elapsed = (end.tv_sec - start.tv_sec)*1e9 + (end.tv_nsec - start.tv_nsec);
		if (elapsed < 1e9)
			fprintf(stderr, "Time spent on rendering: %.2f ms.\n", ((double)elapsed)/1e6);
		else
			fprintf(stderr, "Time spent on rendering: %.3f s.\n", ((double)elapsed)/1e9);
		*/
	}

	/* Cleanup */
	hoedown_buffer_free(ib);
	hoedown_buffer_free(ob);

	if (ferror(stdout)) {
		fprintf(stderr, "I/O errors found while writing output.\n");
		return 5;
	}

	return 0;
}
