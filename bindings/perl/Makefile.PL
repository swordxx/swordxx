use ExtUtils::MakeMaker;

my $sword_lib_path;
my $sword_lib_path_raw;
my $sword_include_path;

sub find_inc {
	my ($inc_dir) = @_;
	if(-f "$inc_dir/swmgr.h") {
		print "$inc_dir.\n";
		$sword_include_path = "-I$inc_dir ";
		return 1;
	} else {
		if($inc_dir !~ /sword$/) {
			return find_inc("$inc_dir/sword");
		}
		return;
	}
}

sub find_lib {
	my ($lib_dir) = @_;
	if(-f "$lib_dir/libsword.a") {
		print "$lib_dir.\n";
		$sword_lib_path = "-L$lib_dir ";
		$sword_lib_path_raw = "$lib_dir";
		return 1;
	} else {
		return;
	}
}

sub search_lib_path {
	print "Searching for Sword library ... ";
	foreach my $lib_dir (@_) {
		if(find_lib($lib_dir)) {
			return 1;
		}
	}
	print join(" ", @_), "\n";
	die "Sword not found!  Set SWORD_LIB_PATH to the directory where your libsword.a is.
You can download Sword from http://www.crosswire.org.
";
}

sub search_inc_path {
	print "Searching for Sword headers ... ";
	foreach my $inc_dir (@_) {
		if(find_inc($inc_dir)) {
			return 1;
		}
	}
	print join(" ", @_), "\n";
	die "Sword headers not found!  Set SWORD_INCLUDE_PATH to the directory where your Sword include files are.\n";
}

sub one_dir_up {
	my ($dir) = @_;
	$dir =~ s!/[^/]+/*$!!g;
	return $dir ? $dir : "/";
}

search_lib_path($ENV{SWORD_LIB_PATH}, $ENV{SWORD_PATH}, one_dir_up($ENV{SWORD_PATH}) . "/lib", "/lib", "/usr/lib", "/usr/local/lib", split(":", $ENV{LD_LIBRARY_PATH}));

search_inc_path(one_dir_up($sword_lib_path_raw) . "/include", $ENV{SWORD_INCLUDE_PATH}, $ENV{SWORD_PATH}, one_dir_up($ENV{SWORD_PATH}) . "/include", "/usr/include", "/usr/local/include");
if(! $sword_include_path ) {
}


$CC = 'g++';

# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    'NAME'		=> 'Sword',
    'VERSION_FROM'	=> 'Sword.pm', # finds $VERSION
    'PREREQ_PM'		=> {}, # e.g., Module::Name => 1.1
    ($] >= 5.005 ?    ## Add these new keywords supported since 5.005
      (ABSTRACT_FROM => 'Sword.pm', # retrieve abstract from module
       AUTHOR        => 'John Keiser <jkeiser@iname.com>') : ()),
    'LIBS'		=> ["$sword_lib_path-lsword -lz"], # e.g., '-lm'
    'DEFINE'		=> '', # e.g., '-DHAVE_SOMETHING'
    'CC'                => $CC,
    'LD'                => '$(CC)',
	# Insert -I. if you add *.h files later:
    'INC'		=> "$sword_include_path", # e.g., '-I/usr/include/other'
	# Un-comment this if you add C files to link with later:
    # 'OBJECT'		=> '$(O_FILES)', # link all the C files too
    'XSOPT'             => '-C++',
    'TYPEMAPS'          => ['perlobject.map' ],
);
