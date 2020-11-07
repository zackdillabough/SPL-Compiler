#ifndef _PROJECT3_H_
#define _PROJECT3_H_

#include "compiler.h"
#include "lexer.h"

class Parser {
public:
    LexicalAnalyzer lexer;
    Token expect(TokenType expected_type, int lineno);
    ConditionalOperatorType expectC(TokenType expected_type, int lineno);
    InstructionNode* parse_program();
    void parse_var_section();
    void parse_id_list();
    InstructionNode* parse_body();
    InstructionNode* parse_stmt_list();
    InstructionNode* parse_stmt();
    void parse_assign_stmt(InstructionNode* instr);
    void parse_expr(InstructionNode* instr);
    int parse_primary(InstructionNode* instr);
    int parse_primary();
    ArithmeticOperatorType parse_op();
    void parse_output_stmt(InstructionNode* instr);
    void parse_input_stmt(InstructionNode* instr);
    void parse_while_stmt(InstructionNode* instr);
    void parse_if_stmt(InstructionNode* instr);
    void parse_condition(InstructionNode* instr);
    ConditionalOperatorType parse_relop();
    void parse_switch_stmt(InstructionNode* instr);
    void parse_for_stmt(InstructionNode* instr);
    InstructionNode* parse_case_list();
    InstructionNode* parse_case();
    InstructionNode* parse_default_case();
    void parse_inputs();
    void parse_num_list();
};

#endif
