#include "version.h"

void
hoedown_version(int *ver_major, int *ver_minor, int *ver_revision, int *ver_extras)
{
	*ver_major = HOEDOWN_VERSION_MAJOR;
	*ver_minor = HOEDOWN_VERSION_MINOR;
	*ver_revision = HOEDOWN_VERSION_REVISION;
	*ver_extras = HOEDOWN_VERSION_EXTRAS;
}
