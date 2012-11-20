/* combo_box_example2.c --- GtkComboBox use MVC example, has image
 * 
 * Filename: combo_box_example2.c
 * Description: 
 * Author: the machine of awareness <machine.of.awareness@gmail.com>
 * Maintainer: 
 * Created: 五 11月  9 23:28:32 2012 (+0800)
 * Version: 
 * Last-Updated: 五 11月  9 23:29:18 2012 (+0800)
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
 * this is a example, to show use MVC by GtkCcomboBox.  the GtkComboBox is
 * control, the GtkListStore is model, and the GtkCellRenderer is display. use a
 * pixbuf and text in it. program use two type GtkCellRenderer, one for pixbuf,
 * and one for text.
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

enum
{
  PIXBUF_COL,
  TEXT_COL
};

static GtkTreeModel*
createModel()
{
  const gchar *files[] = {"caterpillar.png", "momor.png", "hamimi.png", "bush.png"};
  GdkPixbuf *pixbuf;
  GtkTreeIter iter;
  GtkListStore *store;
  gint i;

  /* make store, and pit data in it */
  store = gtk_list_store_new(2, GDK_TYPE_PIXBUF, GTK_TYPE_STRING);
  for (i = 0; i < 4; ++i)
  {
    /* make pixbuf form file */
    pixbuf = gdk_pixbuf_new_from_file(files[i], NULL);
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, PIXBUF_COL, pixbuf, TEXT_COL, files[i], -1);
    gdk_pixbuf_unref(pixbuf);
  }
  return GTK_TREE_MODEL(store);
}

static void
cb_changed(GtkComboBox *combo_box, GtkLabel *label)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *active;

  /* obtain model */
  model = gtk_combo_box_get_model(combo_box);
  /* obtain iter */
  gtk_combo_box_get_active_iter(combo_box, &iter);
  /* obtain the text from iter */
  gtk_tree_model_get(model, &iter, TEXT_COL, &active, -1);
  gtk_label_set_text(label, active);
}

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *combo_box;
  GtkWidget *label;
  GtkWidget *vbox;
  GtkCellRenderer *renderer;

  /* initialition */
  gtk_init(&argc, &argv);

  /* make main window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GtkComboBox");
  gtk_widget_set_size_request(window, 320, 180);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  /* make combo box, and make renderer, pick it in combo box, connect renderer
   * to data source */
  combo_box = gtk_combo_box_new_with_model(createModel());
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);
  renderer = gtk_cell_renderer_pixbuf_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_box), renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_box), renderer, "pixbuf", PIXBUF_COL, NULL);
  renderer = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_box), renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_box), renderer, "text", TEXT_COL, NULL);

  /* make vbox, put combo box in it */
  vbox = gtk_vbox_new(TRUE, 5);
  label = gtk_label_new_with_mnemonic("caterpillar.jpg");
  gtk_box_pack_start(GTK_BOX(vbox), combo_box, TRUE, TRUE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  /* connect signal to tour handler function */
  g_signal_connect(combo_box, "changed", G_CALLBACK(cb_changed), GTK_LABEL(label));

  gtk_widget_show_all(window);
  gtk_main();
  
  return 0;
}

/* combo_box_example2.c ends here */
