int x;
int x;              // 1. Redeclaration

int y;
y = undeclared_var; // 2. Undeclared variable use

{
    int z;
    z = 5;
}
z = 10;              // 3. Scope violation (z not visible here)

bool flag;
flag = 5 + 3.2;       // 4. Type mismatch (numeric result assigned to bool)

int a;
a = flag;             // 5. Invalid assignment (bool -> int)

bool result;
result = x && y;      // 6. Invalid expression (logical op on non-bool operands)

print x;
