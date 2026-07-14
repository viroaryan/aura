// ==================================================
// Aura Programming Language Native Virtual Machine (auravm)
// Version 3.2 — The Functions Update
// Written in C++17 - No external dependencies
// ==================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
#include <memory>
#include <cmath>
#include <cstdlib>
#include <cctype>

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0A00
#include "httplib.h"
#include <functional>
#include <cstdlib>

// ==================================================
// 1. Value System (Dynamic Types)
// ==================================================

struct ValueArray;
struct AuraFunction;
struct NativeFunction;
struct AuraClass;
struct AuraInstance;
struct AuraBoundMethod;

using Value = std::variant<double, bool, std::string, std::shared_ptr<ValueArray>, std::shared_ptr<AuraFunction>, std::shared_ptr<NativeFunction>, std::shared_ptr<AuraClass>, std::shared_ptr<AuraInstance>, std::shared_ptr<AuraBoundMethod>>;

struct ValueArray {
    std::vector<Value> elements;
};

struct NativeFunction {
    std::function<Value(const std::vector<Value>&)> func;
};

struct AuraFunction {
    std::string name;
    int arity;
    int entryPoint;
    std::vector<std::string> paramNames;
};

struct AuraClass {
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<AuraFunction>> methods;
};

struct AuraInstance {
    std::shared_ptr<AuraClass> klass;
    std::unordered_map<std::string, Value> fields;
};

struct AuraBoundMethod {
    Value receiver;
    std::shared_ptr<AuraFunction> method;
};

std::string valueToString(const Value& val) {
    if (std::holds_alternative<double>(val)) {
        double d = std::get<double>(val);
        if (d == static_cast<int>(d)) return std::to_string(static_cast<int>(d));
        return std::to_string(d);
    } else if (std::holds_alternative<bool>(val)) {
        return std::get<bool>(val) ? "true" : "false";
    } else if (std::holds_alternative<std::string>(val)) {
        return std::get<std::string>(val);
    } else if (std::holds_alternative<std::shared_ptr<ValueArray>>(val)) {
        auto arr = std::get<std::shared_ptr<ValueArray>>(val);
        std::string res = "[";
        for (size_t i = 0; i < arr->elements.size(); ++i) {
            if (std::holds_alternative<std::string>(arr->elements[i])) res += "\"" + valueToString(arr->elements[i]) + "\"";
            else res += valueToString(arr->elements[i]);
            if (i + 1 < arr->elements.size()) res += ", ";
        }
        return res + "]";
    } else if (std::holds_alternative<std::shared_ptr<AuraFunction>>(val)) {
        return "<fn " + std::get<std::shared_ptr<AuraFunction>>(val)->name + ">";
    } else if (std::holds_alternative<std::shared_ptr<NativeFunction>>(val)) {
        return "<native fn>";
    } else if (std::holds_alternative<std::shared_ptr<AuraClass>>(val)) {
        return "<class " + std::get<std::shared_ptr<AuraClass>>(val)->name + ">";
    } else if (std::holds_alternative<std::shared_ptr<AuraInstance>>(val)) {
        return "<" + std::get<std::shared_ptr<AuraInstance>>(val)->klass->name + " instance>";
    } else if (std::holds_alternative<std::shared_ptr<AuraBoundMethod>>(val)) {
        return "<bound method " + std::get<std::shared_ptr<AuraBoundMethod>>(val)->method->name + ">";
    }
    return "null";
}

bool valueIsFalsy(const Value& val) {
    if (std::holds_alternative<bool>(val)) return !std::get<bool>(val);
    if (std::holds_alternative<double>(val)) return std::get<double>(val) == 0.0;
    if (std::holds_alternative<std::string>(val)) return std::get<std::string>(val).empty();
    if (std::holds_alternative<std::shared_ptr<ValueArray>>(val)) return false;
    if (std::holds_alternative<std::shared_ptr<AuraFunction>>(val)) return false;
    if (std::holds_alternative<std::shared_ptr<NativeFunction>>(val)) return false;
    if (std::holds_alternative<std::shared_ptr<AuraClass>>(val)) return false;
    if (std::holds_alternative<std::shared_ptr<AuraInstance>>(val)) return false;
    if (std::holds_alternative<std::shared_ptr<AuraBoundMethod>>(val)) return false;
    return true;
}

bool valuesEqual(const Value& a, const Value& b) {
    if (a.index() != b.index()) return false;
    if (std::holds_alternative<double>(a)) return std::get<double>(a) == std::get<double>(b);
    if (std::holds_alternative<bool>(a)) return std::get<bool>(a) == std::get<bool>(b);
    if (std::holds_alternative<std::string>(a)) return std::get<std::string>(a) == std::get<std::string>(b);
    if (std::holds_alternative<std::shared_ptr<ValueArray>>(a)) return std::get<std::shared_ptr<ValueArray>>(a) == std::get<std::shared_ptr<ValueArray>>(b);
    if (std::holds_alternative<std::shared_ptr<AuraFunction>>(a)) return std::get<std::shared_ptr<AuraFunction>>(a) == std::get<std::shared_ptr<AuraFunction>>(b);
    return false; // Native functions are never equal
}

// ==================================================
// 2. OpCodes & Bytecode Chunk
// ==================================================

enum OpCode : uint8_t {
    OP_CONSTANT,      
    OP_TRUE,
    OP_FALSE,
    OP_POP,           
    OP_GET_GLOBAL,    
    OP_DEFINE_GLOBAL, 
    OP_SET_GLOBAL,    
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MOD,
    OP_NOT,
    OP_NEGATE,
    OP_PRINT,         
    OP_JUMP,          
    OP_JUMP_IF_FALSE, 
    OP_LOOP,          
    OP_BUILD_ARRAY,
    OP_INDEX_GET,
    OP_INDEX_SET,
    OP_CALL,
    OP_RETURN_VALUE,
    OP_RETURN,
    OP_CLASS,
    OP_METHOD,
    OP_GET_PROPERTY,
    OP_SET_PROPERTY
};

class Chunk {
public:
    std::vector<uint8_t> code;
    std::vector<Value> constants;
    void write(uint8_t byte) { code.push_back(byte); }
    int addConstant(Value value) { 
        for (int i = 0; i < constants.size(); i++) {
            if (valuesEqual(constants[i], value)) return i;
        }
        constants.push_back(value);
        if (constants.size() > 255) throw std::runtime_error("Too many constants in one chunk (>255)!");
        return constants.size() - 1; 
    }
};

// ==================================================
// 3. Lexer & AST 
// ==================================================

enum AuraTokenType {
    T_VAR, T_PRINT, T_IF, T_ELSE, T_WHILE, T_FOR, T_FN, T_RETURN, T_CLASS,
    T_TRUE, T_FALSE, T_AND, T_OR, T_NOT,
    T_IDENTIFIER, T_NUMBER, T_STRING,
    T_EQUALS, T_EQ, T_NE, T_LT, T_LE, T_GT, T_GE,
    T_PLUS, T_MINUS, T_MUL, T_DIV, T_MOD,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE, T_COMMA, T_SEMICOLON,
    T_LBRACKET, T_RBRACKET, T_DOT,
    T_EOF
};

struct Token { AuraTokenType type; std::string value; int line; };

class Lexer {
public:
    std::string text; size_t pos; int line; char currentChar;
    Lexer(const std::string& src) : text(src), pos(0), line(1) { currentChar = text.empty() ? '\0' : text[0]; }
    void advance() { pos++; currentChar = (pos < text.length()) ? text[pos] : '\0'; }
    void skipWhitespace() {
        while (currentChar != '\0' && std::isspace(static_cast<unsigned char>(currentChar))) {
            if (currentChar == '\n') line++;
            advance();
        }
    }
    Token getNextToken() {
        while (currentChar != '\0') {
            if (std::isspace(static_cast<unsigned char>(currentChar))) { skipWhitespace(); continue; }
            if (currentChar == '/' && pos + 1 < text.length() && text[pos+1] == '/') {
                while (currentChar != '\0' && currentChar != '\n') advance();
                continue;
            }
            if (std::isdigit(static_cast<unsigned char>(currentChar))) {
                std::string res;
                while (std::isdigit(static_cast<unsigned char>(currentChar)) || currentChar == '.') { res += currentChar; advance(); }
                return {T_NUMBER, res, line};
            }
            if (currentChar == '"') {
                std::string res; advance();
                while (currentChar != '\0' && currentChar != '"') {
                    if (currentChar == '\\') {
                        advance();
                        if (currentChar == 'n') res += '\n'; else if (currentChar == 't') res += '\t'; else res += currentChar;
                    } else {
                        res += currentChar;
                    }
                    advance();
                }
                advance(); return {T_STRING, res, line};
            }
            if (std::isalpha(static_cast<unsigned char>(currentChar)) || currentChar == '_') {
                std::string res;
                while (std::isalnum(static_cast<unsigned char>(currentChar)) || currentChar == '_') { res += currentChar; advance(); }
                if (res == "var") return {T_VAR, res, line};
                if (res == "print") return {T_PRINT, res, line};
                if (res == "if") return {T_IF, res, line};
                if (res == "else") return {T_ELSE, res, line};
                if (res == "while") return {T_WHILE, res, line};
                if (res == "for") return {T_FOR, res, line};
                if (res == "fn") return {T_FN, res, line};
                if (res == "return") return {T_RETURN, res, line};
                if (res == "class") return {T_CLASS, res, line};
                if (res == "true") return {T_TRUE, res, line};
                if (res == "false") return {T_FALSE, res, line};
                if (res == "and") return {T_AND, res, line};
                if (res == "or") return {T_OR, res, line};
                if (res == "not") return {T_NOT, res, line};
                return {T_IDENTIFIER, res, line};
            }
            char c = currentChar; advance();
            if (c == '=') { if (currentChar == '=') { advance(); return {T_EQ, "==", line}; } return {T_EQUALS, "=", line}; }
            if (c == '!') { if (currentChar == '=') { advance(); return {T_NE, "!=", line}; } }
            if (c == '<') { if (currentChar == '=') { advance(); return {T_LE, "<=", line}; } return {T_LT, "<", line}; }
            if (c == '>') { if (currentChar == '=') { advance(); return {T_GE, ">=", line}; } return {T_GT, ">", line}; }
            if (c == '+') return {T_PLUS, "+", line}; if (c == '-') return {T_MINUS, "-", line};
            if (c == '*') return {T_MUL, "*", line}; if (c == '/') return {T_DIV, "/", line}; if (c == '%') return {T_MOD, "%", line};
            if (c == '(') return {T_LPAREN, "(", line}; if (c == ')') return {T_RPAREN, ")", line};
            if (c == '{') return {T_LBRACE, "{", line}; if (c == '}') return {T_RBRACE, "}", line};
            if (c == '[') return {T_LBRACKET, "[", line}; if (c == ']') return {T_RBRACKET, "]", line};
            if (c == ';') return {T_SEMICOLON, ";", line}; if (c == ',') return {T_COMMA, ",", line};
            if (c == '.') return {T_DOT, ".", line};
            throw std::runtime_error("Unexpected character: " + std::string(1, c));
        }
        return {T_EOF, "", line};
    }
};

struct ASTNode { virtual ~ASTNode() = default; virtual std::string typeName() const = 0; };
struct ProgramNode : ASTNode { std::vector<std::unique_ptr<ASTNode>> declarations; std::string typeName() const override { return "Program"; } };
struct VarDeclNode : ASTNode { std::string name; std::unique_ptr<ASTNode> expr; std::string typeName() const override { return "VarDecl"; } };
struct PrintNode : ASTNode { std::unique_ptr<ASTNode> expr; std::string typeName() const override { return "Print"; } };
struct IfNode : ASTNode { std::unique_ptr<ASTNode> cond; std::vector<std::unique_ptr<ASTNode>> thenBranch; std::vector<std::unique_ptr<ASTNode>> elseBranch; bool hasElse; std::string typeName() const override { return "If"; } };
struct WhileNode : ASTNode { std::unique_ptr<ASTNode> cond; std::vector<std::unique_ptr<ASTNode>> body; std::string typeName() const override { return "While"; } };
struct ForNode : ASTNode { std::unique_ptr<ASTNode> init; std::unique_ptr<ASTNode> cond; std::unique_ptr<ASTNode> update; std::vector<std::unique_ptr<ASTNode>> body; std::string typeName() const override { return "For"; } };
struct ExpressionStatementNode : ASTNode { std::unique_ptr<ASTNode> expr; std::string typeName() const override { return "ExpressionStatement"; } };
struct BinOpNode : ASTNode { std::unique_ptr<ASTNode> left; std::string op; std::unique_ptr<ASTNode> right; std::string typeName() const override { return "BinOp"; } };
struct UnaryOpNode : ASTNode { std::string op; std::unique_ptr<ASTNode> operand; std::string typeName() const override { return "UnaryOp"; } };
struct LiteralNode : ASTNode { std::string value; std::string valueType; std::string typeName() const override { return "Literal"; } };
struct IdentifierNode : ASTNode { std::string name; std::string typeName() const override { return "Identifier"; } };
struct ArrayLiteralNode : ASTNode { std::vector<std::unique_ptr<ASTNode>> elements; std::string typeName() const override { return "ArrayLiteral"; } };
struct ArrayIndexNode : ASTNode { std::unique_ptr<ASTNode> arrayExpr; std::unique_ptr<ASTNode> indexExpr; std::string typeName() const override { return "ArrayIndex"; } };

struct FunctionDeclNode : ASTNode { 
    std::string name; 
    std::vector<std::string> params; 
    std::vector<std::unique_ptr<ASTNode>> body; 
    std::string typeName() const override { return "FunctionDecl"; } 
};
struct ReturnNode : ASTNode { 
    std::unique_ptr<ASTNode> expr; 
    std::string typeName() const override { return "Return"; } 
};
struct CallNode : ASTNode { 
    std::unique_ptr<ASTNode> callee; 
    std::vector<std::unique_ptr<ASTNode>> arguments; 
    std::string typeName() const override { return "Call"; } 
};
struct ClassDeclNode : ASTNode {
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> methods;
    std::string typeName() const override { return "ClassDecl"; }
};
struct PropertyGetNode : ASTNode {
    std::unique_ptr<ASTNode> object;
    std::string name;
    std::string typeName() const override { return "PropertyGet"; }
};

class Parser {
public:
    Lexer& lexer; Token currentToken, peekToken;
    Parser(Lexer& lex) : lexer(lex) { currentToken = lexer.getNextToken(); peekToken = lexer.getNextToken(); }
    void consume(AuraTokenType expectedType) { if (currentToken.type == expectedType) { currentToken = peekToken; peekToken = lexer.getNextToken(); } else throw std::runtime_error("Syntax error at line " + std::to_string(currentToken.line)); }
    std::unique_ptr<ProgramNode> parse() { auto p = std::make_unique<ProgramNode>(); while (currentToken.type != T_EOF) p->declarations.push_back(parseStatement()); return p; }
    
    std::unique_ptr<ASTNode> parseStatement() {
        if (currentToken.type == T_CLASS) return parseClassDecl();
        if (currentToken.type == T_VAR) return parseVarDecl();
        if (currentToken.type == T_FN) return parseFunctionDecl();
        if (currentToken.type == T_RETURN) return parseReturn();
        if (currentToken.type == T_PRINT) return parsePrint();
        if (currentToken.type == T_IF) return parseIf();
        if (currentToken.type == T_WHILE) return parseWhile();
        if (currentToken.type == T_FOR) return parseFor();
        auto expr = parseExpression();
        auto stmt = std::make_unique<ExpressionStatementNode>(); stmt->expr = std::move(expr);
        return stmt;
    }
    std::unique_ptr<ASTNode> parseClassDecl() {
        consume(T_CLASS); std::string name = currentToken.value; consume(T_IDENTIFIER);
        consume(T_LBRACE);
        std::vector<std::unique_ptr<ASTNode>> methods;
        while (currentToken.type != T_RBRACE && currentToken.type != T_EOF) {
            methods.push_back(parseFunctionDecl());
        }
        consume(T_RBRACE);
        auto node = std::make_unique<ClassDeclNode>();
        node->name = name; node->methods = std::move(methods);
        return node;
    }
    std::unique_ptr<ASTNode> parseFunctionDecl() {
        consume(T_FN);
        std::string name = currentToken.value; consume(T_IDENTIFIER);
        consume(T_LPAREN);
        std::vector<std::string> params;
        if (currentToken.type != T_RPAREN) {
            params.push_back(currentToken.value); consume(T_IDENTIFIER);
            while (currentToken.type == T_COMMA) {
                consume(T_COMMA);
                params.push_back(currentToken.value); consume(T_IDENTIFIER);
            }
        }
        consume(T_RPAREN);
        consume(T_LBRACE);
        std::vector<std::unique_ptr<ASTNode>> body;
        while (currentToken.type != T_RBRACE) body.push_back(parseStatement());
        consume(T_RBRACE);
        auto node = std::make_unique<FunctionDeclNode>();
        node->name = name; node->params = params; node->body = std::move(body);
        return node;
    }
    std::unique_ptr<ASTNode> parseReturn() {
        consume(T_RETURN);
        std::unique_ptr<ASTNode> expr = nullptr;
        if (currentToken.type != T_RBRACE && currentToken.type != T_EOF) {
            expr = parseExpression();
        }
        auto node = std::make_unique<ReturnNode>(); node->expr = std::move(expr); return node;
    }
    std::unique_ptr<ASTNode> parseVarDecl() {
        consume(T_VAR); std::string name = currentToken.value; consume(T_IDENTIFIER); consume(T_EQUALS);
        auto expr = parseExpression();
        auto node = std::make_unique<VarDeclNode>(); node->name = name; node->expr = std::move(expr);
        return node;
    }
    std::unique_ptr<ASTNode> parsePrint() {
        consume(T_PRINT); consume(T_LPAREN); auto expr = parseExpression(); consume(T_RPAREN);
        auto node = std::make_unique<PrintNode>(); node->expr = std::move(expr); return node;
    }
    std::unique_ptr<ASTNode> parseIf() {
        consume(T_IF); auto cond = parseExpression(); consume(T_LBRACE);
        std::vector<std::unique_ptr<ASTNode>> thenBranch;
        while (currentToken.type != T_RBRACE && currentToken.type != T_EOF) thenBranch.push_back(parseStatement());
        consume(T_RBRACE);
        std::vector<std::unique_ptr<ASTNode>> elseBranch; bool hasElse = false;
        if (currentToken.type == T_ELSE) {
            consume(T_ELSE); hasElse = true;
            if (currentToken.type == T_IF) elseBranch.push_back(parseIf());
            else { consume(T_LBRACE); while (currentToken.type != T_RBRACE) elseBranch.push_back(parseStatement()); consume(T_RBRACE); }
        }
        auto node = std::make_unique<IfNode>(); node->cond = std::move(cond); node->thenBranch = std::move(thenBranch); node->elseBranch = std::move(elseBranch); node->hasElse = hasElse; return node;
    }
    std::unique_ptr<ASTNode> parseWhile() {
        consume(T_WHILE); auto cond = parseExpression(); consume(T_LBRACE);
        std::vector<std::unique_ptr<ASTNode>> body;
        while (currentToken.type != T_RBRACE) body.push_back(parseStatement());
        consume(T_RBRACE);
        auto node = std::make_unique<WhileNode>(); node->cond = std::move(cond); node->body = std::move(body); return node;
    }
    std::unique_ptr<ASTNode> parseFor() {
        consume(T_FOR);
        std::unique_ptr<ASTNode> init;
        if (currentToken.type == T_VAR) init = parseVarDecl(); else init = parseExpression();
        consume(T_SEMICOLON);
        auto cond = parseExpression(); consume(T_SEMICOLON);
        auto update = parseExpression(); consume(T_LBRACE);
        std::vector<std::unique_ptr<ASTNode>> body;
        while (currentToken.type != T_RBRACE) body.push_back(parseStatement());
        consume(T_RBRACE);
        auto node = std::make_unique<ForNode>(); node->init = std::move(init); node->cond = std::move(cond); node->update = std::move(update); node->body = std::move(body); return node;
    }
    std::unique_ptr<ASTNode> parseExpression() {
        auto node = parseLogicalOr();
        if (currentToken.type == T_EQUALS) {
            consume(T_EQUALS); auto right = parseExpression();
            auto binOp = std::make_unique<BinOpNode>(); binOp->left = std::move(node); binOp->op = "="; binOp->right = std::move(right); return binOp;
        }
        return node;
    }
    std::unique_ptr<ASTNode> parseLogicalOr() { auto node = parseLogicalAnd(); while (currentToken.type == T_OR) { consume(T_OR); auto right = parseLogicalAnd(); auto b = std::make_unique<BinOpNode>(); b->left = std::move(node); b->op = "or"; b->right = std::move(right); node = std::move(b); } return node; }
    std::unique_ptr<ASTNode> parseLogicalAnd() { auto node = parseComparison(); while (currentToken.type == T_AND) { consume(T_AND); auto right = parseComparison(); auto b = std::make_unique<BinOpNode>(); b->left = std::move(node); b->op = "and"; b->right = std::move(right); node = std::move(b); } return node; }
    std::unique_ptr<ASTNode> parseComparison() { auto node = parseAdditive(); while (currentToken.type >= T_EQ && currentToken.type <= T_GE) { std::string op = currentToken.value; consume(currentToken.type); auto right = parseAdditive(); auto b = std::make_unique<BinOpNode>(); b->left = std::move(node); b->op = op; b->right = std::move(right); node = std::move(b); } return node; }
    std::unique_ptr<ASTNode> parseAdditive() { auto node = parseTerm(); while (currentToken.type == T_PLUS || currentToken.type == T_MINUS) { std::string op = currentToken.value; consume(currentToken.type); auto right = parseTerm(); auto b = std::make_unique<BinOpNode>(); b->left = std::move(node); b->op = op; b->right = std::move(right); node = std::move(b); } return node; }
    std::unique_ptr<ASTNode> parseTerm() { auto node = parseUnary(); while (currentToken.type == T_MUL || currentToken.type == T_DIV || currentToken.type == T_MOD) { std::string op = currentToken.value; consume(currentToken.type); auto right = parseUnary(); auto b = std::make_unique<BinOpNode>(); b->left = std::move(node); b->op = op; b->right = std::move(right); node = std::move(b); } return node; }
    std::unique_ptr<ASTNode> parseUnary() {
        if (currentToken.type == T_MINUS || currentToken.type == T_NOT) { std::string op = currentToken.value; consume(currentToken.type); auto node = std::make_unique<UnaryOpNode>(); node->op = op; node->operand = parseUnary(); return node; }
        return parseFactor();
    }
    std::unique_ptr<ASTNode> parsePrimary() {
        if (currentToken.type == T_NUMBER) { std::string val = currentToken.value; consume(T_NUMBER); auto n = std::make_unique<LiteralNode>(); n->value = val; n->valueType = "NUMBER"; return n; }
        if (currentToken.type == T_STRING) { std::string val = currentToken.value; consume(T_STRING); auto n = std::make_unique<LiteralNode>(); n->value = val; n->valueType = "STRING"; return n; }
        if (currentToken.type == T_TRUE || currentToken.type == T_FALSE) { std::string val = currentToken.value; consume(currentToken.type); auto n = std::make_unique<LiteralNode>(); n->value = val; n->valueType = "BOOL"; return n; }
        if (currentToken.type == T_IDENTIFIER) { auto n = std::make_unique<IdentifierNode>(); n->name = currentToken.value; consume(T_IDENTIFIER); return n; }
        if (currentToken.type == T_LBRACKET) {
            consume(T_LBRACKET);
            auto n = std::make_unique<ArrayLiteralNode>();
            if (currentToken.type != T_RBRACKET) { n->elements.push_back(parseExpression()); while (currentToken.type == T_COMMA) { consume(T_COMMA); n->elements.push_back(parseExpression()); } }
            consume(T_RBRACKET); return n;
        }
        if (currentToken.type == T_LPAREN) { consume(T_LPAREN); auto n = parseExpression(); consume(T_RPAREN); return n; }
        throw std::runtime_error("Unexpected token in expression. Token type: " + std::to_string(currentToken.type) + ", Value: '" + currentToken.value + "', Line: " + std::to_string(currentToken.line));
    }
    std::unique_ptr<ASTNode> parseFactor() {
        auto node = parsePrimary();
        while (true) {
            if (currentToken.type == T_LPAREN) {
                consume(T_LPAREN);
                auto c = std::make_unique<CallNode>();
                c->callee = std::move(node);
                if (currentToken.type != T_RPAREN) {
                    c->arguments.push_back(parseExpression());
                    while (currentToken.type == T_COMMA) {
                        consume(T_COMMA);
                        c->arguments.push_back(parseExpression());
                    }
                }
                consume(T_RPAREN);
                node = std::move(c);
            } else if (currentToken.type == T_LBRACKET) {
                consume(T_LBRACKET); auto index = parseExpression(); consume(T_RBRACKET);
                auto n = std::make_unique<ArrayIndexNode>(); n->arrayExpr = std::move(node); n->indexExpr = std::move(index); node = std::move(n);
            } else if (currentToken.type == T_DOT) {
                consume(T_DOT); std::string propName = currentToken.value; consume(T_IDENTIFIER);
                auto n = std::make_unique<PropertyGetNode>(); n->object = std::move(node); n->name = propName; node = std::move(n);
            } else {
                break;
            }
        }
        return node;
    }
};

// ==================================================
// 4. Bytecode Compiler
// ==================================================

class BytecodeCompiler {
public:
    Chunk* chunk;
    void compile(Chunk* c, ASTNode* node) { chunk = c; compileNode(node); }

private:
    void emitByte(uint8_t byte) { chunk->write(byte); }
    void emitConstant(Value val) { emitByte(OP_CONSTANT); emitByte(chunk->addConstant(val)); }
    int emitJump(uint8_t inst) { emitByte(inst); emitByte(0xff); emitByte(0xff); return chunk->code.size() - 2; }
    void patchJump(int offset) { int jump = chunk->code.size() - offset - 2; chunk->code[offset] = (jump >> 8) & 0xff; chunk->code[offset+1] = jump & 0xff; }
    void emitLoop(int loopStart) { emitByte(OP_LOOP); int offset = chunk->code.size() - loopStart + 2; emitByte((offset >> 8) & 0xff); emitByte(offset & 0xff); }

    void compileNode(ASTNode* node) {
        if (!node) return;
        std::string tn = node->typeName();
        if (tn == "Program") { auto* p = static_cast<ProgramNode*>(node); for (auto& s : p->declarations) compileNode(s.get()); emitByte(OP_RETURN); }
        else if (tn == "FunctionDecl") {
            auto* f = static_cast<FunctionDeclNode*>(node);
            int jumpOver = emitJump(OP_JUMP);
            int entryPoint = chunk->code.size();
            for (auto& s : f->body) compileNode(s.get());
            emitByte(OP_RETURN);
            patchJump(jumpOver);
            auto funcObj = std::make_shared<AuraFunction>();
            funcObj->name = f->name; funcObj->arity = f->params.size(); funcObj->entryPoint = entryPoint; funcObj->paramNames = f->params;
            emitConstant(funcObj); emitByte(OP_DEFINE_GLOBAL); emitByte(chunk->addConstant(f->name));
        }
        else if (tn == "ClassDecl") {
            auto* c = static_cast<ClassDeclNode*>(node);
            emitByte(OP_CLASS); emitByte(chunk->addConstant(c->name));
            emitByte(OP_DEFINE_GLOBAL); emitByte(chunk->addConstant(c->name));
            for (auto& m : c->methods) {
                auto* md = static_cast<FunctionDeclNode*>(m.get());
                int jumpOver = emitJump(OP_JUMP); int entryPoint = chunk->code.size();
                for (auto& s : md->body) compileNode(s.get());
                emitByte(OP_RETURN); patchJump(jumpOver);
                auto funcObj = std::make_shared<AuraFunction>();
                funcObj->name = md->name; funcObj->arity = md->params.size(); funcObj->entryPoint = entryPoint; funcObj->paramNames = md->params;
                emitByte(OP_GET_GLOBAL); emitByte(chunk->addConstant(c->name));
                emitConstant(funcObj);
                emitByte(OP_METHOD); emitByte(chunk->addConstant(md->name));
            }
        }
        else if (tn == "Return") {
            auto* r = static_cast<ReturnNode*>(node);
            if (r->expr) { compileNode(r->expr.get()); emitByte(OP_RETURN_VALUE); }
            else { emitByte(OP_RETURN); }
        }
        else if (tn == "Call") {
            auto* c = static_cast<CallNode*>(node);
            compileNode(c->callee.get());
            for (auto& arg : c->arguments) compileNode(arg.get());
            emitByte(OP_CALL); emitByte(c->arguments.size());
        }
        else if (tn == "VarDecl") { auto* v = static_cast<VarDeclNode*>(node); compileNode(v->expr.get()); emitByte(OP_DEFINE_GLOBAL); emitByte(chunk->addConstant(v->name)); }
        else if (tn == "Print") { auto* p = static_cast<PrintNode*>(node); compileNode(p->expr.get()); emitByte(OP_PRINT); }
        else if (tn == "ExpressionStatement") { auto* es = static_cast<ExpressionStatementNode*>(node); compileNode(es->expr.get()); emitByte(OP_POP); }
        else if (tn == "If") {
            auto* i = static_cast<IfNode*>(node); compileNode(i->cond.get()); int thenJump = emitJump(OP_JUMP_IF_FALSE); emitByte(OP_POP);
            for (auto& s : i->thenBranch) compileNode(s.get());
            int elseJump = emitJump(OP_JUMP); patchJump(thenJump); emitByte(OP_POP);
            if (i->hasElse) for (auto& s : i->elseBranch) compileNode(s.get());
            patchJump(elseJump);
        }
        else if (tn == "While") {
            auto* w = static_cast<WhileNode*>(node); int loopStart = chunk->code.size();
            compileNode(w->cond.get()); int exitJump = emitJump(OP_JUMP_IF_FALSE); emitByte(OP_POP);
            for (auto& s : w->body) compileNode(s.get());
            emitLoop(loopStart); patchJump(exitJump); emitByte(OP_POP);
        }
        else if (tn == "For") {
            auto* f = static_cast<ForNode*>(node);
            compileNode(f->init.get()); int loopStart = chunk->code.size(); compileNode(f->cond.get());
            int exitJump = emitJump(OP_JUMP_IF_FALSE); emitByte(OP_POP);
            for (auto& s : f->body) compileNode(s.get());
            compileNode(f->update.get()); emitByte(OP_POP);
            emitLoop(loopStart); patchJump(exitJump); emitByte(OP_POP);
        }
        else if (tn == "BinOp") {
            auto* b = static_cast<BinOpNode*>(node);
            if (b->op == "=") {
                if (b->left->typeName() == "Identifier") {
                    compileNode(b->right.get()); emitByte(OP_SET_GLOBAL); emitByte(chunk->addConstant(static_cast<IdentifierNode*>(b->left.get())->name));
                } else if (b->left->typeName() == "ArrayIndex") {
                    auto* idx = static_cast<ArrayIndexNode*>(b->left.get());
                    compileNode(idx->arrayExpr.get()); compileNode(idx->indexExpr.get()); compileNode(b->right.get());
                    emitByte(OP_INDEX_SET);
                } else if (b->left->typeName() == "PropertyGet") {
                    auto* pg = static_cast<PropertyGetNode*>(b->left.get());
                    compileNode(pg->object.get()); compileNode(b->right.get());
                    emitByte(OP_SET_PROPERTY); emitByte(chunk->addConstant(pg->name));
                }
            } else if (b->op == "and") { compileNode(b->left.get()); int endJump = emitJump(OP_JUMP_IF_FALSE); emitByte(OP_POP); compileNode(b->right.get()); patchJump(endJump); }
            else if (b->op == "or") { compileNode(b->left.get()); int elseJump = emitJump(OP_JUMP_IF_FALSE); int endJump = emitJump(OP_JUMP); patchJump(elseJump); emitByte(OP_POP); compileNode(b->right.get()); patchJump(endJump); }
            else {
                compileNode(b->left.get()); compileNode(b->right.get());
                if (b->op == "+") emitByte(OP_ADD); else if (b->op == "-") emitByte(OP_SUBTRACT);
                else if (b->op == "*") emitByte(OP_MULTIPLY); else if (b->op == "/") emitByte(OP_DIVIDE);
                else if (b->op == "%") emitByte(OP_MOD); else if (b->op == "==") emitByte(OP_EQUAL);
                else if (b->op == "!=") emitByte(OP_NOT_EQUAL); else if (b->op == "<") emitByte(OP_LESS);
                else if (b->op == "<=") emitByte(OP_LESS_EQUAL); else if (b->op == ">") emitByte(OP_GREATER);
                else if (b->op == ">=") emitByte(OP_GREATER_EQUAL);
            }
        }
        else if (tn == "UnaryOp") { auto* u = static_cast<UnaryOpNode*>(node); compileNode(u->operand.get()); if (u->op == "-") emitByte(OP_NEGATE); if (u->op == "not") emitByte(OP_NOT); }
        else if (tn == "Literal") { auto* l = static_cast<LiteralNode*>(node); if (l->valueType == "NUMBER") emitConstant(std::stod(l->value)); else if (l->valueType == "STRING") emitConstant(l->value); else if (l->valueType == "BOOL") emitByte(l->value == "true" ? OP_TRUE : OP_FALSE); }
        else if (tn == "Identifier") { auto* id = static_cast<IdentifierNode*>(node); emitByte(OP_GET_GLOBAL); emitByte(chunk->addConstant(id->name)); }
        else if (tn == "ArrayLiteral") {
            auto* al = static_cast<ArrayLiteralNode*>(node);
            for (auto& el : al->elements) compileNode(el.get());
            emitByte(OP_BUILD_ARRAY); emitByte(al->elements.size());
        }
        else if (tn == "ArrayIndex") {
            auto* ai = static_cast<ArrayIndexNode*>(node);
            compileNode(ai->arrayExpr.get()); compileNode(ai->indexExpr.get()); emitByte(OP_INDEX_GET);
        }
        else if (tn == "PropertyGet") {
            auto* pg = static_cast<PropertyGetNode*>(node);
            compileNode(pg->object.get()); emitByte(OP_GET_PROPERTY); emitByte(chunk->addConstant(pg->name));
        }
    }
};

// ==================================================
// 5. Virtual Machine Execution
// ==================================================

struct CallFrame {
    std::shared_ptr<AuraFunction> function;
    uint8_t* ip;
    std::unordered_map<std::string, Value> locals;
};

class VirtualMachine {
public:
    Chunk* chunk; std::vector<Value> stack; std::unordered_map<std::string, Value> globals;
    std::vector<CallFrame> frames;
    void interpret(Chunk* c) { 
        chunk = c; stack.clear(); frames.clear(); globals.clear();
        
        auto defineNative = [&](std::string name, std::function<Value(const std::vector<Value>&)> func) {
            auto native = std::make_shared<NativeFunction>();
            native->func = func;
            globals[name] = native;
        };
        
        defineNative("math_sqrt", [](const std::vector<Value>& args) -> Value { return std::sqrt(std::get<double>(args[0])); });
        defineNative("math_abs", [](const std::vector<Value>& args) -> Value { return std::abs(std::get<double>(args[0])); });
        defineNative("math_pow", [](const std::vector<Value>& args) -> Value { return std::pow(std::get<double>(args[0]), std::get<double>(args[1])); });
        defineNative("math_sin", [](const std::vector<Value>& args) -> Value { return std::sin(std::get<double>(args[0])); });
        
        defineNative("file_read", [](const std::vector<Value>& args) -> Value {
            std::ifstream file(std::get<std::string>(args[0]));
            if (!file) return std::string("");
            std::stringstream buffer; buffer << file.rdbuf(); return buffer.str();
        });
        defineNative("file_write", [](const std::vector<Value>& args) -> Value {
            std::ofstream file(std::get<std::string>(args[0]));
            if (file) { file << std::get<std::string>(args[1]); return true; } return false;
        });
        defineNative("http_get", [](const std::vector<Value>& args) -> Value {
            std::string url = std::get<std::string>(args[0]);
            std::string tempFile = "aura_http_temp.txt";
            std::string cmd = "curl -s -o " + tempFile + " \"" + url + "\"";
            if (std::system(cmd.c_str()) != 0) return std::string("Error");
            std::ifstream file(tempFile); std::stringstream buffer; buffer << file.rdbuf();
            std::remove(tempFile.c_str()); return buffer.str();
        });
        defineNative("system_run", [](const std::vector<Value>& args) -> Value {
            std::string cmd = std::get<std::string>(args[0]);
            int res = std::system(cmd.c_str());
            return (double)res;
        });
        defineNative("len", [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<std::string>(args[0])) return (double)std::get<std::string>(args[0]).length();
            if (std::holds_alternative<std::shared_ptr<ValueArray>>(args[0])) return (double)std::get<std::shared_ptr<ValueArray>>(args[0])->elements.size();
            return 0.0;
        });
        defineNative("int", [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) return std::floor(std::get<double>(args[0]));
            if (std::holds_alternative<std::string>(args[0])) return std::stod(std::get<std::string>(args[0]));
            return 0.0;
        });
        
        defineNative("string_char_at", [](const std::vector<Value>& args) -> Value {
            std::string str = std::get<std::string>(args[0]);
            int idx = std::get<double>(args[1]);
            if (idx >= 0 && idx < str.length()) return std::string(1, str[idx]);
            return std::string("");
        });
        defineNative("string_substring", [](const std::vector<Value>& args) -> Value {
            std::string str = std::get<std::string>(args[0]);
            int start = std::get<double>(args[1]);
            int length = std::get<double>(args[2]);
            if (start >= 0 && start < str.length()) return str.substr(start, length);
            return std::string("");
        });
        defineNative("char_is_alpha", [](const std::vector<Value>& args) -> Value {
            std::string c = std::get<std::string>(args[0]);
            if (c.empty()) return false;
            return (bool)std::isalpha(static_cast<unsigned char>(c[0])) || c[0] == '_';
        });
        defineNative("char_is_digit", [](const std::vector<Value>& args) -> Value {
            std::string c = std::get<std::string>(args[0]);
            if (c.empty()) return false;
            return (bool)std::isdigit(static_cast<unsigned char>(c[0]));
        });
        defineNative("char_is_space", [](const std::vector<Value>& args) -> Value {
            std::string c = std::get<std::string>(args[0]);
            if (c.empty()) return false;
            return (bool)std::isspace(static_cast<unsigned char>(c[0]));
        });
        defineNative("array_push", [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<std::shared_ptr<ValueArray>>(args[0])) {
                auto arr = std::get<std::shared_ptr<ValueArray>>(args[0]);
                arr->elements.push_back(args[1]);
                return true;
            }
            return false;
        });

        // ----------------------------------------------------
        // AURA MACHINE LEARNING ENGINE (Native Tensors)
        // ----------------------------------------------------
        defineNative("tensor_create", [](const std::vector<Value>& args) -> Value {
            int rows = std::get<double>(args[0]);
            int cols = std::get<double>(args[1]);
            auto matrix = std::make_shared<ValueArray>();
            for (int i = 0; i < rows; i++) {
                auto row = std::make_shared<ValueArray>();
                for (int j = 0; j < cols; j++) row->elements.push_back(0.0);
                matrix->elements.push_back(row);
            }
            return matrix;
        });

        defineNative("tensor_dot", [](const std::vector<Value>& args) -> Value {
            auto m1 = std::get<std::shared_ptr<ValueArray>>(args[0]);
            auto m2 = std::get<std::shared_ptr<ValueArray>>(args[1]);
            int r1 = m1->elements.size();
            int c1 = std::get<std::shared_ptr<ValueArray>>(m1->elements[0])->elements.size();
            int c2 = std::get<std::shared_ptr<ValueArray>>(m2->elements[0])->elements.size();
            
            auto result = std::make_shared<ValueArray>();
            for (int i = 0; i < r1; i++) {
                auto row = std::make_shared<ValueArray>();
                auto r1_ptr = std::get<std::shared_ptr<ValueArray>>(m1->elements[i]);
                for (int j = 0; j < c2; j++) {
                    double sum = 0.0;
                    for (int k = 0; k < c1; k++) {
                        auto r2_ptr = std::get<std::shared_ptr<ValueArray>>(m2->elements[k]);
                        sum += std::get<double>(r1_ptr->elements[k]) * std::get<double>(r2_ptr->elements[j]);
                    }
                    row->elements.push_back(sum);
                }
                result->elements.push_back(row);
            }
            return result;
        });

        // ----------------------------------------------------
        // AURA WEB ENGINE (Native HTTP Server)
        // ----------------------------------------------------
        defineNative("server_start", [](const std::vector<Value>& args) -> Value {
            int port = std::get<double>(args[0]);
            std::string html_content = std::get<std::string>(args[1]);
            
            httplib::Server svr;
            svr.Get("/", [html_content](const httplib::Request &, httplib::Response &res) {
                res.set_content(html_content, "text/html");
            });
            
            std::cout << "[Aura Web] Server listening on http://localhost:" << port << "\n";
            svr.listen("0.0.0.0", port);
            return true;
        });

        CallFrame mainFrame; mainFrame.function = nullptr; mainFrame.ip = chunk->code.data(); frames.push_back(mainFrame);
        run(); 
    }

private:
    void push(Value value) { stack.push_back(value); }
    Value pop() { Value val = stack.back(); stack.pop_back(); return val; }
    Value peek(int d) { return stack[stack.size() - 1 - d]; }
    
    void run() {
        while (true) {
            uint8_t instruction = *frames.back().ip++;
            switch (instruction) {
                case OP_RETURN: {
                    frames.pop_back();
                    if (frames.empty()) return;
                    push(false); // Implicit return false
                    break;
                }
                case OP_RETURN_VALUE: {
                    Value result = pop();
                    frames.pop_back();
                    if (frames.empty()) return;
                    push(result);
                    break;
                }
                case OP_CALL: {
                    int argCount = *frames.back().ip++;
                    Value callee = peek(argCount);
                    if (std::holds_alternative<std::shared_ptr<NativeFunction>>(callee)) {
                        auto nativeFunc = std::get<std::shared_ptr<NativeFunction>>(callee);
                        std::vector<Value> args;
                        for (int i = 0; i < argCount; i++) args.push_back(peek(argCount - 1 - i));
                        for (int i = 0; i <= argCount; i++) pop();
                        push(nativeFunc->func(args));
                    }
                    else if (std::holds_alternative<std::shared_ptr<AuraFunction>>(callee)) {
                        auto function = std::get<std::shared_ptr<AuraFunction>>(callee);
                        if (argCount != function->arity) throw std::runtime_error("Expected " + std::to_string(function->arity) + " arguments");
                        CallFrame frame; frame.function = function; frame.ip = chunk->code.data() + function->entryPoint;
                        for (int i = argCount - 1; i >= 0; i--) frame.locals[function->paramNames[i]] = pop();
                        pop(); // pop the function object itself
                        frames.push_back(frame);
                    }
                    else if (std::holds_alternative<std::shared_ptr<AuraClass>>(callee)) {
                        auto klass = std::get<std::shared_ptr<AuraClass>>(callee);
                        auto instance = std::make_shared<AuraInstance>(); instance->klass = klass;
                        for (int i = 0; i <= argCount; i++) pop(); // discard args and class for now
                        push(instance);
                    }
                    else if (std::holds_alternative<std::shared_ptr<AuraBoundMethod>>(callee)) {
                        auto bound = std::get<std::shared_ptr<AuraBoundMethod>>(callee);
                        auto function = bound->method;
                        if (argCount != function->arity) throw std::runtime_error("Expected " + std::to_string(function->arity) + " arguments");
                        CallFrame frame; frame.function = function; frame.ip = chunk->code.data() + function->entryPoint;
                        for (int i = argCount - 1; i >= 0; i--) frame.locals[function->paramNames[i]] = pop();
                        pop(); // pop the bound method object itself
                        frame.locals["this"] = bound->receiver;
                        frames.push_back(frame);
                    }
                    else {
                        throw std::runtime_error("Can only call functions");
                    }
                    break;
                }
                case OP_CONSTANT: push(chunk->constants[*frames.back().ip++]); break;
                case OP_TRUE: push(true); break;
                case OP_FALSE: push(false); break;
                case OP_POP: pop(); break;
                case OP_DEFINE_GLOBAL: {
                    std::string name = std::get<std::string>(chunk->constants[*frames.back().ip++]);
                    if (frames.size() > 1) frames.back().locals[name] = pop(); else globals[name] = pop(); break;
                }
                case OP_GET_GLOBAL: {
                    std::string name = std::get<std::string>(chunk->constants[*frames.back().ip++]);
                    bool found = false;
                    for (int i = frames.size() - 1; i > 0; --i) {
                        if (frames[i].locals.count(name)) {
                            push(frames[i].locals[name]); found = true; break;
                        }
                    }
                    if (!found) {
                        if (globals.count(name)) push(globals[name]);
                        else throw std::runtime_error("Undefined variable: " + name);
                    }
                    break;
                }
                case OP_SET_GLOBAL: {
                    std::string name = std::get<std::string>(chunk->constants[*frames.back().ip++]);
                    bool found = false;
                    for (int i = frames.size() - 1; i > 0; --i) {
                        if (frames[i].locals.count(name)) {
                            frames[i].locals[name] = peek(0); found = true; break;
                        }
                    }
                    if (!found) {
                        globals[name] = peek(0);
                    }
                    break;
                }
                case OP_CLASS: {
                    std::string name = std::get<std::string>(chunk->constants[*frames.back().ip++]);
                    auto klass = std::make_shared<AuraClass>(); klass->name = name;
                    push(klass); break;
                }
                case OP_METHOD: {
                    std::string name = std::get<std::string>(chunk->constants[*frames.back().ip++]);
                    Value method = pop(); Value klassVal = peek(0);
                    auto klass = std::get<std::shared_ptr<AuraClass>>(klassVal);
                    klass->methods[name] = std::get<std::shared_ptr<AuraFunction>>(method);
                    break;
                }
                case OP_GET_PROPERTY: {
                    std::string name = std::get<std::string>(chunk->constants[*frames.back().ip++]);
                    Value obj = peek(0);
                    if (std::holds_alternative<std::shared_ptr<AuraInstance>>(obj)) {
                        auto instance = std::get<std::shared_ptr<AuraInstance>>(obj);
                        if (instance->fields.count(name)) { pop(); push(instance->fields[name]); }
                        else if (instance->klass->methods.count(name)) {
                            auto bound = std::make_shared<AuraBoundMethod>(); bound->receiver = obj; bound->method = instance->klass->methods[name];
                            pop(); push(bound);
                        } else throw std::runtime_error("Undefined property " + name);
                    } else throw std::runtime_error("Only instances have properties");
                    break;
                }
                case OP_SET_PROPERTY: {
                    std::string name = std::get<std::string>(chunk->constants[*frames.back().ip++]);
                    Value val = pop(); Value obj = pop();
                    if (!std::holds_alternative<std::shared_ptr<AuraInstance>>(obj)) throw std::runtime_error("Only instances have properties");
                    auto instance = std::get<std::shared_ptr<AuraInstance>>(obj);
                    instance->fields[name] = val; push(val);
                    break;
                }
                case OP_BUILD_ARRAY: {
                    int count = *frames.back().ip++;
                    auto arr = std::make_shared<ValueArray>();
                    for (int i = 0; i < count; i++) arr->elements.push_back(stack[stack.size() - count + i]);
                    stack.erase(stack.end() - count, stack.end());
                    push(arr); break;
                }
                case OP_INDEX_GET: {
                    Value idxVal = pop(); Value arrVal = pop();
                    if (!std::holds_alternative<std::shared_ptr<ValueArray>>(arrVal)) throw std::runtime_error("Can only index arrays");
                    auto arr = std::get<std::shared_ptr<ValueArray>>(arrVal); int idx = std::get<double>(idxVal);
                    if (idx < 0 || idx >= arr->elements.size()) throw std::runtime_error("Array index out of bounds");
                    push(arr->elements[idx]); break;
                }
                case OP_INDEX_SET: {
                    Value val = pop(); Value idxVal = pop(); Value arrVal = pop();
                    auto arr = std::get<std::shared_ptr<ValueArray>>(arrVal); int idx = std::get<double>(idxVal);
                    arr->elements[idx] = val; push(val); break;
                }
                case OP_EQUAL: push(valuesEqual(pop(), pop())); break;
                case OP_NOT_EQUAL: { Value b = pop(); push(!valuesEqual(pop(), b)); break; }
                case OP_LESS: { Value b = pop(); push(std::get<double>(pop()) < std::get<double>(b)); break; }
                case OP_LESS_EQUAL: { Value b = pop(); push(std::get<double>(pop()) <= std::get<double>(b)); break; }
                case OP_GREATER: { Value b = pop(); push(std::get<double>(pop()) > std::get<double>(b)); break; }
                case OP_GREATER_EQUAL: { Value b = pop(); push(std::get<double>(pop()) >= std::get<double>(b)); break; }
                case OP_ADD: {
                    Value b = pop(); Value a = pop();
                    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) push(std::get<double>(a) + std::get<double>(b));
                    else push(valueToString(a) + valueToString(b));
                    break;
                }
                case OP_SUBTRACT: { Value b = pop(); push(std::get<double>(pop()) - std::get<double>(b)); break; }
                case OP_MULTIPLY: { Value b = pop(); push(std::get<double>(pop()) * std::get<double>(b)); break; }
                case OP_DIVIDE: { Value b = pop(); push(std::get<double>(pop()) / std::get<double>(b)); break; }
                case OP_MOD: { Value b = pop(); push(std::fmod(std::get<double>(pop()), std::get<double>(b))); break; }
                case OP_NOT: push(valueIsFalsy(pop())); break;
                case OP_NEGATE: push(-std::get<double>(pop())); break;
                case OP_PRINT: std::cout << valueToString(pop()) << std::endl; break;
                case OP_JUMP: {
                    uint16_t offset = (frames.back().ip[0] << 8) | frames.back().ip[1];
                    frames.back().ip += 2; frames.back().ip += offset; break;
                }
                case OP_JUMP_IF_FALSE: {
                    uint16_t offset = (frames.back().ip[0] << 8) | frames.back().ip[1];
                    frames.back().ip += 2; if (valueIsFalsy(peek(0))) frames.back().ip += offset; break;
                }
                case OP_LOOP: {
                    uint16_t offset = (frames.back().ip[0] << 8) | frames.back().ip[1];
                    frames.back().ip += 2; frames.back().ip -= offset; break;
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    std::ifstream file(argv[1]);
    std::stringstream buffer; buffer << file.rdbuf();
    try {
        Lexer lexer(buffer.str()); Parser parser(lexer); auto ast = parser.parse();
        Chunk chunk; BytecodeCompiler compiler; compiler.compile(&chunk, ast.get());
        VirtualMachine vm; vm.interpret(&chunk);
    } catch (const std::exception& e) { std::cerr << "[VM Error] " << e.what() << "\n"; }
    return 0;
}


