#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <libintl.h>

#include "gdict-app.h"

int main (int argc, char *argv[])
{
        bindtextdomain (GETTEXT_PACKAGE, GNOMELOCALEDIR);
        bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
        textdomain (GETTEXT_PACKAGE);

	gdict_main (&argc, &argv);
	gdict_cleanup ();
	
	return EXIT_SUCCESS;
}
