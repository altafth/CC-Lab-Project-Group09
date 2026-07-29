
# Sample Outputs

Actual terminal output captured from running the compiler, for inclusion in the Project Report (Section 12, Testing chapter).

---

## 1. Valid Program — Full Pipeline

**Command:**
```
./build/compiler examples/sample.mc
```

**Output:**
```
===== Lexical Analysis =====
Lexical Analysis: SUCCESS (no lexical errors)
===== Parsing =====
Parsing successful.
===== Abstract Syntax Tree =====
Program
  Decl (int a) [line 2]
    IntLit (10) [line 2]
  Decl (float b) [line 3]
    FloatLit (20.50) [line 3]
  Decl (bool c) [line 4]
    BoolLit (true) [line 4]
  Decl (int d) [line 7]
    BinOp (/) [line 7]
      Id (a) [line 7]
      IntLit (2) [line 7]
  Decl (int e) [line 8]
    BinOp (%) [line 8]
      Id (a) [line 8]
      IntLit (3) [line 8]
  If [line 14]
    Cond:
      BinOp (&&) [line 10]
        BinOp (!=) [line 10]
          Id (a) [line 10]
          IntLit (0) [line 10]
        BinOp (>) [line 10]
          Id (b) [line 10]
          FloatLit (10.00) [line 10]
    Then:
      Block
        Print [line 11]
          Id (a) [line 11]
  While [line 17]
    Cond:
      UnOp (!) [line 14]
        BinOp (==) [line 14]
          Id (c) [line 14]
          BoolLit (false) [line 14]
    Body:
      Block
        Print [line 15]
          Id (b) [line 15]
        Assign (c) [line 16]
          BoolLit (false) [line 16]
===== Semantic Analysis =====
===== Symbol Table =====
Name      Type     Scope    Line Declared
-----------------------------------------
a         int      0        2
b         float    0        3
c         bool     0        4
d         int      0        7
e         int      0        8
Semantic analysis successful. No errors found.
===== Three Address Code (TAC) =====
t1 = 10
a = t1
t2 = 20.50
b = t2
t3 = true
c = t3
t4 = 2
t5 = a / t4
d = t5
t6 = 3
t7 = a % t6
e = t7
t8 = 0
t9 = a != t8
t10 = 10.00
t11 = b > t10
t12 = t9 && t11
ifFalse t12 goto L1
print a
L1:
L2:
t13 = false
t14 = c == t13
t15 = !t14
ifFalse t15 goto L3
print b
t16 = false
c = t16
goto L2
L3:
```

---

## 2. Lexical Error Detection

**Command:**
```
./build/compiler tests/lexical_error.mc
```

**Output:**
```
===== Lexical Analysis =====
Lexical Error at line 8: unexpected character '$'
Lexical Error at line 9: unexpected character '@'
Lexical Error at line 10: unexpected character '#'
Lexical Error at line 11: unexpected character '~'
Lexical Analysis: FAILED (4 lexical error(s) found)
```

---

## 3. Syntax Error Detection

**Command:**
```
./build/compiler tests/syntax_error.mc
```

**Output:**
```
===== Parsing =====
Syntax Error at line 3: syntax error
Syntax Error at line 7: syntax error
Syntax Error at line 11: syntax error
Syntax Error at line 14: syntax error
```

---

## 4. Semantic Error Detection

**Command:**
```
./build/compiler tests/semantic_error.mc
```

**Output:**
```
===== Semantic Analysis =====
Semantic Error at line 2: redeclaration of variable 'x'
Semantic Error at line 5: undeclared variable 'undeclared_var'
Semantic Error at line 11: assignment to undeclared variable 'z'
Semantic Error at line 14: cannot assign 'float' expression to 'bool' variable 'flag'
Semantic Error at line 17: cannot assign 'bool' expression to 'int' variable 'a'
Semantic Error at line 20: logical operators require bool operands
```
