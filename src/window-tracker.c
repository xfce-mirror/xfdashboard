/*
 * window-tracker: Tracks windows, workspaces, monitors and
 *                 listens for changes
 * 
 * Copyright 2012-2013 Stephan Haller <nomad@froevel.de>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "window-tracker.h"

#include <glib/gi18n-lib.h>

#include "marshal.h"

/* Define this class in GObject system */
G_DEFINE_TYPE(XfdashboardWindowTracker,
				xfdashboard_window_tracker,
				G_TYPE_OBJECT)

/* Private structure - access only by public API if needed */
#define XFDASHBOARD_WINDOW_TRACKER_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), XFDASHBOARD_TYPE_WINDOW_TRACKER, XfdashboardWindowTrackerPrivate))

struct _XfdashboardWindowTrackerPrivate
{
	/* Properties related */
	WnckWindow				*activeWindow;
	WnckWorkspace			*activeWorkspace;

	/* Instance related */
	WnckScreen				*screen;
};

/* Properties */
enum
{
	PROP_0,

	PROP_ACTIVE_WINDOW,
	PROP_ACTIVE_WORKSPACE,
	// TODO: PROP_PRIMARY_MONITOR,

	PROP_LAST
};

static GParamSpec* XfdashboardWindowTrackerProperties[PROP_LAST]={ 0, };

/* Signals */

enum
{
	SIGNAL_ACTIVE_WINDOW_CHANGED,
	SIGNAL_WINDOW_OPENED,
	SIGNAL_WINDOW_CLOSED,
	SIGNAL_WINDOW_GEOMETRY_CHANGED,
	SIGNAL_WINDOW_ACTIONS_CHANGED,
	SIGNAL_WINDOW_STATE_CHANGED,
	SIGNAL_WINDOW_ICON_CHANGED,
	SIGNAL_WINDOW_NAME_CHANGED,
	SIGNAL_WINDOW_WORKSPACE_CHANGED,

	SIGNAL_ACTIVE_WORKSPACE_CHANGED,
	SIGNAL_WORKSPACE_ADDED,
	SIGNAL_WORKSPACE_REMOVED,
	SIGNAL_WORKSPACE_NAME_CHANGED,

	// TODO: SIGNAL_PRIMARY_MONITOR_CHANGED,
	// TODO: SIGNAL_MONITOR_ADDED,
	// TODO: SIGNAL_MONITOR_REMOVED,

	SIGNAL_LAST
};

static guint XfdashboardWindowTrackerSignals[SIGNAL_LAST]={ 0, };

/* IMPLEMENTATION: Private variables and methods */
static XfdashboardWindowTracker *_xfdashboard_window_tracker_singleton=NULL;

/* Position and/or size of window has changed */
static void _xfdashboard_window_tracker_on_window_geometry_changed(XfdashboardWindowTracker *self, gpointer inUserData)
{
	WnckWindow			*window;

	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(WNCK_IS_WINDOW(inUserData));

	window=WNCK_WINDOW(inUserData);

	/* Emit signal */
	g_debug("Window '%s' changed position and/or size", wnck_window_get_name(window));
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_GEOMETRY_CHANGED], 0, window);
}

/* Action items of window has changed */
static void _xfdashboard_window_tracker_on_window_actions_changed(XfdashboardWindowTracker *self,
																	WnckWindowActions inChangedMask,
																	WnckWindowActions inNewValue,
																	gpointer inUserData)
{
	WnckWindow			*window;

	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(WNCK_IS_WINDOW(inUserData));

	window=WNCK_WINDOW(inUserData);

	/* Emit signal */
	g_debug("Window '%s' changed actions to %u with mask %u", wnck_window_get_name(window), inNewValue, inChangedMask);
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_ACTIONS_CHANGED], 0, window, inChangedMask, inNewValue);
}

/* State of window has changed */
static void _xfdashboard_window_tracker_on_window_state_changed(XfdashboardWindowTracker *self,
																	WnckWindowState inChangedMask,
																	WnckWindowState inNewValue,
																	gpointer inUserData)
{
	WnckWindow			*window;

	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(WNCK_IS_WINDOW(inUserData));

	window=WNCK_WINDOW(inUserData);

	/* Emit signal */
	g_debug("Window '%s' changed state to %u with mask %u", wnck_window_get_name(window), inNewValue, inChangedMask);
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_STATE_CHANGED], 0, window, inChangedMask, inNewValue);
}

/* Icon of window has changed */
static void _xfdashboard_window_tracker_on_window_icon_changed(XfdashboardWindowTracker *self, gpointer inUserData)
{
	WnckWindow			*window;

	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(WNCK_IS_WINDOW(inUserData));

	window=WNCK_WINDOW(inUserData);

	/* Emit signal */
	g_debug("Window '%s' changed its icon", wnck_window_get_name(window));
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_ICON_CHANGED], 0, window);
}

/* Name of window has changed */
static void _xfdashboard_window_tracker_on_window_name_changed(XfdashboardWindowTracker *self, gpointer inUserData)
{
	WnckWindow			*window;

	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(WNCK_IS_WINDOW(inUserData));

	window=WNCK_WINDOW(inUserData);

	/* Emit "signal */
	g_debug("Window changed its name to '%s'", wnck_window_get_name(window));
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_NAME_CHANGED], 0, window);
}

/* A window has moved to another workspace */
static void _xfdashboard_window_tracker_on_window_workspace_changed(XfdashboardWindowTracker *self, gpointer inUserData)
{
	WnckWindow			*window;
	WnckWorkspace		*workspace;

	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(WNCK_IS_WINDOW(inUserData));

	window=WNCK_WINDOW(inUserData);

	/* Get workspace window resides on */
	workspace=wnck_window_get_workspace(window);

	/* Emit signal */
	g_debug("Window '%s' moved to workspace %d (%s)",
				wnck_window_get_name(window),
				workspace ? wnck_workspace_get_number(workspace) : -1,
				workspace ? wnck_workspace_get_name(workspace) : "<nil>");
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_WORKSPACE_CHANGED], 0, window, workspace);
}

/* A window was activated */
static void _xfdashboard_window_tracker_on_active_window_changed(XfdashboardWindowTracker *self,
																	WnckWindow *inPreviousWindow,
																	gpointer inUserData)
{
	XfdashboardWindowTrackerPrivate		*priv;
	WnckScreen							*screen;
	WnckWindow							*oldActiveWindow;

	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(inPreviousWindow==NULL || WNCK_IS_WINDOW(inPreviousWindow));
	g_return_if_fail(WNCK_IS_SCREEN(inUserData));

	priv=self->priv;
	screen=WNCK_SCREEN(inUserData);

	/* Get and remember new active window */
	oldActiveWindow=priv->activeWindow;
	priv->activeWindow=wnck_screen_get_active_window(screen);

	/* Emit signal */
	g_debug("Active window changed from '%s' to '%s'",
				oldActiveWindow ? wnck_window_get_name(oldActiveWindow) : "<nil>",
				priv->activeWindow ? wnck_window_get_name(priv->activeWindow) : "<nil>");
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_ACTIVE_WINDOW_CHANGED], 0, oldActiveWindow, priv->activeWindow);
}

/* A window was closed */
static void _xfdashboard_window_tracker_on_window_closed(XfdashboardWindowTracker *self,
															WnckWindow *inWindow,
															gpointer inUserData)
{
	XfdashboardWindowTrackerPrivate		*priv;

	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(WNCK_IS_WINDOW(inWindow));
	g_return_if_fail(WNCK_IS_SCREEN(inUserData));

	priv=self->priv;

	/* Should not happen but if closed window is the last known one
	 * reset to NULL
	 */
	if(priv->activeWindow==inWindow) priv->activeWindow=NULL;

	/* Remove all signal handlers for closed window */
	g_signal_handlers_disconnect_by_data(inWindow, self);

	/* Emit signals */
	g_debug("Window '%s' closed", wnck_window_get_name(inWindow));
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_CLOSED], 0, inWindow);
}

/* A new window was opened */
static void _xfdashboard_window_tracker_on_window_opened(XfdashboardWindowTracker *self,
															WnckWindow *inWindow,
															gpointer inUserData)
{
	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(WNCK_IS_WINDOW(inWindow));
	g_return_if_fail(WNCK_IS_SCREEN(inUserData));

	/* Check if window has a workspace set so it is a so called "real" window */
	if(!wnck_window_get_workspace(inWindow))
	{
		g_debug("Window '%s' has no workspace - do not emit signal!", wnck_window_get_name(inWindow));
		return;
	}

	/* Connect signals on newly opened window */
	g_signal_connect_swapped(inWindow, "geometry-changed", G_CALLBACK(_xfdashboard_window_tracker_on_window_geometry_changed), self);
	g_signal_connect_swapped(inWindow, "actions-changed", G_CALLBACK(_xfdashboard_window_tracker_on_window_actions_changed), self);
	g_signal_connect_swapped(inWindow, "state-changed", G_CALLBACK(_xfdashboard_window_tracker_on_window_state_changed), self);
	g_signal_connect_swapped(inWindow, "icon-changed", G_CALLBACK(_xfdashboard_window_tracker_on_window_icon_changed), self);
	g_signal_connect_swapped(inWindow, "name-changed", G_CALLBACK(_xfdashboard_window_tracker_on_window_name_changed), self);
	g_signal_connect_swapped(inWindow, "workspace-changed", G_CALLBACK(_xfdashboard_window_tracker_on_window_workspace_changed), self);

	/* Emit signal */
	g_debug("Window '%s' created", wnck_window_get_name(inWindow));
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_OPENED], 0, inWindow);
}

/* A workspace changed its name */
static void _xfdashboard_window_tracker_on_workspace_name_changed(XfdashboardWindowTracker *self,
																	gpointer inUserData)
{
	WnckWorkspace				*workspace;

	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(WNCK_IS_WORKSPACE(inUserData));

	workspace=WNCK_WORKSPACE(inUserData);

	/* Emit signal */
	g_debug("Workspace #%d changed name to '%s'", wnck_workspace_get_number(workspace), wnck_workspace_get_name(workspace));
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_WORKSPACE_NAME_CHANGED], 0, workspace);

}

/* A workspace was activated */
static void _xfdashboard_window_tracker_on_active_workspace_changed(XfdashboardWindowTracker *self,
																	WnckWorkspace *inPreviousWorkspace,
																	gpointer inUserData)
{
	XfdashboardWindowTrackerPrivate		*priv;
	WnckScreen							*screen;
	WnckWorkspace						*oldActiveWorkspace;

	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(inPreviousWorkspace==NULL || WNCK_IS_WORKSPACE(inPreviousWorkspace));
	g_return_if_fail(WNCK_IS_SCREEN(inUserData));

	priv=self->priv;
	screen=WNCK_SCREEN(inUserData);

	/* Get and remember new active workspace */
	oldActiveWorkspace=priv->activeWorkspace;
	priv->activeWorkspace=wnck_screen_get_active_workspace(screen);

	/* Emit signal */
	g_debug("Active workspace changed from #%d (%s) to #%d (%s)",
				oldActiveWorkspace ? wnck_workspace_get_number(oldActiveWorkspace) : -1,
				oldActiveWorkspace ? wnck_workspace_get_name(oldActiveWorkspace) : "<nil>",
				priv->activeWorkspace ? wnck_workspace_get_number(priv->activeWorkspace) : -1,
				priv->activeWorkspace ? wnck_workspace_get_name(priv->activeWorkspace) : "<nil>");
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_ACTIVE_WORKSPACE_CHANGED], 0, oldActiveWorkspace, priv->activeWorkspace);
}

/* A workspace was destroyed */
static void _xfdashboard_window_tracker_on_workspace_destroyed(XfdashboardWindowTracker *self,
																WnckWorkspace *inWorkspace,
																gpointer inUserData)
{
	XfdashboardWindowTrackerPrivate		*priv;

	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(WNCK_IS_WORKSPACE(inWorkspace));
	g_return_if_fail(WNCK_IS_SCREEN(inUserData));

	priv=self->priv;

	/* Should not happen but if destroyed workspace is the last known one
	 * reset to NULL
	 */
	if(priv->activeWorkspace==inWorkspace) priv->activeWorkspace=NULL;

	/* Remove all signal handlers for closed window */
	g_signal_handlers_disconnect_by_data(inWorkspace, self);

	/* Emit signal */
	g_debug("Workspace #%d (%s) destroyed", wnck_workspace_get_number(inWorkspace), wnck_workspace_get_name(inWorkspace));
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_WORKSPACE_REMOVED], 0, inWorkspace);
}

/* A new workspace was created */
static void _xfdashboard_window_tracker_on_workspace_created(XfdashboardWindowTracker *self,
																WnckWorkspace *inWorkspace,
																gpointer inUserData)
{
	g_return_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self));
	g_return_if_fail(WNCK_IS_WORKSPACE(inWorkspace));
	g_return_if_fail(WNCK_IS_SCREEN(inUserData));

	/* Connect signals on newly created workspace */
	g_signal_connect_swapped(inWorkspace, "name-changed", G_CALLBACK(_xfdashboard_window_tracker_on_workspace_name_changed), self);

	/* Emit signal */
	g_debug("New workspace #%d (%s) created", wnck_workspace_get_number(inWorkspace), wnck_workspace_get_name(inWorkspace));
	g_signal_emit(self, XfdashboardWindowTrackerSignals[SIGNAL_WORKSPACE_ADDED], 0, inWorkspace);
}

/* IMPLEMENTATION: GObject */

/* Dispose this object */
static void _xfdashboard_window_tracker_dispose(GObject *inObject)
{
	XfdashboardWindowTracker			*self=XFDASHBOARD_WINDOW_TRACKER(inObject);
	XfdashboardWindowTrackerPrivate		*priv=self->priv;

	/* Dispose allocated resources */
	g_debug("Disposing window tracker");
	if(priv->screen)
	{
		// TODO: Disconnect from all windows
		// TODO: Disconnect from all workspaces
		g_signal_handlers_disconnect_by_data(priv->screen, self);
		priv->screen=NULL;
	}

	/* Call parent's class dispose method */
	G_OBJECT_CLASS(xfdashboard_window_tracker_parent_class)->dispose(inObject);
}

/* Set/get properties */
static void _xfdashboard_window_tracker_set_property(GObject *inObject,
														guint inPropID,
														const GValue *inValue,
														GParamSpec *inSpec)
{
	switch(inPropID)
	{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(inObject, inPropID, inSpec);
			break;
	}
}

static void _xfdashboard_window_tracker_get_property(GObject *inObject,
														guint inPropID,
														GValue *outValue,
														GParamSpec *inSpec)
{
	XfdashboardWindowTracker			*self=XFDASHBOARD_WINDOW_TRACKER(inObject);

	switch(inPropID)
	{
		case PROP_ACTIVE_WINDOW:
			g_value_set_object(outValue, self->priv->activeWindow);
			break;

		case PROP_ACTIVE_WORKSPACE:
			g_value_set_object(outValue, self->priv->activeWorkspace);
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(inObject, inPropID, inSpec);
			break;
	}
}

/* Class initialization
 * Override functions in parent classes and define properties
 * and signals
 */
void xfdashboard_window_tracker_class_init(XfdashboardWindowTrackerClass *klass)
{
	GObjectClass		*gobjectClass=G_OBJECT_CLASS(klass);

	/* Override functions */
	gobjectClass->dispose=_xfdashboard_window_tracker_dispose;
	gobjectClass->set_property=_xfdashboard_window_tracker_set_property;
	gobjectClass->get_property=_xfdashboard_window_tracker_get_property;

	/* Set up private structure */
	g_type_class_add_private(klass, sizeof(XfdashboardWindowTrackerPrivate));

	/* Define properties */
	XfdashboardWindowTrackerProperties[PROP_ACTIVE_WINDOW]=
		g_param_spec_object("active-window",
							_("Active window"),
							_("The current active window"),
							WNCK_TYPE_WINDOW,
							G_PARAM_READABLE);

	XfdashboardWindowTrackerProperties[PROP_ACTIVE_WORKSPACE]=
		g_param_spec_object("active-workspace",
							_("Active workspace"),
							_("The current active workspace"),
							WNCK_TYPE_WORKSPACE,
							G_PARAM_READABLE);

	// TODO: XfdashboardWindowTrackerProperties[PROP_PRIMARY_MONITOR]=
		// TODO: g_param_spec_object("primary-monitor",
							// TODO: _("Primary monitor"),
							// TODO: _("The current primary monitor"),
							// TODO: XFDASHBOARD_TYPE_MONITOR,
							// TODO: G_PARAM_READABLE);

	g_object_class_install_properties(gobjectClass, PROP_LAST, XfdashboardWindowTrackerProperties);

	/* Define signals */
	XfdashboardWindowTrackerSignals[SIGNAL_ACTIVE_WINDOW_CHANGED]=
		g_signal_new("active-window-changed",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, active_window_changed),
						NULL,
						NULL,
						_xfdashboard_marshal_VOID__OBJECT_OBJECT,
						G_TYPE_NONE,
						2,
						WNCK_TYPE_WINDOW,
						WNCK_TYPE_WINDOW);

	XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_OPENED]=
		g_signal_new("window-opened",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, window_opened),
						NULL,
						NULL,
						g_cclosure_marshal_VOID__OBJECT,
						G_TYPE_NONE,
						1,
						WNCK_TYPE_WINDOW);

	XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_CLOSED]=
		g_signal_new("window-closed",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, window_closed),
						NULL,
						NULL,
						g_cclosure_marshal_VOID__OBJECT,
						G_TYPE_NONE,
						1,
						WNCK_TYPE_WINDOW);

	XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_GEOMETRY_CHANGED]=
		g_signal_new("window-geometry-changed",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, window_geometry_changed),
						NULL,
						NULL,
						g_cclosure_marshal_VOID__OBJECT,
						G_TYPE_NONE,
						1,
						WNCK_TYPE_WINDOW);

	XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_ACTIONS_CHANGED]=
		g_signal_new("window-actions-changed",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, window_actions_changed),
						NULL,
						NULL,
						g_cclosure_marshal_VOID__OBJECT,
						G_TYPE_NONE,
						1,
						WNCK_TYPE_WINDOW);

	XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_STATE_CHANGED]=
		g_signal_new("window-state-changed",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, window_state_changed),
						NULL,
						NULL,
						g_cclosure_marshal_VOID__OBJECT,
						G_TYPE_NONE,
						1,
						WNCK_TYPE_WINDOW);

	XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_ICON_CHANGED]=
		g_signal_new("window-icon-changed",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, window_icon_changed),
						NULL,
						NULL,
						g_cclosure_marshal_VOID__OBJECT,
						G_TYPE_NONE,
						1,
						WNCK_TYPE_WINDOW);

	XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_NAME_CHANGED]=
		g_signal_new("window-name-changed",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, window_name_changed),
						NULL,
						NULL,
						g_cclosure_marshal_VOID__OBJECT,
						G_TYPE_NONE,
						1,
						WNCK_TYPE_WINDOW);

	XfdashboardWindowTrackerSignals[SIGNAL_WINDOW_WORKSPACE_CHANGED]=
		g_signal_new("window-workspace-changed",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, window_workspace_changed),
						NULL,
						NULL,
						_xfdashboard_marshal_VOID__OBJECT_OBJECT,
						G_TYPE_NONE,
						2,
						WNCK_TYPE_WINDOW,
						WNCK_TYPE_WORKSPACE);


	XfdashboardWindowTrackerSignals[SIGNAL_ACTIVE_WORKSPACE_CHANGED]=
		g_signal_new("active-workspace-changed",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, active_workspace_changed),
						NULL,
						NULL,
						_xfdashboard_marshal_VOID__OBJECT_OBJECT,
						G_TYPE_NONE,
						2,
						WNCK_TYPE_WORKSPACE,
						WNCK_TYPE_WORKSPACE);

	XfdashboardWindowTrackerSignals[SIGNAL_WORKSPACE_ADDED]=
		g_signal_new("workspace-added",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, workspace_added),
						NULL,
						NULL,
						g_cclosure_marshal_VOID__OBJECT,
						G_TYPE_NONE,
						1,
						WNCK_TYPE_WORKSPACE);

	XfdashboardWindowTrackerSignals[SIGNAL_WORKSPACE_REMOVED]=
		g_signal_new("workspace-removed",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, workspace_removed),
						NULL,
						NULL,
						g_cclosure_marshal_VOID__OBJECT,
						G_TYPE_NONE,
						1,
						WNCK_TYPE_WORKSPACE);

	XfdashboardWindowTrackerSignals[SIGNAL_WORKSPACE_NAME_CHANGED]=
		g_signal_new("workspace-name-changed",
						G_TYPE_FROM_CLASS(klass),
						G_SIGNAL_RUN_LAST,
						G_STRUCT_OFFSET(XfdashboardWindowTrackerClass, workspace_name_changed),
						NULL,
						NULL,
						g_cclosure_marshal_VOID__OBJECT,
						G_TYPE_NONE,
						1,
						WNCK_TYPE_WORKSPACE);
}

/* Object initialization
 * Create private structure and set up default values
 */
void xfdashboard_window_tracker_init(XfdashboardWindowTracker *self)
{
	XfdashboardWindowTrackerPrivate	*priv;

	priv=self->priv=XFDASHBOARD_WINDOW_TRACKER_GET_PRIVATE(self);

	g_debug("Initializing window tracker");

	/* Set default values */
	priv->screen=wnck_screen_get_default();
	priv->activeWindow=NULL;
	priv->activeWorkspace=NULL;

	/* The very first call to libwnck should be setting the client type */
	wnck_set_client_type(WNCK_CLIENT_TYPE_PAGER);

	/* Connect signals to screen */
	g_signal_connect_swapped(priv->screen, "window-closed", G_CALLBACK(_xfdashboard_window_tracker_on_window_closed), self);
	g_signal_connect_swapped(priv->screen, "window-opened", G_CALLBACK(_xfdashboard_window_tracker_on_window_opened), self);
	g_signal_connect_swapped(priv->screen, "active-window-changed", G_CALLBACK(_xfdashboard_window_tracker_on_active_window_changed), self);

	g_signal_connect_swapped(priv->screen, "workspace-destroyed", G_CALLBACK(_xfdashboard_window_tracker_on_workspace_destroyed), self);
	g_signal_connect_swapped(priv->screen, "workspace-created", G_CALLBACK(_xfdashboard_window_tracker_on_workspace_created), self);
	g_signal_connect_swapped(priv->screen, "active-workspace-changed", G_CALLBACK(_xfdashboard_window_tracker_on_active_workspace_changed), self);
}

/* Implementation: Public API */

/* Create new instance */
XfdashboardWindowTracker* xfdashboard_window_tracker_get_default(void)
{
	if(G_UNLIKELY(_xfdashboard_window_tracker_singleton==NULL))
	{
		_xfdashboard_window_tracker_singleton=
			XFDASHBOARD_WINDOW_TRACKER(g_object_new(XFDASHBOARD_TYPE_WINDOW_TRACKER, NULL));
	}
		else g_object_ref(_xfdashboard_window_tracker_singleton);

	return(_xfdashboard_window_tracker_singleton);

}

/* Get active window */
WnckWindow* xfdashboard_window_tracker_get_active_window(XfdashboardWindowTracker *self)
{
	g_return_val_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self), NULL);

	return(self->priv->activeWindow);
}

/* Get active workspace */
WnckWorkspace* xfdashboard_window_tracker_get_active_workspace(XfdashboardWindowTracker *self)
{
	g_return_val_if_fail(XFDASHBOARD_IS_WINDOW_TRACKER(self), NULL);

	return(self->priv->activeWorkspace);
}
