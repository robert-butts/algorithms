#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

using std::string;
using std::vector;
using std::ifstream;
using std::getline;
using std::stoi;
using std::cout;
using std::endl;

namespace {
typedef size_t assignments_t;

vector<int> get_data(const string& filename) {
  ifstream file(filename.c_str());
  if(!file.good())
    return vector<int>();

  string line;
  getline(file, line); // get the header
  getline(file, line); // get the count
  const int len = stoi(line);
  vector<int> data(len);
  for(int i = 0; getline(file, line); ++i) {
    data[i] = stoi(line);
  }
  return data;
}

inline void print_data(const vector<int>& data) {
  cout << "[";
  for(vector<int>::const_iterator i = data.begin(), end = data.end(); i != end; ++i)
    cout << *i << " ";
  cout << "]";
}

/// This is a subset of a true radix sort, for input values from [0..99].
/// A true radix sort for arbitrary integers is significantly slower, having to check many more digits.
assignments_t restricted_radix_sort(vector<int>* data) {
  assignments_t assignments = 0;
  vector<int> buckets(100, 0);

  for(vector<int>::const_iterator i = data->begin(), end = data->end(); i != end; ++i) {
    ++assignments;
    ++buckets[*i];
  }

  size_t pos = 0;
  int    val = 0;
  for(vector<int>::const_iterator count = buckets.begin(), end = buckets.end(); count != end; ++count, ++val) {
    for(int j = 0, jend = *count; j != jend; ++j, ++pos) {
      ++assignments;
      (*data)[pos] = val;
    }
  }

  return assignments;
}

} // namespace

int main(int argc, char* argv[]) {
  if(argc < 2) {
    cout << "usage: " << argv[0] << " inputfile" << endl;
    return 0;
  }

  const string filename = argv[1];
  vector<int> data = get_data(filename);
  if(data.size() == 0) {
    cout << "failed to get data" << endl;
    return 0;
  }

  print_data(data);
  cout << endl;
  cout << "sorting..." << endl;
  const assignments_t assignments = restricted_radix_sort(&data);
  cout << "sorted:" << endl;
  print_data(data);
  cout << endl;
  cout << "assignments: " << assignments << endl;
  cout << "comparisons: 0" << endl;
  return 0;
}
