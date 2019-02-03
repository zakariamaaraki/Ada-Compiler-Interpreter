// Reserved keywords----------------------------------------

#define T_integer 253
#define T_float 254
#define T_boolean 255

//...


#define T_abort 257
#define T_else 258
#define T_new 259
#define T_return 260
#define T_abs 261
#define T_elsif 262
#define T_not 263
#define T_reverse 264
#define T_abstract 265
#define T_end 266
#define T_null 267
#define T_accept 268
#define T_entry 269
#define T_select 270
#define T_access 271
#define T_exception 272
#define T_of 273
#define T_separate 274
#define T_aliased 275
#define T_exit 276
#define T_or 277
#define T_some 278
#define T_all 279
#define T_others 280
#define T_subtype 281
#define T_and 282
#define T_for 283
#define T_out 284
#define T_synchronized 285
#define T_array 286
#define T_function 287
#define T_overriding 288
#define T_at 289
#define T_tagged 290
#define T_generic 291
#define T_package 292
#define T_task 293
#define T_begin 294
#define T_goto 295
#define T_pragma 296
#define T_terminate 297
#define T_body 298
#define T_private 299
#define T_then 300
#define T_if 301
#define T_procedure 302
#define T_type 303
#define T_case 304
#define T_in 305
#define T_protected 306
#define T_constant 307
#define T_interface 308
#define T_until 309
#define T_is 310
#define T_raise 311
#define T_use 312
#define T_declare 313
#define T_range 314
#define T_delay 315
#define T_limited 316
#define T_record 317
#define T_when 318
#define T_delta 319
#define T_loop 320
#define T_rem 321
#define T_while 322
#define T_digits 323
#define T_renames 324
#define T_with 325
#define T_do 326
#define T_mod 327
#define T_requeue 328
#define T_xor  329
//----------------------------------------------
//Lexical elements
#define T_IDENTIFIER  330
#define T_NUMERIC 331
#define T_STRING 332
#define T_COMMENT 333
#define T_CHAR 334
//---------------------------------------------
#define T_END 403
#define T_UNKNOWN 404
//---------------------------------------------
#define T_ASSIGN  800  //":="
#define T_INFEG 801 //"<="
#define T_SUPEG 802 //">="
#define T_DIFF 803  // "/=" <=> "!="
#define T_POWER 804 //"**"

#define T_QUOTE '\''
#define T_CONCAT '&'
#define T_VIR ','
#define T_PT '.'
#define T_2PT ':'
#define T_PV ';'
#define T_PLUS '+'
#define T_MOINS '-'
#define T_MULT '*'
#define T_DIV '/'
#define T_INF '<'
#define T_SUP '>'
#define T_PO '('
#define T_PF  ')'
#define T_EG '='
#define T_EOF EOF











struct token_t {
  int type; //0 : Integer, 1:Float, 2:Double, 3:Boolean
  union {
    char stringValue[100];
    int intValue;
    double doubleValue;
   } val;
};
struct token_t token;
FILE *fp;

char * str;
int i = 0;






//FUNCTIONS-----------------------------------
void expression();
void relation();
void simple_expression();
void term_cat();
void term();
void primary();
void factor();

void null_statement();
void assignement_statement();
void exit_statement();
void goto_statement();
void procedure_call_or_assign_statement();
void return_statement();
void requeue_statement();
void delay_statement();
void abort_statement();
void raise_statement();

void nextParam();


void with_use_clause();
void subprogram_body();
void formal_part();
void parameter_specification();
void sequence_statement();
void basic_declaration();
void statements();
void statement();
void simple_statement();
void compound_statement();
void if_statement();
void case_statement();
void loop_statement();
void block_statement();
void case_statement_alt();
void case_statement_alts();
void sequence_statement();
void basic_declaration() ;
void enumeration_type_definition();
void integer_type_definition();
void real_type_definition();
void array_type_definition();
void record_type_definition();
void object_number_declaration();
void enumeration_literal_specification();
void component_list() ;
void component_item();
void type_declaration();

void label();
