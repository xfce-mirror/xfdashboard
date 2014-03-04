/*
 * actor: Abstract base actor
 * 
 * Copyright 2012-2014 Stephan Haller <nomad@froevel.de>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#ifndef __XFOVERVIEW_ACTOR__
#define __XFOVERVIEW_ACTOR__

#include <clutter/clutter.h>

G_BEGIN_DECLS

#define XFDASHBOARD_TYPE_ACTOR					(xfdashboard_actor_get_type())
#define XFDASHBOARD_ACTOR(obj)					(G_TYPE_CHECK_INSTANCE_CAST((obj), XFDASHBOARD_TYPE_ACTOR, XfdashboardActor))
#define XFDASHBOARD_IS_ACTOR(obj)				(G_TYPE_CHECK_INSTANCE_TYPE((obj), XFDASHBOARD_TYPE_ACTOR))
#define XFDASHBOARD_ACTOR_CLASS(klass)			(G_TYPE_CHECK_CLASS_CAST((klass), XFDASHBOARD_TYPE_ACTOR, XfdashboardActorClass))
#define XFDASHBOARD_IS_ACTOR_CLASS(klass)		(G_TYPE_CHECK_CLASS_TYPE((klass), XFDASHBOARD_TYPE_ACTOR))
#define XFDASHBOARD_ACTOR_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS((obj), XFDASHBOARD_TYPE_ACTOR, XfdashboardActorClass))

typedef struct _XfdashboardActor				XfdashboardActor;
typedef struct _XfdashboardActorClass			XfdashboardActorClass;
typedef struct _XfdashboardActorPrivate			XfdashboardActorPrivate;

struct _XfdashboardActor
{
	/* Parent instance */
	ClutterActor				parent_instance;

	/* Private structure */
	XfdashboardActorPrivate		*priv;
};

struct _XfdashboardActorClass
{
	/*< private >*/
	/* Parent class */
	ClutterActorClass			parent_class;

	/*< public >*/
	/* Virtual functions */
};

/* Public API */
GType xfdashboard_actor_get_type(void) G_GNUC_CONST;

ClutterActor* xfdashboard_actor_new(void);

void xfdashboard_actor_install_stylable_property(XfdashboardActorClass *klass, GParamSpec *inParamSpec);
void xfdashboard_actor_install_stylable_property_by_name(XfdashboardActorClass *klass, const gchar *inParamName);
GHashTable* xfdashboard_actor_get_stylable_properties(XfdashboardActorClass *klass);
GHashTable* xfdashboard_actor_get_stylable_properties_full(XfdashboardActorClass *klass);

const gchar* xfdashboard_actor_get_style_classes(XfdashboardActor *self);
void xfdashboard_actor_set_style_classes(XfdashboardActor *self, const gchar *inStyleClass);
void xfdashboard_actor_add_style_class(XfdashboardActor *self, const gchar *inStyleClass);
void xfdashboard_actor_remove_style_class(XfdashboardActor *self, const gchar *inStyleClass);

const gchar* xfdashboard_actor_get_style_pseudo_classes(XfdashboardActor *self);
void xfdashboard_actor_set_style_pseudo_classes(XfdashboardActor *self, const gchar *inStylePseudoClass);
void xfdashboard_actor_add_style_pseudo_class(XfdashboardActor *self, const gchar *inStylePseudoClass);
void xfdashboard_actor_remove_style_pseudo_class(XfdashboardActor *self, const gchar *inStylePseudoClass);

void xfdashboard_actor_style_invalidate(XfdashboardActor *self);

G_END_DECLS

#endif