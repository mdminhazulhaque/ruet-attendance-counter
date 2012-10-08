/*
    RUET Attendance Counter is an application written in GTK+
    that can calculate students' attendance number and show specified marks.
    Copyright 2012, Minhazul Haque Shawon, <minhazul.haq@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <gtk/gtk.h>
#include <math.h>
#include <string.h>
#define size 60
short attend[size] = {0}, marks[size] = {0}, i, x, y;
static short percentage, point, totalClass;
GtkWidget *e[size], *l[size], *m[size], *entryTotalClass, *spinMinimumClass, *spinFullMark;
GtkWidget *label, *frame;
GtkWidget *radioceil, *radiofloor;
GtkWidget *button, *window;
gboolean convType = TRUE; // TRUE = ceil  FALSE = floor
static void valuetype(GtkWidget *radio, gpointer type)
{
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio)))
        if(!strcmp(type,"c"))
            convType = TRUE;
        else
            convType = FALSE;
}
static void calculate()
{
    totalClass =  atoi(gtk_entry_get_text(GTK_ENTRY(entryTotalClass)));
    percentage = gtk_spin_button_get_value_as_int((GTK_SPIN_BUTTON(spinMinimumClass)));
    point = gtk_spin_button_get_value_as_int((GTK_SPIN_BUTTON(spinFullMark)));
    gchar *lab;
    for(i=0; i<size; i++)
    {
        attend[i] = atoi(gtk_entry_get_text(GTK_ENTRY(e[i])));
        if(convType)
            marks[i] = (ceil ( (float) attend[i] * point / totalClass));
        else
            marks[i] = (floor ( (float) attend[i] * point / totalClass));

        if(marks[i]< (percentage/point))
            gtk_label_set_text(GTK_LABEL(m[i]),"F");

        else if(attend[i] > totalClass)
        {
            gtk_label_set_text(GTK_LABEL(m[i]),"Error!");
            marks[i] = 0;
        }

        else
        {
            lab = g_markup_printf_escaped("%d",marks[i]);
            gtk_label_set_text(GTK_LABEL(m[i]),lab);
        }
    } // end of for loop
}
static void save()
{
    GtkWidget *savedlg;
    FILE *db;
    savedlg = gtk_file_chooser_dialog_new("Choose a file",GTK_WINDOW(window),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,GTK_STOCK_OK,
                                          GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
    gtk_widget_show_all(savedlg);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(savedlg), g_get_home_dir());
    gint resp=gtk_dialog_run(GTK_DIALOG(savedlg));
    if(resp==GTK_RESPONSE_OK)
    {
        char *loc = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(savedlg));
        strcat(loc,".txt");
        db = fopen(loc, "w");
        for(i=0; i<size; i++)
            fprintf(db, "Roll %d:\tAttended: %d\tPoint: %d\n", i+1,attend[i], marks[i]);
        fclose(db);
    }
    gtk_widget_destroy(savedlg);
}
void insertButton(int x, int y, const char *name, void callbackfunc())
{
    button = gtk_button_new_with_mnemonic(name);
    gtk_widget_set_size_request(button, 100, 30);
    gtk_fixed_put(GTK_FIXED(frame), button, x, y);
    g_signal_connect (button, "clicked", callbackfunc, NULL);
}
void insertLabel(int x, int y, const char *name)
{
    label =  gtk_label_new(name);
    gtk_fixed_put(GTK_FIXED(frame), label, x, y);
}
int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);
    /// MAIN WINDOW
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_title(GTK_WINDOW(window), "*** RUET Attendance Counter *** by [Minhazul Haq Shawon]");
    g_signal_connect (window, "destroy", gtk_main_quit, NULL);
    /// DEFINING A FRAME
    frame = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), frame);
    /// INSERTING BUTTONS
    insertButton(860, 200, "_Exit", gtk_main_quit);
    insertButton(650, 200, "_Calculate", calculate);
    insertButton(755, 200, "_Save", save);
    /// TOTAL CLASS ENTRY
    entryTotalClass =  gtk_entry_new_with_max_length(2);
    gtk_entry_set_width_chars(GTK_ENTRY(entryTotalClass), 2);
    gtk_entry_prepend_text(GTK_ENTRY(entryTotalClass), "25");
    gtk_fixed_put(GTK_FIXED(frame), entryTotalClass, 110, 200);
    /// INSERTING ALL LABELS
    insertLabel(20, 205, "Total Classes");
    insertLabel(925, 230, "Version: 0.0.1 \t");
    insertLabel(150, 205, "Full Marks");
    insertLabel(300, 205, "Minimum %");
    /// INSERTING LINK BUTTON
    label =  gtk_link_button_new_with_label("http://minhazulhaq.blogspot.com/", "Created by Minhazul Haq Shawon, RUET, 103001");
    gtk_fixed_put(GTK_FIXED(frame), label , 10, 230);
    /// MINIMUM CLASS SPIN BUTTON
    spinMinimumClass = gtk_spin_button_new_with_range(0,100,1);
    gtk_fixed_put(GTK_FIXED(frame), spinMinimumClass ,390, 200);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spinMinimumClass), TRUE);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMinimumClass), 60);
    /// FULL MARK SPIN BUTTON
    spinFullMark = gtk_spin_button_new_with_range(0,100,1);
    gtk_fixed_put(GTK_FIXED(frame), spinFullMark ,225, 200);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spinFullMark), TRUE);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinFullMark), 10);
    /// INSERTING TWO RADIO BUTTONS
    radioceil = gtk_radio_button_new_with_label(NULL, "▲");
    gtk_fixed_put(GTK_FIXED(frame), radioceil ,470, 205);
    g_signal_connect(radioceil,"toggled",G_CALLBACK(valuetype),(gpointer) "c");
    radiofloor = gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radioceil)), "▼");
    gtk_fixed_put(GTK_FIXED(frame), radiofloor ,510, 205);
    g_signal_connect(radiofloor,"toggled",G_CALLBACK(valuetype),(gpointer) "f");
    /// LOOP TO SET ALL ENTRY AND LABEL
    for(i=0,x=20,y=20; i<size; i++,x+=100)
    {
        if(i==0) /// SKIP FIRST TIME
            ;
        else if(i%10==0) /// ARRANGE BY 10 ITEMS
        {
            y+=30;
            x=20;
        }
        /// LABEL: ROLL
        l[i] = gtk_label_new(g_markup_printf_escaped("%d",i+1));
        gtk_fixed_put(GTK_FIXED(frame), l[i],x, y);
        /// ENTRY: INPUT
        e[i] = gtk_entry_new_with_max_length(2);
        gtk_entry_set_width_chars(GTK_ENTRY(e[i]),2);
        gtk_fixed_put(GTK_FIXED(frame), e[i],x+20, y-5);
        /// LABEL: POINT
        m[i] = gtk_label_new(" -- ");
        gtk_fixed_put(GTK_FIXED(frame), m[i],x+50, y);
    }
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
