/* combo_box_example1.c --- GtkComboBox use MVC example
 * 
 * Filename: combo_box_example1.c
 * Description: 
 * Author: the machine of awareness<machine.of.awareness@gmail.com>
 * Maintainer: 
 * Created: 五 11月  9 20:49:33 2012 (+0800)
 * Version: 
 * Last-Updated: 五 11月  9 20:50:05 2012 (+0800)
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
 * this is a example, to show use MVC by GtkCcomboBox.  the GtkComboBox is
 * control, the GtkListStore is model, and the GtkCellRenderer is display. use a
 * cell text in it.
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
cb_changed(GtkComboBox *combo_box, gpointer data)
{
  gchar *str;
  GtkTreeIter iter;
  GtkTreeModel *model;

  str = NULL;

  /* obtain currently selected from combo box, if nothing is selected, do
   * nothing.*/
  if (gtk_combo_box_get_active_iter(combo_box, &iter))
  {
    /* obtain data model from combo box */
    model = gtk_combo_box_get_model(combo_box);
    /* obtain string from model */
    gtk_tree_model_get(model, &iter, 0, &str, -1);
  }
  g_print("selected combox box >> %s <<\n", str);
  if (str)
  {
    g_free(str);
  }
}

static void
cb_delete(GtkWidget *widget, GtkComboBox *combo_box)
{
  GtkTreeIter iter;
  GtkListStore *store;

  /* obtain currently data selected from combo box, if nothing is selected, do
   * nothing. */
  if (gtk_combo_box_get_active_iter(combo_box, &iter))
  {
    /* obtain model form combo box */
    store = GTK_LIST_STORE(gtk_combo_box_get_model(combo_box));
    /* remove item */
    gtk_list_store_remove(store, &iter);
  }
}

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *combo_box;
  GtkListStore *store;
  GtkTreeIter iter;
  GtkCellRenderer *cell;
  GtkWidget *vbox, *frame, *button;

  /* initialition */
  gtk_init(&argc, &argv);
  
  /* create main window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "the machine of awareness");
  gtk_widget_set_size_request(window, 320, 240);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* create data store, add fill some information */
  store = gtk_list_store_new(1, G_TYPE_STRING);
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, "hello world first", -1);
  gtk_list_store_prepend(store, &iter);
  gtk_list_store_set(store, &iter, 0, "hello world twice", -1);
  gtk_list_store_prepend(store, &iter);
  gtk_list_store_set(store, &iter, 0, "hello world third", -1);
  
  /* create vbox */
  vbox  = gtk_vbox_new(FALSE, 6);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  /* create frame */
  frame = gtk_frame_new("text api");
  gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);
  /* create combo box using text api and add some data to it*/
  combo_box = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
  gtk_container_add(GTK_CONTAINER(frame), combo_box);
  g_object_unref(store);

  /* create renderer to data source */
  cell = gtk_cell_renderer_text_new();
  /* pack it to the combo box */
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_box), cell, TRUE);
  /* connect renderer to data source */
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_box), cell, "text", 0, NULL);

  /* connect signal to tour handler function */
  g_signal_connect(combo_box, "changed", G_CALLBACK(cb_changed), NULL);
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);

  /* add button that, when clicked, delete currently selected entry */
  button = gtk_button_new_with_mnemonic("_delete");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  g_signal_connect(button, "clicked", G_CALLBACK(cb_delete), GTK_COMBO_BOX(combo_box));

  gtk_widget_show_all(window);
  gtk_main();
  
  return 0;
}

/* combo_box_example1.c ends here */
