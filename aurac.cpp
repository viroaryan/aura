// ==================================================
// Aura Programming Language Native Compiler (aurac)
// Version 2.0 — Phase 1: Core Language Foundations
// Written in C++17 - Zero dependencies, extreme performance
// ==================================================

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <filesystem>
#include <cmath>

namespace fs = std::filesystem;

// ==================================================
// 1. Lexer Definitions
// ==================================================

enum TokenType {
    T_FN, T_VAR, T_PRINT, T_IF, T_ELSE, T_WHILE, T_RETURN, T_EXTERN, T_STRUCT,
    T_TRUE, T_FALSE, T_AND, T_OR, T_NOT, T_FOR, T_IN, T_BREAK, T_CONTINUE,
    T_IDENTIFIER, T_NUMBER, T_STRING,
    T_EQUALS, T_EQ, T_NE, T_LT, T_LE, T_GT, T_GE,
    T_PLUS, T_MINUS, T_MUL, T_DIV, T_MOD,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE, T_COMMA, T_SEMICOLON,
    T_LBRACKET, T_RBRACKET,
    T_DOT,
    T_EOF
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;

    std::string toString() const {
        std::string typeNames[] = {
            "FN", "VAR", "PRINT", "IF", "ELSE", "WHILE", "RETURN", "EXTERN", "STRUCT",
            "TRUE", "FALSE", "AND", "OR", "NOT", "FOR", "IN", "BREAK", "CONTINUE",
            "IDENTIFIER", "NUMBER", "STRING",
            "EQUALS", "EQ", "NE", "LT", "LE", "GT", "GE",
            "PLUS", "MINUS", "MUL", "DIV", "MOD",
            "LPAREN", "RPAREN", "LBRACE", "RBRACE", "COMMA", "SEMICOLON",
            "LBRACKET", "RBRACKET",
            "DOT",
            "EOF"
        };
        return "Token(" + typeNames[type] + ", \"" + value + "\", Line " + std::to_string(line) + ", Col " + std::to_string(column) + ")";
    }
};

class Lexer {
public:
    std::string text;
    size_t pos;
    int line;
    int lineStart;
    char currentChar;
    std::vector<std::string> lines;

    Lexer(const std::string& src) : text(src), pos(0), line(1), lineStart(0) {
        currentChar = text.empty() ? '\0' : text[0];
        
        // Split lines for diagnostics
        std::string currentLine = "";
        for (char c : text) {
            if (c == '\n') {
                lines.push_back(currentLine);
                currentLine.clear();
            } else if (c != '\r') {
                currentLine.push_back(c);
            }
        }
        lines.push_back(currentLine);
    }

    void advance() {
        pos++;
        if (pos < text.length()) {
            currentChar = text[pos];
        } else {
            currentChar = '\0';
        }
    }

    void skipWhitespace() {
        while (currentChar != '\0' && isspace(static_cast<unsigned char>(currentChar))) {
            if (currentChar == '\n') {
                line++;
                lineStart = pos + 1;
            }
            advance();
        }
    }

    void skipComment() {
        // Single-line comments //
        if (currentChar == '/' && pos + 1 < text.length() && text[pos + 1] == '/') {
            advance();
            advance();
            while (currentChar != '\0' && currentChar != '\n') {
                advance();
            }
        }
        // Multi-line block comments /* ... */
        else if (currentChar == '/' && pos + 1 < text.length() && text[pos + 1] == '*') {
            advance();
            advance();
            while (currentChar != '\0') {
                if (currentChar == '*' && pos + 1 < text.length() && text[pos + 1] == '/') {
                    advance();
                    advance();
                    break;
                }
                if (currentChar == '\n') {
                    line++;
                    lineStart = pos + 1;
                }
                advance();
            }
        }
    }

    Token number() {
        int startCol = pos - lineStart + 1;
        std::string result = "";
        while (currentChar != '\0' && (isdigit(static_cast<unsigned char>(currentChar)) || currentChar == '.')) {
            result.push_back(currentChar);
            advance();
        }
        return Token{T_NUMBER, result, line, startCol};
    }

    Token stringLiteral() {
        int startCol = pos - lineStart + 1;
        std::string result = "";
        advance(); // Skip opening quote
        while (currentChar != '\0' && currentChar != '"') {
            if (currentChar == '\\') {
                advance();
                if (currentChar == 'n') result.push_back('\n');
                else if (currentChar == 't') result.push_back('\t');
                else if (currentChar == '\\') result.push_back('\\');
                else if (currentChar == '"') result.push_back('"');
                else result.push_back(currentChar);
            } else {
                result.push_back(currentChar);
            }
            advance();
        }
        if (currentChar == '"') {
            advance(); // Skip closing quote
        } else {
            throw std::runtime_error("Syntax Error: Unterminated string on line " + std::to_string(line));
        }
        return Token{T_STRING, result, line, startCol};
    }

    Token identifier() {
        int startCol = pos - lineStart + 1;
        std::string result = "";
        while (currentChar != '\0' && (isalnum(static_cast<unsigned char>(currentChar)) || currentChar == '_')) {
            result.push_back(currentChar);
            advance();
        }

        TokenType t = T_IDENTIFIER;
        if (result == "fn") t = T_FN;
        else if (result == "var") t = T_VAR;
        else if (result == "print") t = T_PRINT;
        else if (result == "if") t = T_IF;
        else if (result == "else") t = T_ELSE;
        else if (result == "while") t = T_WHILE;
        else if (result == "return") t = T_RETURN;
        else if (result == "extern") t = T_EXTERN;
        else if (result == "struct") t = T_STRUCT;
        else if (result == "true") t = T_TRUE;
        else if (result == "false") t = T_FALSE;
        else if (result == "and") t = T_AND;
        else if (result == "or") t = T_OR;
        else if (result == "not") t = T_NOT;
        else if (result == "for") t = T_FOR;
        else if (result == "in") t = T_IN;
        else if (result == "break") t = T_BREAK;
        else if (result == "continue") t = T_CONTINUE;

        return Token{t, result, line, startCol};
    }

    Token getNextToken() {
        while (currentChar != '\0') {
            if (isspace(static_cast<unsigned char>(currentChar))) {
                skipWhitespace();
                continue;
            }

            if (currentChar == '/' && pos + 1 < text.length()) {
                char nextChar = text[pos + 1];
                if (nextChar == '/' || nextChar == '*') {
                    skipComment();
                    continue;
                }
            }

            int startCol = pos - lineStart + 1;

            if (isdigit(static_cast<unsigned char>(currentChar))) {
                return number();
            }

            if (currentChar == '"') {
                return stringLiteral();
            }

            if (isalpha(static_cast<unsigned char>(currentChar)) || currentChar == '_') {
                return identifier();
            }

            char c = currentChar;
            if (c == '=') {
                advance();
                if (currentChar == '=') {
                    advance();
                    return Token{T_EQ, "==", line, startCol};
                }
                return Token{T_EQUALS, "=", line, startCol};
            }
            if (c == '!') {
                advance();
                if (currentChar == '=') {
                    advance();
                    return Token{T_NE, "!=", line, startCol};
                }
                throw std::runtime_error("Syntax Error: Unexpected character '!' on line " + std::to_string(line) + ". Did you mean 'not'?");
            }
            if (c == '<') {
                advance();
                if (currentChar == '=') {
                    advance();
                    return Token{T_LE, "<=", line, startCol};
                }
                return Token{T_LT, "<", line, startCol};
            }
            if (c == '>') {
                advance();
                if (currentChar == '=') {
                    advance();
                    return Token{T_GE, ">=", line, startCol};
                }
                return Token{T_GT, ">", line, startCol};
            }
            if (c == '+') { advance(); return Token{T_PLUS, "+", line, startCol}; }
            if (c == '-') { advance(); return Token{T_MINUS, "-", line, startCol}; }
            if (c == '*') { advance(); return Token{T_MUL, "*", line, startCol}; }
            if (c == '/') { advance(); return Token{T_DIV, "/", line, startCol}; }
            if (c == '%') { advance(); return Token{T_MOD, "%", line, startCol}; }
            if (c == '(') { advance(); return Token{T_LPAREN, "(", line, startCol}; }
            if (c == ')') { advance(); return Token{T_RPAREN, ")", line, startCol}; }
            if (c == '{') { advance(); return Token{T_LBRACE, "{", line, startCol}; }
            if (c == '}') { advance(); return Token{T_RBRACE, "}", line, startCol}; }
            if (c == ',') { advance(); return Token{T_COMMA, ",", line, startCol}; }
            if (c == ';') { advance(); return Token{T_SEMICOLON, ";", line, startCol}; }
            if (c == '[') { advance(); return Token{T_LBRACKET, "[", line, startCol}; }
            if (c == ']') { advance(); return Token{T_RBRACKET, "]", line, startCol}; }
            if (c == '.') { advance(); return Token{T_DOT, ".", line, startCol}; }

            throw std::runtime_error("Syntax Error: Unexpected character '" + std::string(1, c) + "' on line " + std::to_string(line));
        }
        return Token{T_EOF, "", line, static_cast<int>(pos) - lineStart + 1};
    }
};

// ==================================================
// 2. Abstract Syntax Tree (AST) Nodes
// ==================================================

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual std::string typeName() const = 0;
};

struct ProgramNode : ASTNode {
    std::vector<std::unique_ptr<ASTNode>> declarations; // functions & structs
    std::string typeName() const override { return "Program"; }
};

struct FunctionDeclNode : ASTNode {
    std::string name;
    std::vector<std::string> params;
    std::vector<std::unique_ptr<ASTNode>> body;
    bool isExtern;
    std::string typeName() const override { return "FunctionDecl"; }
};

struct StructDeclNode : ASTNode {
    std::string name;
    std::vector<std::string> members;
    std::string typeName() const override { return "StructDecl"; }
};

struct VarDeclNode : ASTNode {
    std::string name;
    std::unique_ptr<ASTNode> expr;
    std::string typeName() const override { return "VarDecl"; }
};

struct PrintNode : ASTNode {
    std::unique_ptr<ASTNode> expr;
    std::string typeName() const override { return "Print"; }
};

struct IfNode : ASTNode {
    std::unique_ptr<ASTNode> cond;
    std::vector<std::unique_ptr<ASTNode>> thenBranch;
    std::vector<std::unique_ptr<ASTNode>> elseBranch;
    bool hasElse;
    std::string typeName() const override { return "If"; }
};

struct WhileNode : ASTNode {
    std::unique_ptr<ASTNode> cond;
    std::vector<std::unique_ptr<ASTNode>> body;
    std::string typeName() const override { return "While"; }
};

struct ForNode : ASTNode {
    std::unique_ptr<ASTNode> init;       // var i = 0
    std::unique_ptr<ASTNode> cond;       // i < 10
    std::unique_ptr<ASTNode> update;     // i = i + 1
    std::vector<std::unique_ptr<ASTNode>> body;
    std::string typeName() const override { return "For"; }
};

struct ForInNode : ASTNode {
    std::string varName;                 // item
    std::unique_ptr<ASTNode> iterable;   // myArray
    std::vector<std::unique_ptr<ASTNode>> body;
    std::string typeName() const override { return "ForIn"; }
};

struct BreakNode : ASTNode {
    std::string typeName() const override { return "Break"; }
};

struct ContinueNode : ASTNode {
    std::string typeName() const override { return "Continue"; }
};

struct ReturnNode : ASTNode {
    std::unique_ptr<ASTNode> expr;
    std::string typeName() const override { return "Return"; }
};

struct ExpressionStatementNode : ASTNode {
    std::unique_ptr<ASTNode> expr;
    std::string typeName() const override { return "ExpressionStatement"; }
};

struct FuncCallNode : ASTNode {
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> args;
    std::string typeName() const override { return "FuncCall"; }
};

struct BinOpNode : ASTNode {
    std::unique_ptr<ASTNode> left;
    std::string op;
    std::unique_ptr<ASTNode> right;
    std::string typeName() const override { return "BinOp"; }
};

struct UnaryOpNode : ASTNode {
    std::string op;                      // "-" or "not"
    std::unique_ptr<ASTNode> operand;
    std::string typeName() const override { return "UnaryOp"; }
};

struct LiteralNode : ASTNode {
    std::string value;
    std::string valueType; // "NUMBER", "STRING", or "BOOL"
    std::string typeName() const override { return "Literal"; }
};

struct IdentifierNode : ASTNode {
    std::string name;
    std::string typeName() const override { return "Identifier"; }
};

struct ArrayLiteralNode : ASTNode {
    std::vector<std::unique_ptr<ASTNode>> elements;
    std::string typeName() const override { return "ArrayLiteral"; }
};

struct ArrayIndexNode : ASTNode {
    std::unique_ptr<ASTNode> expr;
    std::unique_ptr<ASTNode> index;
    std::string typeName() const override { return "ArrayIndex"; }
};

// ==================================================
// 3. Parser
// ==================================================

class Parser {
public:
    Lexer& lexer;
    Token currentToken;
    Token peekToken;

    Parser(Lexer& lex) : lexer(lex) {
        currentToken = lexer.getNextToken();
        peekToken = lexer.getNextToken();
    }

    void error(const std::string& msg) {
        int lineNum = currentToken.line;
        int colNum = currentToken.column;
        const auto& lines = lexer.lines;

        std::ostringstream errorMsg;
        errorMsg << "on line " << lineNum << ", column " << colNum << ": " << msg << "\n\n";

        int startLine = std::max(1, lineNum - 1);
        int endLine = std::min((int)lines.size(), lineNum + 1);

        for (int l = startLine; l <= endLine; l++) {
            std::string lineContent = lines[l - 1];
            bool isTarget = (l == lineNum);
            std::string prefix = isTarget ? "  > " + std::to_string(l) + " | " : "    " + std::to_string(l) + " | ";
            errorMsg << prefix << lineContent << "\n";

            if (isTarget) {
                size_t pipeIdx = prefix.find('|');
                std::string padding = std::string(pipeIdx, ' ') + "| " + std::string(colNum - 1, ' ');
                errorMsg << padding << "^\n";
            }
        }
        throw std::runtime_error(errorMsg.str());
    }

    void consume(TokenType expectedType) {
        if (currentToken.type == expectedType) {
            currentToken = peekToken;
            peekToken = lexer.getNextToken();
        } else {
            error("Expected token type of matching syntax, but got mismatch");
        }
    }

    std::unique_ptr<ProgramNode> parse() {
        auto prog = std::make_unique<ProgramNode>();
        while (currentToken.type != T_EOF) {
            if (currentToken.type == T_FN || currentToken.type == T_EXTERN) {
                prog->declarations.push_back(parseFunction());
            } else if (currentToken.type == T_STRUCT) {
                prog->declarations.push_back(parseStruct());
            } else {
                error("Expected function or struct declaration, found '" + currentToken.value + "'");
            }
        }
        return prog;
    }

    std::unique_ptr<StructDeclNode> parseStruct() {
        consume(T_STRUCT);
        std::string name = currentToken.value;
        consume(T_IDENTIFIER);
        consume(T_LBRACE);

        std::vector<std::string> members;
        while (currentToken.type != T_RBRACE && currentToken.type != T_EOF) {
            if (currentToken.type == T_VAR) {
                consume(T_VAR);
            }
            members.push_back(currentToken.value);
            consume(T_IDENTIFIER);
        }
        consume(T_RBRACE);

        auto node = std::make_unique<StructDeclNode>();
        node->name = name;
        node->members = members;
        return node;
    }

    std::unique_ptr<FunctionDeclNode> parseFunction() {
        bool isExtern = false;
        if (currentToken.type == T_EXTERN) {
            consume(T_EXTERN);
            isExtern = true;
        }
        consume(T_FN);
        std::string name = currentToken.value;
        consume(T_IDENTIFIER);
        consume(T_LPAREN);

        std::vector<std::string> params;
        if (currentToken.type != T_RPAREN) {
            params.push_back(currentToken.value);
            consume(T_IDENTIFIER);
            while (currentToken.type == T_COMMA) {
                consume(T_COMMA);
                params.push_back(currentToken.value);
                consume(T_IDENTIFIER);
            }
        }
        consume(T_RPAREN);
        consume(T_LBRACE);

        std::vector<std::unique_ptr<ASTNode>> body;
        while (currentToken.type != T_RBRACE && currentToken.type != T_EOF) {
            body.push_back(parseStatement());
        }
        consume(T_RBRACE);

        auto node = std::make_unique<FunctionDeclNode>();
        node->name = name;
        node->params = params;
        node->body = std::move(body);
        node->isExtern = isExtern;
        return node;
    }

    std::unique_ptr<ASTNode> parseStatement() {
        if (currentToken.type == T_VAR) {
            return parseVarDecl();
        } else if (currentToken.type == T_PRINT) {
            return parsePrint();
        } else if (currentToken.type == T_IF) {
            return parseIf();
        } else if (currentToken.type == T_WHILE) {
            return parseWhile();
        } else if (currentToken.type == T_FOR) {
            return parseFor();
        } else if (currentToken.type == T_RETURN) {
            return parseReturn();
        } else if (currentToken.type == T_BREAK) {
            consume(T_BREAK);
            return std::make_unique<BreakNode>();
        } else if (currentToken.type == T_CONTINUE) {
            consume(T_CONTINUE);
            return std::make_unique<ContinueNode>();
        } else {
            auto expr = parseExpression();
            auto stmt = std::make_unique<ExpressionStatementNode>();
            stmt->expr = std::move(expr);
            return stmt;
        }
    }

    std::unique_ptr<ASTNode> parseVarDecl() {
        consume(T_VAR);
        std::string name = currentToken.value;
        consume(T_IDENTIFIER);
        consume(T_EQUALS);
        auto expr = parseExpression();

        auto node = std::make_unique<VarDeclNode>();
        node->name = name;
        node->expr = std::move(expr);
        return node;
    }

    std::unique_ptr<ASTNode> parsePrint() {
        consume(T_PRINT);
        consume(T_LPAREN);
        auto expr = parseExpression();
        consume(T_RPAREN);

        auto node = std::make_unique<PrintNode>();
        node->expr = std::move(expr);
        return node;
    }

    std::unique_ptr<ASTNode> parseIf() {
        consume(T_IF);
        auto cond = parseExpression();
        consume(T_LBRACE);

        std::vector<std::unique_ptr<ASTNode>> thenBranch;
        while (currentToken.type != T_RBRACE && currentToken.type != T_EOF) {
            thenBranch.push_back(parseStatement());
        }
        consume(T_RBRACE);

        std::vector<std::unique_ptr<ASTNode>> elseBranch;
        bool hasElse = false;
        if (currentToken.type == T_ELSE) {
            consume(T_ELSE);
            hasElse = true;

            // Support "else if" chains
            if (currentToken.type == T_IF) {
                // Parse the nested if as a single statement in the else branch
                elseBranch.push_back(parseIf());
            } else {
                consume(T_LBRACE);
                while (currentToken.type != T_RBRACE && currentToken.type != T_EOF) {
                    elseBranch.push_back(parseStatement());
                }
                consume(T_RBRACE);
            }
        }

        auto node = std::make_unique<IfNode>();
        node->cond = std::move(cond);
        node->thenBranch = std::move(thenBranch);
        node->elseBranch = std::move(elseBranch);
        node->hasElse = hasElse;
        return node;
    }

    std::unique_ptr<ASTNode> parseWhile() {
        consume(T_WHILE);
        auto cond = parseExpression();
        consume(T_LBRACE);

        std::vector<std::unique_ptr<ASTNode>> body;
        while (currentToken.type != T_RBRACE && currentToken.type != T_EOF) {
            body.push_back(parseStatement());
        }
        consume(T_RBRACE);

        auto node = std::make_unique<WhileNode>();
        node->cond = std::move(cond);
        node->body = std::move(body);
        return node;
    }

    std::unique_ptr<ASTNode> parseFor() {
        consume(T_FOR);

        // Detect for..in syntax: "for <identifier> in <expr>"
        if (currentToken.type == T_IDENTIFIER && peekToken.type == T_IN) {
            return parseForIn();
        }

        // C-style for: "for var i = 0; i < 10; i = i + 1 { ... }"
        // Parse init statement
        std::unique_ptr<ASTNode> init;
        if (currentToken.type == T_VAR) {
            init = parseVarDecl();
        } else {
            init = parseExpression();
        }
        consume(T_SEMICOLON);

        // Parse condition
        auto cond = parseExpression();
        consume(T_SEMICOLON);

        // Parse update expression
        auto update = parseExpression();

        // Parse body
        consume(T_LBRACE);
        std::vector<std::unique_ptr<ASTNode>> body;
        while (currentToken.type != T_RBRACE && currentToken.type != T_EOF) {
            body.push_back(parseStatement());
        }
        consume(T_RBRACE);

        auto node = std::make_unique<ForNode>();
        node->init = std::move(init);
        node->cond = std::move(cond);
        node->update = std::move(update);
        node->body = std::move(body);
        return node;
    }

    std::unique_ptr<ASTNode> parseForIn() {
        // Already consumed T_FOR, current is identifier
        std::string varName = currentToken.value;
        consume(T_IDENTIFIER);
        consume(T_IN);

        auto iterable = parseExpression();

        consume(T_LBRACE);
        std::vector<std::unique_ptr<ASTNode>> body;
        while (currentToken.type != T_RBRACE && currentToken.type != T_EOF) {
            body.push_back(parseStatement());
        }
        consume(T_RBRACE);

        auto node = std::make_unique<ForInNode>();
        node->varName = varName;
        node->iterable = std::move(iterable);
        node->body = std::move(body);
        return node;
    }

    std::unique_ptr<ASTNode> parseReturn() {
        consume(T_RETURN);
        std::unique_ptr<ASTNode> expr = nullptr;
        if (currentToken.type == T_NUMBER || currentToken.type == T_STRING ||
            currentToken.type == T_IDENTIFIER || currentToken.type == T_LPAREN ||
            currentToken.type == T_LBRACKET || currentToken.type == T_TRUE ||
            currentToken.type == T_FALSE || currentToken.type == T_NOT ||
            currentToken.type == T_MINUS) {
            expr = parseExpression();
        }
        auto node = std::make_unique<ReturnNode>();
        node->expr = std::move(expr);
        return node;
    }

    // ==================================================
    // Expression Precedence Chain (lowest to highest):
    //   Assignment (=)
    //   → Logical OR (or)
    //   → Logical AND (and)
    //   → Comparison (==, !=, <, >, <=, >=)
    //   → Additive (+, -)
    //   → Multiplicative (*, /, %)
    //   → Unary (-, not)
    //   → Postfix (., [], ())
    //   → Primary (literals, identifiers, grouping)
    // ==================================================

    std::unique_ptr<ASTNode> parseExpression() {
        auto node = parseLogicalOr();
        if (currentToken.type == T_EQUALS) {
            consume(T_EQUALS);
            auto right = parseExpression();
            auto binOp = std::make_unique<BinOpNode>();
            binOp->left = std::move(node);
            binOp->op = "=";
            binOp->right = std::move(right);
            return binOp;
        }
        return node;
    }

    std::unique_ptr<ASTNode> parseLogicalOr() {
        auto node = parseLogicalAnd();
        while (currentToken.type == T_OR) {
            consume(T_OR);
            auto right = parseLogicalAnd();
            auto binOp = std::make_unique<BinOpNode>();
            binOp->left = std::move(node);
            binOp->op = "or";
            binOp->right = std::move(right);
            node = std::move(binOp);
        }
        return node;
    }

    std::unique_ptr<ASTNode> parseLogicalAnd() {
        auto node = parseComparison();
        while (currentToken.type == T_AND) {
            consume(T_AND);
            auto right = parseComparison();
            auto binOp = std::make_unique<BinOpNode>();
            binOp->left = std::move(node);
            binOp->op = "and";
            binOp->right = std::move(right);
            node = std::move(binOp);
        }
        return node;
    }

    std::unique_ptr<ASTNode> parseComparison() {
        auto node = parseAdditive();
        while (currentToken.type == T_LT || currentToken.type == T_GT || currentToken.type == T_LE || currentToken.type == T_GE || currentToken.type == T_EQ || currentToken.type == T_NE) {
            std::string op = currentToken.value;
            consume(currentToken.type);
            auto right = parseAdditive();
            auto binOp = std::make_unique<BinOpNode>();
            binOp->left = std::move(node);
            binOp->op = op;
            binOp->right = std::move(right);
            node = std::move(binOp);
        }
        return node;
    }

    std::unique_ptr<ASTNode> parseAdditive() {
        auto node = parseTerm();
        while (currentToken.type == T_PLUS || currentToken.type == T_MINUS) {
            std::string op = currentToken.value;
            consume(currentToken.type);
            auto right = parseTerm();
            auto binOp = std::make_unique<BinOpNode>();
            binOp->left = std::move(node);
            binOp->op = op;
            binOp->right = std::move(right);
            node = std::move(binOp);
        }
        return node;
    }

    std::unique_ptr<ASTNode> parseTerm() {
        auto node = parseUnary();
        while (currentToken.type == T_MUL || currentToken.type == T_DIV || currentToken.type == T_MOD) {
            std::string op = currentToken.value;
            consume(currentToken.type);
            auto right = parseUnary();
            auto binOp = std::make_unique<BinOpNode>();
            binOp->left = std::move(node);
            binOp->op = op;
            binOp->right = std::move(right);
            node = std::move(binOp);
        }
        return node;
    }

    std::unique_ptr<ASTNode> parseUnary() {
        // Handle unary minus: -expr
        if (currentToken.type == T_MINUS) {
            consume(T_MINUS);
            auto operand = parseUnary(); // Right-recursive for chaining: --x
            auto node = std::make_unique<UnaryOpNode>();
            node->op = "-";
            node->operand = std::move(operand);
            return node;
        }
        // Handle logical not: not expr
        if (currentToken.type == T_NOT) {
            consume(T_NOT);
            auto operand = parseUnary();
            auto node = std::make_unique<UnaryOpNode>();
            node->op = "not";
            node->operand = std::move(operand);
            return node;
        }
        return parseFactor();
    }

    std::unique_ptr<ASTNode> parsePrimary() {
        if (currentToken.type == T_NUMBER) {
            std::string val = currentToken.value;
            consume(T_NUMBER);
            auto node = std::make_unique<LiteralNode>();
            node->value = val;
            node->valueType = "NUMBER";
            return node;
        } else if (currentToken.type == T_STRING) {
            std::string val = currentToken.value;
            consume(T_STRING);
            auto node = std::make_unique<LiteralNode>();
            node->value = val;
            node->valueType = "STRING";
            return node;
        } else if (currentToken.type == T_TRUE) {
            consume(T_TRUE);
            auto node = std::make_unique<LiteralNode>();
            node->value = "true";
            node->valueType = "BOOL";
            return node;
        } else if (currentToken.type == T_FALSE) {
            consume(T_FALSE);
            auto node = std::make_unique<LiteralNode>();
            node->value = "false";
            node->valueType = "BOOL";
            return node;
        } else if (currentToken.type == T_LBRACKET) {
            consume(T_LBRACKET);
            std::vector<std::unique_ptr<ASTNode>> elements;
            if (currentToken.type != T_RBRACKET) {
                elements.push_back(parseExpression());
                while (currentToken.type == T_COMMA) {
                    consume(T_COMMA);
                    elements.push_back(parseExpression());
                }
            }
            consume(T_RBRACKET);
            auto node = std::make_unique<ArrayLiteralNode>();
            node->elements = std::move(elements);
            return node;
        } else if (currentToken.type == T_IDENTIFIER) {
            std::string name = currentToken.value;
            consume(T_IDENTIFIER);
            if (currentToken.type == T_LPAREN) {
                consume(T_LPAREN);
                std::vector<std::unique_ptr<ASTNode>> args;
                if (currentToken.type != T_RPAREN) {
                    args.push_back(parseExpression());
                    while (currentToken.type == T_COMMA) {
                        consume(T_COMMA);
                        args.push_back(parseExpression());
                    }
                }
                consume(T_RPAREN);
                auto node = std::make_unique<FuncCallNode>();
                node->name = name;
                node->args = std::move(args);
                return node;
            }
            auto node = std::make_unique<IdentifierNode>();
            node->name = name;
            return node;
        } else if (currentToken.type == T_LPAREN) {
            consume(T_LPAREN);
            auto node = parseExpression();
            consume(T_RPAREN);
            return node;
        } else {
            error("Unexpected token '" + currentToken.value + "' in expression");
            return nullptr;
        }
    }

    std::unique_ptr<ASTNode> parseFactor() {
        auto node = parsePrimary();

        // Postfix Chain Parser (Array Indexing and Dot Member Lookups)
        while (currentToken.type == T_DOT || currentToken.type == T_LBRACKET) {
            if (currentToken.type == T_DOT) {
                consume(T_DOT);
                std::string member = currentToken.value;
                consume(T_IDENTIFIER);

                auto access = std::make_unique<BinOpNode>();
                access->left = std::move(node);
                access->op = ".";

                auto right = std::make_unique<IdentifierNode>();
                right->name = member;
                access->right = std::move(right);

                node = std::move(access);
            }
            else if (currentToken.type == T_LBRACKET) {
                consume(T_LBRACKET);
                auto index = parseExpression();
                consume(T_RBRACKET);

                auto ai = std::make_unique<ArrayIndexNode>();
                ai->expr = std::move(node);
                ai->index = std::move(index);

                node = std::move(ai);
            }
        }
        return node;
    }
};

// ==================================================
// 4. Code Generator
// ==================================================

class CodeGenerator {
public:
    int indentLevel;

    CodeGenerator() : indentLevel(0) {}

    std::string indent() {
        return std::string(indentLevel * 4, ' ');
    }

    std::string escapeString(const std::string& str) {
        std::string result = "";
        for (char c : str) {
            if (c == '"') result += "\\\"";
            else if (c == '\n') result += "\\n";
            else if (c == '\t') result += "\\t";
            else if (c == '\\') result += "\\\\";
            else result.push_back(c);
        }
        return result;
    }

    std::string generate(const ASTNode* node) {
        if (!node) return "";
        std::string tn = node->typeName();

        if (tn == "Program") {
            const auto* p = static_cast<const ProgramNode*>(node);
            std::string code = "#include <iostream>\n#include <string>\n#include <cmath>\n#include \"aura_stdlib.h\"\n\n";
            for (const auto& decl : p->declarations) {
                code += generate(decl.get()) + "\n";
            }
            return code;
        }
        else if (tn == "StructDecl") {
            const auto* s = static_cast<const StructDeclNode*>(node);
            std::string templateParams = "";
            std::string memberDecls = "";
            for (size_t i = 0; i < s->members.size(); ++i) {
                std::string m = s->members[i];
                templateParams += "typename T_" + m + " = std::string";
                memberDecls += "    T_" + m + " " + m + ";\n";
                if (i + 1 < s->members.size()) templateParams += ", ";
            }
            std::string code = "template <" + templateParams + ">\n";
            code += "struct " + s->name + " {\n";
            code += memberDecls;
            code += "};\n";
            return code;
        }
        else if (tn == "FunctionDecl") {
            const auto* f = static_cast<const FunctionDeclNode*>(node);
            std::string retType = f->name == "main" ? "int" : (f->isExtern ? "double" : "auto");
            std::string paramsCode = "";
            for (size_t i = 0; i < f->params.size(); ++i) {
                paramsCode += f->isExtern ? "double " + f->params[i] : "auto " + f->params[i];
                if (i + 1 < f->params.size()) paramsCode += ", ";
            }

            std::string prefix = f->isExtern ? "extern \"C\" " : "";
            std::string code = prefix + retType + " " + f->name + "(" + paramsCode + ") {\n";
            indentLevel++;
            for (const auto& stmt : f->body) {
                code += indent() + generate(stmt.get()) + ";\n";
            }
            if (f->name == "main") {
                code += indent() + "return 0;\n";
            }
            indentLevel--;
            code += indent() + "}\n";
            return code;
        }
        else if (tn == "VarDecl") {
            const auto* v = static_cast<const VarDeclNode*>(node);
            return "auto " + v->name + " = " + generate(v->expr.get());
        }
        else if (tn == "Print") {
            const auto* p = static_cast<const PrintNode*>(node);
            return "std::cout << " + generate(p->expr.get()) + " << std::endl";
        }
        else if (tn == "If") {
            const auto* i = static_cast<const IfNode*>(node);
            std::string code = "if (" + generate(i->cond.get()) + ") {\n";
            indentLevel++;
            for (const auto& stmt : i->thenBranch) {
                code += indent() + generate(stmt.get()) + ";\n";
            }
            indentLevel--;
            code += indent() + "}";
            if (i->hasElse) {
                // Check if else branch is a single "else if" (IfNode)
                if (i->elseBranch.size() == 1 && i->elseBranch[0]->typeName() == "If") {
                    code += " else " + generate(i->elseBranch[0].get());
                } else {
                    code += " else {\n";
                    indentLevel++;
                    for (const auto& stmt : i->elseBranch) {
                        code += indent() + generate(stmt.get()) + ";\n";
                    }
                    indentLevel--;
                    code += indent() + "}";
                }
            }
            return code;
        }
        else if (tn == "While") {
            const auto* w = static_cast<const WhileNode*>(node);
            std::string code = "while (" + generate(w->cond.get()) + ") {\n";
            indentLevel++;
            for (const auto& stmt : w->body) {
                code += indent() + generate(stmt.get()) + ";\n";
            }
            indentLevel--;
            code += indent() + "}";
            return code;
        }
        else if (tn == "For") {
            const auto* f = static_cast<const ForNode*>(node);
            std::string initCode = generate(f->init.get());
            std::string condCode = generate(f->cond.get());
            std::string updateCode = generate(f->update.get());
            std::string code = "for (" + initCode + "; " + condCode + "; " + updateCode + ") {\n";
            indentLevel++;
            for (const auto& stmt : f->body) {
                code += indent() + generate(stmt.get()) + ";\n";
            }
            indentLevel--;
            code += indent() + "}";
            return code;
        }
        else if (tn == "ForIn") {
            const auto* fi = static_cast<const ForInNode*>(node);
            std::string code = "for (auto& " + fi->varName + " : " + generate(fi->iterable.get()) + ") {\n";
            indentLevel++;
            for (const auto& stmt : fi->body) {
                code += indent() + generate(stmt.get()) + ";\n";
            }
            indentLevel--;
            code += indent() + "}";
            return code;
        }
        else if (tn == "Break") {
            return "break";
        }
        else if (tn == "Continue") {
            return "continue";
        }
        else if (tn == "Return") {
            const auto* r = static_cast<const ReturnNode*>(node);
            if (r->expr) {
                return "return " + generate(r->expr.get());
            }
            return "return";
        }
        else if (tn == "ExpressionStatement") {
            const auto* es = static_cast<const ExpressionStatementNode*>(node);
            return generate(es->expr.get());
        }
        else if (tn == "FuncCall") {
            const auto* fc = static_cast<const FuncCallNode*>(node);
            if (fc->name == "size" && fc->args.size() == 1) {
                return "(" + generate(fc->args[0].get()) + ").size()";
            }
            std::string argsCode = "";
            for (size_t i = 0; i < fc->args.size(); ++i) {
                argsCode += generate(fc->args[i].get());
                if (i + 1 < fc->args.size()) argsCode += ", ";
            }
            return fc->name + "(" + argsCode + ")";
        }
        else if (tn == "BinOp") {
            const auto* b = static_cast<const BinOpNode*>(node);
            if (b->op == ".") {
                return "(" + generate(b->left.get()) + "." + generate(b->right.get()) + ")";
            }
            if (b->op == "and") {
                return "(" + generate(b->left.get()) + " && " + generate(b->right.get()) + ")";
            }
            if (b->op == "or") {
                return "(" + generate(b->left.get()) + " || " + generate(b->right.get()) + ")";
            }
            if (b->op == "%") {
                return "fmod(" + generate(b->left.get()) + ", " + generate(b->right.get()) + ")";
            }
            return "(" + generate(b->left.get()) + " " + b->op + " " + generate(b->right.get()) + ")";
        }
        else if (tn == "UnaryOp") {
            const auto* u = static_cast<const UnaryOpNode*>(node);
            if (u->op == "not") {
                return "(!" + generate(u->operand.get()) + ")";
            }
            return "(-" + generate(u->operand.get()) + ")";
        }
        else if (tn == "Literal") {
            const auto* l = static_cast<const LiteralNode*>(node);
            if (l->valueType == "STRING") {
                return "std::string(\"" + escapeString(l->value) + "\")";
            }
            if (l->valueType == "BOOL") {
                return l->value; // "true" or "false" — same in C++
            }
            return l->value;
        }
        else if (tn == "Identifier") {
            const auto* id = static_cast<const IdentifierNode*>(node);
            return id->name;
        }
        else if (tn == "ArrayLiteral") {
            const auto* al = static_cast<const ArrayLiteralNode*>(node);
            std::string code = "std::vector{";
            for (size_t i = 0; i < al->elements.size(); ++i) {
                code += generate(al->elements[i].get());
                if (i + 1 < al->elements.size()) code += ", ";
            }
            code += "}";
            return code;
        }
        else if (tn == "ArrayIndex") {
            const auto* ai = static_cast<const ArrayIndexNode*>(node);
            return generate(ai->expr.get()) + "[" + generate(ai->index.get()) + "]";
        }
        return "";
    }
};

// ==================================================
// 5. Main CLI Runner
// ==================================================

bool compilerExists(const std::string& name) {
#ifdef _WIN32
    std::string cmd = "where " + name + " >nul 2>nul";
#else
    std::string cmd = "which " + name + " >/dev/null 2>&1";
#endif
    return std::system(cmd.c_str()) == 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Aura Native Compiler v2.0\n";
        std::cout << "Usage: aurac <source_file.aura> [-o <output_name>]\n";
        return 1;
    }

    std::string sourcePath = argv[1];
    if (!fs::exists(sourcePath)) {
        std::cout << "Error: Source file '" << sourcePath << "' does not exist.\n";
        return 1;
    }

    std::string outputName = "";
    for (int i = 2; i < argc; ++i) {
        if (std::string(argv[i]) == "-o" && i + 1 < argc) {
            outputName = argv[i + 1];
            break;
        }
    }

    if (outputName.empty()) {
        fs::path p(sourcePath);
        outputName = p.stem().string();
    }

    // 1. Read source code
    std::ifstream file(sourcePath);
    if (!file.is_open()) {
        std::cout << "Error: Failed to open source file.\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();
    file.close();

    std::cout << "[Aura] Compiling '" << sourcePath << "'...\n";

    // 2. Compile to C++ code
    std::string cppCode;
    try {
        Lexer lexer(sourceCode);
        Parser parser(lexer);
        auto ast = parser.parse();
        CodeGenerator generator;
        cppCode = generator.generate(ast.get());
    } catch (const std::exception& e) {
        std::cout << "\n[Compilation Failed] " << e.what() << "\n";
        return 1;
    }

    // 3. Write C++ code to file
    std::string cppFilePath = outputName + ".cpp";
    std::ofstream outCpp(cppFilePath);
    if (!outCpp.is_open()) {
        std::cout << "Error: Failed to write C++ source code file.\n";
        return 1;
    }
    outCpp << cppCode;
    outCpp.close();
    std::cout << "[Aura] Generated C++ source code: '" << cppFilePath << "'\n";

    // 4. Locate compiler
    std::string compiler = "";
    std::vector<std::string> compilers = {"clang++", "g++", "cl"};
    for (const auto& c : compilers) {
        if (compilerExists(c)) {
            compiler = c;
            break;
        }
    }

    // Check custom winget install path
    std::string localLLVM = "C:\\Users\\aryan\\AppData\\Local\\Microsoft\\WinGet\\Packages\\MartinStorsjo.LLVM-MinGW.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\\llvm-mingw-20260616-ucrt-x86_64\\bin\\clang++.exe";
    if (compiler.empty() && fs::exists(localLLVM)) {
        compiler = localLLVM;
        std::cout << "[Aura] Detected local compiler: '" << compiler << "'\n";
    }

    if (compiler.empty()) {
        std::cout << "\n[Warning] No C++ compiler (g++, clang++, or cl) found in your PATH.\n";
        std::cout << "[Warning] Generated C++ code is saved to '" << cppFilePath << "'.\n";
        std::cout << "[Warning] Install a C++ compiler to generate native binaries.\n";
        return 0;
    }

    // 5. Compile C++ to binary
    std::cout << "[Aura] Using C++ compiler: '" << compiler << "'\n";
#ifdef _WIN32
    std::string exeName = outputName + ".exe";
    bool isWindows = true;
#else
    std::string exeName = outputName;
    bool isWindows = false;
#endif

    std::string compileCmd;
    if (compiler == "cl") {
        compileCmd = "cl /EHsc /std:c++20 " + cppFilePath + " /Fe:" + exeName + " ws2_32.lib";
    } else {
        std::string linkFlags = isWindows ? " -lws2_32" : "";
        compileCmd = "\"" + compiler + "\" -std=c++20 -static -O3 " + cppFilePath + " -o " + exeName + linkFlags;
    }

    std::cout << "[Aura] Running: " << compileCmd << "\n";
    int ret = std::system(compileCmd.c_str());

    if (ret == 0) {
        std::cout << "\n[Success] Standalone native binary created: '" << exeName << "'\n";
        if (compiler == "cl") {
            std::string objFile = outputName + ".obj";
            if (fs::exists(objFile)) {
                fs::remove(objFile);
            }
        }
    } else {
        std::cout << "\n[Error] C++ Compilation failed.\n";
        return 1;
    }

    return 0;
}
