# Hoextdown

[![Build Status](https://api.travis-ci.org/kjdev/hoextdown.png?branch=master)](https://travis-ci.org/kjdev/hoextdown)

`Hoextdown` is an extension to [Hoedown](https://github.com/hoedown/hoedown).

Extended the following functions.

* [Special Attributes](#special-attributes)
* [Task Lists](#task-lists)
* [Line Continue](#line-continue)
* [Header ID](#header-id)
* [Fenced Script](#fenced-script)
* [Script Tags](#script-tags)
* [Meta Block](#meta-block)

## Special Attributes

Add the `HOEDOWN_EXT_SPECIAL_ATTRIBUTE` to Hoedown document flags.

Set the id and class attribute on certain elements using an attribute block.

For instance, put the desired id prefixed by a hash inside curly brackets after
the header at the end of the line, like this

```
Header 1            {#header1}
========

## Header 2 ##      {#header2}
```

Then you can create links to different parts of the same document like this:

```
[Link back to header 1](#header1)
```

To add a class name, which can be used as a hook for a style sheet, use a dot
like this:

```
## The Site ##    {.main}
```

The id and multiple class names can be combined by putting them all into the
same special attribute block:

```
## The Site ##    {.main .shine #the-site}
```

To add a other than id and class names, use a colon like this:

```
## The Site ##    {.main .shine #the-site :color=red}
```

At this time, special attribute blocks can be used with

* headers
* fenced code blocks
* links
* images
* tables

For image and links, put the special attribute block immediately after the
parenthesis containing the address:

```
[link](url){#id .class}
![img](url){#id .class}
```

Or if using reference-style links and images, put it at the end of the
definition line like this:

```
[link][linkref] or [linkref]
![img][linkref]

[linkref]: url "optional title" {#id .class}
```

## Task Lists

Add the `HOEDOWN_HTML_USE_TASK_LIST` to Hoedown html flags.

Add to support task lists, Task lists are lists with items marked as either [ ]
or [x] (incomplete or complete), like this

```
- [ ] a task list item
- [ ] list syntax required
- [ ] normal **formatting**, @mentions, #1234 refs
- [ ] incomplete
- [x] completed
```

## Line Continue

Add the `HOEDOWN_HTML_LINE_CONTINUE` to Hoedown html flags.

Remove the line breaks at the end of the line.

## Header ID

Add the `HOEDOWN_HTML_HEADER_ID` to Hoedown html flags.

Output header id.

```
# Header 1
```

becomes:

```
<h1 id="header-1">Header 1</h1>
```

## Fenced Script

Add the `HOEDOWN_HTML_FENCED_CODE_SCRIPT` to Hoedown html flags.
(`HOEDOWN_EXT_FENCED_CODE` also need to be specified at the same time)

Output the script tag in the fenced code style.

    ``` script@text/javascript
    alert("Example");
    ```

becomes:

```
<script type="text/javascript">
alert("Example");
</script>
```

## Script Tags

Add the `HOEDOWN_EXT_SCRIPT_TAGS` to Hoedown document flags.

Add the parsing process of script tags `<?..?>`.

```
This is <?php echo "an example" ?> test.

<?php
echo "Example";
?>
```

becomes:

```
<p>This is <?php echo "an example? ?> test.</p>

<?php
echo "Example";
?>
```

## Meta Block

Add the `HOEDOWN_EXT_META_BLOCK` to Hoedown document flags.

Add the parsing process of meta block `<!--*..*-->`.

Get a meta block by running in the following program.

```c
/*
  Allocate meta block buffer
 */
hoedown_buffer *meta;
meta = hoedown_buffer_new(64);

/*
  Set HOEDOWN_EXT_META_BLOCK to hoedown_extensions.
  Specifies the meta block buffer to fifth argument.
 */
document = hoedown_document_new(renderer, HOEDOWN_EXT_META_BLOCK, 6, NULL, meta);

/*
  Print meta block buffer
 */
if (meta->size > 0) {
  fprintf(stdout, "-- Meta Block --\n");
  (void)fwrite(meta->data, 1, meta->size, stdout);
}

hoedown_buffer_free(meta);
```

Execution parse result.

```
<!--*
  author: user
  title: Readme markdown parser
*-->

This is hoextdown example.
```

becomes:

```
<p>This is hoextdown example.</p>
--- Meta Block --
  author: user
  title: Readme markdown parser
```
