# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl test.pl'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use Test;
BEGIN { plan tests => 1 };
use Sword;
use Devel::Peek;
ok(1); # If we made it this far, we're ok.
#########################

# Insert your test code below, the Test module is use()ed here so read
# its man page ( perldoc Test ) for help writing this test script.

my $test_name;
my $old_test_name = -1;
my $sub_test_num;
sub print_if {
	if($test_name ne $old_test_name) {
		$old_test_name = $test_name;
		$sub_test_num = 1;
	} else {
		$sub_test_num++;
	}
	print "not " if ! $_[0];
	print "ok $test_name-$sub_test_num\n";
}

$test_name = "list";
my @modules = Sword::get_modules;
print_if (@modules > 0);
$test_name = "get_verse";
# Check get_verse
print_if Sword::get_verse("James 1:19", $module_name) =~ /slow/i;
$test_name = "init";
my $module_name = $modules[0];
my $b = new Sword::VerseIterator($module_name);
# Make sure it initialized
print_if $b;
# Make sure it's initially Genesis 1:1
print_if $b->get_key eq "Genesis 1:1" && $b->get_verse =~ /beginning/i;
$test_name = "iter";
# Test prev/next
print_if $b->next(2) && $b->prev && $b->next(2);
print_if $b->get_key eq "Genesis 1:4" && $b->get_verse =~ /darkness/i;
# Test prev going back before beginning
print_if $b->prev(4);
print_if $b->get_key eq "Genesis 1:1" && $b->get_verse =~ /beginning/i;
# Test failing prev
print_if ! $b->prev;
# Test go to verse
print_if $b->go_to_verse("Revelation 4:11") && $b->get_key eq "Revelation of John 4:11";

$test_name = "iter-thread";
my $c = new Sword::VerseIterator($module_name);
# Test multiple keys at once
print_if $c && $c->get_key eq "Genesis 1:1" && $c->get_verse =~ /beginning/i;
print_if $b->get_key eq "Revelation of John 4:11" && $b->get_verse =~ /worthy/i;

$test_name = "iter-parts";
# Test book/chapter/verse get
print_if $b->get_key eq $b->get_book() . " " . $b->get_chapter_num . ":" . $b->get_verse_num;
print_if $c->get_key eq $c->get_book() . " " . $c->get_chapter_num . ":" . $c->get_verse_num;
# Test book/chapter/verse set
$b->set_chapter_num(8);
$b->set_verse_num(6);
print_if $b->get_key eq "Revelation of John 8:6";
$b->set_book("Genesis");
$b->set_chapter_num(1);
$b->set_verse_num(4);
print_if $b->get_key eq "Genesis 1:4" && $b->get_verse =~ /darkness/i;
$b->set_book("Revelation of John");
$b->set_chapter_num(4);
$b->set_verse_num(11);
print_if $b->get_key eq "Revelation of John 4:11" && $b->get_verse =~ /worthy/i;

$test_name = "module";
my $module = Sword::get_module($module_name);
# Test module attributes
print_if $module->get_name eq $module_name;
print_if $module->get_description;
print_if $module->get_type eq "Biblical Texts";
# Test get iterator
my $d = $module->get_iterator();
print_if $d->get_key eq "Genesis 1:1" && $d->get_verse =~ /beginning/i;
my $e = $module->get_iterator("James 1:19");
print_if $e->get_key eq "James 1:19" && $e->get_verse =~ /slow/i;
# Test get_verse
print_if $module->get_verse("Genesis 1:4") =~ /darkness/i;
# Test verse module stuff
my $retval = $module->verse_greater("James 3:4", "Malachi 4:5");
print_if $retval != -1 && $retval;
$retval = $module->verse_greater("Genesis 8:10", "Malachi 4:5");
print_if $retval != -1 && ! $retval;
$retval = $module->verse_greater("Genesis 12:1", "Genesis 12:1");
print_if $retval != -1 && ! $retval;
print_if $module->next_verse("Malachi 4:6") eq "Matthew 1:1";
print_if $module->prev_verse("Matthew 1:1") eq "Malachi 4:6";
print_if join(" ... ", $module->parse_verse("Revelation of John 21:12")) eq "Revelation of John ... 21 ... 12";

$test_name = "books";
my @books = $module->get_books;
# Test get books list
print_if @books == 66;
print_if $books[0] eq "Genesis" && $books[27] eq "Hosea" && $books[65] eq "Revelation of John";
# Test if # chapters is working
print_if $module->get_num_chapters("Hosea") == 14;
# Test testament stuff
print_if $module->get_testament("Hosea") eq "OT" && $module->get_testament("Mark") eq "NT";
# Test max verses
my @verses_list = $module->get_num_verses_list("I Kings");
print $verses_list[4] == 34;

$test_name = "search";
# Test search
my $found = 0;
foreach my $result ($module->search("pure")) {
	$found = 1 if $result->[0] eq "I John 3:3";
}
print_if $found;
$found = 0;
foreach my $result ($module->search_verses("pure")) {
	$found = 1 if $result eq "I John 3:3";
}
print_if $found;


$test_name = "range";
my $output_str;
my $verse_range_str = "1:5;Judges 8:12-15,17,9:2-3;18;John 4-5:4,Revelation 5;Joshua 4-5;Exodus-Leviticus";
print_if $module->make_verse_range_str($module->parse_verse_range($verse_range_str)) eq "Genesis 1:5; Judges 8:12-15,17, 9:2-3, 18:1-31; John 4:1-5:4; Revelation of John 5:1-14; Joshua 4:1-5:15; Exodus 1:1-Leviticus 27:34";
print_if $module->make_verse_range_str($module->parse_verse_range($verse_range_str, 0, 0, "Romans")) eq "Romans 1:5; Judges 8:12-15,17, 9:2-3, 18:1-31; John 4:1-5:4; Revelation of John 5:1-14; Joshua 4:1-5:15; Exodus 1:1-Leviticus 27:34";
print_if $module->make_verse_range_str($module->parse_verse_range($verse_range_str, 1)) eq "Genesis 1:5; Judges 8:12-15,17, 9:2-3, 18:0-31; John 4:0-5:4; Revelation of John 5:0-14; Joshua 4:0-5:15; Exodus 0:0-Leviticus 27:34";
print_if $module->make_verse_range_str($module->parse_verse_range("Luke 9:27-Acts 2:3", 0, 1)) eq "Luke 9:27-24:53; John 1:1-21:25; Acts 1:1-2:3";

my @verse_range = $module->parse_verse_range("Acts 21:8,4-6,40-22:1;Acts 28:31-Romans 1:1;Malachi 2:2");
my @verse_list = $module->expand_verse_range(@verse_range);
print_if $module->make_verse_range_str(@verse_list) eq "Acts 21:8,4,5,6,40, 22:1, 28:31; Romans 1:1; Malachi 2:2";
@verse_list = $module->sort_verse_list(@verse_list);
print_if $module->make_verse_range_str(@verse_list) eq "Malachi 2:2; Acts 21:4,5,6,8,40, 22:1, 28:31; Romans 1:1";

my @sorted_verse_range = $module->sort_verse_range(@verse_range);
print_if $module->make_verse_range_str(@sorted_verse_range) eq "Malachi 2:2; Acts 21:4-6,8, 21:40-22:1; Acts 28:31-Romans 1:1";

my @difficult_verse_range = $module->parse_verse_range("Acts 4:5-8,4-9,4,28:31-Romans 1:6;Acts 28:30-Romans 1:4;Acts 4:4-12,13-15");
my @made_verse_range = $module->condense_verse_range($module->sort_verse_range(@difficult_verse_range));
print_if $module->make_verse_range_str(@made_verse_range) eq "Acts 4:4-15; Acts 28:30-Romans 1:6";
my @made_verse_list = $module->condense_verse_range($module->sort_verse_list($module->expand_verse_range(@difficult_verse_range)));
print_if $module->make_verse_range_str(@made_verse_list) eq "Acts 4:4-15, 28:30-31; Romans 1:1-6";

my @verse_list1 = ("John 1:2", "John 3:4", "John 4:5", "John 4:5");
my @verse_list2 = ("John 1:2", "John 3:4", "John 3:4", "John 7:8");
print_if $module->make_verse_range_str($module->union_verse_list(\@verse_list1, \@verse_list2)) eq "John 1:2, 3:4, 4:5, 7:8";
print_if $module->make_verse_range_str($module->intersect_verse_list(\@verse_list1, \@verse_list2)) eq "John 1:2, 3:4";

my @verse_range1 = $module->parse_verse_range("John 1:2-12,2:1-4,3:6-7,9-10");
my @verse_range2 = $module->parse_verse_range("John 1:1-2,2:2,4-6,3:5-8");
print_if $module->make_verse_range_str($module->union_verse_range(\@verse_range1, \@verse_range2)) eq "John 1:1-12, 2:1-6, 3:5-10";
print_if $module->make_verse_range_str($module->intersect_verse_range(\@verse_range1, \@verse_range2)) eq "John 1:2, 2:2,4, 3:6-7";

