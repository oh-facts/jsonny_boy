/* date = August 10th 2024 10:08 am */

#ifndef JSON_LEXER_H
#define JSON_LEXER_H

enum JP_TokenKind
{
	JP_TokenKind_Null,
	JP_TokenKind_LBrace,
	JP_TokenKind_RBrace,
	JP_TokenKind_LSquare,
	JP_TokenKind_RSquare,
	JP_TokenKind_Colon,
	JP_TokenKind_Comma,
	JP_TokenKind_String,
	JP_TokenKind_Number,
	JP_TokenKind_Boolean,
	JP_TokenKind_COUNT,
};

global char *jp_tokenkind_str[JP_TokenKind_COUNT] = 
{
	"null",
	"LBrace",
	"RBrace",
	"LSquare",
	"RSquare",
	"Colon",
	"Comma",
	"String",
	"Number",
	"boolean"
};

struct JP_Token
{
	JP_TokenKind kind;
	Str8 lexeme;
};

#define TOKEN_CHUNK_SIZE 100000

struct JP_Token_chunk
{
	JP_Token_chunk *next;
	JP_Token tokens[TOKEN_CHUNK_SIZE];
	u64 num;
};

struct JP_Token_chunk_list
{
	JP_Token_chunk *first;
	JP_Token_chunk *last;
	u64 num;
};

// ===internal===

function b32 jp_is_whitespace(char c);
function b32 jp_is_number(char c);

function JP_Token *jp_push_token(JP_Token_chunk_list *list);
function JP_Token_chunk *jp_push_token_chunk(JP_Token_chunk_list *list);

function JP_Token_chunk_list jp_lex_from_buffer(u8 *buffer, u64 len);

function JP_Token *jp_get_token(JP_Token_chunk *chunk, u64 i);
function void jp_next_token(JP_Token_chunk **chunk, u64 *i);

function void jp_print_tokens(JP_Token_chunk_list *list);

// ===============

#endif //JSON_LEXER_H
