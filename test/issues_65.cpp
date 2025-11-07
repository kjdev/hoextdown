/*
 * https://github.com/kjdev/hoextdown/issues/65
 */
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
extern "C" {
#include "buffer.h"
#include "document.h"
#include "context_test.h"
}

int main()
{
  hoedown_extensions ext = (hoedown_extensions)(HOEDOWN_EXT_FOOTNOTES);

  hoedown_buffer *meta = hoedown_buffer_new(64);
  hoedown_buffer *ob = hoedown_buffer_new(64);

  // First create renderer with NULL doc, then create document, then update renderer's doc pointer
  hoedown_renderer *renderer = hoedown_context_test_renderer_new(nullptr);
  hoedown_document *doc = hoedown_document_new(
    renderer, ext, /*max_nesting=*/16, /*attr_activation=*/0, /*user_block=*/nullptr, meta);

  // Update the renderer's state with the actual document pointer
  ((hoedown_context_test_renderer_state*)renderer->opaque)->doc = doc;

  unsigned char buf[] = "a[^b]\n";
  size_t size = sizeof(buf) - 1;
  hoedown_document_render_inline(doc, ob, (const uint8_t*)buf, size);

  // Properly free all allocated resources
  hoedown_document_free(doc);
  hoedown_context_test_renderer_free(renderer);
  hoedown_buffer_free(ob);
  hoedown_buffer_free(meta);

  return 0;
}
