# Instruction Set

## Bitwise
not - pop one, perform bitwise not, push result
and - pop two, perform bitwise and, push result
or - pop two, perform bitwise or, push result
xor - pop two, perform bitwise xor, push result
lls - pop one, perform left logical shift, push result
rls - pop one, perform right logical shift, push result

## Arithmetic
add - pop two, perform addition, push result
sub - pop two, perform subtraction, push result
mul - pop two, perform multiplication, push result
div - pop two, perform division, push result
mod - pop two, perform modulo, push result
ras - pop one, perform right arithmetic shift, push result

## Conditional
jez #label - pop one, jump to label if equal to zero
jnz #label - pop one, jump to label if not equal to zero
jle #label - pop one, jump to label if less than or equal zero
jlz #label - pop one, jump to label if less than zero

#label - defines a label

## State
goto state - transitions to given state
Note: transitioning states will terminate execution of current action

## Stack
push constant - push constant onto stack
drop - pop one, discard
copy - duplicate top of stack and push it onto stack
swap - pop two, push in reverse order

## Message
send - send top of stack via outbound links
