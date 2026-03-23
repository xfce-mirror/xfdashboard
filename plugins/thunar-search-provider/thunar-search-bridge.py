#!/usr/bin/env python3
import os
import subprocess
import dbus
import dbus.service
import sys
import socket
import signal
import psutil
from dbus.mainloop.glib import DBusGMainLoop
from gi.repository import GLib, Gio

def kill_existing_instances():
    # finds and kills other instances of this script
    current_pid = os.getpid()
    script_name = os.path.basename(__file__)
    for proc in psutil.process_iter(['pid', 'name', 'cmdline']):
        try:
            if proc.info['pid'] == current_pid:
                continue
            cmdline = proc.info['cmdline'] or []
            if script_name in " ".join(cmdline):
                os.kill(proc.info['pid'], signal.SIGTERM)
                try:
                    p = psutil.Process(proc.info['pid'])
                    p.wait(timeout=0.3)
                except (psutil.TimeoutExpired, psutil.NoSuchProcess):
                    try:
                        os.kill(proc.info['pid'], signal.SIGKILL)
                    except ProcessLookupError:
                        pass 
        except (psutil.NoSuchProcess, psutil.AccessDenied):
            continue

def cleanup_and_exit(signum, frame):
    sys.exit(0)

kill_existing_instances()
signal.signal(signal.SIGINT, cleanup_and_exit)
signal.signal(signal.SIGTERM, cleanup_and_exit)

# ensures only one instance runs
lock_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
try:
    lock_socket.bind('\0thunar_search_bridge_lock')
except socket.error:
    sys.exit(0)

class ThunarSearchProvider(dbus.service.Object):
    def __init__(self):
        # defines the dbus bus name and object path for the search provider
        self.bus_name = dbus.service.BusName('org.xfce.Thunar.SearchProvider', bus=dbus.SessionBus())
        dbus.service.Object.__init__(self, self.bus_name, '/org/xfce/Thunar/SearchProvider')

    def perform_search(self, terms):
        query = "*".join(terms)
        home = os.path.expanduser("~")
        try:
            cmd = [
                'find', home, 
                '-maxdepth', '4', 
                '-not', '-path', '*/.*', 
                '-not', '-path', '*/Videos/*', 
                '-not', '-path', '*/Music/*', 
                '-iname', f'*{query}*'
            ]
            results = subprocess.check_output(cmd).decode().splitlines()
            return results[:20]
        except Exception:
            return []

    @dbus.service.method('org.xfdashboard.Shell.SearchProvider2', in_signature='as', out_signature='as')
    def GetInitialResultSet(self, terms):
        # called when search begins
        return self.perform_search(terms)

    @dbus.service.method('org.xfdashboard.Shell.SearchProvider2', in_signature='asas', out_signature='as')
    def GetSubsearchResultSet(self, previous_results, new_terms):
        # called as user types
        return self.perform_search(new_terms)

    @dbus.service.method('org.xfdashboard.Shell.SearchProvider2', in_signature='as', out_signature='aa{sv}')
    def GetResultMetas(self, identifiers):
        metas = []
        for path in identifiers:
            # defaults to generic
            icon_name = 'folder' if os.path.isdir(path) else 'text-x-generic'
            try:
                # retrieve the system icon for the file
                gfile = Gio.File.new_for_path(path)
                info = gfile.query_info('standard::icon', Gio.FileQueryInfoFlags.NONE, None)
                icon = info.get_icon()
                if icon:
                    icon_name = icon.to_string()
            except Exception:
                pass

            metas.append({
                'id': dbus.String(path),
                'name': dbus.String(os.path.basename(path)),
                'gicon': dbus.String(icon_name),
                #'description': dbus.String(path)
            })
        return metas

    @dbus.service.method('org.xfdashboard.Shell.SearchProvider2', in_signature='sasu', out_signature='')
    def ActivateResult(self, identifier, terms, timestamp):
        subprocess.Popen(['thunar', identifier])

    @dbus.service.method('org.xfdashboard.Shell.SearchProvider2', in_signature='asu', out_signature='')
    def LaunchSearch(self, terms, timestamp):
        subprocess.Popen(['thunar', '--search'] + terms)

try:
    DBusGMainLoop(set_as_default=True)
    provider = ThunarSearchProvider()
    GLib.MainLoop().run()
except Exception as e:
    print(f"bridge crashed: {e}")
