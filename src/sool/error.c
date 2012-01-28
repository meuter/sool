#include <sool/error.h>
#include <sool/io.h>
#include <sool/string.h>

#include <sool/mem.h>
#include <execinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void print_backtrace(FILE *stream) {
	void *trace[100];
	size_t size = backtrace(trace, sizeof(trace)/sizeof(void *)), i;

	for (i = 2; i < size; ++i) {
		char command[256], file[256], function[256];
		snprintf(command, 256, "addr2line -C -f -e /proc/%d/exe %p", getpid(), trace[i]);
		FILE *output = popen(command, "r");
		fgets(function, sizeof(function), output);
		fgets(file, sizeof(file), output);
		file[strlen(file)-1] = '\0';
		function[strlen(function)-1] = '\0';
		pclose(output);
		fprintf(stream, "%s:%s\n", file, function);
		if (strcmp(function, "main") == 0)
			break;
	}

}

void assertf(int expression, char *error_message, ...) {
	if (!expression) {
		va_list args;
		va_start(args, error_message);
		fprintf(stderr, "ERROR : ");
		ovfprintf(stderr, error_message, args);
		fprintf(stderr, "\n");
		print_backtrace(stderr);
		va_end(args);
		abort();
	}
}

