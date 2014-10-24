/* version.h - holds Hoedown's version */

#ifndef HOEDOWN_VERSION_H
#define HOEDOWN_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif


/*************
 * CONSTANTS *
 *************/

#define HOEDOWN_VERSION "2.0.0.5"
#define HOEDOWN_VERSION_MAJOR 2
#define HOEDOWN_VERSION_MINOR 0
#define HOEDOWN_VERSION_REVISION 0
#define HOEDOWN_VERSION_EXTRAS 5


/*************
 * FUNCTIONS *
 *************/

/* hoedown_version: retrieve Hoedown's version numbers */
void hoedown_version(int *major, int *minor, int *revision, int *extras);


#ifdef __cplusplus
}
#endif

#endif /** HOEDOWN_VERSION_H **/
