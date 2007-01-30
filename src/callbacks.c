#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define MAXLINE 1024
#define STDLINE 80

#define BEGIN_OF_PACKAGES_VAR "FLL_PACKAGES_"
#define DEFAULT_CONF_DIR "/usr/share/install-meta"
#define APT_GET_CALL "apt-get install"


FILE *temp_create_packages_list;
char INSTALL_PACKAGES_CONF_DIR[MAXLINE];  // = extern variable argv[1] in main.c
char temp_file_packagelist[STDLINE];
int  do_it_at_first_time = 0;


enum
{
  COL_SELECTED,
  COL_SHORT_TEXT,
  COL_LONG_TEXT
} ;


void search_metapackages_names()
{

    // put the search_metapackages_names in a tempfile

   int fd;
   char temp_create_packages[STDLINE];
   char system_call[STDLINE];

   //  create first tempfile for the packages search
   strncpy(temp_create_packages, "/tmp/temp_create_packages_list.XXXXXX", STDLINE);
   fd = mkstemp(temp_create_packages);  // make a tempfile
   if( fd )
      close(fd);
   else
      printf("mkstemp(temp_create_packages)\n");


   //  create second tempfile for the packages list
   strncpy(temp_file_packagelist, "/tmp/temp_packages_list.XXXXXX", STDLINE);
   fd = mkstemp(temp_file_packagelist);  // make a tempfile
   if( fd )
      close(fd);
   else
      printf("mkstemp(temp_file_packagelist)\n");


   // fill the first tempfile 
   temp_create_packages_list = fopen( temp_create_packages, "w+" );
   if( temp_create_packages_list == NULL )
      printf( "The file %s was not opened\n", temp_create_packages);
   else
   {
      fprintf( temp_create_packages_list, "%s\n\n%s\n\n%s%s%s\n\n%s%s%s\n%s%s%s\n%s%s\n", 
"#!/bin/bash",
". /etc/default/distro",
"for i in $(ls ",
INSTALL_PACKAGES_CONF_DIR,
"/*.conf); do source ${i}; done > /dev/null",
"VAR=$(set | grep ",
BEGIN_OF_PACKAGES_VAR,
" | cut -d= -f1 | cut -d_ -f3,4 | grep _ | xargs)",
"VAR=$(for i in $VAR; do echo ",
BEGIN_OF_PACKAGES_VAR,
"${i}[@];done)",
"for i in $VAR; do echo $(echo ${i} | cut -d_ -f3,4 | sed 's/\\[@\\]//'):$(echo ${!i} | sed 's/ /,/g'); done > ",
temp_file_packagelist
      );

   fclose( temp_create_packages_list );
   }


   // fill the second temp file with the name of packages
   strncpy(system_call, "sh ", STDLINE);
   strncat(system_call, temp_create_packages, STDLINE);
   system(system_call);

   /* remove the tempfile */
   unlink(temp_create_packages);
}


void toggle_selected (GtkCellRendererToggle *toggle, 
                      const gchar           *path_str,
                      gpointer data)
{
   GtkTreeModel *model;
   GtkTreePath  *path;
   GtkTreeIter   iter;
   gboolean      selected;

   model  = (GtkTreeModel *) data;
   path = gtk_tree_path_new_from_string (path_str);

   gtk_tree_model_get_iter (model, &iter, path);

   gtk_tree_model_get (model,
                       &iter,
                       COL_SELECTED, &selected,
                       -1);

   gtk_list_store_set ((GtkListStore *) model,
                        &iter,
                        COL_SELECTED, !selected,
                        -1);

   gtk_tree_path_free (path);
}


gboolean
foreach_func (GtkTreeModel *model,
              GtkTreePath  *path,
              GtkTreeIter  *iter,
              gpointer      user_data)
{
  gboolean *toggle;
  gchar *short_text, *long_text;

  /* Note: here we use 'iter' and not '&iter', because we did not allocate
   *  the iter on the stack and are already getting the pointer to a tree iter */

  gtk_tree_model_get (model, iter,
                      COL_SELECTED, &toggle,
                      COL_SHORT_TEXT, &short_text,
                      COL_LONG_TEXT, &long_text,
                      -1);

  if ( toggle )
      fprintf( temp_create_packages_list, "%s%s[@]\n", BEGIN_OF_PACKAGES_VAR, short_text);
      //g_print ("%s, %s\n", short_text, long_text);

  g_free(short_text);  /* the strings for us when retrieving them */
  g_free(long_text);

  return FALSE; /* do not stop walking the store, call us with next row */
}


void
no_file_dialog(GtkWidget *widget)
{

   // hide the main window
   GtkWidget *window1 = lookup_widget(GTK_WIDGET(widget),"window1");
   gtk_widget_hide ( GTK_WIDGET (window1) ); 

   // show the dialog window
   GtkWidget *dialog1 = create_dialog1 ();
   gtk_widget_show (dialog1);

}


gboolean
on_window1_configure_event             (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
 FILE *temp_file_package;
 char *ptr_option, *ptr_confdir, longtext[MAXLINE], *shorttext_p, *longtext_p;

 if( do_it_at_first_time < 1 ) {


   do_it_at_first_time = 1;  // only at start


  /* ================================================== *
   *                   set treeviev                     *
   * ================================================== */
   GtkWidget *treeview1;
   GtkListStore *model;
   GtkCellRenderer *toggle, *cell;
   GtkTreeViewColumn *mointpoint, *fs, *device;
   GtkTreeIter iter_tb;
   long counter = 0;

   /* treeview */
   treeview1   = lookup_widget (GTK_WIDGET (widget), "treeview1");
   model = gtk_list_store_new(3, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING);

   gtk_tree_view_set_model(GTK_TREE_VIEW(treeview1), GTK_TREE_MODEL (model));
   toggle = gtk_cell_renderer_toggle_new ();
   cell = gtk_cell_renderer_text_new();

   device     = gtk_tree_view_column_new_with_attributes("", toggle, "active", COL_SELECTED, NULL);
   fs         = gtk_tree_view_column_new_with_attributes("MetaPackage", cell, "text", 1, NULL);
   mointpoint = gtk_tree_view_column_new_with_attributes("Packages", cell, "text", 2, NULL);

   gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), GTK_TREE_VIEW_COLUMN(device));
   gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), GTK_TREE_VIEW_COLUMN(fs));
   gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), GTK_TREE_VIEW_COLUMN(mointpoint));

   // when the toggle was toggled
   g_signal_connect (toggle, "toggled",
                     G_CALLBACK (toggle_selected),
                     model);


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


   // put the search_metapackages_names in a tempfile
   search_metapackages_names();

   // read the temp_file_packagelist temp file and fill the treeview
   temp_file_package = fopen( temp_file_packagelist, "r" );
   if( temp_file_package == NULL ) {
      printf( "The file %s was not opened\n", temp_file_packagelist);
   }
   else {

     // appand to treeview
     fseek( temp_file_package, 0L, SEEK_SET );
     while (fscanf(temp_file_package, "%s", longtext) != EOF) {

             shorttext_p = strtok(longtext, ":");
             longtext_p = strtok(NULL, ":");

             //strncpy(shorttext, shorttext_p, STDLINE );
             //strncpy(longtext, longtext_p, MAXLINE );

             gtk_list_store_append ( GTK_LIST_STORE (model), &iter_tb);
             gtk_list_store_set ( GTK_LIST_STORE (model), &iter_tb,
                         COL_SHORT_TEXT, shorttext_p,
                         COL_LONG_TEXT, longtext_p,
                         -1);
             counter++;
            }
    }
    fclose(temp_file_package);

    /* remove the tempfile */
    unlink(temp_file_packagelist);

    // no metapackage description found, file empty
    if( counter < 1) 
            no_file_dialog (GTK_WIDGET (widget));

  }
 return FALSE;
}


void
on_button_install_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

   FILE* temp_file_aptgetcall_fd;
   char temp_file_aptgetcall[STDLINE];
   int fd;

   // read the treeview1 (mountpoint) list
   GtkWidget *treeview1 = lookup_widget (GTK_WIDGET (button), "treeview1");
   GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview1));
   char system_call[STDLINE];

   //  create selected package list tempfile
   strncpy(temp_file_packagelist, "/tmp/temp_packages_list.XXXXXX", STDLINE);
   fd = mkstemp(temp_file_packagelist);  // make a tempfile
   if( fd )
      close(fd);
   else
      printf("mkstemp(temp_file_packagelist)\n");


   // create apt-get systemcall tempfile
   strncpy(temp_file_aptgetcall, "/tmp/temp_aptgetcall.XXXXXX", STDLINE);
   fd = mkstemp(temp_file_aptgetcall);  // make a tempfile
   if( fd )
      close(fd);
   else
      printf("mkstemp(temp_file_aptgetcall)\n");
 

   // fill the tempfile with apt-get system call 
   temp_create_packages_list = fopen( temp_file_packagelist, "w+" );
   if( temp_create_packages_list == NULL )
      printf( "The file %s was not opened\n", temp_file_packagelist);
   else
   {
      gtk_tree_model_foreach(GTK_TREE_MODEL(model), foreach_func, NULL);  // put the selected meta packages to the tempfile
      fclose( temp_create_packages_list );
   }


   // install the packages via apt-get
   temp_file_aptgetcall_fd = fopen( temp_file_aptgetcall, "w+" );
   if( temp_file_aptgetcall_fd == NULL )
       printf( "The file %s was not opened\n", temp_file_aptgetcall);
   else {
       fprintf( temp_file_aptgetcall_fd, "%s\n\n%s\n\n%s\n\n%s\n%s%s%s\n\n%s\n\n%s%s%s%s\n\n%s\n",
"#!/bin/bash",
"apt-get update",
". /etc/default/distro",
"echo; echo ======================================",
"for i in $(ls ",
INSTALL_PACKAGES_CONF_DIR,
"/*.conf); do source ${i}; done",
"echo; echo ======================================",
APT_GET_CALL,
" $(for i in $(cat ",
temp_file_packagelist,
"); do echo ${!i}; done | xargs)",
"echo; echo Hit a key; read key"
       );

       fclose( temp_file_aptgetcall_fd );
   }

   // run apt-get
   strncpy(system_call, "x-terminal-emulator -e sh ", STDLINE);
   strncat(system_call, temp_file_aptgetcall, STDLINE);
   system(system_call);


   // remove the tempfile
   unlink(temp_file_packagelist);
   unlink(temp_file_aptgetcall);

}


void
on_exit_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
   gtk_main_quit();
}


gboolean
on_window1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

   gtk_main_quit();
   return FALSE;
}
