#include "TokenTypes.hpp"
#include <cctype>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

class Tokens {
public:
  std::string t_type;
  std::string t_val;
  Tokens(std::string t_type, std::string t_val = "")
      : t_type(t_type), t_val(t_val) {}
  Tokens(char t_type, std::string t_val = "")
      : t_type(std::string(1, t_type)), t_val(t_val) {}
};

class Lexer {
public:
  std::vector<Tokens> t_list;
  char curr_char;
  int next_pos;
  std::string code;
  char advance() {
    ++this->next_pos;
    if (this->next_pos < code.length())
      this->curr_char = this->code[this->next_pos];
    else
      this->curr_char = '\0';
    return this->curr_char;
  }
  Lexer(std::string code_i)
      : code(code_i), t_list(std::vector<Tokens>()), curr_char('\0'),
        next_pos(-1) {}

  void handle_quotes() {
    std::string args = "";
    while (this->curr_char && this->curr_char != TT_QT) {
      args += this->curr_char;
      this->advance();
    }
    this->t_list.emplace_back("TT_STR", args);
  }
  void handle_string() {
    std::string attr = "";
    while (this->curr_char &&
           (std::isalnum(this->curr_char) || this->curr_char == '_')) {
      attr += this->curr_char;
      this->advance();
    }
    this->t_list.emplace_back("TT_ATTR", attr);
    this->next_pos--;
  }

  std::vector<Tokens> make_tokens() {
    while (this->advance()) {
      switch (this->curr_char) {
      case TT_AT:
        this->t_list.emplace_back(TT_AT);
        break;
      case TT_HH:
        this->t_list.emplace_back(TT_HH);
        break;
      case TT_RP:
        this->t_list.emplace_back(TT_RP);
        break;
      case TT_LP:
        this->t_list.emplace_back(TT_LP);
        break;
      case TT_LT:
        this->t_list.emplace_back(TT_LT);
        break;
      case TT_GT:
        this->t_list.emplace_back(TT_GT);
        break;
      case TT_LB:
        this->t_list.emplace_back(TT_LB);
        break;
      case TT_RB:
        this->t_list.emplace_back(TT_RB);
        break;
      case TT_SB:
        this->t_list.emplace_back(TT_SB);
        break;
      case TT_EB:
        this->t_list.emplace_back(TT_EB);
        break;
      case TT_QQ:
        this->t_list.emplace_back(TT_QQ);
        break;
      case TT_EX:
        this->t_list.emplace_back(TT_EX);
        break;
      case TT_CL:
        this->t_list.emplace_back(TT_CL);
        break;
      case TT_SC:
        this->t_list.emplace_back(TT_SC);
        break;
      case TT_DH: {
        if (this->advance() == '>') {
          this->t_list.emplace_back("TT_NXT");
        } else {
          this->next_pos--;
        }
        break;
      }
      case TT_EQ:
        this->t_list.emplace_back(TT_EQ);
        break;
      case TT_QT:
        this->advance();
        this->handle_quotes();
        break;
      default: {
        if (std::isalnum(this->curr_char))
          handle_string();
        break;
      }
      }
    }
    return this->t_list;
  }
};

std::ostream &operator<<(std::ostream &os, Tokens &token) {

  if (token.t_val.empty())
    os << token.t_type;
  else
    os << token.t_type << ":" << token.t_val;
  return os;
}

std::ostream &operator<<(std::ostream &os, std::vector<Tokens> &list) {
  if (list.empty())
    os << "The list is empty \n";
  else {
    os << "[";
    for (auto &token : list) {
      os << token << " ";
    }
    os << " ]";
  }
  return os;
}
int main() {

  while (true) {
    std::cout << "Vakya>> ";
    std::string code;
    std::getline(std::cin, code);
    Lexer lexer(code);
    lexer.make_tokens();
    std::cout << lexer.t_list << "\n";
  }
}
