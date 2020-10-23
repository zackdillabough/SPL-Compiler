#include "lexer.h"


// program -> var_section body inputs *
void parse_program() {
	parse_var_section();

	parse_body();

	parse_inputs();
}


// var_section -> id_list SEMICOLON *
void parse_var_section() {
	parse_id_list();

	expect(SEMICOLON);
}


// id_list -> ID COMMA id_list | ID *
void parse_id_list() {
	expect(ID);

	Token t = lexer.peek(1);

	if(t.token_type == COMMA) {
		expect(COMMA);

		parse_id_list();
	}
}


// body -> LBRACE stmt_list RBRACE *
void parse_body() {
	expect(LBRACE);

	parse_stmt_list();

	expect(RBRACE);
}


// stmt_list -> stmt stmt_list | stmt *
void parse_stmt_list() {
	parse_stmt();

	Token t = lexer.peek(1);

	if(t.token_type == ID
	|| t.token_type == output
	|| t.token_type == input
	|| t.token_type == WHILE
	|| t.token_type == IF
	|| t.token_type == SWITCH
	|| t.token_type == FOR)
		parse_stmt_list();
}


// stmt -> assign_stmt | while_stmt | if_stmt | switch_stmt | for_stmt | output_stmt | input_stmt *
void parse_stmt() {

	Token t = lexer.peek(1);

	if(t.token_type == ID)
		parse_assign_stmt(); 
	else if(t.token_type == WHILE)
		parse_while_stmt(); 
	else if(t.token_type == IF)
		parse_if_stmt(); 
	else if(t.token_type == SWITCH)
		parse_switch_stmt(); 
	else if(t.token_type == FOR)
		parse_for_stmt(); 
	else if(t.token_type == output)
		parse_output_stmt(); 
	else if(t.token_type == input)
		parse_input_stmt(); 

}


// assign_stmt -> ID EQUAL primary SEMICOLON | ID EQUAL expr SEMICOLON *
void parse_assign_stmt() {
	expect(ID);

	expect(EQUAL);

	Token t = lexer.peek(2);

	// problem: FIRST(primary) == FIRST(expr)
	if (t.token_type == PLUS
			|| t.token_type == MINUS
			|| t.token_type == MULT
			|| t.token_type == DIV)
		parse_expr();
	else
		parse_primary();

	expect(SEMICOLON);
}


// expr -> primary op primary *
void parse_expr() {
	parse_primary();

	parse_op();

	parse_primary();
}


// primary -> ID | NUM *
void parse_primary() {
	Token t = lexer.peek(1);

	if (t.token_type == ID)
		expect(ID);
	else
		expect(NUM);
}


// op -> PLUS | MINUS | MULT | DIV *
void parse_op() {
	Token t = lexer.peek(1);

	if (t.token_type == PLUS)
		expect(PLUS);
	else if (t.token_type == MINUS)
		expect(MINUS);
	else if (t.token_type == MULT)
		expect(MULT);
	else
		expect(DIV);
}


// output_stmt -> output ID SEMICOLON *
void parse_output_stmt() {
	expect(output);

	expect(ID);

	expect(SEMICOLON);
}


// input_stmt -> input ID SEMICOLON *
void parse_input_stmt() {
	expect(input);

	expect(ID);

	expect(SEMICOLON);
}


// while_stmt -> WHILE condition body *
void parse_while_stmt() {
	expect(WHILE);

	parse_condition();

	parse_body();
}


// if_stmt -> IF condition body *
void parse_if_stmt() {
	expect(IF);

	parse_condition();

	parse_body();
}


// condition -> primary relop primary *
void parse_condition() {
	parse_primary();

	parse_relop();

	parse_primary();
}


// relop -> GREATER | LESS | NOTEQUAL *
void parse_relop() {
	Token t = lexer.peek(1);

	if (t.token_type == GREATER)
		expect(GREATER);
	else if (t.token_type == LESS)
		expect(LESS);
	else
		expect(NOTEQUAL);


}


// switch_stmt -> SWITCH ID LBRACE case_list RBRACE | SWITCH ID LBRACE case_list default_case RBRACE *
void parse_switch_stmt() {
	expect(SWITCH);

	expect(ID);

	expect(LBRACE);

	parse_case_list();

	Token t = lexer.peek(1);

	if(t.token_type != RBRACE)
		parse_default_case();

	expect(RBRACE);
}


// for_stmt -> FOR LPAREN assign_stmt condition SEMICOLON assign_stmt RPAREN body *
void parse_for_stmt() {
	expect(FOR);

	expect(LPAREN);

	parse_assign_stmt();

	parse_condition();

	expect(SEMICOLON);

	parse_assign_stmt();

	expect(RPAREN);

	parse_body();
}


// case_list -> case case_list | case *
void parse_case_list() {
	parse_case();

	Token t = lexer.peek(1);

	if (t.token_type == CASE)
		parse_case_list();
}


// case -> CASE NUM COLON body *
void parse_case() {
	expect(CASE);

	expect(NUM);

	expect(COLON);

	parse_body();
}


// default_case -> DEFAULT COLON body *
void parse_default_case() {
	expect(DEFAULT);

	expect(COLON);

	parse_body();
}


// inputs -> num_list *
void parse_inputs() {
	parse_num_list();
}


// num_list -> NUM | NUM num_list *
void parse_num_list() {
	expect(NUM);

	Token t = lexer.peek(1);

	if (t.token_type == NUM)
		parse_num_list();
}



int main(int argc, char* argv[]) {
	LexicalAnalyzer lexer;

	parse_program();
}
