NULL =

EXTRA_DIST =
CLEANFILES =

man_MANS = gnome-dictionary.1
EXTRA_DIST += $(man_MANS)

gdictappdir = $(datadir)/applications
gdictapp_in_files = gnome-dictionary.desktop.in
gdictapp_DATA = $(gdictapp_in_files:.desktop.in=.desktop)
EXTRA_DIST += gnome-dictionary.desktop.in.in

$(gdictapp_in_files): $(gdictapp_in_files:.desktop.in=.desktop.in.in)
	$(AM_V_GEN)sed -e "s|\@VERSION\@|@VERSION@|" $< > $@

@INTLTOOL_DESKTOP_RULE@

# dictionary sources definitions
dictsourcedir = $(datadir)/gdict-1.0/sources
dictsource_in_files = \
	default.desktop.in \
	spanish.desktop.in \
	thai.desktop.in \
	$(NULL)
dictsource_DATA = $(dictsource_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST += $(dictsource_in_files)

builderdir = $(datadir)/gnome-dictionary
builder_DATA = \
	gnome-dictionary-preferences.ui	\
	gnome-dictionary-source.ui	\
	$(NULL)

uidir = $(datadir)/gnome-dictionary
ui_DATA = gnome-dictionary-ui.xml

# applet menu definition
appletuidir = $(datadir)/gnome-2.0/ui
appletui_DATA = GNOME_DictionaryApplet.xml

if BUILD_GDICT_APPLET
serverdir       = $(libdir)/bonobo/servers
server_in_files = GNOME_DictionaryApplet.server.in
server_DATA     = $(server_in_files:.server.in=.server)

$(server_in_files): $(server_in_files:.server.in=.server.in.in)
	$(AM_V_GEN)sed 					\
		-e "s|\@VERSION\@|@VERSION@|" 		\
		-e "s|\@LIBEXECDIR\@|$(libexecdir)|" 	\
	$< > $@

@INTLTOOL_SERVER_RULE@
endif # BUILD_GDICT_APPLET

EXTRA_DIST += GNOME_DictionaryApplet.server.in.in

gsettings_SCHEMAS = org.gnome.dictionary.gschema.xml
CLEANFILES += $(gsettings_SCHEMAS)
@INTLTOOL_XML_NOMERGE_RULE@
@GSETTINGS_RULES@
