
#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

#include <string>
#include <unordered_map>

enum class TokenType {
  // Symbols
  TT_AT,
  TT_HH,
  TT_LP,
  TT_RP,
  TT_LT,
  TT_GT,
  TT_DH,
  TT_QT,
  TT_LB,
  TT_RB,
  TT_QM,
  TT_EX,
  TT_CL,
  TT_SC,
  TT_SB,
  TT_EB,
  TT_EQ,
  TT_NXT,
  TT_EOL,

  // Keywords
  TT_DO,
  TT_ON,
  TT_FMT,
  TT_CDN,
  TT_TBL,
  TT_ASC,
  TT_DSC,
  TT_GRP,
  TT_ATTR,
  TT_STR
};

// Keyword token map (reserved words)
extern const std::unordered_map<std::string, TokenType> keywords;

// Convert TokenType to string name (e.g., TT_AT -> "TT_AT")
std::string toString(TokenType type);

// Convert TokenType to its symbol (e.g., TT_AT -> "@")
char symbol(TokenType type);
std::ostream &operator<<(std::ostream &os, TokenType type);

#endif // TOKEN_UTILS_H
