/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_window1 (void)
{
  GtkWidget *window1;
  GdkPixbuf *window1_icon_pixbuf;
  GtkWidget *fixed1;
  GtkWidget *scrolledwindow1;
  GtkWidget *treeview1;
  GtkWidget *label_install;
  GtkWidget *image8;
  GtkWidget *exit;
  GtkWidget *alignment2;
  GtkWidget *hbox2;
  GtkWidget *image5;
  GtkWidget *label10;
  GtkWidget *button_install;
  GtkWidget *alignment4;
  GtkWidget *hbox4;
  GtkWidget *image7;
  GtkWidget *label12;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), _("Install metapackages"));
  gtk_window_set_resizable (GTK_WINDOW (window1), FALSE);
  window1_icon_pixbuf = create_pixbuf ("sidux-meta-icon.png");
  if (window1_icon_pixbuf)
    {
      gtk_window_set_icon (GTK_WINDOW (window1), window1_icon_pixbuf);
      gdk_pixbuf_unref (window1_icon_pixbuf);
    }

  fixed1 = gtk_fixed_new ();
  gtk_widget_show (fixed1);
  gtk_container_add (GTK_CONTAINER (window1), fixed1);
  gtk_container_set_border_width (GTK_CONTAINER (fixed1), 15);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_fixed_put (GTK_FIXED (fixed1), scrolledwindow1, 16, 40);
  gtk_widget_set_size_request (scrolledwindow1, 706, 462);

  treeview1 = gtk_tree_view_new ();
  gtk_widget_show (treeview1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), treeview1);
  gtk_widget_set_size_request (treeview1, 248, 136);
  gtk_tree_view_set_hover_selection (GTK_TREE_VIEW (treeview1), TRUE);
  gtk_tree_view_set_hover_expand (GTK_TREE_VIEW (treeview1), TRUE);

  label_install = gtk_label_new (_("Install additional metapackages"));
  gtk_widget_show (label_install);
  gtk_fixed_put (GTK_FIXED (fixed1), label_install, 24, 0);
  gtk_widget_set_size_request (label_install, 329, 33);
  gtk_misc_set_alignment (GTK_MISC (label_install), 0, 0.5);

  image8 = create_pixmap (window1, "install-meta-title.png");
  gtk_widget_show (image8);
  gtk_fixed_put (GTK_FIXED (fixed1), image8, 480, 0);
  gtk_widget_set_size_request (image8, 241, 33);

  exit = gtk_button_new ();
  gtk_widget_show (exit);
  gtk_fixed_put (GTK_FIXED (fixed1), exit, 32, 512);
  gtk_widget_set_size_request (exit, 150, 28);

  alignment2 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment2);
  gtk_container_add (GTK_CONTAINER (exit), alignment2);

  hbox2 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox2);
  gtk_container_add (GTK_CONTAINER (alignment2), hbox2);

  image5 = gtk_image_new_from_stock ("gtk-quit", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image5);
  gtk_box_pack_start (GTK_BOX (hbox2), image5, FALSE, FALSE, 0);

  label10 = gtk_label_new_with_mnemonic (_("Exit"));
  gtk_widget_show (label10);
  gtk_box_pack_start (GTK_BOX (hbox2), label10, FALSE, FALSE, 0);

  button_install = gtk_button_new ();
  gtk_widget_show (button_install);
  gtk_fixed_put (GTK_FIXED (fixed1), button_install, 552, 512);
  gtk_widget_set_size_request (button_install, 150, 28);

  alignment4 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment4);
  gtk_container_add (GTK_CONTAINER (button_install), alignment4);

  hbox4 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox4);
  gtk_container_add (GTK_CONTAINER (alignment4), hbox4);

  image7 = gtk_image_new_from_stock ("gtk-apply", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image7);
  gtk_box_pack_start (GTK_BOX (hbox4), image7, FALSE, FALSE, 0);

  label12 = gtk_label_new_with_mnemonic (_("Install"));
  gtk_widget_show (label12);
  gtk_box_pack_start (GTK_BOX (hbox4), label12, FALSE, FALSE, 0);

  g_signal_connect ((gpointer) window1, "configure_event",
                    G_CALLBACK (on_window1_configure_event),
                    NULL);
  g_signal_connect ((gpointer) window1, "delete_event",
                    G_CALLBACK (on_window1_delete_event),
                    NULL);
  g_signal_connect ((gpointer) exit, "clicked",
                    G_CALLBACK (on_exit_clicked),
                    NULL);
  g_signal_connect ((gpointer) button_install, "clicked",
                    G_CALLBACK (on_button_install_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window1, window1, "window1");
  GLADE_HOOKUP_OBJECT (window1, fixed1, "fixed1");
  GLADE_HOOKUP_OBJECT (window1, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (window1, treeview1, "treeview1");
  GLADE_HOOKUP_OBJECT (window1, label_install, "label_install");
  GLADE_HOOKUP_OBJECT (window1, image8, "image8");
  GLADE_HOOKUP_OBJECT (window1, exit, "exit");
  GLADE_HOOKUP_OBJECT (window1, alignment2, "alignment2");
  GLADE_HOOKUP_OBJECT (window1, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (window1, image5, "image5");
  GLADE_HOOKUP_OBJECT (window1, label10, "label10");
  GLADE_HOOKUP_OBJECT (window1, button_install, "button_install");
  GLADE_HOOKUP_OBJECT (window1, alignment4, "alignment4");
  GLADE_HOOKUP_OBJECT (window1, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (window1, image7, "image7");
  GLADE_HOOKUP_OBJECT (window1, label12, "label12");

  return window1;
}

GtkWidget*
create_dialog1 (void)
{
  GtkWidget *dialog1;
  GdkPixbuf *dialog1_icon_pixbuf;
  GtkWidget *dialog_vbox1;
  GtkWidget *fixed2;
  GtkWidget *label_no_conffiles;
  GtkWidget *dialog_action_area1;
  GtkWidget *okbutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), _("Info"));
  gtk_window_set_resizable (GTK_WINDOW (dialog1), FALSE);
  dialog1_icon_pixbuf = create_pixbuf ("sidux-meta-icon.png");
  if (dialog1_icon_pixbuf)
    {
      gtk_window_set_icon (GTK_WINDOW (dialog1), dialog1_icon_pixbuf);
      gdk_pixbuf_unref (dialog1_icon_pixbuf);
    }
  gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  fixed2 = gtk_fixed_new ();
  gtk_widget_show (fixed2);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), fixed2, TRUE, TRUE, 0);

  label_no_conffiles = gtk_label_new (_("no metapackage description files found\nor metapackage list empty.\n\ndefault directory is /usr/share/install-meta/\nor use   --dir=<dir>\n\n"));
  gtk_widget_show (label_no_conffiles);
  gtk_fixed_put (GTK_FIXED (fixed2), label_no_conffiles, 0, 0);
  gtk_widget_set_size_request (label_no_conffiles, 288, 80);
  gtk_label_set_justify (GTK_LABEL (label_no_conffiles), GTK_JUSTIFY_CENTER);
  gtk_label_set_width_chars (GTK_LABEL (label_no_conffiles), 2);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) okbutton1, "clicked",
                    G_CALLBACK (gtk_main_quit),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog1, "dialog1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, fixed2, "fixed2");
  GLADE_HOOKUP_OBJECT (dialog1, label_no_conffiles, "label_no_conffiles");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, okbutton1, "okbutton1");

  return dialog1;
}

