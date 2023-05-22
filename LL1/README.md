# TJ2022_Compiler_LL-1-
This is a repository of the homework of 2022 Fall Compiler course at SOE, Tongji University.

You can input an LL(1) grammar in production form like P->α1|α2|... ,

for example:
5
E->TE'
E'->+TE'|~
T->FT'
T'->*FT'|~
F->(E)|i

The program will then generate FIRST and FOLLOW sets and also create an LL1 analysis table automatically.

After showing the table, we've got our syntactic parser. You can enter a sentence and the parser will
judge if it's legal and print the analysis procedure step by step. 
