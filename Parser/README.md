Before even trying to explain the implementation of the parser, here is the Context-Free-Grammar that was used to implement it. It is all written in Backus-Naur Form:

```text

// Program:

	<program> ::= <declartion_list> <funciton_list>


// Declaration List:
	<declartion_list> ::= <declaration> | <declartion> <declaration_list>| \sigma
	
	<declartion> ::= <type> <identifier> ";" | <type> <identifier> "=" <expression> ";"
	
	<type> ::= "int" | "float" | "char" 



// Function List:
	<function_list> ::= <function_definition> | <function_definition><function_list> | \sigma


// Function:

	<function_definition> ::= <type> <function_identifier> "(" <parameter_list> ")" "{" <statement_list> "}"
	
	<parameter_list> ::= <parameter> | <parameter> "," <parameter_list> | \sigma
	<pararmeter> ::= <type> <identifier>
	
<statment_list> :;= <statement> | <statement> <statment_list> | \sigma


// Statements:
	<statement> := <expression_statement> |  <if_statement> |  <while_statement> |  <for_statement>


// Expression Statement:
	<expression_statement> ::= <expression> ";"
	<expression> ::= <identifier> | <literal> | <expression> <operator> <expression> | <unary_operator> <expression>



<literal> ::= <number> | <string_literal>
<identifier> ::= [a-zA-Z_][a-zA-Z0-9_]*
<operator> ::= "+" | "-" | "*" | "/" | "=" | "==" | "<" | ">" | "+" | "-"
<unary_operator> ::= "++" | "--" | "!" | "&" | "*"
<number> ::= [0-9]+
<string_literal> ::= "\"" [a-zA-Z0-9_ ]* "\""`

```
