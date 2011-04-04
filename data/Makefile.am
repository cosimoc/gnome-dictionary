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

EXTRA_DIST += \
	$(builder_DATA) \
	$(ui_DATA) \
	$(NULL)

gsettings_SCHEMAS = org.gnome.dictionary.gschema.xml
CLEANFILES += $(gsettings_SCHEMAS) \
	$(dictsource_DATA) \
	$(gdictapp_in_files) \
	$(gdictapp_DATA)
	
@INTLTOOL_XML_NOMERGE_RULE@
@GSETTINGS_RULES@
