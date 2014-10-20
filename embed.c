// gcc -Wall -o embed embed.c $ (pkg-config --cflags --libs gtk+-2.0)

#include <gtk/gtk.h>

#define EXPR "'GoHello, World!<Esc><C-O>'"

void
send_hello (GtkButton *btn, gint id)
{
	gchar *command = g_strdup_printf ("gvim --servername %d --remote-send " EXPR, id);
	g_spawn_command_line_async (command, NULL);
}

int
main (int arc, char **arv)
{
	gtk_init (&argc, &argv);

	/* Create window */
	GtkWidget *win  = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkWidget *vbox = gtk_vbox_new (FALSE, 0);
	GtkWidget *sock = gtk_socket_new();
	GtkWidget *btn  = gtk_button_new_with_label ("Hello, World!");

	g_signal_connect (sock, "plug-removed", gtk_main_quit, NULL);
	g_signal_connect (win,  "delete-event", gtk_main_quit, NULL);

	gtk_widget_set_size_request (sock, 200, 200);
	gtk_box_pack_start (GTK_BOX (vbox), sock, TRUE,  TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), btn,  FALSE, TRUE, 0);
	gtk_container_add (GTK_CONTAINER (win), vbox);
	gtk_widget_show_all (win);

	/* Embed vim */
	GdkNativeWindow id = gtk_socket_get_id (GTK_SOCKET (sock));
	gchar *command = g_strdup_printf ( "gvim --servername %d --socketid %d", id, id);
	g_spawn_command_line_async (command, NULL);
	g_signal_connect (btn, "clicked", G_CALLBACK (send_hello), (gpointer)id);

	/* Run */
	gtk_main();
	return 0;
}
