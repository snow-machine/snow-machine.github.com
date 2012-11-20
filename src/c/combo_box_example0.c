/* combo_box_example0.c --- GtkComboBox simple use
 * 
 * Filename: combo_box_example0.c
 * Description: 
 * Author: the machine of awareness
 * Maintainer: 
 * Created: 五 11月  9 22:26:55 2012 (+0800)
 * Version: 
 * Last-Updated: 五 11月  9 22:27:29 2012 (+0800)
 *           By: the machine of awareness
 *     Update #: 1
 * URL: 
 * Doc URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * It is my leanning gtk example. 
 * 
 * this is a example, to show use GtkCcomboBox. use text type.
 * 
 */

/* Change Log:
 * 
 * 
 */

/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/* Code: */


#include <gtk/gtk.h>

static void
cb_changed(GtkComboBox *combo_box, GtkLabel *label)
{
  gchar *str;

  str = gtk_combo_box_get_active_text(combo_box);
  gtk_label_set_text(label, str);

  /* g_object_unref(str); */
}

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *combo_box;
  GtkWidget *label;

  /* initialition */
  gtk_init(&argc, &argv);

  /* make main window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GtkComboBox");
  gtk_widget_set_size_request(window, 200, 100);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* make a text combo box, and add some data */
  combo_box = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo_box), "caterpillar");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo_box), "momor");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo_box), "hamimi");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo_box), "bush");
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);

  /* make vbox, and add combo box in it */
  vbox  = gtk_vbox_new(TRUE, 5);
  label = gtk_label_new_with_mnemonic("display something");
  gtk_box_pack_start(GTK_BOX(vbox), combo_box, TRUE, TRUE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  /* connect signal to tour handler function */
  g_signal_connect(combo_box, "changed", G_CALLBACK(cb_changed), GTK_LABEL(label));

  gtk_widget_show_all(window);
  gtk_main();
  
  return 0;
}

/* combo_box_example0.c ends here */
