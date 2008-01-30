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
  dialog1_icon_pixbuf = create_pixbuf ("metapackage-installer-16.xpm");
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

GtkWidget*
create_package_info (void)
{
  GtkWidget *package_info;
  GdkPixbuf *package_info_icon_pixbuf;
  GtkWidget *fixed4;
  GtkWidget *label_file;
  GtkWidget *button1;
  GtkWidget *label_pre;
  GtkWidget *label_list;
  GtkWidget *label_post;
  GtkWidget *scrolledwindow4;
  GtkWidget *textview_pre_processing;
  GtkWidget *scrolledwindow2;
  GtkWidget *treeview2;
  GtkWidget *scrolledwindow5;
  GtkWidget *textview_post_processing;

  package_info = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (package_info), _("Package Info"));
  gtk_window_set_resizable (GTK_WINDOW (package_info), FALSE);
  package_info_icon_pixbuf = create_pixbuf ("metapackage-installer-16.xpm");
  if (package_info_icon_pixbuf)
    {
      gtk_window_set_icon (GTK_WINDOW (package_info), package_info_icon_pixbuf);
      gdk_pixbuf_unref (package_info_icon_pixbuf);
    }

  fixed4 = gtk_fixed_new ();
  gtk_widget_show (fixed4);
  gtk_container_add (GTK_CONTAINER (package_info), fixed4);
  gtk_container_set_border_width (GTK_CONTAINER (fixed4), 15);

  label_file = gtk_label_new ("");
  gtk_widget_show (label_file);
  gtk_fixed_put (GTK_FIXED (fixed4), label_file, 0, 0);
  gtk_widget_set_size_request (label_file, 729, 25);
  gtk_misc_set_alignment (GTK_MISC (label_file), 0, 0.5);

  button1 = gtk_button_new_from_stock ("gtk-close");
  gtk_widget_show (button1);
  gtk_fixed_put (GTK_FIXED (fixed4), button1, 616, 520);
  gtk_widget_set_size_request (button1, 97, 30);

  label_pre = gtk_label_new (_("PRE PROCESSING script:"));
  gtk_widget_show (label_pre);
  gtk_fixed_put (GTK_FIXED (fixed4), label_pre, 0, 32);
  gtk_widget_set_size_request (label_pre, 728, 24);
  gtk_misc_set_alignment (GTK_MISC (label_pre), 0, 0.5);

  label_list = gtk_label_new (_("PACKAGE LIST:"));
  gtk_widget_show (label_list);
  gtk_fixed_put (GTK_FIXED (fixed4), label_list, 0, 176);
  gtk_widget_set_size_request (label_list, 728, 24);
  gtk_misc_set_alignment (GTK_MISC (label_list), 0, 0.5);

  label_post = gtk_label_new (_("POST PROCESSING sript:"));
  gtk_widget_show (label_post);
  gtk_fixed_put (GTK_FIXED (fixed4), label_post, 0, 376);
  gtk_widget_set_size_request (label_post, 728, 24);
  gtk_misc_set_alignment (GTK_MISC (label_post), 0, 0.5);

  scrolledwindow4 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow4);
  gtk_fixed_put (GTK_FIXED (fixed4), scrolledwindow4, 0, 56);
  gtk_widget_set_size_request (scrolledwindow4, 729, 113);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow4), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow4), GTK_SHADOW_IN);

  textview_pre_processing = gtk_text_view_new ();
  gtk_widget_show (textview_pre_processing);
  gtk_container_add (GTK_CONTAINER (scrolledwindow4), textview_pre_processing);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (textview_pre_processing), FALSE);

  scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow2);
  gtk_fixed_put (GTK_FIXED (fixed4), scrolledwindow2, 0, 200);
  gtk_widget_set_size_request (scrolledwindow2, 729, 169);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow2), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow2), GTK_SHADOW_IN);

  treeview2 = gtk_tree_view_new ();
  gtk_widget_show (treeview2);
  gtk_container_add (GTK_CONTAINER (scrolledwindow2), treeview2);

  scrolledwindow5 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow5);
  gtk_fixed_put (GTK_FIXED (fixed4), scrolledwindow5, 0, 400);
  gtk_widget_set_size_request (scrolledwindow5, 729, 112);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow5), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow5), GTK_SHADOW_IN);

  textview_post_processing = gtk_text_view_new ();
  gtk_widget_show (textview_post_processing);
  gtk_container_add (GTK_CONTAINER (scrolledwindow5), textview_post_processing);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (textview_post_processing), FALSE);

  g_signal_connect ((gpointer) package_info, "delete_event",
                    G_CALLBACK (gtk_widget_destroy),
                    NULL);
  g_signal_connect ((gpointer) button1, "clicked",
                    G_CALLBACK (on_button1_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (package_info, package_info, "package_info");
  GLADE_HOOKUP_OBJECT (package_info, fixed4, "fixed4");
  GLADE_HOOKUP_OBJECT (package_info, label_file, "label_file");
  GLADE_HOOKUP_OBJECT (package_info, button1, "button1");
  GLADE_HOOKUP_OBJECT (package_info, label_pre, "label_pre");
  GLADE_HOOKUP_OBJECT (package_info, label_list, "label_list");
  GLADE_HOOKUP_OBJECT (package_info, label_post, "label_post");
  GLADE_HOOKUP_OBJECT (package_info, scrolledwindow4, "scrolledwindow4");
  GLADE_HOOKUP_OBJECT (package_info, textview_pre_processing, "textview_pre_processing");
  GLADE_HOOKUP_OBJECT (package_info, scrolledwindow2, "scrolledwindow2");
  GLADE_HOOKUP_OBJECT (package_info, treeview2, "treeview2");
  GLADE_HOOKUP_OBJECT (package_info, scrolledwindow5, "scrolledwindow5");
  GLADE_HOOKUP_OBJECT (package_info, textview_post_processing, "textview_post_processing");

  return package_info;
}

GtkWidget*
create_window_main (void)
{
  GtkWidget *window_main;
  GdkPixbuf *window_main_icon_pixbuf;
  GtkWidget *fixed5;
  GtkWidget *notebook1;
  GtkWidget *fixed6;
  GtkWidget *scrolledwindow1;
  GtkWidget *treeview1;
  GtkWidget *button_install;
  GtkWidget *alignment4;
  GtkWidget *hbox4;
  GtkWidget *image7;
  GtkWidget *label12;
  GtkWidget *checkbutton_yes;
  GtkWidget *exit;
  GtkWidget *alignment2;
  GtkWidget *hbox2;
  GtkWidget *image5;
  GtkWidget *label10;
  GtkWidget *button_collapse;
  GtkWidget *image11;
  GtkWidget *button_expand;
  GtkWidget *image10;
  GtkWidget *label16;
  GtkWidget *fixed7;
  GtkWidget *scrolledwindow6;
  GtkWidget *viewport1;
  GtkWidget *label19;
  GtkWidget *exit2;
  GtkWidget *alignment5;
  GtkWidget *hbox5;
  GtkWidget *image12;
  GtkWidget *label18;
  GtkWidget *button_nonfree;
  GtkWidget *alignment6;
  GtkWidget *hbox6;
  GtkWidget *image13;
  GtkWidget *label20;
  GtkWidget *button_back;
  GtkWidget *alignment7;
  GtkWidget *hbox7;
  GtkWidget *image14;
  GtkWidget *label21;
  GtkWidget *label17;
  GtkWidget *label_install;
  GtkWidget *image8;
  GtkWidget *label22;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  window_main = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window_main), _("Install metapackages"));
  gtk_window_set_position (GTK_WINDOW (window_main), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_resizable (GTK_WINDOW (window_main), FALSE);
  window_main_icon_pixbuf = create_pixbuf ("metapackage-installer-16.xpm");
  if (window_main_icon_pixbuf)
    {
      gtk_window_set_icon (GTK_WINDOW (window_main), window_main_icon_pixbuf);
      gdk_pixbuf_unref (window_main_icon_pixbuf);
    }
  gtk_window_set_gravity (GTK_WINDOW (window_main), GDK_GRAVITY_CENTER);

  fixed5 = gtk_fixed_new ();
  gtk_widget_show (fixed5);
  gtk_container_add (GTK_CONTAINER (window_main), fixed5);
  gtk_container_set_border_width (GTK_CONTAINER (fixed5), 10);

  notebook1 = gtk_notebook_new ();
  gtk_widget_show (notebook1);
  gtk_fixed_put (GTK_FIXED (fixed5), notebook1, 0, 58);
  gtk_widget_set_size_request (notebook1, 764, 466);

  fixed6 = gtk_fixed_new ();
  gtk_widget_show (fixed6);
  gtk_container_add (GTK_CONTAINER (notebook1), fixed6);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_fixed_put (GTK_FIXED (fixed6), scrolledwindow1, 48, 8);
  gtk_widget_set_size_request (scrolledwindow1, 700, 380);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_IN);

  treeview1 = gtk_tree_view_new ();
  gtk_widget_show (treeview1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), treeview1);
  gtk_widget_set_size_request (treeview1, 248, 96);

  button_install = gtk_button_new ();
  gtk_widget_show (button_install);
  gtk_fixed_put (GTK_FIXED (fixed6), button_install, 600, 398);
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

  checkbutton_yes = gtk_check_button_new_with_mnemonic (_("use option --yes for apt-get install"));
  gtk_widget_show (checkbutton_yes);
  gtk_fixed_put (GTK_FIXED (fixed6), checkbutton_yes, 288, 399);
  gtk_widget_set_size_request (checkbutton_yes, 297, 28);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton_yes), TRUE);

  exit = gtk_button_new ();
  gtk_widget_show (exit);
  gtk_fixed_put (GTK_FIXED (fixed6), exit, 48, 398);
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

  button_collapse = gtk_button_new ();
  gtk_widget_show (button_collapse);
  gtk_fixed_put (GTK_FIXED (fixed6), button_collapse, 8, 48);
  gtk_widget_set_size_request (button_collapse, 28, 28);
  gtk_tooltips_set_tip (tooltips, button_collapse, _("collapse all "), NULL);

  image11 = gtk_image_new_from_stock ("gtk-go-forward", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image11);
  gtk_container_add (GTK_CONTAINER (button_collapse), image11);

  button_expand = gtk_button_new ();
  gtk_widget_show (button_expand);
  gtk_fixed_put (GTK_FIXED (fixed6), button_expand, 8, 16);
  gtk_widget_set_size_request (button_expand, 28, 28);
  gtk_tooltips_set_tip (tooltips, button_expand, _("expand all"), NULL);

  image10 = gtk_image_new_from_stock ("gtk-go-down", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image10);
  gtk_container_add (GTK_CONTAINER (button_expand), image10);

  label16 = gtk_label_new (_("<b>Metapackages</b>"));
  gtk_widget_show (label16);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label16);
  gtk_label_set_use_markup (GTK_LABEL (label16), TRUE);

  fixed7 = gtk_fixed_new ();
  gtk_widget_show (fixed7);
  gtk_container_add (GTK_CONTAINER (notebook1), fixed7);

  scrolledwindow6 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow6);
  gtk_fixed_put (GTK_FIXED (fixed7), scrolledwindow6, 40, 40);
  gtk_widget_set_size_request (scrolledwindow6, 470, 252);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow6), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (viewport1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow6), viewport1);
  gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport1), GTK_SHADOW_ETCHED_IN);

  label19 = gtk_label_new (_("<span font_desc=\"11\"><b><u>Hints for hardware with non-free needs:</u></b>\n\nsidux contains <b>only dfsg free software</b>, so you may want to add <b>contrib</b> and <b>non-free </b>to your \n<b>/etc/apt/sources.list</b>.\n\nYou can add contrib and non-free sources with the button <b>\"Install non-free\"</b>. This will add more metapackages.\n\nsee non-free category in the metapackages list </span> \n\n<span foreground=\"Red\" font_desc=\"12\"><b>firmware</b> needs non-free</span>\n\n<span foreground=\"Darkblue\" font_desc=\"12\">you can also try <b><i>\"smxi\"</i> script</b> for non free</span>"));
  gtk_widget_show (label19);
  gtk_container_add (GTK_CONTAINER (viewport1), label19);
  gtk_label_set_use_markup (GTK_LABEL (label19), TRUE);
  gtk_label_set_line_wrap (GTK_LABEL (label19), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label19), 0, 0);
  gtk_misc_set_padding (GTK_MISC (label19), 20, 10);

  exit2 = gtk_button_new ();
  gtk_widget_show (exit2);
  gtk_fixed_put (GTK_FIXED (fixed7), exit2, 560, 248);
  gtk_widget_set_size_request (exit2, 176, 28);

  alignment5 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment5);
  gtk_container_add (GTK_CONTAINER (exit2), alignment5);

  hbox5 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox5);
  gtk_container_add (GTK_CONTAINER (alignment5), hbox5);

  image12 = gtk_image_new_from_stock ("gtk-quit", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image12);
  gtk_box_pack_start (GTK_BOX (hbox5), image12, FALSE, FALSE, 0);

  label18 = gtk_label_new_with_mnemonic (_("Exit"));
  gtk_widget_show (label18);
  gtk_box_pack_start (GTK_BOX (hbox5), label18, FALSE, FALSE, 0);

  button_nonfree = gtk_button_new ();
  gtk_widget_show (button_nonfree);
  gtk_fixed_put (GTK_FIXED (fixed7), button_nonfree, 560, 56);
  gtk_widget_set_size_request (button_nonfree, 176, 28);

  alignment6 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment6);
  gtk_container_add (GTK_CONTAINER (button_nonfree), alignment6);

  hbox6 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox6);
  gtk_container_add (GTK_CONTAINER (alignment6), hbox6);

  image13 = gtk_image_new_from_stock ("gtk-ok", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image13);
  gtk_box_pack_start (GTK_BOX (hbox6), image13, FALSE, FALSE, 0);

  label20 = gtk_label_new_with_mnemonic (_("Install non-free"));
  gtk_widget_show (label20);
  gtk_box_pack_start (GTK_BOX (hbox6), label20, FALSE, FALSE, 0);

  button_back = gtk_button_new ();
  gtk_widget_show (button_back);
  gtk_fixed_put (GTK_FIXED (fixed7), button_back, 560, 96);
  gtk_widget_set_size_request (button_back, 176, 28);

  alignment7 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment7);
  gtk_container_add (GTK_CONTAINER (button_back), alignment7);

  hbox7 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox7);
  gtk_container_add (GTK_CONTAINER (alignment7), hbox7);

  image14 = gtk_image_new_from_stock ("gtk-go-back", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image14);
  gtk_box_pack_start (GTK_BOX (hbox7), image14, FALSE, FALSE, 0);

  label21 = gtk_label_new_with_mnemonic (_("Back"));
  gtk_widget_show (label21);
  gtk_box_pack_start (GTK_BOX (hbox7), label21, FALSE, FALSE, 0);

  label17 = gtk_label_new (_("<b><span color=\"Red\">non-free sources</span></b>"));
  gtk_widget_show (label17);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label17);
  gtk_label_set_use_markup (GTK_LABEL (label17), TRUE);

  label_install = gtk_label_new (_("<span foreground=\"#A4A58B\" font_desc=\"Bold 14\">Install additional metapackages</span>"));
  gtk_widget_show (label_install);
  gtk_fixed_put (GTK_FIXED (fixed5), label_install, 8, 0);
  gtk_widget_set_size_request (label_install, 518, 30);
  gtk_label_set_use_markup (GTK_LABEL (label_install), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label_install), 0, 0.5);

  image8 = create_pixmap (window_main, "install-meta-title.png");
  gtk_widget_show (image8);
  gtk_fixed_put (GTK_FIXED (fixed5), image8, 568, 0);
  gtk_widget_set_size_request (image8, 160, 62);

  label22 = gtk_label_new (_("To use this feature, make sure you are connected to the internet!"));
  gtk_widget_show (label22);
  gtk_fixed_put (GTK_FIXED (fixed5), label22, 8, 30);
  gtk_widget_set_size_request (label22, 734, 30);
  gtk_label_set_use_markup (GTK_LABEL (label22), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label22), 0, 0.5);

  g_signal_connect ((gpointer) window_main, "show",
                    G_CALLBACK (on_window_main_show),
                    NULL);
  g_signal_connect ((gpointer) window_main, "delete_event",
                    G_CALLBACK (on_window_main_delete_event),
                    NULL);
  g_signal_connect ((gpointer) treeview1, "button_press_event",
                    G_CALLBACK (on_treeview1_button_press_event),
                    NULL);
  g_signal_connect ((gpointer) treeview1, "motion_notify_event",
                    G_CALLBACK (on_treeview1_motion_notify_event),
                    NULL);
  g_signal_connect ((gpointer) button_install, "clicked",
                    G_CALLBACK (on_button_install_clicked),
                    NULL);
  g_signal_connect ((gpointer) exit, "clicked",
                    G_CALLBACK (on_exit_clicked),
                    NULL);
  g_signal_connect ((gpointer) button_collapse, "clicked",
                    G_CALLBACK (on_button_collapse_clicked),
                    NULL);
  g_signal_connect ((gpointer) button_expand, "clicked",
                    G_CALLBACK (on_button_expand_clicked),
                    NULL);
  g_signal_connect ((gpointer) exit2, "clicked",
                    G_CALLBACK (on_exit2_clicked),
                    NULL);
  g_signal_connect ((gpointer) button_nonfree, "clicked",
                    G_CALLBACK (on_button_nonfree_clicked),
                    NULL);
  g_signal_connect ((gpointer) button_back, "clicked",
                    G_CALLBACK (on_button_back_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window_main, window_main, "window_main");
  GLADE_HOOKUP_OBJECT (window_main, fixed5, "fixed5");
  GLADE_HOOKUP_OBJECT (window_main, notebook1, "notebook1");
  GLADE_HOOKUP_OBJECT (window_main, fixed6, "fixed6");
  GLADE_HOOKUP_OBJECT (window_main, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (window_main, treeview1, "treeview1");
  GLADE_HOOKUP_OBJECT (window_main, button_install, "button_install");
  GLADE_HOOKUP_OBJECT (window_main, alignment4, "alignment4");
  GLADE_HOOKUP_OBJECT (window_main, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (window_main, image7, "image7");
  GLADE_HOOKUP_OBJECT (window_main, label12, "label12");
  GLADE_HOOKUP_OBJECT (window_main, checkbutton_yes, "checkbutton_yes");
  GLADE_HOOKUP_OBJECT (window_main, exit, "exit");
  GLADE_HOOKUP_OBJECT (window_main, alignment2, "alignment2");
  GLADE_HOOKUP_OBJECT (window_main, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (window_main, image5, "image5");
  GLADE_HOOKUP_OBJECT (window_main, label10, "label10");
  GLADE_HOOKUP_OBJECT (window_main, button_collapse, "button_collapse");
  GLADE_HOOKUP_OBJECT (window_main, image11, "image11");
  GLADE_HOOKUP_OBJECT (window_main, button_expand, "button_expand");
  GLADE_HOOKUP_OBJECT (window_main, image10, "image10");
  GLADE_HOOKUP_OBJECT (window_main, label16, "label16");
  GLADE_HOOKUP_OBJECT (window_main, fixed7, "fixed7");
  GLADE_HOOKUP_OBJECT (window_main, scrolledwindow6, "scrolledwindow6");
  GLADE_HOOKUP_OBJECT (window_main, viewport1, "viewport1");
  GLADE_HOOKUP_OBJECT (window_main, label19, "label19");
  GLADE_HOOKUP_OBJECT (window_main, exit2, "exit2");
  GLADE_HOOKUP_OBJECT (window_main, alignment5, "alignment5");
  GLADE_HOOKUP_OBJECT (window_main, hbox5, "hbox5");
  GLADE_HOOKUP_OBJECT (window_main, image12, "image12");
  GLADE_HOOKUP_OBJECT (window_main, label18, "label18");
  GLADE_HOOKUP_OBJECT (window_main, button_nonfree, "button_nonfree");
  GLADE_HOOKUP_OBJECT (window_main, alignment6, "alignment6");
  GLADE_HOOKUP_OBJECT (window_main, hbox6, "hbox6");
  GLADE_HOOKUP_OBJECT (window_main, image13, "image13");
  GLADE_HOOKUP_OBJECT (window_main, label20, "label20");
  GLADE_HOOKUP_OBJECT (window_main, button_back, "button_back");
  GLADE_HOOKUP_OBJECT (window_main, alignment7, "alignment7");
  GLADE_HOOKUP_OBJECT (window_main, hbox7, "hbox7");
  GLADE_HOOKUP_OBJECT (window_main, image14, "image14");
  GLADE_HOOKUP_OBJECT (window_main, label21, "label21");
  GLADE_HOOKUP_OBJECT (window_main, label17, "label17");
  GLADE_HOOKUP_OBJECT (window_main, label_install, "label_install");
  GLADE_HOOKUP_OBJECT (window_main, image8, "image8");
  GLADE_HOOKUP_OBJECT (window_main, label22, "label22");
  GLADE_HOOKUP_OBJECT_NO_REF (window_main, tooltips, "tooltips");

  return window_main;
}

