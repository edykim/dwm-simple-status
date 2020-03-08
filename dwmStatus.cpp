/*
 * Copyright (C) 2020 edykim
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * @repository https://github.com/edykim/dwm-simple-status
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <string>
#include <chrono>
#include <thread>
#include <X11/Xlib.h>

#define SEPARATOR " | "
#define DATETIME_FORMAT "%a, %B %d, %Y" SEPARATOR "%I:%M %p"

using namespace std;

string datetime() {
  ostringstream os;
  time_t t = time(nullptr);
  struct tm* timeinfo = localtime(&t);
  os << std::put_time(timeinfo, DATETIME_FORMAT);
  return os.str();
}

string battery() {
  ifstream statusFile, capacityFile;
  string status, capacity, output;

  statusFile.open("/sys/class/power_supply/BAT0/status");
  if (statusFile.is_open()) {
    getline(statusFile, status);
    statusFile.close();
  }
  capacityFile.open("/sys/class/power_supply/BAT0/capacity");
  if (capacityFile.is_open()) {
    getline(capacityFile, capacity);
    capacityFile.close();
  }

  output = capacity + "%";

  if (status != "Discharging") {
    output += " (" + status +")";
  }

  return output;
}

void setText(string text) {
  const char* name = text.c_str();
  Display *dis;

  dis = XOpenDisplay((char *) 0);
  XStoreName(dis, DefaultRootWindow(dis), name);
  XSync(dis, 0);

  XCloseDisplay(dis);
}

int main() {
  string separator = SEPARATOR;
  string cache = "";
  while(true) {
    string text = battery() + separator + datetime();
    if (cache != text) {
      setText(text);
      cache = text;
    }
    this_thread::sleep_for(chrono::milliseconds(1000));
  }
  return 0;
}
