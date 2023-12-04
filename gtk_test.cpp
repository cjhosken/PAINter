#include "include/gtk3/gtk/gtk.h"

int main() {
        gtk_init(NULL, NULL);
GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File",
                                                    NULL,
                                                    GTK_FILE_CHOOSER_ACTION_SAVE,
                                                    "Cancel",
                                                    GTK_RESPONSE_CANCEL,
                                                    "Save",
                                                    GTK_RESPONSE_ACCEPT,
                                                    NULL);

    // Set default filename (optional)
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "Untitled document");

    // Show the save dialog
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_ACCEPT) {
        // Get the filename chosen by the user
        gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        g_print("Selected file for saving: %s\n", filename);
        g_free(filename);
    }

    // Destroy the dialog
    gtk_widget_destroy(dialog);

    return 0;
}

// CHATGPT OPENAI