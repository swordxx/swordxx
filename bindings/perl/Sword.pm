package Sword;

use 5.006;
use strict;
use warnings;

require Exporter;
require DynaLoader;

our @ISA = qw(Exporter DynaLoader);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use Sword ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(
	get_modules
	get_verse
) ] );

our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw(
	
);
our $VERSION = '0.31';

bootstrap Sword $VERSION;

# Preloaded methods go here.
sub get_verse {
	my ($verse, $mod) = @_;
	if(!$mod) {
		$mod = (get_modules())[0];
	}

	my $vi = new Sword::VerseIterator $mod;
	if(!$vi->go_to_verse($verse)) {
		return;
	} else {
		return $vi->get_verse();
	}
}

my %base_modules;

sub get_module {
	my ($mod) = @_;
	$base_modules{$mod} = new Sword::Module($mod) if !$base_modules{$mod};
	return $base_modules{$mod}
}

1;


package Sword::VerseIterator;

sub set_book {
	my $this = shift;

	my ($book) = @_;
	$this->go_to_verse($book . " 1:1");
}

1;


package Sword::Module;

sub new {
 	my $class = shift;
	$class = ref($class) || $class;
	my $this = {};
	bless $this, $class;

	my ($module_name) = @_;
	$this->{MOD_NAME} = $module_name;
	$this->{VI} = new Sword::VerseIterator($module_name);

	return $this;
}

sub _initialize_book_list {
	my $this = shift;
	if(! $this->{BOOKS} ) {
		$this->{BOOKS} = {};
		$this->{BOOK_ARRAY} = [];
		foreach my $book ($this->{VI}->_get_books) {
			$book->{real_book_num} = @{$this->{BOOK_ARRAY}};
			$this->{BOOKS}{$book->{name}} = $book;
			push @{$this->{BOOK_ARRAY}}, $book->{name};
		}
	}
}

sub get_books {
	my $this = shift;
	$this->_initialize_book_list();

	return @{$this->{BOOK_ARRAY}};
}

sub get_num_chapters {
	my $this = shift;
	$this->_initialize_book_list();

	my ($book) = @_;
	$book = $this->_canonicalize_book($book);
	return $this->{BOOKS}{$book}{num_chapters};
}

sub get_num_verses_list {
	my $this = shift;
	$this->_initialize_book_list();

	my ($book) = @_;
	$book = $this->_canonicalize_book($book);
	return @{$this->{BOOKS}{$book}{chapter_maxes}};
}

sub get_testament {
	my $this = shift;
	$this->_initialize_book_list();

	my ($book) = @_;
	$book = $this->_canonicalize_book($book);
	return $this->{BOOKS}{$book}{testament_num} ? "NT" : "OT";
}

sub get_book_num {
	my $this = shift;
	$this->_initialize_book_list();

	my ($book) = @_;
	$book = $this->_canonicalize_book($book);
	return $this->{BOOKS}{$book}{book_num};
}

sub get_iterator {
	my $this = shift;
	$this->_initialize_book_list();

	my ($verse) = @_;
	my $retval = new Sword::VerseIterator($this->{MOD_NAME});
	$retval->go_to_verse($verse) if $verse;
	return $retval;
}

sub get_verse {
	my $this = shift;
	$this->_initialize_book_list();

	my ($verse) = @_;
	$this->{VI}->go_to_verse($verse);
	return $this->{VI}->get_verse;
}

sub search {
	my $this = shift;

	my ($term) = @_;
	return $this->{VI}->_search($term);
}

sub search_verses {
	my $this = shift;

	my ($term) = @_;
	return $this->{VI}->_search_verses($term);
}

sub get_name {
	my $this = shift;

	return $this->{VI}->_get_module_name();
}

sub get_description {
	my $this = shift;

	return $this->{VI}->_get_module_desc();
}

sub get_type {
	my $this = shift;

	return $this->{VI}->_get_module_type();
}

sub _canonicalize_book {
	my $this = shift;
	my ($book) = @_;
	$this->_initialize_book_list();
	return $book if $this->{BOOKS}{$book};
	$this->{VI}->set_book($book);
	return $this->{VI}->get_book();
}


# Verse stuff

sub verse_greater {
	my $this = shift;

	my ($verse1, $verse2) = @_;
	# Problem w/ sword comparing NT and OT?
	#return $this->{VI}->_verse_greater($verse1, $verse2);

	return -1 if ! $this->{VI}->go_to_verse($verse1);
	my $chap1 = $this->{VI}->get_chapter_num();
	my $verse_num1 = $this->{VI}->get_verse_num();
	my $book_struct_1 = $this->{BOOKS}{$this->{VI}->get_book()};

	return -1 if ! $this->{VI}->go_to_verse($verse2);
	my $chap2 = $this->{VI}->get_chapter_num();
	my $verse_num2 = $this->{VI}->get_verse_num();
	my $book_struct_2 = $this->{BOOKS}{$this->{VI}->get_book()};

	return 1 if $book_struct_1->{real_book_num} > $book_struct_2->{real_book_num};
	return 0 if $book_struct_1->{real_book_num} < $book_struct_2->{real_book_num};
	return 1 if $chap1 > $chap2;
	return 0 if $chap1 < $chap2;
	return 1 if $verse_num1 > $verse_num2;
	return 0 if $verse_num1 < $verse_num2;
	return 0;
}

# This method assumes that the lists are sorted.
sub union_verse_list {
	my $this = shift;

	my ($list1, $list2) = @_;
	my @retval;

	my $i1 = 0;
	my $i2 = 0;
	my $val;
	while($i1 < @{$list1} || $i2 < @{$list2}) {
		if($i1 < @{$list1}) {
			if($i2 < @{$list2} && $this->verse_greater($list1->[$i1], $list2->[$i2])) {
				$val = $list2->[$i2];
			} else {
				$val = $list1->[$i1];
			}
		} else {
			$val = $list2->[$i2];
		}

		push @retval, $val;

		# Move past duplicates
		while($i1 < @{$list1} && $list1->[$i1] eq $val) {
			$i1++;
		}
		while($i2 < @{$list2} && $list2->[$i2] eq $val) {
			$i2++;
		}
	}

	return @retval;
}


# This method assumes that the lists are sorted.
sub intersect_verse_list {
	my $this = shift;

	my ($list1, $list2) = @_;
	my @retval;

	my $i1 = 0;
	my $i2 = 0;
	my $val;
	while($i1 < @{$list1} && $i2 < @{$list2}) {
		if($list1->[$i1] eq $list2->[$i2]) {
			$val = $list1->[$i1];
			push @retval, $list1->[$i1];
		} elsif($this->verse_greater($list1->[$i1], $list2->[$i2])) {
			$val = $list2->[$i2];
		} else {
			$val = $list1->[$i1];
		}

		# Move past duplicates
		while($i1 < @{$list1} && $list1->[$i1] eq $val) {
			$i1++;
		}
		while($i2 < @{$list2} && $list2->[$i2] eq $val) {
			$i2++;
		}
	}

	return @retval;
}

# XXX this is somewhat inefficient right now (though not as bad as intersect)
sub union_verse_range {
	my $this = shift;
	my ($list1, $list2) = @_;

	my @full_list = (@{$list1}, @{$list2});
	@full_list = $this->sort_verse_range(@full_list);
	return $this->condense_verse_range(@full_list);
}

# XXX this is inefficient as heck right now
sub intersect_verse_range {
	my $this = shift;
	my ($list1, $list2) = @_;

	my @vlist1 = $this->expand_verse_range(@{$list1});
	my @vlist2 = $this->expand_verse_range(@{$list2});

	my @retval = $this->intersect_verse_list(\@vlist1, \@vlist2);

	return $this->condense_verse_range(@retval);
}

sub _parse_verse_ref {
	my $this = shift;

	my ($verse_ref, $interpret_as_chapter) = @_;

	return ("", -1, -1) if(!$verse_ref);
	
	if (! ($verse_ref =~ /^\s*(.*[A-Za-z])?\s*(\d*\s*:?\s*\d*)\s*$/)) {
		die "   >> No book, chapter or verse found ($verse_ref).\n"; 
	}

	my $book = $1 ? $this->_canonicalize_book($1) : "";
	my $num_ref = $2;
	$num_ref =~ s/\s*//g;
	return ($book, -1, -1) if ! $num_ref;
	print "   >> Bad numeric reference ($num_ref).\n" if ! ($num_ref =~ /^(\d+):?(\d*)$/);
	return ($book, -1, $1) if ! $book && ! $2 && ! $interpret_as_chapter;
	return ($book, $1, -1) if ! $2;
	return ($book, $1, $2);
}

sub parse_verse_range {
	my $this = shift;

	my ($verse_list, $include_zeroes, $split_books, $default_book) = @_;

	my @retval;

	#
	# The default book to use if the person decides to specify just
	# chapters and verses.  If the person specifies a book in the
	# query string, that is the book that will be used thereafter.
	#
	my $prev_book = $default_book ? $default_book : "Genesis";
	#
	# Note that you can't specify a verse without first having specified
	# a chapter (at some point), so this value won't be used.
	#
	my $prev_chapter = $include_zeroes ? 0 : 1;

	foreach my $outer_part (split(/;/, $verse_list)) {
		my $interpret_as_chapter = 1;
		foreach my $part (split(/,/, $outer_part)) {
			die "Too many dashes or nothing in front of the dash: $part" if !($part =~ /^([^-]+)-?([^-]*)$/);

			#
			# Get first verse part
			#
			my ($book1, $chap1, $verse1) = $this->_parse_verse_ref($1, $interpret_as_chapter);
			
			# Save the specified book/chapter/verse.
			my ($orig_book2, $orig_chap2, $orig_verse2) = ($book1, $chap1, $verse1);
			# Start interpreting raw numbers as chapters if book was specified.
			$interpret_as_chapter = 1 if $book1;
			# Start interpreting raw numbers as verses if verse was specified.
			$interpret_as_chapter = 0 if $verse1 != -1;

			#
			# Figure out the implied parts from the parts given
			#
			# book:               chapter 1, verse 1
			# book+chapter:       verse 1
			# book+verse:         impossible
			# book+chapter+verse: nothing
			# chapter:            prev. book, verse 1
			# chapter+verse:      prev. book
			# verse:              prev. book, prev. chapter
			# nothing:            impossible
			#
			# TRANSLATION:
			# BOOK:    prev. book
			# CHAPTER: chapter 1 if book specified, prev. chapter if no book specified
			# VERSE:   verse 1
			#
			$chap1 = ($include_zeroes ? 0 : 1) if $book1 && $chap1 == -1;
			$book1 = $prev_book if !$book1;
			$chap1 = $prev_chapter if $chap1 == -1;
			$verse1 = ($include_zeroes ? 0 : 1) if $verse1 == -1;

			#
			# Verify chapter/verse is in range
			#
			if($chap1 > $this->{BOOKS}{$book1}{num_chapters}) {
				die "$book1 $chap1:$verse1 invalid: $chap1 is not a chapter in $book1 (valid chapters are 1-" . $this->{BOOKS}{$book1}{num_chapters} . ")";
			}
			if($verse1 > @{$this->{BOOKS}{$book1}{chapter_maxes}}[$chap1 - 1]) {
				die "$book1 $chap1:$verse1 invalid: $verse1 is not a verse in $book1 $chap1 (valid chapters are 1-" . @{$this->{BOOKS}{$book1}{chapter_maxes}}[$chap1 - 1] . ")";
			}


			#
			# Get second verse part
			#
			my ($book2, $chap2, $verse2) = $this->_parse_verse_ref($2, $interpret_as_chapter);

			#
			# Copy any elements specified from the first one into the second.
			# "Genesis" = "Genesis-Genesis"
			# "4:9" = "4:9-4:9"
			# "5:1-12" = "5:1-5:12"
			# "Exodus 4:8-Leviticus" = "Exodus 4:8-Leviticus"
			#
			if(!$book2) {
				$book2 = $orig_book2;
				if($chap2 == -1) {
					$chap2 = $orig_chap2;
					if($verse2 == -1) {
						$verse2 = $orig_verse2;
					}
				}
			}

			#
			# Figure out the implied parts from the parts given
			#
			# book:               last chapter, last verse
			# book+chapter:       last verse
			# book+verse:         impossible
			# book+chapter+verse: nothing
			# chapter:            same book, last verse
			# chapter+verse:      same book
			# verse:              same book, same chapter
			# nothing:            impossible (elements would be copied from first part)
			#
			# TRANSLATION:
			# BOOK:    same book
			# CHAPTER: last chapter if book specified, same chapter if no book specified
			# VERSE:   last verse
			#
			$chap2 = $chap1 if !$book2 && $chap2 == -1;
			$book2 = $book1 if !$book2;
			$chap2 = $this->{BOOKS}{$book2}{num_chapters} if $chap2 == -1;
			$verse2 = @{$this->{BOOKS}{$book2}{chapter_maxes}}[$chap2 - 1] if $verse2 == -1;
			
			#
			# Verify chapter/verse is in range
			#
			if($chap2 > $this->{BOOKS}{$book2}{num_chapters}) {
				die "$book2 $chap2:$verse2 invalid: $chap2 is not a chapter in $book2 (valid chapters are 1-" . $this->{BOOKS}{$book2}{num_chapters} . ")";
			}
			if($verse2 > @{$this->{BOOKS}{$book2}{chapter_maxes}}[$chap2 - 1]) {
				die "$book2 $chap2:$verse2 invalid: $verse2 is not a verse in $book2 $chap2 (valid chapters are 1-" . @{$this->{BOOKS}{$book2}{chapter_maxes}}[$chap2 - 1] . ")";
			}

			#print "   >  $book1 $chap1:$verse1 - $book2 $chap2:$verse2\n";

			# Save previous book/chapter
			$prev_book = $book2;
			$prev_chapter = $chap2;

			if ($this->verse_greater("$book1 $chap1:$verse1", "$book2 $chap2:$verse2")) {
				die "Range $book1 $chap1:$verse1 - $book2 $chap2:$verse2 invalid: second verse must be the same or higher than the first verse.";
			}

			if(! $split_books || $book1 eq $book2) {
				push @retval, [ "$book1 $chap1:$verse1", "$book2 $chap2:$verse2" ];
			} else {
				# First book
				my $last_chapter = $this->{BOOKS}{$book1}{num_chapters};
				push @retval, [ "$book1 $chap1:$verse1", "$book1 $last_chapter:" . @{$this->{BOOKS}{$book1}{chapter_maxes}}[$last_chapter - 1] ];

				# Middle books
				for(my $i=$this->{BOOKS}{$book1}{real_book_num}+1; $i<$this->{BOOKS}{$book2}{real_book_num}; $i++) {
					my $middle_book = $this->{BOOK_ARRAY}[$i];
					my $last_chapter = $this->{BOOKS}{$middle_book}{num_chapters};
					push @retval, [ 
						"$middle_book " . ($include_zeroes ? "0:0" : "1:1"),
						"$middle_book $last_chapter:" . @{$this->{BOOKS}{$middle_book}{chapter_maxes}}[$last_chapter - 1]
					];
				}

				# Last book
				push @retval, [ "$book2 " . ($include_zeroes ? "0:0" : "1:1"), "$book2 $chap2:$verse2" ];
			}
		}
	}

	return @retval;
};

sub next_verse {
	my $this = shift;

	my ($verse) = @_;
	$this->{VI}->go_to_verse($verse);
	$this->{VI}->next;
	return $this->{VI}->get_key;
}

sub prev_verse {
	my $this = shift;

	my ($verse) = @_;
	$this->{VI}->go_to_verse($verse);
	$this->{VI}->prev;
	return $this->{VI}->get_key;
}

sub parse_verse {
	my $this = shift;

	my ($verse) = @_;

	$verse =~ /^(.*[A-Za-z])\s*(\d+)\s*:\s*(\d+)\s*$/;
	return ($this->_canonicalize_book($1), $2, $3);
}

sub sort_verse_list {
	my $this = shift;

	my @verse_list = @_;
	return sort { ($a eq $b) ? 0 : ($this->verse_greater($a, $b) ? 1 : -1) } @verse_list;
}

sub _compare_range {
	my $this = shift;

	my ($a, $b) = @_;
	if($a->[0] eq $b->[0]) {
		if($a->[1] eq $b->[1]) {
			return 0;
		} else {
			return ($this->verse_greater($a->[1], $b->[1]) ? 1 : -1);
		}
	} else {
		return ($this->verse_greater($a->[0], $b->[0]) ? 1 : -1);
	}
}

sub sort_verse_range {
	my $this = shift;

	my @verse_range = @_;
	return sort { $this->_compare_range($a, $b) } @verse_range;
}

sub _as_verse_range {
	# THIS IS NOT AN OBJECT METHOD
	return ref($_[0]) ? @{$_[0]} : ($_[0], $_[0]);
}

# This function will normalize an existing verse range or create a new one from a verse list.
# This function assumes verses are in order (by lower part of range).
# XXX This does not merge stuff with zeroes very well, I suspect.
sub condense_verse_range {
	my $this = shift;
	my @verse_range = @_;
	my @retval;

	for(my $i=0;$i<@verse_range;$i++) {
		my ($new_low, $new_high) = _as_verse_range($verse_range[$i]);
		my $i2;
		for($i2=$i+1; $i2<@verse_range; $i2++) {
			my ($vr2_low, $vr2_high) = _as_verse_range($verse_range[$i2]);

			# Check if it's 1 above the current verse and not in the same book
			my $next_verse = $this->next_verse($new_high);
			if($next_verse eq $vr2_low) {
				my ($high_book) = $this->parse_verse($new_high);
				my ($vr2_low_book) = $this->parse_verse($vr2_low);
				last if $high_book ne $vr2_low_book;
			} elsif($this->verse_greater($vr2_low, $new_high)) {
				last;
			}

			$new_high = $vr2_high if $this->verse_greater($vr2_high, $new_high);
		}
		push @retval, [ $new_low, $new_high ];
		$i = $i2 - 1;
	}

	return @retval;
}

sub expand_verse_range {
	my $this = shift;
	my @verse_range = @_;
	my @retval;

	foreach my $vr (@verse_range) {
		my ($vr_low, $vr_high) = _as_verse_range($vr);
		die "Verse not found in range $vr_low-$vr_high\n" if ! $this->{VI}->go_to_verse($vr_low);
		do {
			push @retval, $this->{VI}->get_key;
		} while($this->{VI}->get_key ne $vr_high && $this->{VI}->next);
	}

	return @retval;
}

sub make_verse_range_str {
	my $this = shift;
	my @verse_range = @_;
	my $retval;

	my $prev_book = "";
	my $prev_chapter = -1;
	foreach my $vr (@verse_range) {
		my ($vr_low, $vr_high) = _as_verse_range($vr);
		my ($book_low, $chapter_low, $verse_low) = $this->parse_verse($vr_low);
		my ($book_high, $chapter_high, $verse_high) = $this->parse_verse($vr_high);
		if($book_low ne $prev_book || $book_high ne $book_low) {
			# New book.  Put semicolon and such.
			$retval .= "; " if $prev_book;
			$retval .= "$book_low $chapter_low:$verse_low";
		} elsif($chapter_low != $prev_chapter || $chapter_high != $chapter_low) {
			# Same book.  New chapter.
			$retval .= ", $chapter_low:$verse_low"
		} else {
			$retval .= ",$verse_low";
		}

		if($book_high ne $book_low) {
			$retval .= "-$book_high $chapter_high:$verse_high";
		} elsif($chapter_high != $chapter_low) {
			$retval .= "-$chapter_high:$verse_high";
		} elsif($verse_high != $verse_low) {
			$retval .= "-$verse_high";
		}

		$prev_book = $book_high;
		$prev_chapter = $chapter_high;
	}
	return $retval;
}

1;

__END__
# Below is stub documentation for your module. You better edit it!

=head1 NAME

Sword - Perl extension to grab Bible verses from Sword (http://www.crosswire.org/sword).

=head1 SYNOPSIS

  use Sword;
  my $mod = new Sword::Module("KJV");
  my $vi = $mod->get_iterator();
  do {
	  print $vi->get_key(), " : ", $vi->get_verse(), "\n";
  } while($vi->next());


=head1 DESCRIPTION

=head2 Summary

  There are three basic ways of navigating the Bible with Sword:

  1. go directly to verses by name and then step through the Bible up and
  down

  This prints out everything from I John 2:3 to the end of the Bible:

  use Sword;
  my $mod = new Sword::Module("KJV");
  my $vi = $mod->get_iterator("I John 2:3");
  do {
     print $vi->get_key(), " : ", $vi->get_verse(), "\n";
  } while($vi->next());
  # $vi->prev() would go to the beginning of the Bible backwards


  2. Browse by book, chapter and verse

  This prints out the first verse of every book in the Bible:

  use Sword;
  my $mod = new Sword::Module("KJV");
  my $vi = $mod->get_iterator();
  foreach my $book ($mod->get_books()) {
     $vi->set_book($book);
     $vi->set_chapter_num(1);
     $vi->set_verse_num(1);
     print $vi->get_key(), " : ", $vi->get_verse(), "\n";
     # You can use get_book, get_chapter_num
  }


  3. Search for verses

  And of course you can search!  This gets everything in the KJV with the
  word "sword" in it:

  use Sword;
  my $mod = new Sword::Module("KJV");
  foreach my $result ($mod->search("sword")) {
     print "$result->[0] : $result->[1]\n";
  }

  This package has several other lesser-known but useful capabilities, as well,
  including the ability to merge separate verse lists and work with ranges of
  verses.


=head2 PACKAGE Sword

This package contains the generic routines not specific to any module.

=item get_modules([$type])

  my @modules = Sword::get_modules;

Get an array of module names by type.  Type is Biblical Texts by default.  ALL not supported yet.

=item get_module($module_name)

  my $mod = Sword::get_module("KJV");

Get an actual Sword::Module object using the given module name.

=item get_locales

  my @locales = Sword::get_locales;

Get an array of available locale names

=item set_locale

  Sword::set_locale($locale);

Sets the default locale for book names and verses and such.  Subsequent Modules and VerseIterators will be affected.

=item get_verse($verse, [$module_name])

  print Sword::get_verse("Genesis 1:4"), "\n";

Gets a verse from a module, by key.  Module name defaults to the first Biblical Text module.


=head2 CLASS Sword::Module

  my $mod = Sword::get_module("KJV");
  my $mod = new Sword::Module("KJV");

Sword::Module represents a sword module, which is really a single text (like KJV or NIV).  It is best to use
Sword::get_module() to get this.

=item new($module_name)

  my $mod = new Sword::Module("KJV");

Get a module object with the specified name HEY.


=item get_verse($verse)

  print $mod->get_verse("John 3:16"), "\n";

Get the specifed verse.

=item get_iterator([$verse])

  my $vi = $mod->get_iterator("Malachi 4:6");
  $vi->next;
  print "The verse after Malachi 4:6 is ", $vi->get_key, "\n";
	
Get a Sword::VerseIterator (see appropriate documentation.  Assumes Genesis 1:1 if none specified.

=item search($term)

  foreach my $result ($mod->search("hope")) { print "$result->[0] : $result->[1]\n"; }

Search for the specified terms using Sword, and return an array of results.  Each result is a reference
to a 2-element array containing the verse # and the actual verse.

=item search_verses($term)

  foreach my $result ($mod->search_verses("hope")) { print "$result : ", $mod->get_verse($result), "\n"; }
	
Search for the specified terms using Sword, and return a simple array containing the verse #'s only.

=item verse_greater($verse1, $verse2)

  print "James 1:19 is ", $mod->verse_greater("James 1:19", "Hebrews 11:12") ? "after" : "before", " Hebrews 11:12\n";

Find out whether verse 1 is greater than verse 2.
Returns -1 on error, 0 for false and 1 for true.

=item next_verse($verse)

  print "The verse after James 1:19 is ", $mod->next_verse("James 1:19"), "\n";

Get the verse after the given verse.  This goes past book boundaries.

=item prev_verse($verse)

  print "The verse before James 1:19 is ", $mod->prev_verse("James 1:19"), "\n";

Get the verse before the given verse.  This goes past book boundaries.

=item parse_verse($verse)

  my @parsed = $mod->parse_verse("Revelation 4:5"); print "Revelation 4:5 parses to $parsed[0] $parsed[1]:$parsed[2]\n";

Parse the given verse, canonicalizing the book (turning Revelation into "Revelation of John", for example).
This returns three pieces, the book, the chapter and the verse.  It returns 3 undefined values if it fails.


=item get_books

  print "Books in the Bible: ", join(", ", $mod->get_books()), "\n";

Get an array of the books in this module.  (Currently biblical modules that do not use some books are not supported.

=item get_num_chapters($book)

  for(my $i=0;$i<$mod->get_num_chapters("Genesis");$i++) { print $mod->get_verse("Genesis " . $i+1 . ":1" }

Get the number of chapters in a book.

=item get_num_verses_list($book)

  my @verses_list = $mod->get_num_verses_list("Malachi"); print "Genesis 4 has $verses_list[3] chapters.\n";

Get an array containing the number of verses in each chapter in a book.

=item get_testament($book)

  print "Testament of Malachi is ", $mod->get_testament("Malachi"), "\n";

Returns the testament of a book (NT or OT for New Testament or Old Testament)

=item get_book_num($book)

  print "Malachi is the ", ($mod->get_book_num("Malachi")+1), "th book in the Old Testament.\n";

Get the number of the specified book in its testament (Genesis = 0, Matthew = 0)


=item get_name

  print "Module Name: ", $mod->get_name, "\n";

Get the name of the module (e.g. KJV).

=item get_description

  print "Module Description: ", $mod->get_description, "\n";

Get the description of the module.

=item get_type

  print "Module Type: ", $mod->get_type, "\n";

Get the type of the module (e.g. Biblical Texts).


=item sort_verse_list(@verse_list)

  my @sorted_list = $mod->sort_verse_list("Acts 1:2", "John 2:3", "Malachi 1:5");

Sort a verse list (an array of verse names like "Acts 1:2") according to book/chapter/verse.

=item union_verse_list($verse_list1, $verse_list2)

  my @list1 = $mod->search_verses("trumpet");
  my @list2 = $mod->search_verses("horn");
  my @new_verse_list = $mod->union_verse_list(\@list1, \@list2);

Take two arrays of verse names ("James 1:19", etc.) and merges them together, removing duplicates.
(Tip: union a list with an empty list to just remove duplicates.)

This assumes the two lists are sorted.

=item intersect_verse_list($verse_list1, $verse_list2)

  my @list1 = $mod->search_verses("hope");
  my @list2 = $mod->search_verses("love");
  my @new_verse_list = $mod->intersect_verse_list(\@list1, \@list2);

Take two arrays of verse names ("James 1:19", etc.), takes all the verses in common, and puts them
into a new list, removing duplicates.

This assumes the two lists are sorted.

=item parse_verse_range($verse_range_str, [$include_zeroes, $split_books, $default_book])

  my @verse_ranges = $mod->parse_verse_range("James 1:19-24;Genesis 1-3;Exodus 4:2-4,6;Haggai");

Parse a list of verses and turn it into a verse range structure, which is a list of lists, each
containing 2 elements, the lower and then the upper bound, like so (this is the result of the
above example):

  ( [ "James 1:19", "James 1:24" ],
    [ "Genesis 1:1", "Genesis 3:24" ],
    [ "Exodus 4:2", "Exodus 4:4" ],
    [ "Exodus 4:6", "Exodus 4:6" ],
    [ "Haggai 1:1", "Haggai 2:23 ] )
	
The $include_zeroes parameter specifies whether the default chapter/verse should be 1 or 0
when it is unspecified.  It will be 1 if include_zeroes is false or unspecified, or 0 if it
is true.

The $split_books parameter specifies what happens when the user specifies multiple books
(e.g. "Genesis 20:12-Exodus 2:2").  Normally the system just creates one range entry
( "Genesis 20:12", "Exodus 2:2" ).  With this turned on the system creates a range entry
for each book ("Genesis 20:12", "Genesis 50:26" and "Exodus 1:1", "Exodus 2:2").  This
works even across multiple books (i.e. "Genesis-Revelation").

=item condense_verse_range(@verse_range_or_list)

  my @condensed_range = $mod->condense_verse_range($mod->parse_verse_range("Acts 8:1; Acts 8:2-5; Genesis 2:3-4,2-5"));
  my @condensed_range = $mod->condense_verse_range("Acts 1:1", "Acts 1:2", "Acts 1:3", "James 1:19");

Condense an existing range down to its minimum range (put overlapping or adjacent verses together).

This assumes the verse range or list is sorted.

=item sort_verse_range(@verse_range)

  my @sorted_verse_range = $mod->sort_verse_range($mod->parse_verse_range("Acts 8:2-4,1; Acts 9; Genesis 2"));

Sort a verse range (see parse_verse_range for structure) according to low end of
range (low to high) and then size of range (from smallest to largest).  This does not
remove duplicates.

=item union_verse_range($verse_range1, $verse_range2)

  my @range1 = $mod->parse_verse_range("John 1:2-12,2:1-4,3:6-7,9-10");
  my @range2 = $mod->parse_verse_range("John 1:1-2,2:2,4-6,3:5-8");
  my @new_verse_range = $mod->union_verse_range(\@range1, \@range2);

Take two verse range lists (see parse_verse_range for structure) and merges them together,
removing duplicates.  (Tip: union a range with an empty list to simply remove duplicates.)

This assumes the two ranges are sorted.

=item intersect_verse_range($verse_list1, $verse_list2)

  my @range1 = $mod->parse_verse_range("John 1:2-12,2:1-4,3:6-7,9-10");
  my @range2 = $mod->parse_verse_range("John 1:1-2,2:2,4-6,3:5-8");
  my @new_verse_range = $mod->intersect_verse_range(\@range1, \@range2);

Take two verse range lists (see parse_verse_range for structure), takes all the verses in common,
and puts them into a new range set, removing duplicates.

This assumes the two ranges are sorted.
=item expand_verse_range(@verse_range)

  my @verse_list = $mod->expand_verse_range($mod->parse_verse_range("Acts 8:2-4,1; Acts 9"));

Take a verse range (see parse_verse_range for structure) and turn it into a simple list of verses.

=item make_verse_range_str(@verse_range)

  my @verse_list = $mod->make_verse_range_str($mod->parse_verse_range("James 1:19-24;Genesis 1-3;Exodus 4:2-4,6;Haggai"));

Take a verse range (see parse_verse_range for structure) or verse list and turn it into a human-readable
string suitable to pass into parse_verse_range.


=head2 CLASS Sword::VerseIterator

  my $vi = $mod->get_iterator("Genesis 1:1");
  my $vi = new Sword::VerseIterator("KJV");

VerseIterator provides functions for moving up and down through the Bible more efficiently than get_verse, next_verse and prev_verse in Sword::Module.  It is best to get this using Sword::Module->get_iterator($verse).

=item new($module_name)

  my $vi = new Sword::VerseIterator("KJV");

Create a new VerseIterator on the specified module.


=item get_verse

  print "Current verse (", $vi->get_key(), ") is ", $vi->get_verse(), "\n";

Get the current verse text (meaning the actual Bible text at that verse).

=item get_key

  print "Current verse (", $vi->get_key(), ") is ", $vi->get_verse(), "\n";

Get the current verse key (e.g. "Malachi 5:5")

=item go_to_verse($verse)

  die "Could not go to Malachi 5:5" if !$vi->go_to_verse("Malachi 5:5");

Go to the specified verse (e.g. "Malachi 5:5").
Returns 0 on success, nonzero on on failure.

=item next([$how_far])

  die "Could not go to the next verse after " . $vi->get_key if !$vi->next;

Advance the iterator forward through the Bible.  how_far (default 1) tells how many verses to go.
Returns 0 on success, nonzero on on failure.

=item prev([$how_far])

  die "Could not go to the verse before " . $vi->get_key if !$vi->prev;

Advance the iterator backward through the Bible.  how_far (default 1) tells how many verses to go.
Returns 0 on success, nonzero on on failure.


=item get_book

  print "Current verse is ", $vi->get_book, " ", $vi->get_chapter_num, ":", $vi->get_verse_num, ".\n";

Get the current book (canonical--i.e. Revelation of John even if you specified Revelation in
go_to_verse or set_book).

=item get_chapter_num

  print "Current verse is ", $vi->get_book, " ", $vi->get_chapter_num, ":", $vi->get_verse_num, ".\n";

Get the current chapter #.

=item get_verse_num

  print "Current verse is ", $vi->get_book, " ", $vi->get_chapter_num, ":", $vi->get_verse_num, ".\n";

Get the current verse # within the chapter.

=item set_book

  $vi->set_book("Genesis");
  $vi->set_chapter_num(4);
  $vi->set_verse_num(5);
  print $vi->get_key, " : ", $vi->get_verse, "\n";

Set the book the iterator is on.  This resets chapter and verse to 1.

=item set_chapter_num

  $vi->set_book("Genesis");
  $vi->set_chapter_num(4);
  $vi->set_verse_num(5);
  print $vi->get_key, " : ", $vi->get_verse, "\n";

Set the chapter in the current book.  This resets verse to 1.

=item set_verse_num

  $vi->set_book("Genesis");
  $vi->set_chapter_num(4);
  $vi->set_verse_num(5);
  print $vi->get_key, " : ", $vi->get_verse, "\n";

Set the verse in the current chapter.


=item get_module_name

  print "The module we're using is ", $vi->get_module_name, "\n";

Get the name of the module this VerseIterator is going through.


=head1 AUTHOR

John Keiser, jkeiser@iname.com

=head1 SEE ALSO

L<perl>.

=cut
