# system_software

## math_interpreter
This utility can solve basic math expressions like these:
+ 45 + 77
+ (2 + 2) * 2 - 99 / 9 + 24 * ((12-2) * 9)
+ 2 * 2.4 + 2 / .3 + - .1

Supported operations and their priority:
1. binary \+, binary \-
2. \* \/
3. unary \+, unary \-

Unfixed bugs:
+ Program can't cope with useless brackets nesting like these (note: plan to fix it with string analyse which will delete superfluous):
  * ((20))
  * 4 + ((10) + 5) 