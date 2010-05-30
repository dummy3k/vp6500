#include <stdarg.h>
#include "log.hpp"

FILE* pLogFile = NULL;

void logError(char *fmt, ...) {
	if(!pLogFile) {
        pLogFile = fopen(LOG_FILE, "w");
	}

	va_list ap ;
	va_start(ap, fmt) ;

	fprintf(pLogFile, "[ERROR] ");
	vfprintf(pLogFile, fmt, ap) ;
	fprintf(pLogFile, "\n");

    printf("[ERROR] ");
	vprintf(fmt, ap);
	printf("\n");

	fflush(pLogFile);
	va_end(ap);
}

void logWarn(char *fmt, ...) {
	if(!pLogFile) {
        pLogFile = fopen(LOG_FILE, "w");
	}

	va_list ap ;
	va_start(ap, fmt) ;

	fprintf(pLogFile, "[WARN]  ");
	vfprintf(pLogFile, fmt, ap) ;
	fprintf(pLogFile, "\n");

    printf("[WARN]  ");
	vprintf(fmt, ap);
	printf("\n");

	fflush(pLogFile);
	va_end(ap);
}

void logInfo(char *fmt, ...) {
	if(!pLogFile) {
        pLogFile = fopen(LOG_FILE, "w");
	}

	va_list ap ;
	va_start(ap, fmt) ;

	fprintf(pLogFile, "[INFO]  ");
	vfprintf(pLogFile, fmt, ap) ;
	fprintf(pLogFile, "\n");

    printf("[INFO]  ");
	vprintf(fmt, ap);
	printf("\n");

	fflush(pLogFile);
	va_end(ap);
}

void logDebug(char *fmt, ...) {
	if(!pLogFile) {
        pLogFile = fopen(LOG_FILE, "w");
	}

	va_list ap ;
	va_start(ap, fmt) ;

	fprintf(pLogFile, "[DEBUG] ");
	vfprintf(pLogFile, fmt, ap) ;
	fprintf(pLogFile, "\n");

	fflush(pLogFile);
	va_end(ap);
}
