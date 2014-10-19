#include "document.h"
#include "html.h"

#include "common.h"
/*#include <time.h>*/


/* FEATURES INFO / DEFAULTS */

enum renderer_type {
	RENDERER_HTML,
	RENDERER_HTML_TOC
};

struct extension_category_info {
	unsigned int flags;
	const char *option_name;
	const char *label;
};

struct extension_info {
	unsigned int flag;
	const char *option_name;
	const char *description;
};

struct html_flag_info {
	unsigned int flag;
	const char *option_name;
	const char *description;
};

static struct extension_category_info categories_info[] = {
	{HOEDOWN_EXT_BLOCK, "block", "Block extensions"},
	{HOEDOWN_EXT_SPAN, "span", "Span extensions"},
	{HOEDOWN_EXT_FLAGS, "flags", "Other flags"},
	{HOEDOWN_EXT_NEGATIVE, "negative", "Negative flags"},
};

static struct extension_info extensions_info[] = {
	{HOEDOWN_EXT_TABLES, "tables", "Parse PHP-Markdown style tables."},
	{HOEDOWN_EXT_FENCED_CODE, "fenced-code", "Parse fenced code blocks."},
	{HOEDOWN_EXT_FOOTNOTES, "footnotes", "Parse footnotes."},

	{HOEDOWN_EXT_AUTOLINK, "autolink", "Automatically turn safe URLs into links."},
	{HOEDOWN_EXT_STRIKETHROUGH, "strikethrough", "Parse ~~stikethrough~~ spans."},
	{HOEDOWN_EXT_UNDERLINE, "underline", "Parse _underline_ instead of emphasis."},
	{HOEDOWN_EXT_HIGHLIGHT, "highlight", "Parse ==highlight== spans."},
	{HOEDOWN_EXT_QUOTE, "quote", "Render \"quotes\" as <q>quotes</q>."},
	{HOEDOWN_EXT_SUPERSCRIPT, "superscript", "Parse super^script."},
	{HOEDOWN_EXT_MATH, "math", "Parse TeX $$math$$ syntax, Kramdown style."},

	{HOEDOWN_EXT_NO_INTRA_EMPHASIS, "disable-intra-emphasis", "Disable emphasis_between_words."},
	{HOEDOWN_EXT_SPACE_HEADERS, "space-headers", "Require a space after '#' in headers."},
	{HOEDOWN_EXT_MATH_EXPLICIT, "math-explicit", "Instead of guessing by context, parse $inline math$ and $$always block math$$ (requires --math)."},

	{HOEDOWN_EXT_DISABLE_INDENTED_CODE, "disable-indented-code", "Don't parse indented code blocks."},
};

static struct html_flag_info html_flags_info[] = {
	{HOEDOWN_HTML_SKIP_HTML, "skip-html", "Strip all HTML tags."},
	{HOEDOWN_HTML_ESCAPE, "escape", "Escape all HTML."},
	{HOEDOWN_HTML_HARD_WRAP, "hard-wrap", "Render each linebreak as <br>."},
	{HOEDOWN_HTML_USE_XHTML, "xhtml", "Render XHTML."},
};

static const char *category_prefix = "all-";
static const char *negative_prefix = "no-";

#define DEF_IUNIT 1024
#define DEF_OUNIT 64
#define DEF_MAX_NESTING 16


/* PRINT HELP */

void
print_help(const char *basename)
{
	size_t i;
	size_t e;

	/* usage */
	printf("Usage: %s [OPTION]... [FILE]\n\n", basename);

	/* description */
	printf("Process the Markdown in FILE (or standard input) and render it to standard output, using the Hoedown library. "
	       "Parsing and rendering can be customized through the options below. The default is to parse pure markdown and output HTML.\n\n");

	/* main options */
	printf("Main options:\n");
	print_option('n', "max-nesting=N", "Maximum level of block nesting parsed. Default is " str(DEF_MAX_NESTING) ".");
	print_option('t', "toc-level=N", "Maximum level for headers included in the TOC. Zero disables TOC (the default).");
	print_option(  0, "html", "Render (X)HTML. The default.");
	print_option(  0, "html-toc", "Render the Table of Contents in (X)HTML.");
	print_option('T', "time", "Show time spent in rendering.");
	print_option('i', "input-unit=N", "Reading block size. Default is " str(DEF_IUNIT) ".");
	print_option('o', "output-unit=N", "Writing block size. Default is " str(DEF_OUNIT) ".");
	print_option('h', "help", "Print this help text.");
	print_option('v', "version", "Print Hoedown version.");
	printf("\n");

	/* extensions */
	for (i = 0; i < count_of(categories_info); i++) {
		struct extension_category_info *category = categories_info+i;
		printf("%s (--%s%s):\n", category->label, category_prefix, category->option_name);
		for (e = 0; e < count_of(extensions_info); e++) {
			struct extension_info *extension = extensions_info+e;
			if (extension->flag & category->flags) {
				print_option(  0, extension->option_name, extension->description);
			}
		}
		printf("\n");
	}

	/* html-specific */
	printf("HTML-specific options:\n");
	for (i = 0; i < count_of(html_flags_info); i++) {
		struct html_flag_info *html_flag = html_flags_info+i;
		print_option(  0, html_flag->option_name, html_flag->description);
	}
	printf("\n");

	/* ending */
	printf("Flags and extensions can be negated by prepending 'no' to them, as in '--no-tables', '--no-span' or '--no-escape'. "
	       "Options are processed in order, so in case of contradictory options the last specified stands.\n\n");

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
	hoedown_renderer *renderer = NULL;
	void (*renderer_free)(hoedown_renderer*) = NULL;
	hoedown_document *document;

	/* Parse options */
	/* TODO */

	/* Read everything */
	ib = hoedown_buffer_new(iunit);

	while (!feof(in)) {
		if (ferror(in)) {
			fprintf(stderr, "I/O errors found while reading input.\n");
			return 5;
		}
		hoedown_buffer_grow(ib, ib->size + iunit);
		ib->size += fread(ib->data + ib->size, 1, iunit, in);
	}

	if (in != stdin) fclose(in);

	/* Create the renderer */
	switch (renderer_type) {
		case RENDERER_HTML:
			renderer = hoedown_html_renderer_new(html_flags, toc_level);
			renderer_free = hoedown_html_renderer_free;
			break;
		case RENDERER_HTML_TOC:
			renderer = hoedown_html_toc_renderer_new(toc_level);
			renderer_free = hoedown_html_renderer_free;
			break;
	};

	/* Perform Markdown rendering */
	ob = hoedown_buffer_new(ounit);
	document = hoedown_document_new(renderer, extensions, max_nesting);

	/*clock_gettime(CLOCK_MONOTONIC, &start);*/
	hoedown_document_render(document, ob, ib->data, ib->size);
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

	hoedown_document_free(document);
	renderer_free(renderer);

	if (ferror(stdout)) {
		fprintf(stderr, "I/O errors found while writing output.\n");
		return 5;
	}

	return 0;
}
