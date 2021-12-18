#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <iomanip>
using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

struct products {
  string type;
  int quantity;
  int cost;
};

struct store_info {
  string name;
  string location;
  vector<products> prod_vect;
};

// This function iterates through a vector containing all the data for each
// store
// and prints the numbers of stores as well as the number of distinct items in
// each store.
void get_store_info(const vector<store_info>& store_v) {
  int vsize = static_cast<int>(store_v.size());
  cout << "Store Related Information (ordered by in-file order):" << endl;
  cout << "There are " << vsize << " store(s)." << endl;
  for (int stor = 0; stor < vsize; stor++) {
    cout << store_v[stor].name << " has " << store_v[stor].prod_vect.size()
         << " distinct items." << endl;
  }
}

// First, this function iterates though the store vector containing all the data
// and inserts
// all the products including duplicates into a multimap. Then it iterates
// through a multimap,
// finds duplicate keys and returns the sum of their values. In other words the
// function
// returns the total combined quantity of a specific product from all stores.
int check_for_dupes(const vector<store_info>& store_v,
                    const string& item_type) {
  std::multimap<string, int> stock_withdupes;
  int ssize = static_cast<int>(store_v.size());
  for (int stor = 0; stor < ssize; stor++) {
    int prod_vec_size = static_cast<int>(store_v[stor].prod_vect.size());
    for (int item = 0; item < prod_vec_size; item++) {
      string quant = std::to_string(store_v[stor].prod_vect[item].quantity);
      int int_quant = stoi(quant);
      // This source helped me determine how tp insert a key, value pair into a
      // map
      // https://www.geeksforgeeks.org/map-insert-in-c-stl/
      stock_withdupes.insert({store_v[stor].prod_vect[item].type, int_quant});
    }
  }

  int sum_ofdupes = 0;
  // This for loop iterates through the multimap, finds the duplicates
  // and adds up all the quantities for the specified item_type.
  for (auto it = stock_withdupes.begin(); it != stock_withdupes.end(); ++it) {
    if (it->first == item_type) {
      sum_ofdupes += it->second;
    }
  }

  return sum_ofdupes;
}

// This function iterates through the vector of store_info and
// gathers all data for products and their quantity for output.
void get_item_info(const vector<store_info>& store_v) {
  cout << endl;
  cout << "Item Related Information (ordered alphabetically):" << endl;
  map<string, int> stock_tosort;
  int ssize = static_cast<int>(store_v.size());

  for (int stor = 0; stor < ssize; stor++) {
    int prod_vec_size = static_cast<int>(store_v[stor].prod_vect.size());
    for (int item = 0; item < prod_vec_size; item++) {
      // call check_for_dupes to find the total quantity of a specific product
      // from all stores combined.
      int quant_sum =
          check_for_dupes(store_v, store_v[stor].prod_vect[item].type);
      // insert the specific product with the appropriate total quantity
      // into the stock_tosort map that will be sorted automatically
      stock_tosort.insert({store_v[stor].prod_vect[item].type, quant_sum});
    }
  }

  cout << "There are " << stock_tosort.size()
       << " distinct item(s) available for purchase." << endl;
  // This source helped me determine how to iterate through a map
  // https://stackoverflow.com/questions/26281979/c-loop-through-map
  for (auto iter = stock_tosort.begin(); iter != stock_tosort.end(); ++iter) {
    cout << "There are " << iter->second << " " << iter->first << "(s)."
         << endl;
  }

  cout << endl;
}

// This source helped me determine how to sort a 2D vector by the first column.
// https://iq.opengenus.org/sort-2d-vector-in-cpp/
bool sort_cost_options(const vector<string>& v1, const vector<string>& v2) {
  return v1[0] < v2[0];
}

// This function checks if all all the quantity we wanted has been
// bought or if all the quantity available has been bought.
void check_done(vector<vector<string> >& cost_options,
                int& quant_wantbuy,
                const int& tot_quantity_available,
                int& tot_quantity_topurchase,
                double& total_price) {
  if (quant_wantbuy == 0 or tot_quantity_available == tot_quantity_topurchase) {
    // This source reminded me how to adjust the decimal precision
    // https://www.cplusplus.com/reference/iomanip/setprecision/
    cout << std::fixed << std::setprecision(2);
    cout << "Total price: $" << total_price << endl;
    int cost_opsize = static_cast<int>(cost_options.size());

    for (int ch = 0; ch < cost_opsize; ch++) {
      cout << "Order " << cost_options[ch][1] << " from " << cost_options[ch][2]
           << " in " << cost_options[ch][3] << endl;
    }

    cout << endl;
  }
}
// This function checks if the quantity I want to buy is greater than or equal
// to the quantity available.
// It also checks if the quantity has been met with this purchase.
void check_quant_greater(vector<vector<string> >& cost_options,
                         int& quant_wantbuy,
                         int& tot_quantity_topurchase,
                         double& total_price,
                         int& op,
                         const int& cost_opsize) {
  int purchase_quant =
      stoi(cost_options[op][1]);  // quantity in this situation stays the same
  tot_quantity_topurchase += purchase_quant;
  total_price +=
      (stod(cost_options[op][0]) * purchase_quant);  // update total price
  quant_wantbuy -= purchase_quant;  // update quantity left needed to buy
  // check if the desired quantity to purchase has been met.
  if (quant_wantbuy == 0) {
    if (cost_opsize > 1) {
      // erase the rest of the vector including the cost_options that were not
      // bought from
      cost_options.erase(cost_options.begin() + op + 1, cost_options.end());
      op = cost_opsize;
    }
  }
}
// This function checks if the quantity I want to buy is less than the quantity
// available
void check_quant_less(vector<vector<string> >& cost_options,
                      int& quant_wantbuy,
                      int& tot_quantity_topurchase,
                      double& total_price,
                      int& op,
                      const int& cost_opsize) {
  int purchase_quant = quant_wantbuy;

  tot_quantity_topurchase += purchase_quant;

  cost_options[op][1] =
      std::to_string(quant_wantbuy);  // update quantity to buy from store
  total_price +=
      (stod(cost_options[op][0]) * purchase_quant);  // update total price
  quant_wantbuy -= purchase_quant;
  if (cost_opsize > 1) {
    // This source reminded me how to use .erase for removing elements from a
    // vector
    // https://en.cppreference.com/w/cpp/container/vector/erase
    cost_options.erase(cost_options.begin() + op + 1, cost_options.end());
    op = cost_opsize;
  }
}

// This function sorts a vector of the different purchase options for a specific
// item by price.
// With the sorted vector of purchase options,the function iterates through the
// cost options by price and
// orders from the cheapest options first. The wanted quantity of food to buy is
// updated after each order.
double get_best_price(vector<vector<string> >& cost_options,
                      int& quant_wantbuy) {
  // This source helped me determine how to sort a 2D vector by the first column
  // https://iq.opengenus.org/sort-2d-vector-in-cpp/
  std::sort(cost_options.begin(), cost_options.end(), sort_cost_options);
  double total_price = 0;
  int tot_quantity_available = 0;
  int tot_quantity_topurchase = 0;
  int cost_opsize = static_cast<int>(cost_options.size());
  for (int r = 0; r < cost_opsize; r++) {
    tot_quantity_available += stoi(cost_options[r][1]);
  }
  // iterate through the cost options
  for (int op = 0; op < cost_opsize; op++) {
    // check if the quantity I want to buy is greater than or equal to the
    // quantity available
    if (quant_wantbuy >= stoi(cost_options[op][1])) {
      check_quant_greater(cost_options, quant_wantbuy, tot_quantity_topurchase,
                          total_price, op, cost_opsize);
    }
    // check if the quantity I want to buy is less than the quantity available
    else if (quant_wantbuy < stoi(cost_options[op][1]) && quant_wantbuy > 0) {
      check_quant_less(cost_options, quant_wantbuy, tot_quantity_topurchase,
                       total_price, op, cost_opsize);
    }
    // check if purchase quantity has been met or if all available has been
    // bought
    check_done(cost_options, quant_wantbuy, tot_quantity_available,
               tot_quantity_topurchase, total_price);
  }
  return total_price;
}

// This function creates a 2d vector of all the purchase options
// of a specified product on the shopping list.
auto get_cost_options(const vector<store_info>& store_v,
                      const string& product_tobuy) {
  int store_cnt = 0;
  vector<vector<string> > cost_options;
  int ssize = static_cast<int>(store_v.size());
  // iterate through each store
  for (int stor = 0; stor < ssize; stor++) {
    int prod_vec_size = static_cast<int>(store_v[stor].prod_vect.size());
    // iterate through items in store
    for (int item = 0; item < prod_vec_size; item++) {
      // check if item is same as item on shopping list
      if (store_v[stor].prod_vect[item].type == product_tobuy) {
        string cost_option;
        int quant_available = store_v[stor].prod_vect[item].quantity;
        string store_tobuyfrom = store_v[stor].name;
        string store_locat = store_v[stor].location;
        store_cnt++;
        int prod_cost = store_v[stor].prod_vect[item].cost;

        double prod_cos = static_cast<double>(prod_cost) / 100;
        vector<string> cost_op1;
        cost_op1.push_back(std::to_string(prod_cos));
        cost_op1.push_back(std::to_string(quant_available));
        cost_op1.push_back(store_tobuyfrom);
        cost_op1.push_back(store_locat);
        cost_options.push_back(cost_op1);
      }
    }
  }
  cout << store_cnt << " store(s) sell " << product_tobuy << "." << endl;
  return cost_options;
}

// This function iterates through a vector of the shopping list and creates a
// new
// 2d vector containing all the different purchase options for each item on the
// shopping list organized by the store.
void shopping(const vector<store_info>& store_v,
              const vector<string>& shopping_list) {
  cout << "Shopping:" << endl;
  int shoplist_size = static_cast<int>(shopping_list.size());
  int quant_wantbuy;

  vector<vector<string> > cost_options;
  double total_price = 0;
  // iterate through shopping list
  for (int shop_item = 0; shop_item < shoplist_size; shop_item++) {
    cost_options.clear();
    cout << "Trying to order " << shopping_list[shop_item] << "(s)." << endl;

    size_t space = shopping_list[shop_item].find(" ");
    int name_size = static_cast<int>(shopping_list[shop_item].size());
    quant_wantbuy = stoi(
        shopping_list[shop_item].substr(0, name_size - (name_size - (space))));
    string product_tobuy =
        shopping_list[shop_item].substr(space + 1, name_size - (space + 1));
    string store_tobuyfrom;
    string store_locat;

    auto cost_options = get_cost_options(store_v, product_tobuy);
    // call to best price function
    double tp = get_best_price(cost_options, quant_wantbuy);
    total_price += tp;
  }

  cout << std::fixed << std::setprecision(2);
  cout << "Be sure to bring $" << total_price
       << " when you leave for the stores." << endl;
}

int main() {
  vector<store_info> store_v;
  int store_lcount = 0;
  string line;
  // This source reminded me how to use std::getline() to read through a string
  // https://en.cppreference.com/w/cpp/string/basic_string/getline
  vector<string> shopping_list;
  store_info store;
  products inventory;
  while (std::getline(cin, line, '\n')) {
    // check for empty line
    // (new store)
    if (line == "\n" || line == "") {
      store_v.push_back(store);
      store.prod_vect.clear();
      // reset line count for next store
      store_lcount = 1;
      continue;
    }

    if (store_lcount == 1) {
      // first line is name of store
      store.name = line;
    } else if (store_lcount == 2) {
      // second line is location of store
      store.location = line;
      if (isdigit(line[0])) {
        shopping_list.push_back(line);
      }
    } else if (store_lcount > 2) {
      // After the second line is the products and their info.
      if (isdigit(line[0])) {
        shopping_list.push_back(line);
      }
      // In this else statement I used the index location of the comma in
      // the lines to manipulate the line and grab certain values like cost and
      // quantity
      else {
        int lsize = static_cast<int>(line.size());
        int comma1 = line.find(',');
        int comma2 = line.find(',', comma1 + 1);
        inventory.type = line.substr(0, comma1);
        string quant = line.substr(comma1 + 1, comma2 - (comma1 + 1));
        inventory.quantity = stoi(quant);
        string scost = line.substr(comma2 + 2, lsize - 1);
        // cost in cents
        inventory.cost = stod(scost) * 100;

        store.prod_vect.push_back(inventory);
      }
    }

    store_lcount++;
  }
  // Call the main 3 functions
  get_store_info(store_v);
  get_item_info(store_v);
  shopping(store_v, shopping_list);
  return 0;
}