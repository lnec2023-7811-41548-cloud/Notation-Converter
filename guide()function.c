void guide() {
    printf("=== Notation Converter Guide ===\n\n");
    printf("This tool converts arithmetic expressions between three notations:\n");
    printf("  • Infix   (e.g., 1 + 2)\n");
    printf("  • Prefix  (e.g., + 1 2)\n");
    printf("  • Postfix (e.g., 1 2 +)\n\n");

    printf("Usage Syntax:\n");
    printf("  not-conv --in <infix|prefix|postfix> --out <infix|prefix|postfix> \"expression\"\n\n");

    printf("Parameters:\n");
    printf("  --in       Required. Input expression format.\n");
    printf("  --out      Required. Desired output format.\n");
    printf("  \"expr\"     Required. Quoted arithmetic expression using single-digit numbers and operators + - * /\n\n");

    printf("Examples:\n");
    printf("  not-conv --in infix --out prefix \"1 + 2\"\n");
    printf("      → + 1 2\n");
    printf("  not-conv --in postfix --out infix \"1 2 +\"\n");
    printf("      → 1 + 2\n\n");

    printf("Notes:\n");
    printf("  - Only single-digit operands are supported.\n");
    printf("  - Expressions must be properly formed (operands = operators + 1).\n");
    printf("  - Multi-digit numbers or unsupported characters will raise an error.\n");
}