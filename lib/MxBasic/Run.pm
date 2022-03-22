package MxBasic::Run;
# Команды

use common::sense;

sub new {
	my $cls = shift;
	bless {
		act => {
			"help" => "[раздел]	Помощь",
			"new" =>  "название	Создать метод",
			"open" => "название	Открыть метод",
			"list" => "[от][-до]	Список строк",
		},
		@_
	}, $cls;
}

my %ACT = (
	
);

sub run {
	local ($_) = @_;
	return if /^\s*$/;

	if( /^(\d+)/ ) { save_line(); return; }

	for my $act (sort keys %ACT) {
		my $act_first = substr $act, 0, 1;
		if( /^($act_first|$act)(?:[^a-zA-Z]|$)/ ) {
			
			eval { &{"${act}_act"}(); };
			print RED, "$@", RESET, $@ =~ /\n$/? (): "\n" if $@;
			
			return;
		}
	}

	print RED, "?", RESET, "\n";
}

sub help_act {
	if( /\s+/ ) {
		my $help = $';
		die "?\n" if !(my $h = $ACT{$help});
		my $man = $${"${help}_act"};
		print "`$help`\n";
		print "$help $h\n$man";
		return;
	}

	print "КОМАНДЫ ОБОЛОЧКИ\n\n";
	print "$_ $ACT{$_}\n" for sort keys %ACT;
	print "\n"
}

our $new_act = '
	$ - создать класс $
	swap $, $ - создать метод в классе $ c аргументом класса $
	sin#(#) - создать метод sin в классе # c аргументом класса #
	#sin! - создать метод в классе # c аргументом класса !
	(-)# - у класса # сделать левосторонний унарный метод-оператор -
	$(#) - у класса $ сделать правосторонний унарный метод-оператор #
	!(+)# - у класса ! сделать бинарный метод-оператор + c аргументом класса #
';
sub new_act {
	my $file = method_file(@_);
	die "Существует.\n" if -e $file;
	open my $f, ">", $file or die "$file: $!\n";
	close $f;
	$FILE = $file;
}

sub open_act {
	my $file = method_file(@_);
	die "Не существует.\n" if !-f $file;
	open my $f, "<", $file or die "$file: $!\n";
	close $f;
	$FILE = $file;
}

sub list_act {
	die "?\n" if !/^[a-z]+\s*(\d*)(?:-(\d*))?\s*$/i;
	my ($from, $to) = ($1, $2);

	$to = "inf" if $to == 0;

	open my $f, "<", $FILE or die "$FILE: $!";
	while(<$f>) {
		my ($n) = /^\d+/;
		print if $from <= $n;
		last if $n >= $to;
	}
	close $f;
}

1;