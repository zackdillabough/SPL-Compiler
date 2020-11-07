#include <unordered_map>
#include "project3.h"
#include "compiler.h"

// variable storage
std::unordered_map<std::string, int> location;

void syntax_error(int lineno) {
	std::printf("SYNTAX ERROR !!!\n");
	std::printf("called from line %d\n", lineno);
	exit(1);
}

Token Parser::expect(TokenType expected_type, int lineno) {
	Token t = lexer.GetToken();

	if (t.token_type != expected_type)
		syntax_error(lineno);

	return t;
}

ConditionalOperatorType Parser::expectC(TokenType expected_type, int lineno) {
	Token t = lexer.GetToken();
    ConditionalOperatorType type;

	if (t.token_type != expected_type)
		syntax_error(__LINE__);
    else if (t.token_type == GREATER)
        type = CONDITION_GREATER;
    else if (t.token_type == LESS)
        type = CONDITION_LESS;
    else if (t.token_type == NOTEQUAL)
        type = CONDITION_NOTEQUAL;

	return type;
}

// program -> var_section body inputs *
InstructionNode* Parser::parse_program() {
	parse_var_section();

	// get program intermediary code
	InstructionNode* p = parse_body();

	parse_inputs();
	//parse_inputs(p); //sponge
	
	return p;
}


// var_section -> id_list SEMICOLON *
void Parser::parse_var_section() {
	parse_id_list();

	expect(SEMICOLON, __LINE__);
}


// id_list -> ID COMMA id_list | ID *
void Parser::parse_id_list() {
	Token t = expect(ID, __LINE__);

	// add variable to location table
	location.insert(std::pair<std::string, int>(t.lexeme, next_available++));

	t = lexer.peek(1);

	if(t.token_type == COMMA) {
		expect(COMMA, __LINE__);

		parse_id_list();
	}
}


// body -> LBRACE stmt_list RBRACE *
InstructionNode* Parser::parse_body() {
	expect(LBRACE, __LINE__);

	InstructionNode* instr_list;
	instr_list = parse_stmt_list();

	expect(RBRACE, __LINE__);

	return instr_list;
}


// stmt_list -> stmt stmt_list | stmt *
InstructionNode* Parser::parse_stmt_list() {
	InstructionNode* instr;
	InstructionNode* instr_list = new InstructionNode();


	instr = parse_stmt();

	Token t = lexer.peek(1);

	if(t.token_type == ID
	|| t.token_type == OUTPUT
	|| t.token_type == INPUT
	|| t.token_type == WHILE
	|| t.token_type == IF
	|| t.token_type == SWITCH
	|| t.token_type == FOR) {

        InstructionNode* current = instr;

        while (current->next != NULL)
            current = current->next;

		instr_list = parse_stmt_list();


		current->next = instr_list;
	} 

	return instr;
}


// stmt -> assign_stmt | while_stmt | if_stmt | switch_stmt | for_stmt | output_stmt | input_stmt *
InstructionNode* Parser::parse_stmt() {

	Token t = lexer.peek(1);

	InstructionNode* instr = new InstructionNode();

	if(t.token_type == ID) {
		instr->type = ASSIGN;
		parse_assign_stmt(instr); 
    } else if(t.token_type == WHILE) {
		instr->type = CJMP;
		parse_while_stmt(instr); 
    } else if(t.token_type == IF) {
		instr->type = CJMP;
		parse_if_stmt(instr); 
	} else if(t.token_type == SWITCH) {
	    instr->type = NOOP;
		parse_switch_stmt(instr); 
    } else if(t.token_type == FOR) {
	    instr->type = ASSIGN;
		parse_for_stmt(instr); 
    } else if(t.token_type == OUTPUT) {
		instr->type = OUT;
		parse_output_stmt(instr); 
    } else if(t.token_type == INPUT) {
		instr->type = IN;
		parse_input_stmt(instr); 
    }

	return instr;
}


// assign_stmt -> ID EQUAL primary SEMICOLON | ID EQUAL expr SEMICOLON *
void Parser::parse_assign_stmt(InstructionNode* instr) {
	Token t = expect(ID, __LINE__);

	// get variable index from location table
	instr->assign_inst.left_hand_side_index = location[t.lexeme];

	expect(EQUAL, __LINE__);

	t = lexer.peek(2);

	// case when RHS is an expression
	if (t.token_type == PLUS
			|| t.token_type == MINUS
			|| t.token_type == MULT
			|| t.token_type == DIV) {
		parse_expr(instr);
	
	// case when RHS is a constant
    } else {
	    instr->assign_inst.op = OPERATOR_NONE;
		instr->assign_inst.operand1_index = parse_primary();
    }

	expect(SEMICOLON, __LINE__);
}


// expr -> primary op primary *
void Parser::parse_expr(InstructionNode* instr) {
	instr->assign_inst.operand1_index = parse_primary();

	instr->assign_inst.op = parse_op();

	instr->assign_inst.operand2_index = parse_primary();
}


// primary -> ID | NUM *
int Parser::parse_primary() {
	Token t = lexer.peek(1);

	if (t.token_type == ID) {
		t = expect(ID, __LINE__);
		if (location.count(t.lexeme) == 0)
		    syntax_error(__LINE__);
    } else {
		t = expect(NUM, __LINE__);
		if (location.count(t.lexeme) == 0) {
		    mem[next_available] = stoi(t.lexeme);
            location.insert(std::pair<std::string, int>(t.lexeme, next_available++));
        }
    }

	return location[t.lexeme];
}


// op -> PLUS | MINUS | MULT | DIV *
ArithmeticOperatorType Parser::parse_op() {
	Token t = lexer.peek(1);
    ArithmeticOperatorType op;
    
	if (t.token_type == PLUS) {
		expect(PLUS, __LINE__);
        op = OPERATOR_PLUS;
    } else if (t.token_type == MINUS) {
		expect(MINUS, __LINE__);
        op = OPERATOR_MINUS;
    } else if (t.token_type == MULT) {
		expect(MULT, __LINE__);
        op = OPERATOR_MULT;
    } else {
		expect(DIV, __LINE__);
        op = OPERATOR_DIV;
    }

    return op;
}


// output_stmt -> output ID SEMICOLON *
void Parser::parse_output_stmt(InstructionNode* instr) {
	Token t = expect(OUTPUT, __LINE__);

	t = expect(ID, __LINE__);
    instr->output_inst.var_index = location[t.lexeme];

	expect(SEMICOLON, __LINE__);
}


// input_stmt -> input ID SEMICOLON *
void Parser::parse_input_stmt(InstructionNode* instr) {
	Token t = expect(INPUT, __LINE__);

	t = expect(ID, __LINE__);
    instr->input_inst.var_index = location[t.lexeme];

	expect(SEMICOLON, __LINE__);
}


// while_stmt -> WHILE condition body *
void Parser::parse_while_stmt(InstructionNode* instr) {
	expect(WHILE, __LINE__);

	parse_condition(instr);

	instr->next = parse_body();

	InstructionNode* jmp = new InstructionNode();
	jmp->jmp_inst.target = instr;
	jmp->type = JMP;
	
	InstructionNode* current = instr;
	while (current->next != 0)
		current = current->next;

	current->next = jmp;

	InstructionNode* noOp = new InstructionNode();
	noOp->type = NOOP;
	instr->cjmp_inst.target = noOp;

	jmp->next = noOp;

	instr->cjmp_inst.target = noOp;
}


// if_stmt -> IF condition body *
void Parser::parse_if_stmt(InstructionNode* instr) {
	expect(IF, __LINE__);

	parse_condition(instr);

	instr->next = parse_body();

	InstructionNode* noOp = new InstructionNode();
	noOp->type = NOOP;

	instr->cjmp_inst.target = noOp;

	InstructionNode* ptr = instr;

	while(ptr->next != 0)
		ptr = ptr->next; 

	ptr->next = noOp;


}


// condition -> primary relop primary *
void Parser::parse_condition(InstructionNode* instr) {
	instr->cjmp_inst.operand1_index = parse_primary();
	
	instr->cjmp_inst.condition_op = parse_relop();

	instr->cjmp_inst.operand2_index = parse_primary();
}


// relop -> GREATER | LESS | NOTEQUAL *
ConditionalOperatorType Parser::parse_relop() {
	Token t = lexer.peek(1);

	// sponge
	if (t.token_type == GREATER)
		return expectC(GREATER, __LINE__);
	else if (t.token_type == LESS)
		return expectC(LESS, __LINE__);
	else
		return expectC(NOTEQUAL, __LINE__);
}

// for_stmt -> FOR LPAREN assign_stmt condition SEMICOLON assign_stmt RPAREN body *
void Parser::parse_for_stmt(InstructionNode* instr) {

	InstructionNode* noOp = new InstructionNode();
	noOp->type = NOOP;

    InstructionNode* cond = new InstructionNode();
    cond->type = CJMP;
    cond->cjmp_inst.target = noOp;

	InstructionNode* jmp = new InstructionNode();
	jmp->type = JMP;
	jmp->jmp_inst.target = cond;
	jmp->next = noOp;

    InstructionNode* assn = new InstructionNode();
    assn->type = ASSIGN;


	expect(FOR, __LINE__);
	expect(LPAREN, __LINE__);

	parse_assign_stmt(instr);

    parse_condition(cond);
    instr->next = cond;

	expect(SEMICOLON, __LINE__);

	parse_assign_stmt(assn);

	expect(RPAREN, __LINE__);

	cond->next = parse_body();

	InstructionNode* ptr = cond->next;

	while (ptr->next != 0)
	    ptr = ptr->next;

	ptr->next = assn;

	assn->next = jmp;
}


// switch_stmt -> SWITCH ID LBRACE case_list RBRACE | SWITCH ID LBRACE case_list default_case RBRACE *
void Parser::parse_switch_stmt(InstructionNode* instr) {
	expect(SWITCH, __LINE__);

	Token t = expect(ID, __LINE__);
	int switch_var = location[t.lexeme];

	expect(LBRACE, __LINE__);

	instr->next = parse_case_list();


    InstructionNode* current = instr->next;

    InstructionNode* noOp = new InstructionNode();
    noOp->type = NOOP;


    while(current->next != 0 && current->type == CJMP) {
        current->cjmp_inst.operand1_index = switch_var;
        current->cjmp_inst.condition_op = CONDITION_NOTEQUAL;

        InstructionNode* ptr = current->cjmp_inst.target;

        while(ptr->next != 0) 
            ptr = ptr->next;

        InstructionNode* jmp = new InstructionNode();
        jmp->type = JMP;
        jmp->jmp_inst.target = noOp;
        ptr->next = jmp;

        current = current->next;

    }

    current->cjmp_inst.operand1_index = location[t.lexeme];
    current->cjmp_inst.condition_op = CONDITION_NOTEQUAL;

    InstructionNode* ptr = current->cjmp_inst.target;

    while(ptr->next != 0) 
        ptr = ptr->next;

    InstructionNode* jmp = new InstructionNode();
    jmp->type = JMP;
    jmp->jmp_inst.target = noOp;
    ptr->next = jmp;


	t = lexer.peek(1);

	if(t.token_type != RBRACE) {
		current->next = parse_default_case();

		while (current->next != 0) {
		    current = current->next;
        }
    } 

    current->next = noOp;

	expect(RBRACE, __LINE__);
}


// case_list -> case case_list | case *
InstructionNode* Parser::parse_case_list() {
	
	InstructionNode* instr = parse_case();

	Token t = lexer.peek(1);

	if (t.token_type == CASE) {
	    instr->next = parse_case_list();
    }

	return instr;
}

// case -> CASE NUM COLON body *
InstructionNode* Parser::parse_case() {

	expect(CASE, __LINE__);

    InstructionNode* instr = new InstructionNode();
    instr->type = CJMP;

	Token t = expect(NUM, __LINE__);
	if (location.count(t.lexeme) == 0) {
	    mem[next_available] = stoi(t.lexeme);
	    location.insert(std::pair<std::string, int>(t.lexeme, next_available++));
    }

	instr->cjmp_inst.operand2_index = location[t.lexeme];

	expect(COLON, __LINE__);

	instr->cjmp_inst.target = parse_body();

	return instr;
}


// default_case -> DEFAULT COLON body *
InstructionNode* Parser::parse_default_case() {
	expect(DEFAULT, __LINE__);

	expect(COLON, __LINE__);

    InstructionNode* instr = parse_body();

    return instr;
}


// inputs -> num_list *
void Parser::parse_inputs() {
	parse_num_list();
}


// num_list -> NUM | NUM num_list *
void Parser::parse_num_list() {
	Token t = expect(NUM, __LINE__);

	// put input int into inputs array
	inputs.push_back(std::stoi(t.lexeme));

	t = lexer.peek(1);

	if (t.token_type == NUM)
		parse_num_list();
}



struct InstructionNode * parse_generate_intermediate_representation() {
    Parser parser;
    InstructionNode* p = parser.parse_program();
    return p;
}
