# MyClock
A Clock Gtkwidget can use for other Gtk programe ,here is a demo using the widget and a screenshoot

#include <gtk/gtk.h>

//include the head file
#include "MyClock.h"

int main(int argc,char *argv[]){
	gtk_init(&argc,&argv);
	gint i,j;
	GtkWidget *win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  
  //Create the Widget like other Gtkwidget,and set the local time
	MyClock *clock=my_clock_new_now_local(FALSE);
  
  //Set the Widget properties,"hour"、"minute" and "second" also vaild properties.
	g_object_set(clock,"interval",1000,"run",TRUE,NULL);

  //Add it to Other GtkContainer.
	gtk_container_add(win,clock);
	g_signal_connect(win,"delete-event",gtk_main_quit,NULL);
	gtk_widget_show_all(win);
	gtk_main();
	return 0;
}


[]!(https://github.com/macos2/MyClock/blob/master/screen_shoot.png)

Depend
------
`Gtk+-3.0` `librsvg-2.0` and the relate dependencies.
