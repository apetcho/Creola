#include <gtest/gtest.h>

#include "creola/core/lexer.hpp"

using namespace creola::core;


TEST(LexerTest, next){
    // TOKENS: {Token(let), Token(x), Token(=), Token(2)}
    Lexer lexer("let x = 2");
    //auto tokens = tokenize(lexer);

    // [1] Token counts
    EXPECT_EQ(lexer.next().text, Str("let"));
    EXPECT_EQ(lexer.next().text, Str("x"));
    EXPECT_EQ(lexer.next().text, Str("="));
    EXPECT_EQ(lexer.next().text, Str("2"));
    EXPECT_EQ(lexer.next().kind, TokenKind::End);

    // [2] Lexer::read_number
    // [3] Lexer::read_string
    // [4] Lexer::read_identifier
    // [5] Lexer::match
    // [2] Lexer::is_eos
}