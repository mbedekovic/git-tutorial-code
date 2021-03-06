#include <algorithm>
#include <iostream>
#include <locale>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class LustObject {
public:
  // executes the command. pass empty arguments by default.
  virtual void handle(istringstream&& arguments = istringstream()) = 0;
  virtual void print_help() = 0;
};

class FactorialCommand : public LustObject {
public:
  virtual void handle(istringstream&& arguments) {
    int argument;

    if (!(arguments >> argument)) {
      cout << "fact: could not read integer argument." << endl;
      return;
    }

    if (argument < 0) {
      cout << "fact: argument has to be non-negative!" << endl;
      return;
    }

   cout << calculate_factorial(argument) << endl;
  }

  virtual void print_help() {
    cout << " fact <integer>" << endl;
    cout << "   Calculates the factorial of <integer>." << endl;
  }

private:
  short calculate_factorial(int argument) {
    // Hmmm...
    long long result = 1;
    for (int i = 1; i <= argument; i++) {
      result *= i;
    }
    return result;
  }
};

class QuitCommand : public LustObject {
public:
  virtual void handle(istringstream&& arguments) {
    cout << "Bye!" << endl;
    exit(0);
  }

  virtual void print_help() {
    cout << " quit" << endl;
    cout << "   Quits." << endl;
  }
};
///////////////////////////////////////////////////////////////////////////
class SquareCommand : public LustObject {
public:
  virtual void handle(istringstream&& arguments) {
    int argument;

    if (!(arguments >> argument)) {
      cout << "square: could not read integer argument." << endl;
      return;
    }

   cout << calculate_square(argument) << endl;
  }

  virtual void print_help() {
    cout << " square <integer>" << endl;
    cout << "   Calculates the square of <integer>." << endl;
  }

private:
  short calculate_square(int argument) {
    // Hmmm...
    long long result = 1;
    result = argument * argument;
    return result;
  }
};

class QuitCommand : public LustObject {
public:
  virtual void handle(istringstream&& arguments) {
    cout << "Bye!" << endl;
    exit(0);
  }

  virtual void print_help() {
    cout << " quit" << endl;
    cout << "   Quits." << endl;
  }
};
/////////////////////////////////////////////////////////////
class HelpCommand : public LustObject {
public:
  HelpCommand(map<string, unique_ptr<LustObject> > *commands)
     : commands_(commands) { };

  virtual void handle(istringstream&& arguments) {
    cout << "List of all commands" << endl;
    cout << "--------------------" << endl;

    for (auto it = commands_->begin(); it != commands_->end(); it++) {
      it->second->print_help();
    }
  }

  virtual void print_help() {
    cout << " help" << endl;
    cout << "   Prints help for all commands." << endl;
  }

private:
  map<string, unique_ptr<LustObject> > *commands_;
};


int main() {
  cout << "Hello! Welcome to the LARICS Universal Shell Terminal (LUST)!" << endl;
  cout << "Enter 'help' for a list of commands. Press Ctrl-D or enter 'quit' to quit." << endl;

  // map for storing all commands
  map<string, unique_ptr<LustObject> > commands;
  commands["fact"] = unique_ptr<LustObject>(new FactorialCommand());
  commands["square"] = unique_ptr<LustObject>(new SquareCommand());
  commands["quit"] = unique_ptr<LustObject>(new QuitCommand());
  // help command needs a pointer to the parent map in order to call each
  // command's print_help() function
  commands["help"] = unique_ptr<LustObject>(new HelpCommand(&commands));

  while(cin) {
    string cmd_line, cmd_name;
    cout << ">> ";

    // read current line and try to extract command name
    getline(cin, cmd_line);
    istringstream cmd_line_stream(cmd_line);
    if (!(cmd_line_stream >> cmd_name)) {
      continue;
    }

    std::transform(cmd_name.begin(), cmd_name.end(),
       cmd_name.begin(), ::tolower);

    // look up the appropriate command in commands map
    auto command_iterator = commands.find(cmd_name);

    if (command_iterator == commands.end()) {
      cout << "lust: no such command '" << cmd_name << "'." << endl;
      continue;
    } else {
      // command found, pass its handler the rest of the read arguments
      auto & command = command_iterator->second;
      command->handle(std::move(cmd_line_stream));
    }

  }
  cout << endl;

  commands["quit"]->handle();
  return 0;
}
