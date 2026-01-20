#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 100

// representation of tree nodes using struct
typedef struct NODE{
    struct NODE *left;
    struct NODE *right;
    char value;
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

// String operations
void removeSpaces(char* expression, char* result);
int split(char* str, char* tokens[]); // Split string into tokens

// functions for operators and operands
int isOperator(char ch);
int countOperands(char* tokens[], int n);
int countOperators(char* tokens[], int n);

// function to check if a notation is valid or invalid
const char* checkNotation(char *expr);

// tree functions
NODE* createNode(char value);
void freeTree(NODE* root);

// Stack for operator chars (used in shunting yard)
char operatorStack[MAX];
int operatorTop = -1;

void pushOperator(char c);
char popOperator();
char peekOperator();
int precedence(char operator);

// needs to review
void shuntingYard(const char* infix, char* postfix);

// postfix from shunting yard to tree
NODE* nodeStack[MAX];
int nodeTop = -1;

void pushNode(NODE* node);
NODE* popNode();

// Build Expression Tree from postfix
NODE* buildTreeFromPostfix(const char* postfix);

// Function to recursively build the expression tree from the prefix expression
NODE* buildTreeFromPrefix(char* prefix, int* index);

void trimQuotes(char* str);

// main function
int main(int argc, char *argv[]) {
    char *input = NULL;
    char *output = NULL;
    char *expression = NULL;
    char result[MAX] = "";

    // Check if the user is asking for help or a guide
    if(argc == 1){
        //show help
    }
    if (argc == 2) {
        if(strcmp(argv[1], "--help") == 0){
            // show help
        }else if (strcmp(argv[1], "--guide") == 0) {
            // show guide
        }else{
            printf("Error: Unknown command '%s'. Use --help.\n", argv[1]);
        }
    }else if (argc == 6) {
        if(strcmp(argv[1], "--in") == 0 && strcmp(argv[3], "--out") == 0){
            input = argv[2]; // infix|postfix|prefix
            output = argv[4]; // infix|postfix|prefix
            expression = argv[5]; // expression
            trimQuotes(expression);

            if(!input && !output){
                printf("Error: Missing --in and --out format specifier\n");
                return 1;
            }else if(!input){
                printf("Error: Missing --in format specifier\n");
                return 1;
            }else if(!output){
                printf("Error: Missing --out format specifier\n");
                return 1;
            }

            if(!expression){
                printf("Error: Missing expression format specifier\n");
                return 1;
            }
        
            // infix to postfix or prefix
            if(strcmp(input, "infix") == 0){
                if(strcmp(checkNotation(expression), input) == 0){
                    if(strcmp(output, "postfix") == 0){
                        infix_to_postfix(expression, result);
                        printf(">> %s\n", result);
                    }else if(strcmp(output, "prefix") == 0){
                        infix_to_prefix(expression, result);
                        printf(">> %s\n", result);
                    }else if(strcmp(output, "infix") == 0){
                        printf(">> %s\n", expression);
                    }else{
                        printf("Error: Invalid --out format specifier '%s': Should either be infix, postfix, or prefix\n", output);
                    }
                }else{
                    printf("Error: Input expression \"%s\" is not in infix notation\n", expression);
                }
            // prefix to postfix or infix 
            }else if(strcmp(input, "prefix") == 0){
                if(strcmp(checkNotation(expression), input) == 0){
                    if(strcmp(output, "postfix") == 0){
                        prefix_to_postfix(expression, result);
                        printf(">> %s\n", result);
                    }else if(strcmp(output, "prefix") == 0){
                        printf(">> %s\n", expression);
                    }else if(strcmp(output, "infix") == 0){
                        prefix_to_infix(expression, result);
                        printf(">> %s\n", result); 
                    }else{
                        printf("Error: Invalid --out format specifier '%s': Should either be infix, postfix, or prefix\n", output);
                    }
                }else{
                    printf("Error: Input expression \"%s\" is not in prefix notation\n", expression);
                }
            // postfix to infix or prefix
            }else if(strcmp(input, "postfix") == 0){
                if(strcmp(checkNotation(expression), input) == 0){
                    if(strcmp(output, "postfix") == 0){
                        printf(">> %s\n", expression);
                    }else if(strcmp(output, "prefix") == 0){
                        postfix_to_prefix(expression, result);
                        printf(">> %s\n", result); 
                    }else if(strcmp(output, "infix") == 0){
                        postfix_to_infix(expression, result);
                        printf(">> %s\n", result);
                    }else{
                        printf("Error: Invalid --out format specifier '%s': Should either be infix, postfix, or prefix\n", output);
                    }
                }else{
                    printf("Error: Input expression \"%s\" is not in postfix notation\n", expression);
                }
            }else{
                printf("Error: Invalid --in format specifier '%s': Should either be infix, postfix, or prefix\n", input);
            }
        }
    }else {
        printf("Error: not-conv must either be followed by --help, --guide, or --in [infix|prefix|postfix]\n\t--out [infix|prefix|postfix] \"expression\" \n ");
        // show help
        return 1;
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

int countOperands(char* tokens[], int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (isdigit(tokens[i][0]))
            count++;
    }
    return count;
}

int countOperators(char* tokens[], int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (isOperator(tokens[i][0]))
            count++;
    }
    return count;
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
    static const char* PREFIX = "prefix";
    static const char* POSTFIX = "postfix";
    static const char* INFIX = "infix";
    static const char* INVALID = "invalid";

    char* tokens[100];
    char exprCopy[256];
    strcpy(exprCopy, expr);

    int n = split(exprCopy, tokens);
    int operands = countOperands(tokens, n);
    int operators = countOperators(tokens, n);

    if (operands != operators + 1) {
        return INVALID;
    }

    if (isOperator(tokens[0][0])) {
        return PREFIX;
    } else if (isOperator(tokens[n - 1][0])) {
        return POSTFIX;
    } else {
        return INFIX;
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