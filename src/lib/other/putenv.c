/*
 * (c) copyright 1989 by the Vrije Universiteit, Amsterdam, The Netherlands.
 * See the copyright notice in the ACK home directory, in the file "Copyright".
 */
/* $Header: putenv.c,v 1.3 90/05/14 12:30:18 ceriel Exp $ */

#include	<stdlib.h>
#include	<string.h>

#define	ENTRY_INC	10
#define	rounded(x)	(((x / ENTRY_INC) + 1) * ENTRY_INC)

extern _CONST char **environ;

int
putenv(name)
_CONST char *name;
{
	register _CONST char **v = environ;
	register char *r;
	static int size = 0;
	/* When size != 0, it contains the number of entries in the
	 * table (including the final NULL pointer). This means that the
	 * last non-null entry  is environ[size - 2].
	 */

	if (!name) return 0;
	if (environ == NULL) return 1;
	if (r = strchr(name, '=')) {
		register _CONST char *p, *q;

		*r = '\0';

		if (v != NULL) {
			while ((p = *v) != NULL) {
				q = name;
				while (*q && (*q++ == *p++))
					/* EMPTY */ ;
				if (*q || (*p != '=')) {
					v++;
				} else {
					/* The name was already in the
					 * environment.
					 */
					*r = '=';
					*v = name;
					return 0;
				}
			}
		}
		*r = '=';
		v = environ;
	}

	if (!size) {
		register _CONST char **p;
		register int i = 0;

		if (v)
			do {
				i++;
			} while (*v++);
		if (!(v = malloc(rounded(i) * sizeof(char **))))
			return 1;
		size = i;
		p = environ;
		environ = v;
		while (*v++ = *p++);		/* copy the environment */
		v = environ;
	} else if (!(size % ENTRY_INC)) {
		if (!(v = realloc(environ, rounded(size) * sizeof(char **))))
			return 1;
		environ = v;
	}
	v[size - 1] = name;
	v[size] = NULL;
	size++;
	return 0;
}
