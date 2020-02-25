/*
  Common functionality across all SEB code.
  Created by Shao-Qian Mah, Jan 25, 2020.
*/

// Convert ID to guaranteed length 2 string.
// Return "" if malformed.
String idToString(int id) {
  if (id >= 1 && id < 10) {
    return "0" + String(id);
  }
  else if (id >= 10 && id <= 99) {
    return String(id);
  }
  return "";
}
