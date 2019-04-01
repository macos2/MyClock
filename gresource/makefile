all:gresource.c gresource.h 

clean:
	rm gresource.c gresource.h

gresource.c: gresource.xml clock.svg digtal.svg face.svg hour_mark.svg hour.svg minute_mark.svg minute.svg
		glib-compile-resources --generate-source gresource.xml
gresource.h: gresource.xml clock.svg digtal.svg face.svg hour_mark.svg hour.svg minute_mark.svg minute.svg
		glib-compile-resources --generate-header gresource.xml

