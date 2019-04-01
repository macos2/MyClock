/*
 * MyClock.c
 *
 *  Created on: 2019年3月30日
 *      Author: tom
 */

#include "MyClock.h"
#include "gresource.h"
typedef struct {
	guint source_id;
	gint interval;
	guint8 hour, minute, second;
	RsvgHandle *face_svg,*minute_svg,*hour_svg,*second_svg,*minute_mark_svg,*hour_mark_svg,*digtal_svg;
} MyClockPrivate;

enum {
	prop_hour = 1, prop_minute, prop_second, prop_run, prop_interval, //ms
	n_prop,
} MyClockProp;

static GParamSpec *param[n_prop] = { NULL, };

G_DEFINE_TYPE_WITH_CODE(MyClock, my_clock, GTK_TYPE_DRAWING_AREA,
		G_ADD_PRIVATE(MyClock));

gboolean timeout_func(MyClock *clock) {
	MyClockPrivate *priv = my_clock_get_instance_private(clock);
	if (priv->source_id == 0||priv->interval==0)
		return G_SOURCE_REMOVE;
	gint i=1;
	GDateTime *time = g_date_time_new_from_unix_utc(0), *time2;
	if(priv->interval<0)i=-1;
	time2 = g_date_time_add_seconds(time,
			priv->hour * 3600 + priv->minute * 60 + priv->second
					+ i);
	priv->hour = g_date_time_get_hour(time2);
	priv->minute = g_date_time_get_minute(time2);
	priv->second = g_date_time_get_second(time2);
	g_date_time_unref(time);
	g_date_time_unref(time2);
	gtk_widget_queue_draw(clock);
	return G_SOURCE_CONTINUE;

}
;

void my_clock_finalize(MyClock *self) {
	MyClockPrivate *priv = my_clock_get_instance_private(self);
	if (priv->source_id != 0) {
		g_source_remove(priv->source_id);
		priv->source_id = 0;
	}
	rsvg_handle_free(priv->hour_svg);
	rsvg_handle_free(priv->minute_svg);
	rsvg_handle_free(priv->second_svg);
	rsvg_handle_free(priv->face_svg);
	rsvg_handle_free(priv->hour_mark_svg);
	rsvg_handle_free(priv->minute_mark_svg);
	rsvg_handle_free(priv->digtal_svg);
}
;

void my_clock_set_property(MyClock *self, guint property_id,
		const GValue *value, GParamSpec *pspec) {

	MyClockPrivate *priv = my_clock_get_instance_private(self);
	gint i;
	GDateTime *temp;
	switch (property_id) {
	case prop_hour:
		priv->hour = g_value_get_uchar(value);
		break;
	case prop_minute:
		priv->minute = g_value_get_uchar(value);
		break;
	case prop_second:
		priv->second = g_value_get_uchar(value);
		break;
	case prop_run:
		if (priv->source_id == 0 && g_value_get_boolean(value)) {
			priv->source_id = g_timeout_add(abs(priv->interval), timeout_func,
					self);
		} else if (priv->source_id != 0 && !g_value_get_boolean(value)) {
			g_source_remove(priv->source_id);
			priv->source_id = 0;
		}
		break;
	case prop_interval:
		priv->interval = g_value_get_int(value);
		if (priv->interval == 0) {
			g_source_remove(priv->source_id);
			priv->source_id = 0;
		} else if (priv->source_id > 0) {
			g_source_remove(priv->source_id);
			priv->source_id = g_timeout_add(abs(priv->interval), timeout_func,
					self);
		}
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(self, property_id, pspec);
	}
	gtk_widget_queue_draw(self);
}
;
void my_clock_get_property(MyClock *self, guint property_id, GValue *value,
		GParamSpec *pspec) {
	gboolean run = FALSE;
	MyClockPrivate *priv = my_clock_get_instance_private(self);
	switch (property_id) {
	case prop_hour:
		g_value_set_uchar(value, priv->hour);
		break;
	case prop_minute:
		g_value_set_uchar(value, priv->minute);
		break;
	case prop_second:
		g_value_set_uchar(value, priv->second);
		break;
	case prop_run:
		if (priv->source_id != 0)
			run = TRUE;
		g_value_set_boolean(value, run);
		break;
	case prop_interval:
		g_value_set_int(value, priv->interval);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(self, property_id, pspec);
	}
}
;
gboolean my_clock_draw(MyClock *self, cairo_t *cr) {
	GtkAllocation alloc;
	RsvgDimensionData svg_size;
	MyClockPrivate *priv = my_clock_get_instance_private(self);
	gtk_widget_get_allocation(self,&alloc);
	rsvg_handle_get_dimensions(priv->face_svg,&svg_size);
	gdouble i=alloc.height,r;
	gint j;
	if(alloc.width<alloc.height)i=alloc.width;
	i=i/svg_size.height;
	cairo_save(cr);
	cairo_translate(cr,alloc.width/2.,alloc.height/2.);
	cairo_scale(cr,i,i);
	rsvg_handle_render_cairo(priv->face_svg,cr);
	rsvg_handle_render_cairo(priv->digtal_svg,cr);

	for (j=0;j<60;j++){
	cairo_save(cr);
	r=j/60.*2.*G_PI;
	cairo_rotate(cr,r);
	rsvg_handle_render_cairo(priv->minute_mark_svg,cr);
	cairo_restore(cr);
	}

	for (j=0;j<12;j++){
	cairo_save(cr);
	r=j/12.*2.*G_PI;
	cairo_rotate(cr,r);
	rsvg_handle_render_cairo(priv->hour_mark_svg,cr);
	cairo_restore(cr);
	}

	cairo_save(cr);
	cairo_rotate(cr,priv->second/60.*2.*G_PI-G_PI/2.);
	rsvg_handle_render_cairo(priv->second_svg,cr);
	cairo_restore(cr);

	cairo_save(cr);
	cairo_rotate(cr,(priv->minute/60.+priv->second/3600.)*2*G_PI-G_PI/2.);
	rsvg_handle_render_cairo(priv->minute_svg,cr);
	cairo_restore(cr);

	cairo_save(cr);
	cairo_rotate(cr,(priv->hour%12)/12.*(2*G_PI)+priv->minute/60.*(2.*G_PI/12.)+priv->second/60.*(2*G_PI/12./60.)-G_PI/2.);
	rsvg_handle_render_cairo(priv->hour_svg,cr);
	cairo_restore(cr);
	cairo_restore(cr);
	return TRUE;
}
;

static void my_clock_class_init(MyClockClass *klass) {
	GObjectClass *obj_class = klass;
	GtkWidgetClass *gtk_class = klass;
	obj_class->finalize = my_clock_finalize;
	obj_class->get_property = my_clock_get_property;
	obj_class->set_property = my_clock_set_property;
	param[prop_hour] = g_param_spec_uchar("hour", "hour", "hour", 0, 23, 0,
			G_PARAM_READWRITE);
	param[prop_minute] = g_param_spec_uchar("minute", "minute", "minute", 0, 59,
			0, G_PARAM_READWRITE);
	param[prop_second] = g_param_spec_uchar("second", "second", "second", 0, 59,
			0, G_PARAM_READWRITE);
	param[prop_run] = g_param_spec_boolean("run", "run", "run", FALSE,
			G_PARAM_READWRITE);
	param[prop_interval] = g_param_spec_int("interval", "interval", "interval",
	G_MININT, G_MAXINT, 1000, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	g_object_class_install_properties(obj_class, n_prop, param);
	gtk_class->draw = my_clock_draw;
}

RsvgHandle *rsvg_handle_new_from_gresource(const gchar *path){
	RsvgHandle *svg;
	gchar *uri=g_strdup_printf ("resource://%s",path);
	GFile *f=g_file_new_for_uri(uri);
	g_free(uri);
	if(f==NULL)return NULL;
	svg=rsvg_handle_new_from_gfile_sync (f,RSVG_HANDLE_FLAG_UNLIMITED,NULL,NULL);
	g_object_unref(f);
	return svg;
};


static void my_clock_init(MyClock *self) {
	MyClockPrivate *priv = my_clock_get_instance_private(self);
	priv->source_id = 0;
	priv->face_svg=rsvg_handle_new_from_gresource("/org/gtk/face.svg");
	priv->hour_svg=rsvg_handle_new_from_gresource("/org/gtk/hour.svg");
	priv->minute_svg=rsvg_handle_new_from_gresource("/org/gtk/minute.svg");
	priv->second_svg=rsvg_handle_new_from_gresource("/org/gtk/second.svg");
	priv->hour_mark_svg=rsvg_handle_new_from_gresource("/org/gtk/hour_mark.svg");
	priv->minute_mark_svg=rsvg_handle_new_from_gresource("/org/gtk/minute_mark.svg");
	priv->digtal_svg=rsvg_handle_new_from_gresource("/org/gtk/digtal.svg");
	if(priv->hour_svg==NULL)g_print("Svg init fail:resource:///org/gtk/digtal.svg\n");
}

MyClock * my_clock_new(guint hour, guint minute, guint second, gboolean run) {
	return g_object_new(MY_TYPE_CLOCK, "hour", hour, "minute", minute, "second",
			second, "run", run, NULL);
}
;
MyClock * my_clock_new_now_local(gboolean run) {
	MyClock *c = g_object_new(MY_TYPE_CLOCK, NULL);
	MyClockPrivate *priv = my_clock_get_instance_private(c);
	GDateTime *time = g_date_time_new_now_local();
	priv->hour = g_date_time_get_hour(time);
	priv->minute = g_date_time_get_minute(time);
	priv->second = g_date_time_get_second(time);
	g_date_time_unref(time);
	g_object_set(c, "run", run, NULL);
	return c;
}
;
void my_clock_set_interval(MyClock *clock, gint interval_ms) {
	MyClockPrivate *priv = my_clock_get_instance_private(clock);
	g_object_set(clock, "interval", interval_ms, NULL);
}
;
gint my_clock_get_interval(MyClock *clock) {
	MyClockPrivate *priv = my_clock_get_instance_private(clock);
	return priv->interval;
}
;
