#include <argparse.h>

// For testing and debugging
void print_opts(struct options_t* opts) {
  std::cout << "Options Data:" << std::endl;
  std::cout << "\t-i: " << 
    (opts->inputfilename ? std::string(opts->inputfilename) : "nullptr")
    << std::endl; 
  std::cout << "\t-o: " <<
    (opts->outputfilename ? std::string(opts->outputfilename) : "nullptr")
    << std::endl;
  std::cout << "\t-s: " << opts->steps         << std::endl;
  std::cout << "\t-t: " << opts->theta         << std::endl;
  std::cout << "\t-d: " << opts->dt            << std::endl;
  std::cout << "\t-V: " << opts->visualization << std::endl;
}

void set_default_opts(struct options_t* opts) {
    //Set Default Values
  opts->inputfilename = nullptr;
  opts->outputfilename = nullptr;
  opts->steps = -1;
  opts->theta = -1;
  opts->dt = 0.005; // specified default
  opts->visualization = false;
}

bool contains_undefined_opts(struct options_t* opts) {
  return (opts->inputfilename == nullptr ||
          opts->outputfilename == nullptr ||
          opts->steps == -1 ||
          opts->theta == -1);
}

std::string get_undefined_opts_string(struct options_t* opts) {
  std::vector<char> v;
  if (opts->inputfilename == nullptr)  v.push_back('i');
  if (opts->outputfilename == nullptr) v.push_back('o');
  if (opts->steps == -1)               v.push_back('s');
  if (opts->theta == -1)               v.push_back('t');

  if (v.empty()) return "";
  // Write option characters separated by commas ","
  std::stringstream result;
  result << v.front();
  for (auto it = v.begin()+1; it != v.end(); ++it) {
    result << ", " << *it;
  }
  // Get data from stringstream as string and return.
  return result.str();
}

void get_opts(int argc, char** argv, struct options_t* opts) {
  // print_opts(opts);

  if(argc == 1) {
    std::cout << "Usage:"                   << std::endl;
    std::cout << "\t-i <inputfilename>"     << std::endl;
    std::cout << "\t-o <outputfilename>"    << std::endl;
    std::cout << "\t-s <steps>"             << std::endl;
    std::cout << "\t-t <theta>"             << std::endl;
    std::cout << "\t-V [use visualization]" << std::endl;
    exit(EXIT_SUCCESS);
  }

  //Set Default Values
  set_default_opts(opts);
  //print_opts(opts);

// TODO
  int c = 0;
  // char* optarg;  // stores string following option character
  // int optopt;    // stores unrecognized option character
  while((c = getopt(argc, argv, "i:k:d:m:t:cs:")) != -1) { // [!] change
    // Debugging
    // print_opts(opts);
    // std::cout << "c: " << (char)c << std::endl;
    // std::cout << "ind: " << ind << std::endl;
    // std::cout << "optind: " << optind << std::endl;
    // std::cout << "optarg: " << (optarg ? optarg : "nullptr") << std::endl;
    // std::cin.get();
    switch (c) {
      case 'i':
        opts->inputfilename = optarg;
        break;
      case 'o':
        opts->outputfilename = optarg;
        break;
      case 's':
        opts->steps = atoi(optarg);
        break;
      case 't':
        opts->theta = strtod(optarg, NULL);
        if (opts->theta == -1) {
          std::cout << "Error: can't parse " << optarg << " to double.\n";
          exit(EXIT_FAILURE);
        }
        break;
      case 'd':
        opts->dt = strtod(optarg, NULL);
        if (opts->dt == -1) {
          std::cout << "Error: can't parse " << optarg << " to double.\n";
          exit(EXIT_FAILURE);
        }
        break;
      case 'V':
        opts->visualization = true;
        break;
      default:
        std::cout << "Error: unknown option or missing argument.\n";
        exit(EXIT_FAILURE);
        break;
    }
  }
  // std::cout << "We made it out of the while loop." << std::endl;
  //print_opts(opts);

  if(contains_undefined_opts(opts)) {
    std::cout << "Error: these options have missing or invalid values: " 
              <<  get_undefined_opts_string(opts) << std::endl;
    exit(EXIT_FAILURE);
  }
}