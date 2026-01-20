void help() {
    printf("Usage: not-conv <command> [<args>]\n\n");
    printf("Commands:\n");
    printf("  --in <notation>     Specify the input notation (infix, prefix, postfix)\n");
    printf("  --out <notation>    Specify the output notation (infix, prefix, postfix)\n");
    printf("  \"expression\"        The expression to convert (quoted if it contains spaces)\n");
    printf("  --help, --h         Show this help message\n");
    printf("  --guide, --g        Show detailed usage guide\n\n");
    printf("Examples:\n");
    printf("  not-conv --in infix --out postfix \"1 + 2\"\n");
    printf("  not-conv --in prefix --out infix \"+ 1 2\"\n");
    printf("  not-conv --in postfix --out prefix \"1 2 +\"\n");
}