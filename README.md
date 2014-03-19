# Hoextdown

`Hoextdown` is an extension to [Hoedown](https://github.com/hoedown/hoedown).

Extended the following functions.

* [Special Attributes](#special-attributes)
* [Task Lists](#task-lists)
* [Name-based TOC ID](#name-based-toc-id)
* [Line Continue](#line-continue)

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

## Name-based TOC ID

Changed to be the name based on the ID of the table of contents.

```
# Header 1
```

prefix will be 'toc\_', like this

```
[toc to header](#toc_header-1)
```

## Line Continue

Add the `HOEDOWN_HTML_LINE_CONTINUE` to Hoedown html flags.

Remove the line breaks at the end of the line.
