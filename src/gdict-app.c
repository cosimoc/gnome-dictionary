/* gdict-app.c - main application class
 *
 * This file is part of GNOME Dictionary
 *
 * Copyright (C) 2005 Emmanuele Bassi
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gi18n.h>

#include "gdict-common.h"
#include "gdict-pref-dialog.h"
#include "gdict-app.h"

static GdictApp *singleton = NULL;

struct _GdictAppClass
{
  GObjectClass parent_class;
};

G_DEFINE_TYPE (GdictApp, gdict_app, G_TYPE_OBJECT);

static gchar **gdict_lookup_words = NULL;
static gchar **gdict_match_words = NULL;

static gchar *gdict_source_name = NULL;
static gchar *gdict_database_name = NULL;
static gchar *gdict_strategy_name = NULL;

static GOptionEntry gdict_app_goptions[] = {
  {
    "look-up", 0,
    0,
    G_OPTION_ARG_STRING_ARRAY, &gdict_lookup_words,
    N_("Words to look up"), N_("WORD")
  },
  {
    "match", 0,
    0,
    G_OPTION_ARG_STRING_ARRAY, &gdict_match_words,
    N_("Words to match"), N_("WORD")
  },
  {
    "source", 's',
    0,
    G_OPTION_ARG_STRING, &gdict_source_name,
    N_("Dictionary source to use"), N_("NAME")
  },
  {
    "database", 'D',
    0,
    G_OPTION_ARG_STRING, &gdict_database_name,
    N_("Database to use"), N_("NAME")
  },
  {
    "strategy", 'S',
    0,
    G_OPTION_ARG_STRING, &gdict_strategy_name,
    N_("Strategy to use"), N_("NAME")
  },
  {
    G_OPTION_REMAINING, 0, 0,
    G_OPTION_ARG_STRING_ARRAY, &gdict_lookup_words,
    N_("Words to look up"), N_("WORDS")
  },
  { NULL },
};

static void
gdict_app_finalize (GObject *object)
{
  GdictApp *app = GDICT_APP (object);

  if (app->loader)
    g_object_unref (app->loader);

  G_OBJECT_CLASS (gdict_app_parent_class)->finalize (object);
}

static void
gdict_app_class_init (GdictAppClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  
  gobject_class->finalize = gdict_app_finalize;
}

static void
gdict_app_init (GdictApp *app)
{
}

static void
gdict_activate (GApplication *application,
                GdictApp     *gdict_app)
{
  GtkWidget *window;

}

static int
gdict_command_line (GApplication *application,
                    GApplicationCommandLine *cmd_line,
                    GdictApp                *gdict_app)
{
  GOptionContext *context;
  GError *error;
  GSList *l;
  gsize words_len, i;
  gint argc;
  char **argv;

  argv = g_application_command_line_get_arguments (cmd_line, &argc);

  /* create the new option context */
  context = g_option_context_new (N_(" - Look up words in dictionaries"));
  
  g_option_context_set_translation_domain (context, GETTEXT_PACKAGE);
  g_option_context_add_main_entries (context, gdict_app_goptions, GETTEXT_PACKAGE);
  g_option_context_add_group (context, gdict_get_option_group ());
  g_option_context_add_group (context, gtk_get_option_group (FALSE));

  error = NULL;
  if (!g_option_context_parse (context, &argc, &argv, &error))
    {
      g_critical ("Failed to parse argument: %s", error->message);
      g_error_free (error);
      g_option_context_free (context);
      return 1;
    }

  g_option_context_free (context);

  if (gdict_lookup_words == NULL &&
      gdict_match_words == NULL)
    {
      GtkWidget *window = gdict_window_new (GDICT_WINDOW_ACTION_CLEAR,
                                            singleton->loader,
                                            gdict_source_name,
                                            gdict_database_name,
                                            gdict_strategy_name,
                                            NULL);
      gtk_window_set_application (GTK_WINDOW (window), singleton->app);
      gtk_widget_show (window);

      return 0;
    }

  if (gdict_lookup_words != NULL)
    words_len = g_strv_length (gdict_lookup_words);
  else
    words_len = 0;

  for (i = 0; i < words_len; i++)
    {
      const gchar *word = gdict_lookup_words[i];
      GtkWidget *window;

      window = gdict_window_new (GDICT_WINDOW_ACTION_LOOKUP,
                                 singleton->loader,
                                 gdict_source_name,
                                 gdict_database_name,
                                 gdict_strategy_name,
                                 word);
      
      gtk_window_set_application (GTK_WINDOW (window), singleton->app);
      gtk_widget_show (window);
    }

  if (gdict_match_words != NULL)
    words_len = g_strv_length (gdict_match_words);
  else
    words_len = 0;

  for (i = 0; i < words_len; i++)
    {
      const gchar *word = gdict_match_words[i];
      GtkWidget *window;

      window = gdict_window_new (GDICT_WINDOW_ACTION_MATCH,
      				 singleton->loader,
				 gdict_source_name,
                                 gdict_database_name,
                                 gdict_strategy_name,
				 word);
      
      gtk_window_set_application (GTK_WINDOW (window), singleton->app);
      gtk_widget_show (window);
    }

  return 0;
}

void
gdict_main (int    *argc,
            char ***argv)
{
  gchar *loader_path;

  if (!gdict_create_data_dir ())
    exit (1);

  g_type_init ();
  gtk_init (argc, argv);

  g_set_application_name (_("Dictionary"));
  gtk_window_set_default_icon_name ("accessories-dictionary");

  /* the main application instance */
  singleton = g_object_new (gdict_app_get_type (), NULL);

  /* add user's path for fetching dictionary sources */  
  singleton->loader = gdict_source_loader_new ();
  loader_path = gdict_get_data_dir (); 
  gdict_source_loader_add_search_path (singleton->loader, loader_path);
  g_free (loader_path);

  singleton->app = gtk_application_new ("org.gnome.Dictionary", G_APPLICATION_HANDLES_COMMAND_LINE);
  g_signal_connect (singleton->app, "command-line", G_CALLBACK (gdict_command_line), singleton);

  g_application_run (G_APPLICATION (singleton->app), *argc, *argv);
}

void
gdict_cleanup (void)
{
  if (!singleton)
    {
      g_warning ("You must initialize GdictApp using gdict_init()\n");
      return;
    }

  g_object_unref (singleton->app);
  g_object_unref (singleton);
}
