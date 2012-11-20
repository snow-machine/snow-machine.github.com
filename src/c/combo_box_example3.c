/* combo_box_example3.c --- GtkComboBox complex example
 * 
 * Filename: combo_box_example3.c
 * Description: 
 * Author: the machine of awareness <machine.of.awareness@gmail.com>
 * Maintainer: 
 * Created: 日 11月 11 10:26:17 2012 (+0800)
 * Version: 
 * Last-Updated: 日 11月 11 10:31:03 2012 (+0800)
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
 * this is a example, to show use MVC by GtkCcomboBox. the GtkComboBox is
 * control, the GtkTreeStore is model, and the GtkCellRenderer is display. use a
 * pixbuf, text, and spin_button in it. program use three GtkCellRenderer, one
 * for pixbuf, one for text, and one for spin_button. model(GtkTreeStore) two
 * layer, use child and parent. clicked button can change model(GtkTreeStore)
 * content.
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

/* this enumeration simplifies using data store, since column numbers can be
 * accessed via more meaningful names */
enum
{
  /* text related columns */
  TEXT_C = 0,                   /* column with text strings */
  TEXT_VIS_C,                   /* visibility column for text strings */
  TEXT_COL_C,                   /* text color column */

  /* image related columns */
  PIXBUF_C,                     /* column with GdkPixBufs */
  PIXBUF_VIS_C,                 /* visibility column for pixbufs */

  /* progress related columns */
  PROGRESS_C,                   /* column with process information [0, 100] */
  PROGRESS_VIS_C,               /* visibility column for progress */
  
  /* last element of enumeration holds the number of columns */
  N_COLS
};

/* structure that holds widgets we need in our callback function */
typedef struct _Data Data;
struct _Data
{
  /* our combo box */
  GtkWidget    *combo;
  /* just a conveniance to aviod calling gtk_combo_box_get_model every time we
   * need to access data */
  GtkTreeModel *store;
  /* check buttons that control visibility of renderers */
  GtkWidget    *vis_pixbuf;
  GtkWidget    *vis_text;
  GtkWidget    *vis_progress;
  /* entries for modifying vlaues */
  GtkWidget    *e_pixbuf;
  GtkWidget    *e_text;
  GtkWidget    *e_progress;
};

/* callback function for updating current item */
static void
cb_clicked(GtkButton *button, Data *data)
{
  const gchar *stock_id, *string;
  gint         value;
  gboolean     pix, text, prog;
  GtkTreeIter  iter;
  
  /* if nothing selected, do nothing */
  if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(data->combo), &iter))
  {
    return;
  }
  /* fill variables with proper data */
  stock_id = gtk_entry_get_text(GTK_ENTRY(data->e_pixbuf));
  string   = gtk_entry_get_text(GTK_ENTRY(data->e_text));
  value    = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(data->e_progress));
  pix      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->vis_pixbuf));
  text     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->vis_text));
  prog     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->vis_progress));

  /* update data store for current iter */
  gtk_tree_store_set(GTK_TREE_STORE(data->store), &iter,
                     TEXT_C, string,
                     TEXT_VIS_C, text,
                     TEXT_COL_C, "blue",
                     PIXBUF_C, stock_id,
                     PIXBUF_VIS_C, pix,
                     PROGRESS_C, value,
                     PROGRESS_VIS_C, prog,
                     -1);
}

/* callback function for change signal, in this function, we'll set the widgets
 * that control line */
static void
cb_changed(GtkComboBox *combo, Data *data)
{
  /* vars */
  GtkTreeIter  iter;
  gchar       *stock_id, *string;
  gint         value;
  gboolean     pix, text, prog;
  gboolean     active;

  stock_id = NULL;
  string = NULL;

  active = gtk_combo_box_get_active_iter(GTK_COMBO_BOX(data->combo), &iter);
  if (active==TRUE)
  {
    gtk_tree_model_get(data->store, &iter,
                       TEXT_C, &string,
                       TEXT_VIS_C, &text,
                       PIXBUF_C, &stock_id,
                       PIXBUF_VIS_C, &pix,
                       PROGRESS_C, &value,
                       PROGRESS_VIS_C, &prog,
                       -1);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data->vis_text), text);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data->vis_pixbuf), pix);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data->vis_progress), prog);
    gtk_entry_set_text(GTK_ENTRY(data->e_text), string);
    gtk_entry_set_text(GTK_ENTRY(data->e_pixbuf), stock_id);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(data->e_progress), value);

    /* free strings */
    g_free(stock_id);
    g_free(string);
  }
  gtk_widget_set_sensitive(data->vis_text, active);
  gtk_widget_set_sensitive(data->vis_pixbuf, active);
  gtk_widget_set_sensitive(data->vis_progress, active);
  gtk_widget_set_sensitive(data->e_text, active);
  gtk_widget_set_sensitive(data->e_pixbuf, active);
  gtk_widget_set_sensitive(data->e_progress, active);
}


/* this function creates tree data structure and fills it with data */
static GtkTreeModel*
create_model(Data *data)
{
  GtkTreeStore *store;
  GtkTreeIter   parent, child;

  /* create data store, we'll be using GtkTreeStore today, to show you how combo
   * box handles tree structure */
  store = gtk_tree_store_new(
    N_COLS,
    G_TYPE_STRING,              /* text */
    G_TYPE_BOOLEAN,             /* text visibility */
    G_TYPE_STRING,              /* text color */
    G_TYPE_STRING,              /* pixbuf */
    G_TYPE_BOOLEAN,             /* pixbuf visibility */
    G_TYPE_INT,                 /* progress bar */
    G_TYPE_BOOLEAN              /* progress bar visibility */
    );
  /* fill our store with some data */
  gtk_tree_store_append(store, &parent, NULL);
  gtk_tree_store_set(store, &parent, TEXT_C, "Root 1",
                     TEXT_VIS_C, TRUE,
                     TEXT_COL_C, "black",
                     PIXBUF_C, GTK_STOCK_OK,
                     PIXBUF_VIS_C, TRUE,
                     PROGRESS_C, 100,
                     PROGRESS_VIS_C, TRUE,
                     -1);
  gtk_tree_store_append(store, &child, &parent);
  gtk_tree_store_set(store, &child, TEXT_C, "Leaf 1.1",
                     TEXT_VIS_C, TRUE,
                     TEXT_COL_C, "red",
                     PIXBUF_C, GTK_STOCK_ADD,
                     PIXBUF_VIS_C, TRUE,
                     PROGRESS_C, 100,
                     PROGRESS_VIS_C, TRUE,
                     -1);
  gtk_tree_store_append(store, &child, &parent);
  gtk_tree_store_set(store, &child, TEXT_C, "Leaf 1.2",
                     TEXT_VIS_C, TRUE,
                     TEXT_COL_C, "green",
                     PIXBUF_C, GTK_STOCK_APPLY,
                     PIXBUF_VIS_C, TRUE,
                     PROGRESS_C, 100,
                     PROGRESS_VIS_C, TRUE,
                     -1);
  gtk_tree_store_append(store, &child, &parent);
  gtk_tree_store_set(store, &child, TEXT_C, "Leaf 1.3",
                     TEXT_VIS_C, TRUE,
                     TEXT_COL_C, "blue",
                     PIXBUF_C, GTK_STOCK_CDROM,
                     PIXBUF_VIS_C, TRUE,
                     PROGRESS_C, 100,
                     PROGRESS_VIS_C, TRUE,
                     -1);
  gtk_tree_store_append(store, &child, &parent);
  gtk_tree_store_set(store, &child, TEXT_C, "Leaf 1.4",
                     TEXT_VIS_C, TRUE,
                     TEXT_COL_C, "orange",
                     PIXBUF_C, GTK_STOCK_QUIT,
                     PIXBUF_VIS_C, TRUE,
                     PROGRESS_C, 100,
                     PROGRESS_VIS_C, TRUE,
                     -1);
  
  gtk_tree_store_append(store, &parent, NULL);
  gtk_tree_store_set( store, &parent, TEXT_C, "Root 2",
                      TEXT_VIS_C, TRUE,
                      TEXT_COL_C, "black",
                      PIXBUF_C, GTK_STOCK_FILE,
                      PIXBUF_VIS_C, TRUE,
                      PROGRESS_C, 100,
                      PROGRESS_VIS_C, TRUE,
                      -1);
  gtk_tree_store_append(store, &child, &parent);
  gtk_tree_store_set(store, &child, TEXT_C, "Leaf 2.1",
                     TEXT_VIS_C, TRUE,
                     TEXT_COL_C, "blue",
                     PIXBUF_C, GTK_STOCK_EXECUTE,
                     PIXBUF_VIS_C, TRUE,
                     PROGRESS_C, 100,
                     PROGRESS_VIS_C, TRUE,
                     -1);

  gtk_tree_store_append(store, &child, &parent);
  gtk_tree_store_set(store, &child, TEXT_C, "Leaf 2.2",
                     TEXT_VIS_C, TRUE,
                     TEXT_COL_C, "red",
                     PIXBUF_C, GTK_STOCK_HOME,
                     PIXBUF_VIS_C, TRUE,
                     PROGRESS_C, 100,
                     PROGRESS_VIS_C, TRUE,
                     -1);

  gtk_tree_store_append(store, &child, &parent);
  gtk_tree_store_set(store, &child, TEXT_C, "Leaf 2.3",
                     TEXT_VIS_C, TRUE,
                     TEXT_COL_C, "gray",
                     PIXBUF_C, GTK_STOCK_INFO,
                     PIXBUF_VIS_C, TRUE,
                     PROGRESS_C, 100,
                     PROGRESS_VIS_C, TRUE,
                     -1);
  gtk_tree_store_append(store, &child, &parent);
  gtk_tree_store_set(store, &child, TEXT_C, "Leaf 2.4",
                     TEXT_VIS_C, TRUE,
                     TEXT_COL_C, "green",
                     PIXBUF_C, GTK_STOCK_PRINT,
                     PIXBUF_VIS_C, TRUE,
                     PROGRESS_C, 100,
                     PROGRESS_VIS_C, TRUE,
                     -1);

  data->store = GTK_TREE_MODEL(store);
  return data->store;
}

/* main */
int main(int argc, char *argv[])
{
  GtkWidget       *window;
  GtkWidget       *table;
  GtkWidget       *button;
  GtkCellRenderer *cell;
  Data             data;

  /* initialition */
  gtk_init(&argc, &argv);

  /* main window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GtkComboBox");
  gtk_widget_set_size_request(window, 640, 320);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* table */
  table = gtk_table_new(4, 3, FALSE);
  gtk_container_add(GTK_CONTAINER(window), table);

  /* create combox box */
  data.combo = gtk_combo_box_new();
  g_signal_connect(data.combo, "changed", G_CALLBACK(cb_changed), &data);
  gtk_table_attach(GTK_TABLE(table), data.combo, 0, 3, 0, 1,
                   GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0, 0);

  /* create data store */
  data.store = create_model(&data);

  /* add data store to combo box */
  gtk_combo_box_set_model(GTK_COMBO_BOX(data.combo), data.store);
  g_object_unref(data.store);

  /* create pixbuf cell renderer */
  cell = gtk_cell_renderer_pixbuf_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(data.combo), cell, FALSE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(data.combo), cell,
                                 "stock-id", PIXBUF_C,
                                 "visible", PIXBUF_VIS_C,
                                 NULL);

  /* create text cell renderer */
  cell = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(data.combo), cell, FALSE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(data.combo), cell,
                                 "text", TEXT_C,
                                 "visible", TEXT_VIS_C,
                                 "foreground", TEXT_COL_C,
                                 NULL);

  /* create progress cell renderer */
  cell = gtk_cell_renderer_progress_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(data.combo), cell, FALSE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(data.combo), cell,
                                 "value", PROGRESS_C,
                                 "visible", PROGRESS_VIS_C,
                                 NULL);

  /* create check buttons for controling visibility */
  data.vis_pixbuf   = gtk_check_button_new_with_label("image visible");
  gtk_table_attach(GTK_TABLE(table), data.vis_pixbuf, 0, 1, 1, 2,
                   GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0, 0);
  data.vis_text     = gtk_check_button_new_with_label("text visible");
  gtk_table_attach(GTK_TABLE(table), data.vis_text, 1, 2, 1, 2,
                   GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0, 0);
  data.vis_progress = gtk_check_button_new_with_label("progress visible");
  gtk_table_attach(GTK_TABLE(table), data.vis_progress, 2, 3, 1, 2,
                   GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0, 0);

  
  /* create entries for modifying values */
  data.e_pixbuf   = gtk_entry_new();
  gtk_table_attach(GTK_TABLE(table), data.e_pixbuf, 0, 1, 2, 3,
                   GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0, 0);
  data.e_text     = gtk_entry_new();
  gtk_table_attach(GTK_TABLE(table), data.e_text, 1, 2, 2, 3,
                   GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0, 0);
  data.e_progress = gtk_spin_button_new_with_range(0, 100, 1);
  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(data.e_progress), TRUE);
  gtk_table_attach(GTK_TABLE(table), data.e_progress, 2, 3, 2, 3,
                   GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0, 0);

  /* create button for applying changes */
  button = gtk_button_new_from_stock(GTK_STOCK_APPLY);
  g_signal_connect(button, "clicked", G_CALLBACK(cb_clicked), &data);
  gtk_table_attach(GTK_TABLE(table), button, 0, 3, 3, 4,
                   GTK_EXPAND|GTK_FILL, GTK_SHRINK, 0, 0);

  /* manually call cb_changed function to set controllers to right state */
  cb_changed(GTK_COMBO_BOX(data.combo), &data);

  gtk_widget_show_all(window);
  gtk_main();
  
  return 0;
}

/* combo_box_example3.c ends here */
