#ifndef TRUTH_H
#define TRUTH_H

#define ERR(msg)                              \
	fprintf(stderr, "Error: %s\n", msg);  \
	exit(1)

#ifndef NOLOG
#define LOG(msg) \
	if (verbose) { fprintf(stderr, "%d:\t%s\n", __LINE__, msg); }
#else
#define LOG(msg) ;
#endif

typedef enum { AND = 1, OR, NOT, XOR } Operation;
typedef enum {
	NullOperand = 0,
	StatementOperand = 1,
	NumericOperand
} OperandType;
typedef struct {
	OperandType t;
	union {
		struct Statement *s;
		int oper;
	} data;
} Operand;
typedef struct {
	Operand operands[2];
	Operation op;
} Statement;

Statement *parse(char **program, int *opcount);
void run(Statement *program, int opcount);

#endif
