#ifdef WIN32
#include <windows.h>
#include <locale.h>
// #pragma setlocale(".65001")
// #pragma code_page(65001)
#endif
#include <gtk/gtk.h>

gint	count		= 1;

void on_button_clicked(GtkWidget* buttun, gpointer userdata);

// #ifdef WIN32
#if 0
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int		argc	= __argc;
	char**	argv	= __argv;
#else
int main(int argc, char* argv[])
{
#endif
	GtkWidget*		window;
	GtkWidget*		button;
	gchar*			pWndTitle		= "一个功能完善的窗口";
	gchar*			pBtnTitle		= "按此按钮会在终端上显示一行信息";
#if 0
	wchar_t			szBuf[1024]		= {0};
	char			szWndTitle[128]	= {0};
	char			szBtnTitle[128] = {0};

	MultiByteToWideChar(CP_ACP, 0, pWndTitle, strlen(pWndTitle), szBuf, sizeof(szBuf)/sizeof(wchar_t));
	WideCharToMultiByte(CP_UTF8, 0, szBuf, wcslen(szBuf), szWndTitle, sizeof(szWndTitle), NULL, NULL);
	pWndTitle = szWndTitle;
	memset(szBuf, 0, sizeof(szBuf));
	MultiByteToWideChar(CP_ACP, 0, pBtnTitle, strlen(pBtnTitle), szBuf, sizeof(szBuf)/sizeof(wchar_t));
	WideCharToMultiByte(CP_UTF8, 0, szBuf, wcslen(szBuf), szBtnTitle, sizeof(szBtnTitle), NULL, NULL);
	pBtnTitle = szBtnTitle;
#endif
#ifdef WIN32
	setlocale(LC_ALL, "zh_CN.GB2312");
	//g_locale_to_utf8();
#endif
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// 1.2 完善的窗口

	g_signal_connect(G_OBJECT(window), "delete_event",
	                 G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_title (GTK_WINDOW(window), pWndTitle);
	gtk_window_set_default_size (GTK_WINDOW(window), 500, 300);
	gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	// 1.3 Hello World
	gtk_container_set_border_width(GTK_CONTAINER(window), 40);
	button = gtk_button_new_with_label(pBtnTitle);
	g_signal_connect(G_OBJECT(button), "clicked",
		G_CALLBACK(on_button_clicked), (gpointer)count);
	gtk_container_add(GTK_CONTAINER(window), button);

	gtk_widget_show(button);
	// 退出窗体	
	gtk_widget_show(window);
	gtk_main();
	
	return 0;
}

// 1.3 Hello World
void on_button_clicked(GtkWidget* buttun, gpointer userdata)
{
	gchar*			pHello			= "你好， 这是Hello功能的测试.";
#if 0
	wchar_t			szBuf[1024]		= {0};
	char			szHello[128]	= {0};

	MultiByteToWideChar(CP_ACP, 0, pHello, strlen(pHello), szBuf, sizeof(szBuf)/sizeof(wchar_t));
	WideCharToMultiByte(CP_UTF8, 0, szBuf, wcslen(szBuf), szHello, sizeof(szHello), NULL, NULL);
	pHello = szHello;
#endif
	g_print(pHello);
	g_print("%d\n", (gint)userdata);
	count = count + 1;
}