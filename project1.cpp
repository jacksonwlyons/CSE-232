#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::stoll;
using std::string;

// This function takes in a string and returns the integer sum
// of all the digits in the string.
int get_line_sum(const string& ca_str) {
  // This source helped me determine how to convert the unsigned string size.
  // https://stackoverflow.com/questions/45318189/c-static-cast-int-str-size-1-means
  unsigned str_size = ca_str.size();
  int int_strsize = static_cast<int>(str_size);
  int line_sum = 0;
  for (int i = 0; i <= (int_strsize - 1); i++) {
    char dig = ca_str[i];
    // char to int conversion found from
    // https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
    int int_dig = dig - '0';
    line_sum += int_dig;
  }
  // abs() function found from
  // https://www.cplusplus.com/reference/cstdlib/abs/
  return abs(line_sum);
}

// Based off the inputted rule string, the corresponding value
// for the 3 digit neighborhood (str_nhood) will be returned.
char get_next_state(const string& str_nhood, const string& rule_str) {
  // Use of .find() referenced from
  // https://www.cplusplus.com/reference/string/string/find/
  string::size_type find_nhood = rule_str.find(str_nhood);
  string str_output_val;
  char output_val;
  if (find_nhood != string::npos) {
    // substr info found from
    // https://www.cplusplus.com/reference/string/string/substr/
    str_output_val = rule_str.substr(find_nhood + 7, 1);
    output_val = str_output_val[0];
  } else {
    output_val = '0';
  }
  return output_val;
}
// This function takes in the start line string size and creates
// a string of zeroes based on the size of the start line.
// By doing this I don't have to worry about replacing certain values
// from the default inputted string.
string update_chg_line(const int& int_strsize) {
  string chg_line = "";
  for (int i = 0; i < (int_strsize); i++) {
    // use of .append() referenced from
    // https://www.cplusplus.com/reference/string/string/append/
    chg_line.append("0");
  }
  return chg_line;
}
// This function takes in an index integer, a string representing the
// line to be changed, an integer representing the line size, and a
// replacement string used to update the change line at the appropriate index.
// Based off the inputted index (i), this function will replace the value at
// that index with the inputted replacement string (replace_str1).
string replace_vals(const int& i,
                    string& chg_line,
                    const int& int_strsize,
                    const string& replace_str1) {
  // .replace function discovered from
  // https://www.cplusplus.com/reference/string/string/replace/
  if (i >= int_strsize - 1) {
    chg_line = chg_line.replace((i - int_strsize) + 1, 1, replace_str1);
  } else {
    chg_line = chg_line.replace(i + 1, 1, replace_str1);
  }

  return chg_line;
}
// To account for string wrapping in this function (update_line), the first two
// digits are
// appended
// to the end of a copy of the inputted line string. To find the possible 3
// digit combinations
// this function loops through the copied string and creates three digit
// substrings to compare
// to the rule string. If any of the 3 digit combinations in the copied string
// match any of the rules,
// the function will call the replace_vals function and update the line
// accordingly.
void update_line(string& line_str, const string& rule_str) {
  string wrap_line = line_str;
  wrap_line += line_str.substr(0, 2);

  string replace_str1 = "";

  unsigned str_size = line_str.size();
  int int_strsize = static_cast<int>(str_size);
  string chg_line = update_chg_line(int_strsize);
  for (int i = 0; i <= (int_strsize - 1); i++) {
    string possible_nhood = wrap_line.substr(i, 3);

    string::size_type result = rule_str.find(possible_nhood);
    if (result != string::npos) {
      char update_val = get_next_state(possible_nhood, rule_str);
      // char to string conversion discovered from
      // https://www.techiedelight.com/convert-char-to-string-cpp/
      replace_str1.push_back(update_val);

      chg_line = replace_vals(i, chg_line, int_strsize, replace_str1);
      // string.erase() method referenced from
      // https://www.cplusplus.com/reference/string/string/erase/
      replace_str1.erase(0, 1);
    }
  }
  line_str = chg_line;
}
// This function (r_string_update) takes in the return string
// (string to be appended to and updated), a string representing the current
// line,
// and an int representing the sum of the digits in the current line.
// This function just concatenates the line and line sum values to the return
// string.
string rstring_update(string& return_str,
                      const int& strt_sum,
                      const string& current_line) {
  // string concatenation discovered from
  // https://stackoverflow.com/questions/662918/how-do-i-concatenate-multiple-c-strings-on-one-line
  return_str += string(current_line);
  return_str += string(" sum = ");
  auto str_sum = std::to_string(strt_sum);
  return_str += str_sum;
  return_str.append("\n");
  return return_str;
}
// In this function (run_cellular_automata) I first determined the sum of the
// starting line,
// and appended both the string and sum to the return string. To account for
// this,
// in my while loop I have it set to stop when the iteration number is equal to
// 1.
// I did this because finding the sum of the starting string technically counts
// as one iteration.
string run_cellular_automata(const string& rule_str,
                             int& iter_num,
                             string& strt_line) {
  string return_str = "";
  int strt_sum = get_line_sum(strt_line);

  return_str = rstring_update(return_str, strt_sum, strt_line);
  while (iter_num > 1) {
    // This function (update_line) takes in a reference to the input line, and a
    // string of rules.
    // The function will return an updated string based on the string of rules
    // and the inputted
    // starting line.
    update_line(strt_line, rule_str);
    strt_sum = get_line_sum(strt_line);
    return_str = rstring_update(return_str, strt_sum, strt_line);
    iter_num--;
  }
  return return_str;
}

int main() {
  string rule_str;

  int iter_num;
  string strt_line;

  string multi_line_str = "";
  // This source helped me iterate though each line of input until a period
  // https://stackoverflow.com/questions/19673332/detecting-end-of-input-using-stdgetline
  while (std::getline(cin, rule_str)) {
    if (rule_str == ".") {
      break;
    } else {
      multi_line_str += string(rule_str);
      multi_line_str.append("\n");
    }
  }

  cin >> iter_num;
  cin >> strt_line;

  // This function takes in a string of rules, an iteration number (for how many
  // times to
  // update the line and return the sum), and a starting string. This function
  // first takes
  // in the starting string, returns the sum, and then it starts to update
  // the line (using rstring_update()) and returns the sum accordingly.
  string cellular = run_cellular_automata(multi_line_str, iter_num, strt_line);

  cout << cellular << endl;

  return 0;
}