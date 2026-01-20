/** 
 * NOTATION CONVERTER 
 * 
 * MEMBERS:
 *    LOVELY NERI CENA 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 100

// representation of tree nodes using struct
typedef struct NODE{
    struct NODE *left; // left child pointer
    struct NODE *right; // right child pointer
    char value; // node value
} NODE;

// tree traversals
void postorder(NODE* node, char* result);
void inorder(NODE* node, char* result, int isRoot);
void preorder(NODE* node, char* result);

// conversion functions
void infix_to_prefix(char* expression, char* result);
void infix_to_postfix(char* expression, char* result);
void postfix_to_infix(char* expression, char* result);
void prefix_to_infix(char* expression, char* result);
void prefix_to_postfix(char* expression, char* result);
void postfix_to_prefix(char* expression, char* result);

// help operators
void help();
void guide();

// error handling functions
int isValidNotation(char *notation);

// String operations
void removeSpaces(char* expression, char* result); // remove spaces between tokens
int split(char* str, char* tokens[]); // Split string into tokens
void trimQuotes(char* str); // removes quotes from the expression

// functions for operators and operands
int isOperator(char ch);

// function to check if a notation is valid or invalid
const char* checkNotation(char *expr);

// tree functions
NODE* createNode(char value);
void freeTree(NODE* root);

// character stack for operator (used in shunting yard)
char operatorStack[MAX];
int operatorTop = -1;

// character stack functions (used in shunting yard)
void pushOperator(char c);
char popOperator();
char peekOperator();
int precedence(char operator); 

// function for the shunting yard algorithm (infix -> postfix)
void shuntingYard(const char* infix, char* postfix);

// NODE stack for postfix to expression tree (used in buildTreeFromPostfix)
NODE* nodeStack[MAX];
int nodeTop = -1;

// NODE stack functions (used in buildTreeFromPostfix)
void pushNode(NODE* node);
NODE* popNode();

// Function to build expression tree from postfix using stacks
NODE* buildTreeFromPostfix(const char* postfix);

// Function to recursively build the expression tree from the prefix expression
NODE* buildTreeFromPrefix(char* prefix, int* index);

// main function
int main(int argc, char *argv[]) {
    char *input = NULL;
    char *output = NULL;
    char *expression = NULL;
    char result[MAX] = "";

    // Check if the user is asking for help or a guide
    if(argc == 1){
        help();
        return 0;
    }else if(argc == 2) {
        if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "--h") == 0){
            help();
            return 0;
        }else if (strcmp(argv[1], "--guide") == 0 || strcmp(argv[1], "--g") == 0) {
            guide();
            return 0;
        }else{
            printf("Error: Unknown argument '%s'\n", argv[1]);
            return 1;
        }
    }else if(argc >= 3 && argc < 6) {
        // Too few arguments for a valid conversion
        printf("Error: Missing required arguments.\n");
        printf("Expected format: not-conv --in [infix|prefix|postfix] --out [infix|prefix|postfix] \"expression\"\n");
        help();
        return 1;
    }else if(argc > 6){
        printf("Error: Too many arguments provided.\n");
        help();
        return 1;
    }

    if(strcmp(argv[1], "--in") != 0 || strcmp(argv[3], "--out") != 0){
        printf("Error: Unknown argument '%s' or '%s'\n", argv[1], argv[3]);
        printf("Expected format: not-conv --in [infix|prefix|postfix] --out [infix|prefix|postfix] \"expression\"\n");
        return 1;
    }

    input = argv[2]; // infix|postfix|prefix
    output = argv[4]; // infix|postfix|prefix
    expression = argv[5]; // expression
    trimQuotes(expression);

    if(!isValidNotation(input) || !isValidNotation(output)){
        printf("Error: Invalid format specifier '%s' or '%s'\n", input, output);
        return 1;
    }

    if(strcmp(checkNotation(expression), "invalid digit") == 0){
        printf("Error: Invalid operands in expression '%s'\n", expression);
        return 1;
    }else if(strcmp(checkNotation(expression), "invalid operator") == 0){
        printf("Error: Invalid operators in expression '%s'\n", expression);
        return 1;
    }else if(strcmp(checkNotation(expression), "invalid structure") == 0){
        printf("Error: Malformed expression — operands must be exactly one more than operators.\n");
        return 1;
    }else if(strcmp(checkNotation(expression), input) != 0){
        printf("Error: Input expression '%s' is not in %s notation\n", expression, input);
        return 1;
    }

    // --in parameter and --parameter are equal, print the same expression
    if (strcmp(input, output) == 0) {
        printf("  %s\n", expression);
        return 0;
    }

    // infix to postfix or prefix
    if(strcmp(input, "infix") == 0){
        if(strcmp(output, "postfix") == 0){
            infix_to_postfix(expression, result);
            printf("  %s\n", result);
        }else if(strcmp(output, "prefix") == 0){
            infix_to_prefix(expression, result);
            printf("  %s\n", result);
        }  
    }else if(strcmp(input, "prefix") == 0){ // prefix to postfix or infix 
        if(strcmp(output, "postfix") == 0){
            prefix_to_postfix(expression, result);
            printf("  %s\n", result);
        }else if(strcmp(output, "infix") == 0){
            prefix_to_infix(expression, result);
            printf("  %s\n", result); 
        }
    }else if(strcmp(input, "postfix") == 0){ // postfix to infix or prefix
        if(strcmp(output, "prefix") == 0){
            postfix_to_prefix(expression, result);
            printf("  %s\n", result); 
        }else if(strcmp(output, "infix") == 0){
            postfix_to_infix(expression, result);
            printf("  %s\n", result);
        }       
    }
    return 0;
}

// function that creates a node
NODE* createNode(char value){
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

void freeTree(NODE* root){
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// for checking notations
int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

// Split string into tokens
int split(char* str, char* tokens[]) {
    int count = 0;
    char* token = strtok(str, " ");
    while (token != NULL) {
        tokens[count++] = token;
        token = strtok(NULL, " ");
    }
    return count;
}

const char* checkNotation(char *expr){
    // return values
    static const char* prefix = "prefix";
    static const char* postfix = "postfix";
    static const char* infix = "infix";
    static const char* invalidStructure = "invalid structure";
    static const char* invalidOperator = "invalid operator";
    static const char* invalidDigit = "invalid digit";

    char* tokens[MAX];
    char expressionCopy[MAX];
    strcpy(expressionCopy, expr);

    int n = split(expressionCopy, tokens); // splits the expression into tokens
    int operands = 0;
    int operators = 0;

    for (int i = 0; i < n; i++) {
        if(strlen(tokens[i]) == 1 ){ // if token length is 1 
            if (isdigit(tokens[i][0])) { // if token is a number [0 - 9]
                operands++;
            }else if (strchr("+-*/", tokens[i][0])) { // if token is a valid operator [ + - * /]
                operators++;
            }else{
                return invalidOperator; // a token is not a valid operator
            }
        }else{
            return invalidDigit; // a token is a multi-digit number
        }
    }

    if (operands != operators + 1) {
        return invalidStructure; // an expression is invalid if num of operands is not equal to num of operators + 1
    }

    if (isOperator(tokens[0][0])) { // if first token is an operator, prefix
        return prefix;
    } else if (isOperator(tokens[n - 1][0])) { // if last token is an operator, postfix
        return postfix;
    } else { // else, infix
        return infix;
    }
}

// for shunting yard and tree building
void pushOperator(char c) {
    if (operatorTop < MAX - 1)
        operatorStack[++operatorTop] = c;
}

char popOperator() {
    if (operatorTop >= 0)
        return operatorStack[operatorTop--];
    return '\0';
}

char peekOperator() {
    if (operatorTop >= 0)
        return operatorStack[operatorTop];
    return '\0';
}

int precedence(char operator) {
    switch (operator) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
    }
    return 0;
}

// needs to review
void shuntingYard(const char* infix, char* postfix) {
    int i = 0, k = 0;
    char c;

    while ((c = infix[i++]) != '\0') {
        if (isdigit(c)) {
            postfix[k++] = c;
            postfix[k++] = ' ';
        }
        else if (c == '(') {
            pushOperator(c);
        }
        else if (c == ')') {
            while (peekOperator() != '(' && operatorTop != -1) {
                postfix[k++] = popOperator();
                postfix[k++] = ' ';
            }
            popOperator(); // pop the '('
        }
        else if (isOperator(c)) {
            while (isOperator(peekOperator()) && precedence(peekOperator()) >= precedence(c)) {
                postfix[k++] = popOperator();
                postfix[k++] = ' ';
            }
            pushOperator(c);
        }
    }

    // Pop remaining operators
    while (operatorTop != -1) {
        postfix[k++] = popOperator();
        postfix[k++] = ' ';
    }

    postfix[k - 1] = '\0'; // replace last space with null terminator
}
void pushNode(NODE* node) {
    if (nodeTop < MAX - 1)
        nodeStack[++nodeTop] = node;
}

NODE* popNode() {
    if (nodeTop >= 0)
        return nodeStack[nodeTop--];
    return NULL;
}


// Build Expression Tree from postfix
NODE* buildTreeFromPostfix(const char* postfix) {
    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];

        if (isdigit(c)) {
            pushNode(createNode(c));
        } else if (isOperator(c)) {
            NODE* right = popNode();
            NODE* left = popNode();
            NODE* opNode = createNode(c);
            opNode->left = left;
            opNode->right = right;
            pushNode(opNode);
        }
    }

    return popNode(); // root of the expression tree
}

// Function to recursively build the expression tree from the prefix expression
NODE* buildTreeFromPrefix(char* prefix, int* index) {
    char current = prefix[*index];
    (*index)++;

    NODE* node = createNode(current);

    // If it's an operator, recursively build left and right subtrees
    if (isOperator(current)) {
        node->left = buildTreeFromPrefix(prefix, index);
        node->right = buildTreeFromPrefix(prefix, index);
    }

    // If it's an operand, just return the node (leaf)
    return node;
}

void trimQuotes(char* str){
    int len = strlen(str);
    if (str[0] == '"' && str[len - 1] == '"') {
        for (int i = 1; i < len - 1; i++) {
            str[i - 1] = str[i];
        }
        str[len - 2] = '\0'; // terminate after removing last quote
    }
}

void infix_to_prefix(char* expression, char* result){
    char postfix[MAX];
    shuntingYard(expression, postfix); // convert infix to postfix
    removeSpaces(postfix, postfix);
    NODE* root = buildTreeFromPostfix(postfix); // postfix to expression tree
    preorder(root, result);
}

void infix_to_postfix(char* expression, char* result){
    shuntingYard(expression, result); // convert infix to postfix
}

void removeSpaces(char* expression, char* result){
    int j = 0;
    for (int i = 0; expression[i] != '\0'; i++) {
        if (expression[i] == ' ') continue;
        result[j++] = expression[i];
    }
    result[j] = '\0';  // Null-terminate the string
}

void prefix_to_infix(char* expression, char* result){
    char prefix[MAX];
    removeSpaces(expression, prefix);
    int index = 0;
    NODE* root = buildTreeFromPrefix(prefix, &index);
    inorder(root, result, 1);
}

void prefix_to_postfix(char* expression, char* result){
    char prefix[MAX];
    removeSpaces(expression, prefix); // removes white spaces then store it as prefix
    int index = 0;
    NODE* root = buildTreeFromPrefix(prefix, &index);
    postorder(root, result);
}

void postfix_to_prefix(char* expression, char* result){
    char postfix[MAX];
    removeSpaces(expression, postfix);
    NODE* root = buildTreeFromPostfix(postfix);
    preorder(root, result);
}

void postfix_to_infix(char* expression, char* result){
    char postfix[MAX];
    removeSpaces(expression, postfix);
    NODE* root = buildTreeFromPostfix(postfix);
    inorder(root, result, 1);
}

void preorder(NODE* node, char* result){
    if (node == NULL) return;
    strncat(result, &node->value, 1);  // Add root value
    strcat(result, " "); // add space after token
    preorder(node->left, result);       // Visit left subtree
    preorder(node->right, result);      // Visit right subtree
}

void inorder(NODE* node, char* result, int isRoot){
    if (node == NULL) return;
    if (isOperator(node->value) && !isRoot) strcat(result, "( "); // add space after token
    inorder(node->left, result, 0);       // Visit left subtree
    strncat(result, &node->value, 1);  // Add root value
    strcat(result, " "); // add space after token
    inorder(node->right, result, 0);      // Visit right subtree
    if (isOperator(node->value) && !isRoot) strcat(result, ") "); // add space after token
}

void postorder(NODE* node, char* result){
    if (node == NULL) return;
    postorder(node->left, result);       // Visit left subtree
    postorder(node->right, result);      // Visit right subtree
    strncat(result, &node->value, 1);  // Add root value
    strcat(result, " "); // add space after token
}

int isValidNotation(char *notation){
    return strcmp(notation, "infix") == 0 || strcmp(notation, "prefix") == 0 || strcmp(notation, "postfix") == 0;
}

void help(){

}

void guide(){

}

 