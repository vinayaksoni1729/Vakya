#include "Token_Utils.hpp"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

class Tokens {
public:
  TokenType t_type;
  std::string t_val;
  Tokens() {}
  Tokens(TokenType t_type, const std::string &t_val = "")
      : t_type(t_type), t_val(t_val) {}
  bool operator==(const Tokens &other) const {
    return this->t_type == other.t_type && this->t_val == other.t_val;
  }
};

class Lexer {
public:
  std::string code;
  std::vector<Tokens> t_list;
  bool at_triggered;
  char curr_char;
  size_t next_pos;
  Tokens prev_token;
Lexer(std::string code_i)
  : code(code_i), t_list(std::vector<Tokens>()), curr_char('\0'),
    next_pos(-1), prev_token(), at_triggered(false) {}
  char advance() {
    ++this->next_pos;
    if (this->next_pos < code.length())
      this->curr_char = this->code[this->next_pos];
    else
      this->curr_char = '\0';
    if (this->t_list.size() > 0)
      prev_token = this->t_list[this->t_list.size() - 1];
    return this->curr_char;
  }

  void handle_quotes() {
    std::string args = "";
    while (this->curr_char && this->curr_char != symbol(TokenType::TT_QT)) {
      args += this->curr_char;
      this->advance();
    }
    this->t_list.emplace_back(TokenType::TT_STR, args);
  }
  bool is_keyword(std::string given_word) {
    auto it = keywords.find(given_word);
    if (it != keywords.end() && this->prev_token.t_type == TokenType::TT_AT) {
      this->t_list.emplace_back(it->second);
      return true;
    }
    return false;
  }

  void handle_string() {
    std::string attr = "";
    while (this->curr_char &&
           (std::isalnum(this->curr_char) || this->curr_char == '_')) {
      attr += this->curr_char;
      this->advance();
    }
    if (this->at_triggered) {
      this->at_triggered = false;
      auto it = keywords.find(attr);
      if (it != keywords.end()) {
        this->t_list.emplace_back(it->second); // recognized keyword
        this->next_pos--;
        return;
      }
    }
    if (!this->is_keyword(attr))
      this->t_list.emplace_back(TokenType::TT_ATTR, attr);

    this->next_pos--;
  }

  std::vector<Tokens> make_tokens() {
    while (this->advance()) {
      switch (this->curr_char) {
      case '@':
        this->at_triggered = true;
        break;
      case '#':
        this->t_list.emplace_back(TokenType::TT_HH);
        break;
      case '(':
        this->t_list.emplace_back(TokenType::TT_LP);
        break;
      case ')':
        this->t_list.emplace_back(TokenType::TT_RP);
        break;
      case '<':
        this->t_list.emplace_back(TokenType::TT_LT);
        break;
      case '>':
        this->t_list.emplace_back(TokenType::TT_GT);
        break;
      case '[':
        this->t_list.emplace_back(TokenType::TT_LB);
        break;
      case ']':
        this->t_list.emplace_back(TokenType::TT_RB);
        break;
      case '{':
        this->t_list.emplace_back(TokenType::TT_SB);
        break;
      case '}':
        this->t_list.emplace_back(TokenType::TT_EB);
        break;
      case '?':
        this->t_list.emplace_back(TokenType::TT_QM);
        break;
      case '!':
        this->t_list.emplace_back(TokenType::TT_EX);
        break;
      case ':':
        this->t_list.emplace_back(TokenType::TT_CL);
        break;
      case ';':
        this->t_list.emplace_back(TokenType::TT_SC);
        break;
      case '-': {
        if (this->advance() == '>') {
          this->t_list.emplace_back(TokenType::TT_NXT);
        } else {
          this->next_pos--;
        }
        break;
      }
      case '=':
        this->t_list.emplace_back(TokenType::TT_EQ);
        break;
      case '"':
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
  std::vector<Tokens> exit_code = {Tokens(TokenType::TT_ATTR, "exit"),
                                   Tokens(TokenType::TT_LP),
                                   TokenType(TokenType::TT_RP)};
  while (true) {
    std::cout << "Vakya>> ";
    std::string code;
    std::getline(std::cin, code);
    Lexer lexer(code);
    lexer.make_tokens();
    std::cout << lexer.t_list << "\n";
    if (lexer.t_list == exit_code)
      break;
  }
}
