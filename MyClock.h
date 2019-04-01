/*
 * MyClock.h
 *
 *  Created on: 2019年3月30日
 *      Author: tom
 */

#ifndef MYCLOCK_H_
#define MYCLOCK_H_
#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <librsvg/rsvg.h>

G_BEGIN_DECLS
#define MY_TYPE_CLOCK my_clock_get_type()
G_DECLARE_DERIVABLE_TYPE(MyClock,my_clock,MY,CLOCK,GtkDrawingArea);

typedef struct _MyClockClass{
	GtkDrawingAreaClass parent_class;
};

MyClock * my_clock_new(guint hour,guint minute,guint second,gboolean run);
MyClock * my_clock_new_now_local(gboolean run);
void my_clock_set_interval(MyClock *clock,gint interval_ms);
gint my_clock_get_interval(MyClock *clock);

G_END_DECLS


#endif /* MYCLOCK_H_ */
