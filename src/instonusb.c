/*
instonusb.c from Slackel <http://www.slackel.gr>
Copyright (C) 2019 Dimitris Tzemos <dijemos@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License at <http://www.gnu.org/licenses/> for more details.
*/

#include <gtk/gtk.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <libintl.h>
#include <locale.h>
#include <sys/wait.h> 

#include <sys/stat.h>
#include <unistd.h>

#define _(String) gettext (String)
#define N_(String) String
#include "config.h"
#include "instonusb.h"

guint64 file_length(char* filename)
{
FILE *f = fopen ( filename, "r" );
guint64 pos;
guint64 end;

pos = ftell (f);
fseek (f, 0, SEEK_END);
end = ftell (f);
fseek (f, pos, SEEK_SET);
end=end/1024/1024;
return end;
}

void do_action (gboolean copy) {
	gchar *commandline, **command, *output, *fstype, *usbfstype, *isofile, *iso_arch, *usblabel, *persistent_size, *pass;
	GtkComboBox *listwidget;
	GtkTreeIter iter;
	GtkListStore *list;
	GtkWidget *usb_label_name, *isoname, *passphrase;
	isoname = (GtkWidget *) gtk_builder_get_object(widgetstree, "isoname");
	isofile = g_strdup(gtk_entry_get_text (GTK_ENTRY(isoname)));
	if (strstr(isofile,"64")!= NULL) {
			iso_arch="64";
		}
		else { 
			iso_arch="32";
		}
	
	usb_label_name = (GtkWidget *) gtk_builder_get_object(widgetstree, "usb_label_name");
	usblabel = g_strdup(gtk_entry_get_text (GTK_ENTRY(usb_label_name)));
	if (strlen(usblabel) == 0) {
		g_free(usblabel);
		usblabel = g_strdup("LIVE");
		}
	
	passphrase = (GtkWidget *) gtk_builder_get_object(widgetstree, "passphrase");
	pass = g_strdup(gtk_entry_get_text (GTK_ENTRY(passphrase)));
	if (strlen(pass) == 0) {
		g_free(pass);
		pass = g_strdup("");
		}
		
	const gchar *DW[] = { "copydevices" };

	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "usbfilesystem");
	gtk_combo_box_get_active_iter(listwidget, &iter);
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_tree_model_get((GtkTreeModel *) list, &iter, 0, &usbfstype, -1);
	if (strlen(usbfstype) == 0) {
		g_free(usbfstype);
		usbfstype = g_strdup("vfat");
	}
	
	if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "create_persistent_file"))) {
		listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "persistent_size");
		gtk_combo_box_get_active_iter(listwidget, &iter);
		list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
		gtk_tree_model_get((GtkTreeModel *) list, &iter, 0, &persistent_size, -1);
		if (strlen(persistent_size) == 0) {
			g_free(persistent_size);
			persistent_size = g_strdup("");
			}
		}
	else {
	    persistent_size = g_strdup("");
	   } 		

	
	fullpercent = FALSE;
	pulsebar = TRUE;
	progressbar_handler_id = g_timeout_add(100, progressbar_handler, NULL);
	if (location != NULL) {
		g_free(location);
	}
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, DW[(guint) copy]);
	gtk_combo_box_get_active_iter(listwidget, &iter);
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_tree_model_get((GtkTreeModel *) list, &iter, 0, &location, -1);
	if (location == NULL) {
		return;
	}		

	if (copy) {
		if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "create_persistent_file"))) {
			totalsize=g_ascii_strtoull(persistent_size, NULL, 10);
			commandline = g_strdup_printf("install_iso_on_usb.sh --persistent %s %s %s %s\n", iso_arch, location, persistent_size, pass);
			}
		else {	
			totalsize = file_length(isofile);
			commandline = g_strdup_printf("install_iso_on_usb.sh --usb %s %s %s %s %s\n", isofile, location, usbfstype, iso_arch, usblabel);
			}
	}

	g_shell_parse_argv(commandline, NULL, &command, NULL);
	g_free(commandline);
	g_spawn_async(NULL, command, NULL, G_SPAWN_SEARCH_PATH|G_SPAWN_DO_NOT_REAP_CHILD, NULL, NULL, &pid, NULL);
	g_child_watch_add(pid, on_process_end, NULL);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "copy_btn"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "cancel_btn"), TRUE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "usbfilesystem"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "copydevices"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "isofile_button"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "isoname"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "usb_label_name"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "usbbutton"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "persistent_size"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "create_persistent_file"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "passphrase"), FALSE);
	gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "checkbuttonpass"), FALSE);
	
	g_strfreev(command);
}                 


void on_create_persistent_file_toggled(GtkWidget *widget, gpointer user_data) {
	GtkWidget *label, *label3,*label4;
	label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label1"); 
	label3 = (GtkWidget *) gtk_builder_get_object(widgetstree, "label3");
	label4 = (GtkWidget *) gtk_builder_get_object(widgetstree, "label4");
	
	if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "create_persistent_file"))) {
		gtk_label_set_text(GTK_LABEL(label),gtk_label_get_text(GTK_LABEL(label4)));
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "persistent_size"), TRUE);
	}
	else {
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "persistent_size"), FALSE);
		gtk_label_set_text(GTK_LABEL(label),gtk_label_get_text(GTK_LABEL(label3)));
		
	}
}


char *lastnchars(int s, const char *input)
{
	char * ret;
    ret = malloc(s+1);
   
   if (ret == NULL)
	{
      printf("Unable to allocate memory.\n");
      exit(1);
	}  
	 for (int i = 0; i <= s; i++)
	{
	 ret[i] = input[strlen(input) - (s - i)];
	   }
return ret;
}

void on_checkbuttonpass_toggled (GtkWidget *widget, gpointer user_data) {
 	if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "checkbuttonpass"))) {
		gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "passphrase")), TRUE);
	} else {
		gtk_entry_set_visibility (GTK_ENTRY ((GtkWidget *) gtk_builder_get_object(widgetstree, "passphrase")), FALSE);
		}
}

void on_copy_btn_clicked (GtkWidget *widget, gpointer user_data) {
	GtkWidget *isoname, *isofiledialog;
	gchar *isofile;
	isofiledialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "isofiledialog");
	isoname = (GtkWidget *) gtk_builder_get_object(widgetstree, "isoname");
	isofile = g_strdup(gtk_entry_get_text (GTK_ENTRY(isoname)));
	if (strlen(gtk_entry_get_text (GTK_ENTRY(isoname))) != 0) {
	    if (strcmp(lastnchars(3,g_strdup(gtk_entry_get_text (GTK_ENTRY(isoname)))),"iso") == 0) {
			do_action(TRUE); 
         }
		else
			{
			 gtk_widget_show(isofiledialog);
				}
	}
	else {
		gtk_widget_show(isofiledialog);
		}	
}

void on_cancel_btn_clicked (GtkWidget *widget, gpointer user_data) {
	kill (pid, SIGTERM);
}


void on_exitp (GtkWidget *widget, gpointer user_data) {
	if (pid != 0) {
		kill (pid, SIGTERM);
	}
	gtk_main_quit();
}

gboolean progressbar_handler(gpointer data) {
	GtkProgressBar *progressbar;
	gchar *output;
	guint64 installsize;
	gdouble progressfraction;
	gchar *s_progressfraction;
	
	progressbar = (GtkProgressBar *) gtk_builder_get_object(widgetstree,"progressbar");
	
	if (pulsebar && ! fullpercent && g_file_test("/mnt/install",  G_FILE_TEST_IS_DIR)) {
		pulsebar = FALSE;
		g_source_remove(progressbar_handler_id);
		progressbar_handler_id = g_timeout_add(5000, progressbar_handler, NULL);
	}
	
	if (pulsebar) {
		gtk_progress_bar_pulse(progressbar);
	} else {
		g_spawn_command_line_sync("du -s -m /mnt/install", &output, NULL, NULL, NULL);
		installsize = g_ascii_strtoull(output, NULL, 10);
		progressfraction = installsize * 100 / totalsize;

		if (progressfraction >= 100) {
			gtk_progress_bar_set_text(progressbar, "100 %");
			gtk_progress_bar_set_fraction(progressbar, 1.0);
			fullpercent = TRUE;
			pulsebar = TRUE;
			g_source_remove(progressbar_handler_id);
			progressbar_handler_id = g_timeout_add(100, progressbar_handler, NULL);
		} else {
			gtk_progress_bar_set_fraction(progressbar, progressfraction / 100);
			s_progressfraction = g_strdup_printf("%2.0f %c", progressfraction, '%');
			gtk_progress_bar_set_text(progressbar, s_progressfraction);
			g_free(s_progressfraction);
		}
	}
	return TRUE;
}


void clearlocations() {
	GtkComboBox *listwidget;
	GtkListStore *list;
	// clear copydevices
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "copydevices");
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	gtk_list_store_clear (list) ;
}

void initlocations() {
	GtkComboBox *listwidget;
	GtkTreeIter iter;
	GtkListStore *list;
	gchar **lines, *output, *device;
	gint i;
	gint status;
	
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "copydevices");
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	copydevicescount = 0;
	g_spawn_command_line_sync("instonusb-location-detection.sh copy", &output, NULL, &status, NULL);
	if (status == 0) {
		lines = g_strsplit(output, "\n", 0);
		for (i=0; lines[i] != NULL && strlen(lines[i])>0; i++) {
			gtk_list_store_append(list, &iter);
			gtk_list_store_set(list, &iter, 0, lines[i], -1);
		}
		copydevicescount = i;
		g_strfreev(lines);
	}
	g_free(output);
	if (copydevicescount != 0){
		gtk_combo_box_set_active_iter(listwidget, &iter);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "copy_btn"), TRUE);
	}
}

void initpersistent() {
	GtkComboBox *listwidget;
	GtkTreeIter iter;
	GtkListStore *list;	
	GtkWidget *label;
	label = (GtkWidget *) gtk_builder_get_object(widgetstree, "label1"); 
	listwidget = (GtkComboBox *) gtk_builder_get_object(widgetstree, "persistent_size");
	list = (GtkListStore *) gtk_combo_box_get_model(listwidget);
	/*gtk_list_store_prepend(list, &iter);
	gtk_list_store_set(list, &iter, 0, "", -1);
	gtk_combo_box_set_active_iter(listwidget, &iter);*/
	
	if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "create_persistent_file"))) {
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "persistent_size"), TRUE);
	}
	else {
			gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "persistent_size"), FALSE);
		}
}

void on_process_end (GPid thepid, gint status, gpointer data) {
	GtkWidget *dialog;
	GtkProgressBar *progressbar;

	pid = 0;
	g_free(location);
	location = NULL;
	g_source_remove(progressbar_handler_id);
	
	progressbar = (GtkProgressBar *) gtk_builder_get_object(widgetstree,"progressbar");
	gtk_progress_bar_set_fraction(progressbar, 0);
	gtk_progress_bar_set_text(progressbar, "");

	if (copydevicescount != 0){
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "copy_btn"), TRUE);
	}
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "cancel_btn"), FALSE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "usbfilesystem"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "copydevices"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "isofile_button"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "isoname"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "usb_label_name"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "usbbutton"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "persistent_size"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "create_persistent_file"), TRUE);	
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "passphrase"), TRUE);
		gtk_widget_set_sensitive ((GtkWidget *) gtk_builder_get_object(widgetstree, "checkbuttonpass"), TRUE);
	
	if (0 == status) {
			if (gtk_toggle_button_get_active((GtkToggleButton*) gtk_builder_get_object(widgetstree, "create_persistent_file"))) {
				dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogpersistentfinished");
				}
			else {
				dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogfinished");
				}
		} 
	else {
		dialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "dialogerror");
	}
	
	gtk_widget_show(dialog);
}

void on_isofile_button_clicked(GtkWidget *widget, gpointer user_data){
	GtkWidget *isofilechooserdialog;
	isofilechooserdialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "isofilechooserdialog");
	gtk_widget_show(isofilechooserdialog);
}


void on_isofile_ok_button_clicked(GtkWidget *widget, gpointer user_data){
	GtkWidget *isoname, *isofilechooserdialog;
	gchar *filename;
	isofilechooserdialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "isofilechooserdialog");
	
	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (isofilechooserdialog));
    //     open_file (filename);
     //    g_free (filename);
	isoname = (GtkWidget *) gtk_builder_get_object(widgetstree, "isoname");
	gtk_entry_set_text (GTK_ENTRY(isoname),filename);
}

void on_usbbutton_clicked (GtkWidget *widget, gpointer user_data) {
	clearlocations();
	initlocations();
}

void on_about_activate (GtkWidget *widget, gpointer user_data) {
	GtkWidget *aboutdialog;
	
	aboutdialog = (GtkWidget *) gtk_builder_get_object(widgetstree, "aboutdialog");
	gtk_widget_show(aboutdialog);
}

void on_quit_activate (GtkWidget *widget, gpointer user_data) {
	if (pid != 0) {
		kill (pid, SIGTERM);
	}
	gtk_main_quit();
}


int main (int argc, char *argv[]) {
	GtkWindow *mainwindow;
	GtkWidget *menubar1;
	GtkWidget *about;
	GtkAboutDialog *aboutdialog;
	gchar *path;
		
	setlocale(LC_ALL, "");
	bindtextdomain(PROJECT_NAME, LOCALE_DIR);
	textdomain(PROJECT_NAME);
	
	path = g_strdup_printf("/sbin:/usr/sbin:/usr/local/sbin:%s", g_getenv("PATH"));
	g_setenv("PATH", path, TRUE);
	g_free(path);
	
	gtk_init(&argc, &argv);
	widgetstree = gtk_builder_new();
	gtk_builder_add_from_file(widgetstree, UI_FILE, NULL);
	gtk_builder_connect_signals(widgetstree, NULL);
	
	pid = 0;
	location = NULL;
		
	mainwindow = (GtkWindow *) gtk_builder_get_object(widgetstree, "mainwindow");
	gtk_window_set_icon_from_file(mainwindow, APP_ICON, NULL);
	aboutdialog = (GtkAboutDialog *) gtk_builder_get_object(widgetstree, "aboutdialog");
	
	gtk_about_dialog_set_version(aboutdialog, PROJECT_VERSION);
	gtk_widget_show((GtkWidget *) mainwindow);
	
	initlocations();
	initpersistent();
	
	gtk_main();
	return 0;
}
