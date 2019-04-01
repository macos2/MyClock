/*
 * main.c
 *
 *  Created on: 2019年3月30日
 *      Author: tom
 */
#include <gtk/gtk.h>
#include "MyClock.h"

int main(int argc,char *argv[]){
	gtk_init(&argc,&argv);
	gint i,j;
	GtkWidget *win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	MyClock *clock=my_clock_new_now_local(FALSE);
	g_object_set(clock,"interval",1000,"run",TRUE,NULL);
	gtk_container_add(win,clock);
	g_signal_connect(win,"delete-event",gtk_main_quit,NULL);
	gtk_widget_show_all(win);
	gtk_main();
	return 0;
}
