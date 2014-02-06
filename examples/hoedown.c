#include "markdown.h"
#include "html.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ_UNIT 1024
#define OUTPUT_UNIT 64

int
main(int argc, char **argv)
{
	hoedown_buffer *ib, *ob;
	FILE *in = stdin;

	hoedown_renderer *renderer;
	hoedown_markdown *markdown;

	/* opening the file if given from the command line */
	if (argc > 1) {
		in = fopen(argv[1], "r");
		if (!in) {
			fprintf(stderr, "Unable to open input file \"%s\": %s\n", argv[1], strerror(errno));
			return 1;
		}
	}

	/* reading everything */
	ib = hoedown_buffer_new(READ_UNIT);
	if (!ib) {
		fprintf(stderr, "Couldn't allocate input buffer.\n");
		return 1;
	}

	while (!feof(in)) {
		if (ferror(in)) {
			fprintf(stderr, "I/O errors found while reading input.\n");
			return 1;
		}
		if (hoedown_buffer_grow(ib, ib->size + READ_UNIT) != HOEDOWN_BUF_OK) {
			fprintf(stderr, "Couldn't grow input buffer.\n");
			return 1;
		}
		ib->size += fread(ib->data + ib->size, 1, READ_UNIT, in);
	}

	if (in != stdin)
		fclose(in);

	/* performing markdown parsing */
	ob = hoedown_buffer_new(OUTPUT_UNIT);
	if (!ob) {
		fprintf(stderr, "Couldn't allocate output buffer.\n");
		return 1;
	}

	renderer = hoedown_html_renderer_new(0, 0);
	markdown = hoedown_markdown_new(0, 16, renderer);

	hoedown_markdown_render(ob, ib->data, ib->size, markdown);

	hoedown_markdown_free(markdown);
	hoedown_html_renderer_free(renderer);

	/* writing the result to stdout */
	(void)fwrite(ob->data, 1, ob->size, stdout);

	/* cleanup */
	hoedown_buffer_free(ib);
	hoedown_buffer_free(ob);

	if (ferror(stdout)) {
		fprintf(stderr, "I/O errors found while writing output.\n");
		return 1;
	}

	return 0;
}
