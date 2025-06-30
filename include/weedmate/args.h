// args.h
// project: weedmate

#ifndef ARGS_H_
#define ARGS_H_

// === Macros ===
#define ARGS_HANDLED		1
#define ARGS_NOT_HANDLED	0
#define ARGS_INVALID	   -1

// === Structs/Enums ===
typedef enum cmdtype_e {
	CMD_DELETE,
	CMD_CREATE,
	CMD_HELP,
	CMD_VERSION,
	CMD_NONE
} cmdtype_e;

typedef struct argmap_t {
	const char *shortFlag;
	const char *longFlag;
	const char *description;
	cmdtype_e cmdtype;
} argmap_t;

// === Functions ===
int handleArgs					( int argc, char *argv[] );

#endif // ARGS_H_
