package Lex;

use common::sense;

use Moose;	
	
# Позиция символа в строке
has pos => (is => "ro", isa => "Int");
# Лексема (класс символа), например: int, word и т.д.
has lex => (is => "ro", isa => "Str");
# Выделенная строка
has str => (is => "ro", isa => "Str");
# Аргументы функции
has args => (is => "ro", isa => "ArrayRef[Lex]", default => sub {[]});

1;