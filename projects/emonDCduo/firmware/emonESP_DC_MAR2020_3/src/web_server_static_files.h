#include "./web_server_files/web_server.lib_js.h"
#include "./web_server_files/web_server.home_html.h"
#include "./web_server_files/web_server.wifi_html.h"
#include "./web_server_files/web_server.style_css.h"
#include "./web_server_files/web_server.config_js.h"

StaticFile staticFiles[] = {
  { "/lib.js", lib_js, sizeof(lib_js) - 1, _CONTENT_TYPE_JS },
  { "/home.html", home_html, sizeof(home_html) - 1, _CONTENT_TYPE_HTML },
  { "/wifi.html", wifi_html, sizeof(wifi_html) - 1, _CONTENT_TYPE_HTML },
  { "/style.css", style_css, sizeof(style_css) - 1, _CONTENT_TYPE_CSS },
  { "/config.js", config_js, sizeof(config_js) - 1, _CONTENT_TYPE_JS },
};
