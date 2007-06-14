/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "interface.h"
#include "support.h"



extern char INSTALL_PACKAGES_CONF_DIR[1024];
extern char CHROOT[1024];

int
main (int argc, char *argv[])
{
  int r;
  GtkWidget *window_main;
  //char option[6];

#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

  //programm option handling
  for (r=1; r<argc; r++)
  {

       // directory from the *.bm metapackage files
       if ( strncmp( argv[r], "--dir=/", 7 )  == 0 )
            strncpy(INSTALL_PACKAGES_CONF_DIR, argv[r], 1024);

       // install the packages to chroot (for sidux-installer start)
       if ( strncmp( argv[r], "--chroot=/", 10 )  == 0 )
            strncpy( CHROOT, argv[r], 1024);
  }


  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  window_main = create_window_main ();
  gtk_widget_show (window_main);

  gtk_main ();
  return 0;
}

