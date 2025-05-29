// version.h
// project: weedmate

#ifndef VERSION_H_
#define VERSION_H_

#define WEEDMATE_VERSION_MAJOR		0
#define WEEDMATE_VERSION_MINOR		1
#define WEEDMATE_VERSION_PATCH		5

#define _STR(x) 	#x
#define STR(x) 		_STR(x)

#define WEEDMATE_VERSION_STRING		\
	STR(WEEDMATE_VERSION_MAJOR) "." \
	STR(WEEDMATE_VERSION_MINOR) "." \
	STR(WEEDMATE_VERSION_PATCH)

#endif // VERSION_H_
