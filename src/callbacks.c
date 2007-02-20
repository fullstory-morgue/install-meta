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
  COL_SELECTED,
  COL_SHORT_TEXT,
  COL_LONG_TEXT
} ;


void search_metapackages_names()
{

    // put the search_metapackages_names in a tempfile

   int fd;
   char temp_create_package_list_sh[STDLINE];
   char system_call[STDLINE];

   //  create first tempfile for the packages search
   strncpy(temp_create_package_list_sh, "/tmp/temp_create_package_list_sh_fd.XXXXXX", STDLINE);
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
   temp_create_package_list_sh_fd = fopen( temp_create_package_list_sh, "w+" );
   if( temp_create_package_list_sh_fd == NULL )
      printf( "The file %s was not opened\n", temp_create_package_list_sh);
   else
   {
      // build the script who creates the packagelist
     fprintf( temp_create_package_list_sh_fd, "%s\n%s\n%s\n%s\n%s%s\n%s\n%s\n%s\n%s\n%s%s\n", 
               "#!/bin/bash",
               "set -e",
               "DPKG_ARCH=$(dpkg --print-installation-architecture)",
               "source /etc/default/distro",
               "cd ", INSTALL_PACKAGES_CONF_DIR,
               "for modul in $(ls *.bm); do",
               "   source ${modul}",
               "   echo $(echo ${modul} | cut -d. -f1):${FLL_DESCRIPTION}",
               "   unset FLL_PACKAGES FLL_PACKAGE_DEPMODS FLL_DESCRIPTION FLL_PRE_PROCESSING FLL_POST_PROCESSING",
               "done > ",temp_file_packagelist
      );

   fclose( temp_create_package_list_sh_fd );
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
  // read each line in the treeview and create the packagelist tempfile for selected metapackages
  gboolean *toggle;
  gchar *short_text, *long_text;

  /* Note: here we use 'iter' and not '&iter', because we did not allocate
   *  the iter on the stack and are already getting the pointer to a tree iter */

  gtk_tree_model_get (model, iter,
                      COL_SELECTED, &toggle,
                      COL_SHORT_TEXT, &short_text,
                      COL_LONG_TEXT, &long_text,
                      -1);
  // if the metapackage was selected
  if ( toggle )
      fprintf( temp_create_package_list_sh_fd, "%s.bm\n", 
                                                short_text);


  g_free(short_text);  /* the strings for us when retrieving them */
  g_free(long_text);

  return FALSE; /* do not stop walking the store, call us with next row */
}


void
no_file_dialog(GtkWidget *widget)
{
   //Error dialog window

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
 GtkWidget *treeview1;
 GtkListStore *model;
 GtkCellRenderer *toggle, *cell;
 GtkTreeViewColumn *mointpoint, *fs, *device;
 GtkTreeIter iter_tb;
 long counter = 0;


 if( do_it_at_first_time < 1 ) {

   do_it_at_first_time = 1;  // only at start

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
   model = gtk_list_store_new(3, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING);

   gtk_tree_view_set_model(GTK_TREE_VIEW(treeview1), GTK_TREE_MODEL (model));
   toggle = gtk_cell_renderer_toggle_new ();
   cell = gtk_cell_renderer_text_new();

   device     = gtk_tree_view_column_new_with_attributes("", toggle, "active", COL_SELECTED, NULL);
   fs         = gtk_tree_view_column_new_with_attributes("MetaPackage", cell, "text", 1, NULL);
   mointpoint = gtk_tree_view_column_new_with_attributes("Description", cell, "text", 2, NULL);

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


   // put the metapackages_names in a tempfile
   search_metapackages_names();

   // read the temp_file_packagelist temp file and fill the treeview
   temp_file_package = fopen( temp_file_packagelist, "r" );
   if( temp_file_package == NULL ) {
      printf( "The file %s was not opened\n", temp_file_packagelist);
   }
   else {

     // appand to treeview
     fseek( temp_file_package, 0L, SEEK_SET );
     while (fscanf(temp_file_package, "%[^\n]\n", longtext) != EOF) {
             shorttext_p = strtok(longtext, ":");
             longtext_p = strtok(NULL, ":");

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
on_exit_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
   char system_call[MAXLINE];

   // umount for knx-installer
   strncpy(system_call, "#!/bin/bash\n", MAXLINE );
   strncat(system_call, "mounted=$(mount |grep \"hdinstall/\" |awk '{print $1}')\n", MAXLINE );
   strncat(system_call, "if [ -n \"$mounted\" ]; then\n", MAXLINE );
   strncat(system_call, "for i in $mounted; do\n", MAXLINE );
   strncat(system_call, "    umount $i > /dev/null 2>&1 >> /dev/null\n", MAXLINE );
   strncat(system_call, "done\n", MAXLINE );
   strncat(system_call, "fi\n", MAXLINE );
   strncat(system_call, "umount /media/hdinstall > /dev/null 2>&1 >> /dev/null\n", MAXLINE );

   system(system_call);

   // install successful dialog
   if ( strlen(CHROOT) > 0 && strcmp( ptr_chroot, "--chroot" )  == 0 ) {
      // hide the main window
      GtkWidget *window1 = lookup_widget(GTK_WIDGET(button),"window1");
      gtk_widget_hide ( GTK_WIDGET (window1) ); 

      // show the dialog window
      GtkWidget *dialog2 = create_dialog2 ();
      gtk_widget_show (dialog2);
   }
   else
      gtk_main_quit();

}


void
on_button_install_pressed              (GtkButton       *button,
                                        gpointer         user_data)
{
   // hide the main window
   GtkWidget *window1 = lookup_widget(GTK_WIDGET(button),"window1");
   gtk_widget_hide ( GTK_WIDGET (window1) ); 
}


void
on_button_install_released             (GtkButton       *button,
                                        gpointer         user_data)
{


}


gboolean
on_window1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

   gtk_main_quit();
   return FALSE;
}

void
on_button_install_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
   FILE* temp_file_aptgetcall_sh_fd;
   char temp_file_aptgetcall_sh[STDLINE], is_chroot[STDLINE];
   int fd;

   // read the treeview1 (mountpoint) list
   GtkWidget *treeview1 = lookup_widget (GTK_WIDGET (button), "treeview1");
   GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview1));
   char system_call[MAXLINE];


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
        strncat(system_call, " apt-get install ${FLL_PACKAGES[@]}", MAXLINE );
   }
   else {
        // if packages should be install into normal system
        strncpy(is_chroot, "CHROOT=n", STDLINE);
        strncpy( system_call, "apt-get install ${FLL_PACKAGES[@]}", MAXLINE );
   }



   // install the packages via apt-get, create the bash file to do that
   temp_file_aptgetcall_sh_fd = fopen( temp_file_aptgetcall_sh, "w+" );
   if( temp_file_aptgetcall_sh_fd == NULL )
       printf( "The file %s was not opened\n", temp_file_aptgetcall_sh);
   else {
       //  create the bash file for install the packages
       fprintf( temp_file_aptgetcall_sh_fd, "%s\n%s\n%s\n%s\n%s%s\n%s\n%s%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s%s%s%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s%s%s%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s%s%s\n%s\n%s\n",
               "#!/bin/bash",
               is_chroot,
               "source /etc/default/distro\n[ \"$FLL_DISTRO_MODE\" = live ] && fix-unionfs",
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
   GtkWidget *window1 = lookup_widget(GTK_WIDGET(button),"window1");
   gtk_widget_show ( GTK_WIDGET (window1) );
}

