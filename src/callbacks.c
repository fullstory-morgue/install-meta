#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <glib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include <gdk/gdk.h>
//#include <gdk/gdkkeysyms.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"


#define MAXLINE 1024
#define STDLINE 80

#define DEFAULT_CONF_DIR "/usr/share/install-meta"



FILE *temp_create_package_list_sh_fd;
char INSTALL_PACKAGES_CONF_DIR[MAXLINE];  // = extern variable argv[1] in main.c
char APT_GET_CALL[MAXLINE];
char CHROOT[MAXLINE];  // = extern variable argv[1] in main.c
char temp_file_packagelist[STDLINE];
char *target_mnt_point, *hd_device, *ptr_chroot;
int  do_it_at_first_time = 0;


enum
{
  COL_ICON,
  COL_SELECTED,
  COL_SHORT_TEXT,
  COL_LONG_TEXT
} ;


enum
{
  PACKAGE_TEXT,
  DESCRIPTION_TEXT
} ;


 void
 onRowActivated (GtkTreeView        *view,
                 GtkTreePath        *path,
                 GtkTreeViewColumn  *col,
                 gpointer            user_data)
  {
    GtkWidget *treeview2;
    GtkListStore *model2;
    GtkCellRenderer  *cell;
    GtkTreeViewColumn  *package, *description;
    GtkTreeModel *model;
    GtkTreeIter   iter, iter_parent, iter_tb;
    gchar  *package_name, *category_name;
    PangoFontDescription *font_desc;
    GdkColor color;
    FILE *temp_file_package;
    int fd;
    char temp_meta_filename[STDLINE], column[MAXLINE], *text_left, *text_right, *text_description;
    char system_call[MAXLINE];


    // the model (treeview) from the main window
    model = gtk_tree_view_get_model (view);

    if (!gtk_tree_model_get_iter(model, &iter, path))
      return; /* path describes a non-existing row - should not happen */

    // get name of doubleclicked metapackage
    gtk_tree_model_get (model, &iter,
	                    COL_SHORT_TEXT, &package_name,
	                    -1);
    // get name of category, parent line
    gboolean has_parent = gtk_tree_model_iter_parent (model, &iter_parent ,&iter);
    if (!has_parent)
      return;

    gtk_tree_model_get (model, &iter_parent,
	                    COL_SHORT_TEXT, &category_name,
	                    -1);

   ///////////////// which package was doubleclicked /////////////////
   //  create tempfile to search for the name of the clicked packages 
   strncpy(temp_meta_filename, "/tmp/temp_meta_filename.XXXXXX", STDLINE);
   fd = mkstemp(temp_meta_filename);  // make a tempfile
   if( fd )
      close(fd);
   else
      printf("error: mkstemp(temp_meta_filename_sh)\n");

   // get the real category name without <b>, <span ...
   category_name = strtok(category_name, "<");
   category_name = strtok(NULL, "<");
   category_name = strtok(category_name, ">");
   category_name = strtok(NULL, ">");


   // create systemcall
   strncpy(system_call, "#!/bin/bash", MAXLINE );
   strncat(system_call, "\nsource /etc/default/distro", MAXLINE );
   strncat(system_call, "\ncd ", MAXLINE );
   strncat(system_call, INSTALL_PACKAGES_CONF_DIR, MAXLINE );
   strncat(system_call, "\nBMFILE=", MAXLINE );
   strncat(system_call, category_name, MAXLINE );
   strncat(system_call, "-", MAXLINE );
   strncat(system_call, package_name, MAXLINE );
   strncat(system_call, ".bm", MAXLINE );
   strncat(system_call, "\necho filename~${BMFILE} > ", MAXLINE);
   strncat(system_call, temp_meta_filename, MAXLINE );
   strncat(system_call, "\nsource ${BMFILE}", MAXLINE );
   strncat(system_call, "\n   for pkgmod in ${FLL_PACKAGE_DEPMODS[@]}; do", MAXLINE );
   strncat(system_call, "\nsource packages.d/${pkgmod}.bm", MAXLINE );
   strncat(system_call, "\ndone", MAXLINE );
   strncat(system_call, "\nsource packages.d/i18n.bm", MAXLINE );
   strncat(system_call, "\nIFS=$'\\n'", MAXLINE );
   strncat(system_call, "\nfor pre_processing in ${FLL_PRE_PROCESSING[@]}; do", MAXLINE );
   strncat(system_call, "\n    echo pre~${pre_processing} >> ", MAXLINE );
   strncat(system_call,           temp_meta_filename, MAXLINE );
   strncat(system_call, "\ndone", MAXLINE );
   strncat(system_call, "\nfor package_list in ${FLL_PACKAGES[@]}; do", MAXLINE );
   strncat(system_call, "\n    echo package_list~${package_list}~$(apt-cache show ${package_list} |grep \"^Description:\" |sed 's/^Description: //') >> ", MAXLINE );
   strncat(system_call,           temp_meta_filename, MAXLINE );
   strncat(system_call, "\ndone", MAXLINE );
   strncat(system_call, "\nfor post_processing in ${FLL_POST_PROCESSING[@]}; do", MAXLINE );
   strncat(system_call, "\n    echo post~${post_processing} >> ", MAXLINE );
   strncat(system_call,           temp_meta_filename, MAXLINE );
   strncat(system_call, "\ndone", MAXLINE );
   strncat(system_call, "\nIFS=$' \\t\\n'", MAXLINE );

   system(system_call);


   ///////////////////////////////////////////////////////////////////////////
   // open the package_info window
   GtkWidget* package_info = create_package_info ();

   // label_file sets, font, color, etc.
   GtkWidget* label = lookup_widget ( GTK_WIDGET (package_info), "label_file");

   gdk_color_parse ("blue", &color);
   gtk_widget_modify_fg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &color);
   font_desc = pango_font_description_from_string ("14");
   gtk_widget_modify_font ( GTK_WIDGET(label), font_desc);
   pango_font_description_free (font_desc);


   // textview_pre_processing
   GtkWidget* textview_pre_processing = lookup_widget (GTK_WIDGET ( package_info ), "textview_pre_processing");
   GtkTextBuffer* buffer_pre = gtk_text_view_get_buffer (GTK_TEXT_VIEW ( textview_pre_processing ));


   // package list treeview
   treeview2   = lookup_widget (GTK_WIDGET ( package_info ), "treeview2");
   model2 = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

   gtk_tree_view_set_model(GTK_TREE_VIEW(treeview2), GTK_TREE_MODEL (model2));
   cell = gtk_cell_renderer_text_new();

   package     = gtk_tree_view_column_new_with_attributes("Package", cell, "text", 0, NULL);
   description = gtk_tree_view_column_new_with_attributes("Description", cell, "text", 1, NULL);

   gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), GTK_TREE_VIEW_COLUMN(package));
   gtk_tree_view_append_column(GTK_TREE_VIEW(treeview2), GTK_TREE_VIEW_COLUMN(description));


   // textview_post_processing
   GtkWidget* textview_post_processing = lookup_widget (GTK_WIDGET ( package_info ), "textview_post_processing");
   GtkTextBuffer* buffer_post = gtk_text_view_get_buffer (GTK_TEXT_VIEW ( textview_post_processing ));


    // read the created tempfile
    temp_file_package = fopen( temp_meta_filename, "r" );
    if( temp_file_package == NULL ) {
       printf( "The file %s was not opened\n", temp_meta_filename);
    }
    else {


    fseek( temp_file_package, 0L, SEEK_SET );
    while (fscanf(temp_file_package, "%[^\n]\n", column) != EOF) {
             text_left = strtok(column, "~");
             text_right = strtok(NULL, "~");

             // read the metapackage filename from tempfile
             if ( strcmp( text_left, "filename" )  == 0 )
                      // write the filename(path) from doubleclicked metapackage in the label_file
                      gtk_label_set_text( GTK_LABEL ( label ), text_right );


             // read the prescript from tempfile
             if ( strcmp( text_left, "pre" )  == 0 ) {
                      gtk_text_buffer_insert_at_cursor (buffer_pre, text_right , -1);
                      gtk_text_buffer_insert_at_cursor (buffer_pre, "\n" , -1);
                }


             // appand to treeview packagelist
             if ( strcmp( text_left, "package_list" )  == 0 ) {
                text_description = strtok(NULL, "~");

                gtk_list_store_append ( GTK_LIST_STORE (model2), &iter_tb);
                gtk_list_store_set ( GTK_LIST_STORE (model2), &iter_tb,
                         PACKAGE_TEXT, text_right,
                         DESCRIPTION_TEXT, text_description,
                         -1);
                }


             // read the prescript from tempfile
             if ( strcmp( text_left, "post" )  == 0 ) {
                      gtk_text_buffer_insert_at_cursor (buffer_post, text_right , -1);
                      gtk_text_buffer_insert_at_cursor (buffer_post, "\n" , -1);
                }
 
         }
    }
    fclose(temp_file_package);

    /* remove the tempfile */
    unlink( temp_meta_filename );


    // label sets
    label = lookup_widget ( GTK_WIDGET (package_info), "label_pre");
    gdk_color_parse ("darkred", &color);
    gtk_widget_modify_fg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &color);
    font_desc = pango_font_description_from_string ("11");
    gtk_widget_modify_font ( GTK_WIDGET(label), font_desc);
    pango_font_description_free (font_desc);

    // label sets
    label = lookup_widget ( GTK_WIDGET (package_info), "label_list");
    gdk_color_parse ("darkred", &color);
    gtk_widget_modify_fg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &color);
    font_desc = pango_font_description_from_string ("11");
    gtk_widget_modify_font ( GTK_WIDGET(label), font_desc);
    pango_font_description_free (font_desc);

    // label sets
    label = lookup_widget ( GTK_WIDGET (package_info), "label_post");
    gdk_color_parse ("darkred", &color);
    gtk_widget_modify_fg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &color);
    font_desc = pango_font_description_from_string ("11");
    gtk_widget_modify_font ( GTK_WIDGET(label), font_desc);
    pango_font_description_free (font_desc);


    // show the info window
    gtk_widget_show (package_info);

    //gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
    //g_free(tree_path_str);
  }



void search_metapackages_names()
{

   char which_packages[80];

    // put the search_metapackages_names in a tempfile
   FILE *temp_create_package_list_at_start_fd;
   int fd;
   char temp_create_package_list_sh[STDLINE];
   char system_call[STDLINE];

   //  create first tempfile for the packages search
   strncpy(temp_create_package_list_sh, "/tmp/temp_create_package_list_at_start_fd.XXXXXX", STDLINE);
   fd = mkstemp(temp_create_package_list_sh);  // make a tempfile
   if( fd )
      close(fd);
   else
      printf("mkstemp(temp_create_package_list_sh)\n");


   //  create second tempfile for the packages list
   strncpy(temp_file_packagelist, "/tmp/temp_packages_list.XXXXXX", STDLINE);
   fd = mkstemp(temp_file_packagelist);  // make a tempfile
   if( fd )
      close(fd);
   else
      printf("mkstemp(temp_file_packagelist)\n");


   // fill the first tempfile 
   temp_create_package_list_at_start_fd = fopen( temp_create_package_list_sh, "w+" );
   if( temp_create_package_list_at_start_fd == NULL )
      printf( "The file %s was not opened\n", temp_create_package_list_sh);
   else
   {
      // show only firmware if install-gui starts install-meta
      if ( strlen(CHROOT) > 0 && strcmp( ptr_chroot, "--chroot" )  == 0 ) {
           strncpy(which_packages, "list=\"\" || list=\"$(ls Non_free-Firmware*.bm)\"", 80);
      }
      else {
           strncpy(which_packages, "list=\"$(ls *.bm|grep -v ^Non_free)\" || list=\"$(ls *.bm)\"", 80);
      }

      // build the script who creates the packagelist
      fprintf( temp_create_package_list_at_start_fd, "%s\n%s\n%s\n%s\n%s%s\n%s%s\n%s\n%s\n%s\n%s\n%s%s\n", 
               "#!/bin/bash",
               "set -e",
               "DPKG_ARCH=$(dpkg --print-installation-architecture)",
               "source /etc/default/distro",
               "cd ", INSTALL_PACKAGES_CONF_DIR,
               "[ -z \"$(grep deb\\ .*debian\\.org.*main.*contrib /etc/apt/sources.list)\" ] && ",
               which_packages,
               "for modul in ${list}; do",
               "   source ${modul}",
               "   echo $(echo ${modul} | cut -d. -f1)~${FLL_DESCRIPTION}",
               "   unset FLL_PACKAGES FLL_PACKAGE_DEPMODS FLL_DESCRIPTION FLL_PRE_PROCESSING FLL_POST_PROCESSING",
               "done > ",temp_file_packagelist
      );

   fclose( temp_create_package_list_at_start_fd );
   }


   // fill the second temp file with the name of packages (start the script above)
   strncpy(system_call, "sh ", STDLINE);
   strncat(system_call, temp_create_package_list_sh, STDLINE);
   system(system_call);

   /* remove the tempfile */
   unlink(temp_create_package_list_sh);


}


void toggle_selected (GtkCellRendererToggle *toggle, 
                      const gchar           *path_str,
                      gpointer data)
{
   // store the toggle selection if it is selected
   GtkTreeModel *model;
   GtkTreePath  *path;
   GtkTreeIter   iter, child_iter;
   gboolean      selected;

   model  = (GtkTreeModel *) data;
   path = gtk_tree_path_new_from_string (path_str);

   gtk_tree_model_get_iter (model, &iter, path);

   gtk_tree_model_get (model,
                       &iter,
                       COL_SELECTED, &selected,
                       -1);

   gtk_tree_store_set ((GtkTreeStore *) model,
                        &iter,
                        COL_SELECTED, !selected,
                        -1);

   if ( gtk_tree_model_iter_has_child ( model, &iter ) ) {
        gtk_tree_model_iter_children (model, &child_iter, &iter);

             do {
              gtk_tree_store_set ((GtkTreeStore *) model,
                        &child_iter,
                        COL_SELECTED, !selected,
                        -1);
             } while(gtk_tree_model_iter_next(model, &child_iter));
      }

   gtk_tree_path_free (path);
}


gboolean
foreach_func (GtkTreeModel *model,
              GtkTreePath  *path,
              GtkTreeIter  *iter,
              gpointer      user_data)
{
  GtkTreeIter iter_parent;
  //gboolean *toggle;
  gint toggle;
  gchar *short_text, *long_text, *category_name;
  // read each line in the treeview and create the packagelist tempfile for selected metapackages


  /* Note: here we use 'iter' and not '&iter', because we did not allocate
   *  the iter on the stack and are already getting the pointer to a tree iter */

  gtk_tree_model_get (model, iter,
                      COL_SELECTED, &toggle,
                      COL_SHORT_TEXT, &short_text,
                      COL_LONG_TEXT, &long_text,
                      -1);

    // get name of category, parent line
    gboolean has_parent = gtk_tree_model_iter_parent (model, &iter_parent ,iter);
    if (!has_parent)
      return FALSE;

    gtk_tree_model_get (model, &iter_parent,
	                    COL_SHORT_TEXT, &category_name,
	                    -1);

   // get the real category name without <b>, <span ...
   category_name = strtok(category_name, "<");
   category_name = strtok(NULL, "<");
   category_name = strtok(category_name, ">");
   category_name = strtok(NULL, ">");

   printf( "%s-%s.bm:%i\n", category_name, short_text, toggle);

  // if the metapackage was selected
  if ( toggle == 1 ) {
      fprintf( temp_create_package_list_sh_fd, "%s-%s.bm\n", category_name, short_text);
  }


  g_free(short_text);  /* the strings for us when retrieving them */
  g_free(long_text);

  return FALSE; /* do not stop walking the store, call us with next row */
}


void
no_file_dialog(GtkWidget *widget)
{
   //Error dialog window

   // hide the main window
   GtkWidget *window_main = lookup_widget(GTK_WIDGET(widget),"window_main");
   gtk_widget_hide ( GTK_WIDGET (window_main) ); 

   // show the dialog window
   GtkWidget *dialog1 = create_dialog1 ();
   gtk_widget_show (dialog1);

}


static void
tree_expand(GtkTreeView * treeview1,  GtkTreeIter   *iter, 
                GtkTreePath * path, gpointer data)
{

   GdkPixbuf     *icon;
   GError        *error = NULL;

   GtkTreeModel *model = gtk_tree_view_get_model(treeview1);

   gtk_tree_model_get(model, iter, COL_ICON, &icon, -1);

   icon = gdk_pixbuf_new_from_file("/usr/share/install-meta/pixmaps/install-meta-folder-open.png", &error);
   if (error)
   {
      g_warning ("Could not load icon: %s\n", error->message);
      g_error_free(error);
      error = NULL;
   }

   gtk_tree_store_set(GTK_TREE_STORE(model), iter,
                       COL_ICON, icon, 
                       -1);

}


static void
tree_collapse(GtkTreeView * treeview1, GtkTreeIter * iter,
                    GtkTreePath * path, gpointer data)
{
   GdkPixbuf     *icon;
   GError        *error = NULL;

   GtkTreeModel *model = gtk_tree_view_get_model(treeview1);

   gtk_tree_model_get(model, iter, COL_ICON, &icon, -1);

   icon = gdk_pixbuf_new_from_file("/usr/share/install-meta/pixmaps/install-meta-folder.png", &error);
   if (error)
   {
      g_warning ("Could not load icon: %s\n", error->message);
      g_error_free(error);
      error = NULL;
   }

   gtk_tree_store_set(GTK_TREE_STORE(model), iter,
                       COL_ICON, icon, 
                       -1);

}


void
if_exit (GtkWidget *widget)
{
   char system_call[MAXLINE];


   // install successful dialog
   if ( strlen(CHROOT) > 0 && strcmp( ptr_chroot, "--chroot" )  == 0 ) {

      // umount for fll-installer
      strncpy(system_call, "#!/bin/bash\n", MAXLINE );
      strncat(system_call, "mounted=$(mount |grep \"hdinstall/\" |awk '{print $1}')\n", MAXLINE );
      strncat(system_call, "if [ -n \"$mounted\" ]; then\n", MAXLINE );
      strncat(system_call, "for i in $mounted; do\n", MAXLINE );
      strncat(system_call, "    umount $i > /dev/null 2>&1 >> /dev/null\n", MAXLINE );
      strncat(system_call, "done\n", MAXLINE );
      strncat(system_call, "fi\n", MAXLINE );
      strncat(system_call, "umount /media/hdinstall > /dev/null 2>&1 >> /dev/null\n", MAXLINE );

      system(system_call);

   }

   gtk_main_quit();
}


void
on_exit_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
   if_exit (GTK_WIDGET (button));
}


void
on_button_install_pressed              (GtkButton       *button,
                                        gpointer         user_data)
{
   // hide the main window
   GtkWidget *window_main = lookup_widget(GTK_WIDGET(button),"window_main");
   gtk_widget_hide ( GTK_WIDGET (window_main) ); 
}


void
on_button_install_released             (GtkButton       *button,
                                        gpointer         user_data)
{


}


void
on_button_install_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
   FILE* temp_file_aptgetcall_sh_fd;
   char temp_file_aptgetcall_sh[STDLINE], is_chroot[STDLINE], system_call[MAXLINE];
   int fd;


   // hide the main window after gparted has done
   GtkWidget *window_main = lookup_widget(GTK_WIDGET(button),"window_main");
   gtk_widget_hide ( GTK_WIDGET (window_main) );
   while (gtk_events_pending ())
          gtk_main_iteration ();


   // read the treeview1 (mountpoint) list
   GtkWidget *treeview1 = lookup_widget (GTK_WIDGET (button), "treeview1");
   GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview1));


   //  create selected package list tempfile
   strncpy(temp_file_packagelist, "/tmp/temp_packages_list.XXXXXX", STDLINE);
   fd = mkstemp(temp_file_packagelist);  // make a tempfile
   if( fd )
      close(fd);
   else
      printf("mkstemp(temp_file_packagelist)\n");


   // create apt-get systemcall tempfile
   strncpy(temp_file_aptgetcall_sh, "/tmp/temp_aptgetcall.XXXXXX", STDLINE);
   fd = mkstemp(temp_file_aptgetcall_sh);  // make a tempfile
   if( fd )
      close(fd);
   else
      printf("mkstemp(temp_file_aptgetcall_sh)\n");
 

   // fill the tempfile with apt-get system call 
   temp_create_package_list_sh_fd = fopen( temp_file_packagelist, "w+" );
   if( temp_create_package_list_sh_fd == NULL )
      printf( "The file %s was not opened\n", temp_file_packagelist);
   else
   {
      gtk_tree_model_foreach(GTK_TREE_MODEL(model), foreach_func, NULL);  // put the selected meta packages to the tempfile
      fclose( temp_create_package_list_sh_fd );
   }

   // if option --chroot=<device>  is given (life mode install to hd)
   if ( strlen(CHROOT) > 0 && strcmp( ptr_chroot, "--chroot" )  == 0 ) {
        strncpy(is_chroot, "CHROOT=y", STDLINE);

        strncpy(system_call, "chroot /media/", MAXLINE );
        strncat(system_call, hd_device, MAXLINE );
        strncat(system_call, " apt-get update\n", MAXLINE );
        strncat(system_call, "chroot /media/", MAXLINE );
        strncat(system_call, hd_device, MAXLINE );

        strncat(system_call, " apt-get install ", MAXLINE );

   }
   else {
        // if packages should be install into normal system
        strncpy(is_chroot, "CHROOT=n", STDLINE);

        strncpy(system_call, "apt-get install ", MAXLINE );
   }

   GtkWidget* checkbutton = lookup_widget( GTK_WIDGET(button),"checkbutton_yes");
   if( gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON( checkbutton )) == TRUE ) {
       strncat(system_call,  "--yes ", MAXLINE );
   }
   strncat(system_call, "${FLL_PACKAGES[@]}", MAXLINE );


   // install the packages via apt-get, create the bash file to do that
   temp_file_aptgetcall_sh_fd = fopen( temp_file_aptgetcall_sh, "w+" );
   if( temp_file_aptgetcall_sh_fd == NULL )
       printf( "The file %s was not opened\n", temp_file_aptgetcall_sh);
   else {
       //  create the bash file for install the packages
       fprintf( temp_file_aptgetcall_sh_fd, "%s\n%s\n%s\n%s%s\n%s\n%s%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s%s%s%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s%s%s%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s%s%s\n%s\n%s\n",
               "#!/bin/bash",
               is_chroot,
               "apt-get update",
               "cd ", INSTALL_PACKAGES_CONF_DIR,
               "echo; echo ======================================",
               "for modul in $(cat ",temp_file_packagelist,"); do ",
               "   source ${modul}",
               "   for pkgmod in ${FLL_PACKAGE_DEPMODS[@]}; do",
               "	source packages.d/${pkgmod}.bm",
               "   done",
               "   source packages.d/i18n.bm",
               "   echo; echo ======================================",
               "   echo start installation for ${modul}",
               "   echo --------------------------------------",

               "   TMP=\"$(mktemp -p /tmp/ install-meta-preprocessing-XXXXXXXXXX)\"",
               "   IFS=$'\\n'",
               "   for pre_processing in ${FLL_PRE_PROCESSING[@]}; do",
               "        echo ${pre_processing} >> \"$TMP\"",
               "   done",
               "   IFS=$' \\t\\n'",
               "   [ \"${CHROOT}\" = y ] && cp $TMP /media/", hd_device, "/tmp && chroot /media/", hd_device, " sh ${TMP} || sh ${TMP} ",
               "   rm \"$TMP\"",

                   system_call,
               "   unset FLL_PACKAGES FLL_PACKAGE_DEPMODS FLL_DESCRIPTION FLL_PRE_PROCESSING",

               "   TMP=\"$(mktemp -p /tmp/ install-meta-postprocessing-XXXXXXXXXX)\"",
               "   IFS=$'\\n'",
               "   for post_processing in ${FLL_POST_PROCESSING[@]}; do",
               "        echo ${post_processing} >> \"$TMP\"",
               "   done",
               "   [ \"${CHROOT}\" = y ] && cp $TMP /media/", hd_device, "/tmp && chroot /media/", hd_device, " sh ${TMP} || sh ${TMP} ",
               "   unset FLL_POST_PROCESSING",
               "   rm \"$TMP\"",
 
               "done",
               "IFS=$' \\t\\n'",
               "i=_",
               "while [ \"${i}\" != \"\" ]; do",
               "      echo -e \"\\n==================================\"",
               "      echo \"enter your code or close the window <ALT> + <F4>\"",
               "      echo -n \"# \";read i",
               "      [ \"${CHROOT}\" = y ] && chroot /media/", hd_device, " ${i} || ${i}",
               "done",
               "exit"
       );

       fclose( temp_file_aptgetcall_sh_fd );
   }


   // run apt-get, (start the script above)
   strncpy(system_call, "x-terminal-emulator -e sh ", MAXLINE);
   strncat(system_call, temp_file_aptgetcall_sh, MAXLINE);
   // start the install via x-terminal-emulator
   system(system_call);


   // remove the tempfile
   unlink(temp_file_packagelist);
   unlink(temp_file_aptgetcall_sh);

   // show the main window
   gtk_widget_show ( GTK_WIDGET (window_main) );
}


void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
     GtkWidget *window = lookup_widget(GTK_WIDGET( button ),"package_info");
     gtk_widget_destroy ( GTK_WIDGET (window) );
}


gboolean
on_treeview1_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
   GtkWidget *view = lookup_widget (GTK_WIDGET (widget), "treeview1");
   GtkTreeModel *model1;
   GtkTreePath *path;
   GtkTreeViewColumn *column;
   GtkTreeIter iter;
   gint cx, cy;

   gtk_tree_view_get_path_at_pos( GTK_TREE_VIEW( view ), event->x, event->y, &path, &column, &cx, &cy);
   if (path == NULL)
       return FALSE;

   model1 = gtk_tree_view_get_model ( GTK_TREE_VIEW( view ) );

   gtk_tree_model_get_iter(model1, &iter, path);

   const gchar* title = gtk_tree_view_column_get_title ( column );


   if( strncmp( title, "Info", 4) == 0 && cx > 33 )   // position x from the i icon in Info column
        onRowActivated ( GTK_TREE_VIEW( view ), path, NULL, user_data );

   gtk_tree_path_free (path);


   //if (event->type == GDK_2BUTTON_PRESS)
   //    toggle_sym_value(menu);


   return FALSE;
}


void
on_button_expand_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
   GtkWidget *view = lookup_widget (GTK_WIDGET (button), "treeview1");
   gtk_tree_view_expand_all ( GTK_TREE_VIEW( view ) );
}


void
on_button_collapse_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
   GtkWidget *view = lookup_widget (GTK_WIDGET (button), "treeview1");
   gtk_tree_view_collapse_all ( GTK_TREE_VIEW( view ) );
}


gboolean
on_treeview1_motion_notify_event       (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
   GtkWidget *view = lookup_widget (GTK_WIDGET (widget), "treeview1");
   GtkTreePath *path;
   GtkTreeIter   iter, iter_parent;
   GtkTreeModel *model;
   GtkTreeViewColumn *column;
   GdkCursor *cursor;
   gint cx, cy;


   cursor = gdk_cursor_new_for_display (gdk_display_get_default(), 132);
   gdk_window_set_cursor (widget->window, cursor);


   gtk_tree_view_get_path_at_pos( GTK_TREE_VIEW( view ), event->x, event->y, &path, &column, &cx, &cy);
   if (path == NULL)
      return FALSE;

    // the model (treeview) from the main window
    model = gtk_tree_view_get_model (GTK_TREE_VIEW( view ));

    if (!gtk_tree_model_get_iter(model, &iter, path))
      return FALSE; /* path describes a non-existing row - should not happen */

    // get parent line
    gboolean has_parent = gtk_tree_model_iter_parent (model, &iter_parent ,&iter);
    if (!has_parent)
       return FALSE;


   const gchar* title = gtk_tree_view_column_get_title ( column );


   if( strncmp( title, "Info", 4) == 0 && cx > 33 )  { // position x from the i icon in Info column
       cursor = gdk_cursor_new_for_display (gdk_display_get_default(), 60);
       gdk_window_set_cursor (widget->window, cursor);
   }

   gtk_tree_path_free (path);

   return FALSE;
}


void
append_data_to_tree_store (GtkWidget *widget)

{
   // fill the treestore new
   FILE *temp_file_package;
   char category[MAXLINE], longtext[MAXLINE], category_last[STDLINE], *shorttext_p, *longtext_p;
   GtkTreeIter iter_tb, iter_category;
   GdkPixbuf     *icon, *icon_package;
   GError        *error = NULL;
   long counter = 0;


   // get treestore and model
   GtkWidget* treeview1   = lookup_widget (GTK_WIDGET ( widget ), "treeview1");
   GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview1));


   icon = gdk_pixbuf_new_from_file("/usr/share/install-meta/pixmaps/install-meta-info.png", &error);
   if (error)
   {
      g_warning ("Could not load icon: %s\n", error->message);
      g_error_free(error);
      error = NULL;
   }

   icon_package = gdk_pixbuf_new_from_file("/usr/share/install-meta/pixmaps/install-meta-folder.png", &error);
   if (error)
   {
      g_warning ("Could not load icon: %s\n", error->message);
      g_error_free(error);
      error = NULL;
   }


   // put the metapackages_names in a tempfile
   search_metapackages_names();

   // read the temp_file_packagelist temp file and fill the treeview
   temp_file_package = fopen( temp_file_packagelist, "r" );
   if( temp_file_package == NULL ) {
      printf( "The file %s was not opened\n", temp_file_packagelist);
   }
   else {
     strncpy( category_last, "", STDLINE);
     // appand to treeview
     fseek( temp_file_package, 0L, SEEK_SET );
     while (fscanf(temp_file_package, "%[^\n]\n", longtext) != EOF) {

             shorttext_p = strtok(longtext, "~");
             longtext_p = strtok(NULL, "~");

             //look for category = - in filename
             if ( strpbrk( shorttext_p, "-" ) == NULL )
                  printf("category \\(- in filename\\) not found\n");
             else {

                  strncpy(category, "<b><span color=\"darkgreen\">" , MAXLINE);
                  strncat(category, strtok(shorttext_p, "-") , MAXLINE);
                  strncat(category, "</span></b>" , MAXLINE);
                  shorttext_p = strtok(NULL, "-");
             }

             if ( strcmp( category_last, category ) != 0 ) {
                //category
                gtk_tree_store_append(GTK_TREE_STORE (model), &iter_category, NULL);
                gtk_tree_store_set(GTK_TREE_STORE (model), &iter_category, 
                         COL_ICON, icon_package, 
                         COL_SHORT_TEXT, category, 
                         -1);
             }

             //data 
             gtk_tree_store_append ( GTK_TREE_STORE (model), &iter_tb, &iter_category);
             gtk_tree_store_set ( GTK_TREE_STORE (model), &iter_tb,
                         COL_ICON, icon,
                         COL_SELECTED, FALSE,
                         COL_SHORT_TEXT, shorttext_p,
                         COL_LONG_TEXT, longtext_p,
                         -1);

             strncpy ( category_last, category, STDLINE );

             counter++;
            }
    }
    fclose(temp_file_package);

    /* remove the tempfile */
    unlink(temp_file_packagelist);

}


void
on_window_main_show                    (GtkWidget       *widget,
                                        gpointer         user_data)
{

 if( do_it_at_first_time < 1 ) {

   do_it_at_first_time = 1;  // only at start

   char *ptr_option, *ptr_confdir;
   GtkWidget *treeview1;
   GtkTreeStore *model;
   GtkCellRenderer *toggle, *pixrenderer, *cell, *cell2;
   GtkTreeViewColumn *mointpoint, *fs, *pixm, *device;

   // handle the system call options
   ptr_chroot = strtok(CHROOT, "=");
   target_mnt_point = strtok(NULL, "=");
   hd_device = strtok(target_mnt_point, "/");
   hd_device = strtok(NULL, "/");
   if ( hd_device == NULL ) {
        hd_device = "_";
   }

  /* ================================================== *
   *                   set treeviev                     *
   * ================================================== */

   /* treeview */
   treeview1   = lookup_widget (GTK_WIDGET (widget), "treeview1");


   // create new model
   model = gtk_tree_store_new(4, GDK_TYPE_PIXBUF, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING);

   gtk_tree_view_set_model(GTK_TREE_VIEW(treeview1), GTK_TREE_MODEL (model));
   toggle = gtk_cell_renderer_toggle_new ();
   pixrenderer = gtk_cell_renderer_pixbuf_new ();
   cell = gtk_cell_renderer_text_new();
   cell2 = gtk_cell_renderer_text_new();


   //g_object_set (pixrenderer, "clickable", TRUE, NULL);
   pixm       = gtk_tree_view_column_new_with_attributes("Info", pixrenderer, "pixbuf", COL_ICON, NULL);
                //gtk_tree_view_column_set_clickable ( GTK_TREE_VIEW_COLUMN (pixm), TRUE);
   device     = gtk_tree_view_column_new_with_attributes("", toggle, "active", COL_SELECTED, NULL);
   fs         = gtk_tree_view_column_new_with_attributes("MetaPackage", cell, "markup", COL_SHORT_TEXT, NULL);
   mointpoint = gtk_tree_view_column_new_with_attributes("Description", cell2, "text", COL_LONG_TEXT, NULL);


   gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), GTK_TREE_VIEW_COLUMN(pixm));
   gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), GTK_TREE_VIEW_COLUMN(device));
   gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), GTK_TREE_VIEW_COLUMN(fs));
   gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), GTK_TREE_VIEW_COLUMN(mointpoint));

   // when the toggle was toggled
   g_signal_connect (toggle, "toggled",
                     G_CALLBACK (toggle_selected),
                     model);

   // when tree was expanden
   g_signal_connect_after(G_OBJECT(treeview1), "row-expanded",
                           G_CALLBACK(tree_expand), NULL);

   // when tree was collapsed
   g_signal_connect(G_OBJECT(treeview1), "row-collapsed",
                     G_CALLBACK(tree_collapse), NULL);


  /* ================================================================= *
   *         append rows and fill in some data  to the treeview        *
   * ================================================================= */

   // confdir handling
   ptr_option = strtok(INSTALL_PACKAGES_CONF_DIR, "=");
   if ( strlen(INSTALL_PACKAGES_CONF_DIR) > 0 && strcmp( ptr_option, "--dir" )  == 0 ) {
        ptr_confdir = strtok(NULL, "=");
        strncpy( INSTALL_PACKAGES_CONF_DIR, ptr_confdir, MAXLINE );
   }
   else {
        strncpy( INSTALL_PACKAGES_CONF_DIR, DEFAULT_CONF_DIR, MAXLINE );
   }

   // are conf files available?
   if (chdir(INSTALL_PACKAGES_CONF_DIR) < 0) {
        no_file_dialog(GTK_WIDGET (widget));
   }



   // append to treestore
   append_data_to_tree_store  (GTK_WIDGET (widget));

   // switch to tab non-free if install-gui starts install-meta
   if ( strlen(CHROOT) > 0 && strcmp( ptr_chroot, "--chroot" )  == 0 ) {
       GtkWidget *notebook1 = lookup_widget (GTK_WIDGET (widget), "notebook1");
       gtk_notebook_next_page ( GTK_NOTEBOOK(notebook1) );
       //gtk_notebook_set_current_page( GTK_NOTEBOOK(notebook1) 2 );
   }


 }
}


gboolean
on_window_main_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

   if_exit (GTK_WIDGET (widget));

   return FALSE;
}


void
on_exit2_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
   if_exit (GTK_WIDGET (button));
}


void
on_button_nonfree_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
   char system_call[MAXLINE];


   system("[ -z \"$(grep deb\\ .*debian\\.org.*main.*contrib /etc/apt/sources.list)\" ] && \
   sed -ie 's/deb \\(.*\\)debian.org\\/debian unstable \\(.*\\)/deb \\1debian.org\\/debian unstable main contrib non-free/' /etc/apt/sources.list || \
   echo found deb *contrib in sources.list for debian");

   system("[ -z \"$(grep deb\\ .*debian\\.org.*main.*contrib /etc/apt/sources.list)\" ] && \
   sed -ie 's/deb \\(.*\\)debian.org\\/debian\\/ sid \\(.*\\)/deb \\1debian.org\\/debian\\/ sid main contrib non-free/' /etc/apt/sources.list || \
   echo found deb *contrib in sources.list for debian");

   system("[ -z \"$(grep deb-src\\ .*debian\\.org.*main.*contrib /etc/apt/sources.list)\" ] && \
   sed -ie 's/deb-src \\(.*\\)debian.org\\/debian unstable \\(.*\\)/deb-src \\1debian.org\\/debian unstable main contrib non-free/' /etc/apt/sources.list || \
   echo found deb-src *contrib in sources.list for debian");

   system("[ -z \"$(grep deb-src\\ .*debian\\.org.*main.*contrib /etc/apt/sources.list)\" ] && \
   sed -ie 's/deb-src \\(.*\\)debian.org\\/debian\\/ sid \\(.*\\)/deb-src \\1debian.org\\/debian\\/ sid main contrib non-free/' /etc/apt/sources.list || \
   echo found deb-src *contrib in sources.list for debian");

   system("[ -z \"$(grep deb\\ .*sidux\\.com.*main.*contrib /etc/apt/sources.list)\" ] && \
   sed -ie 's/deb \\(.*\\)sidux.com\\/debian\\/ sid \\(.*\\)/deb \\1sidux.com\\/debian\\/ sid main contrib non-free firmware fix.main fix.contrib fix.non-free/' /etc/apt/sources.list || \
   echo found deb *contrib in sources.list for sidux");

   system("[ -z \"$(grep deb-src\\ .*sidux\\.com.*main.*contrib /etc/apt/sources.list)\" ] && \
   sed -ie 's/deb-src \\(.*\\)sidux.com\\/debian\\/ sid \\(.*\\)/deb-src \\1sidux.com\\/debian\\/ sid main contrib non-free firmware fix.main fix.contrib fix.non-free/' /etc/apt/sources.list || \
   echo found deb-src *contrib in sources.list for sidux");


   //copy sources.list if in install mode
   if ( strlen(CHROOT) > 0 && strcmp( ptr_chroot, "--chroot" )  == 0 ) {

      // umount for knx-installer
      strncpy(system_call, "[ -f /media/", MAXLINE );
      strncat(system_call, hd_device, MAXLINE );
      strncat(system_call, "/etc/apt/sources.list.dfsg ] || cp /media/", MAXLINE );
      strncat(system_call, hd_device, MAXLINE );
      strncat(system_call, "/etc/apt/sources.list /media/", MAXLINE );
      strncat(system_call, hd_device, MAXLINE );
      strncat(system_call, "/etc/apt/sources.list.dfsg\ncp /etc/apt/sources.list /media/", MAXLINE );
      strncat(system_call, hd_device, MAXLINE );
      strncat(system_call, "/etc/apt/sources.list", MAXLINE );

      system(system_call);
   }


   // get treestore and model
   GtkWidget* treeview1   = lookup_widget (GTK_WIDGET (button), "treeview1");
   GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview1));


   //make treeview empty
   gtk_tree_store_clear(GTK_TREE_STORE(model));


   // append new data to treestore
   append_data_to_tree_store  (GTK_WIDGET (button));

   GtkWidget *notebook1 = lookup_widget (GTK_WIDGET (button), "notebook1");
   gtk_notebook_prev_page ( GTK_NOTEBOOK(notebook1) );


}


void
on_button_back_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
   GtkWidget *notebook1;

   notebook1 = lookup_widget (GTK_WIDGET (button), "notebook1");
   gtk_notebook_prev_page ( GTK_NOTEBOOK(notebook1) );
}

