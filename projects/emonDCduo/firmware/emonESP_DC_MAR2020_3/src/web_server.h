/*
 * -------------------------------------------------------------------
 * EmonESP Serial to Emoncms gateway
 * -------------------------------------------------------------------
 * Adaptation of Chris Howells OpenEVSE ESP Wifi
 * by Trystan Lea, Glyn Hudson, OpenEnergyMonitor
 * All adaptation GNU General Public License as below.
 *
 * -------------------------------------------------------------------
 *
 * This file is part of OpenEnergyMonitor.org project.
 * EmonESP is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * EmonESP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with EmonESP; see the file COPYING.  If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _EMONESP_WEB_SERVER_H
#define _EMONESP_WEB_SERVER_H

#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>


extern AsyncWebServer server;
extern String currentfirmware;

extern void web_server_setup();
extern void web_server_loop();
void dumpRequest(AsyncWebServerRequest *request);


// Content Types
extern const char _CONTENT_TYPE_HTML[];
#define CONTENT_TYPE_HTML FPSTR(_CONTENT_TYPE_HTML)

extern const char _CONTENT_TYPE_TEXT[];
#define CONTENT_TYPE_TEXT FPSTR(_CONTENT_TYPE_TEXT)

extern const char _CONTENT_TYPE_CSS[];
#define CONTENT_TYPE_CSS FPSTR(_CONTENT_TYPE_CSS)

extern const char _CONTENT_TYPE_JSON[];
#define CONTENT_TYPE_JSON FPSTR(_CONTENT_TYPE_JSON)

extern const char _CONTENT_TYPE_JS[];
#define CONTENT_TYPE_JS FPSTR(_CONTENT_TYPE_JS)

#endif // _EMONESP_WEB_SERVER_H