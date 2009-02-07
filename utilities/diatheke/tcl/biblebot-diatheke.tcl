# Diatheke/Tcl 3.0 by Chris Little <chrislit@gotjesus.org>
# Based on code schema of <cking@acy.digex.net>

# *
# * Copyright 1999-2009 CrossWire Bible Society (http://www.crosswire.org)
# *	CrossWire Bible Society
# *	P. O. Box 2528
# *	Tempe, AZ  85280-2528
# *
# * This program is free software; you can redistribute it and/or modify it
# * under the terms of the GNU General Public License as published by the
# * Free Software Foundation version 2.
# *
# * This program is distributed in the hope that it will be useful, but
# * WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# * General Public License for more details.
# *


# This script is intended for use with libraries and modules from
# the SWORD Project, available at http://www.crosswire.org/
#
# To install Diatheke/Tcl, install an eggdrop IRC bot, copy the
# Diatheke/Tcl .tcl file to your eggdrop scripts directory, and
# add a line to the end of your bot startup script to load the
# Diatheke/Tcl script.  This script has been tested with eggdrop 1.3.23
# but should function fine in later versions.
#
# This is script assumes you have every single SWORD module installed
# that was available at the time of release.  Since you probably do not,
# remove all modules listed in the pub_books function that you do not
# have installed to hide them from end-users.
#
# The !history command only works if you have LOGGING turned on in
# diatheke (the command line C program).  This command is commented out
# of the help function and must be uncommented to make it show up for
# end-users.
#
# When you get the script loaded, type !biblehelp in a channel where
# the bot is present and it will give you a full list of commands.
# More commands will appear to you depending upon which mode the bot is
# in and whether you are an op on the bot or voiced in the channel.

set diaver 4.0

#modify this to reflect actual location of diatheke and dict binaries
set diatheke "/usr/bin/diatheke"
set dict "/usr/bin/dict"

proc publookupverse {vlookup} {
    global botnick chan bibver diatheke

    set arg "-b"
    set n [string first "@" $vlookup]
    if {$n > -1 && $n < 2} {
	append arg n
    }

    set n [string first "#" $vlookup]
    if {$n > -1 && $n < 2} {
	append arg f
    }
    set vlookup [string trimleft $vlookup "#"]
    set vlookup [string trimleft $vlookup "@"]
    set vlookup [string trimleft $vlookup "#"]

    catch {exec $diatheke -f plaintext -o $arg -b $bibver -k "$vlookup" >& /tmp/fooout.$botnick}
    catch {set foofile [open /tmp/fooout.$botnick]}
    while {[gets $foofile fooverse] >= 0} {
	set len [string length $fooverse]
	set i 0
	set j 255
	while {$j < $len} {
	    if {[regexp ">" $fooverse]} {
		while {[string index $fooverse $j] != ">" && [string index $fooverse $j] != "\n"} {set j [expr $j - 1]}
	    } else {
		while {[string index $fooverse $j] != " " && [string index $fooverse $j] != "\n"} {set j [expr $j - 1]}
	    }
	    set foo2 [string range $fooverse $i $j]
	    set foo2 [string trim $foo2]
	    regsub -all -nocase {(<FI>|<CM>|<FB>)} $foo2 {} foo2
	    regsub -all {<RF>} $foo2 {(footnote: } foo2
	    regsub -all {<Rf>} $foo2 {)} foo2
	    putmsg $chan "$foo2"
	    set i [expr $j + 1]
	    set j [expr $j + 256]
	    if {$j > $len} {set j $len}
	}
	set foo2 [string range $fooverse $i end]
	set foo2 [string trim $foo2]
	regsub -all -nocase {(<FI>|<CM>|<FB>)} $foo2 {} foo2
	regsub -all {<RF>} $foo2 {(footnote: } foo2
	regsub -all {<Rf>} $foo2 {)} foo2
	putmsg $chan "$foo2"
    }
    
    catch {close $foofile}    
    exec rm /tmp/fooout.$botnick
    return 1
}


proc pub_lookup {nick uhost hand channel arg} {
    global von chan bibver
    set chan $channel
    if {$von==0} {
	putmsg $nick "Verse display is currently off."
	return 0
    }
    if {($von==2) && (![matchattr $hand 3]) && (![matchattr $hand o])} {
	putmsg $nick "Only ops can display verses at this time."
	return 0
    }
	
    publookupverse $arg

}

bind pub - !kjv setver_kjv
bind pub - !english setver_kjv

proc setver_kjv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver KJV
    pub_lookup $nick $uhost $hand $channel $arg
}


bind pub - !niv setver_niv

proc setver_niv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NIV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !dr setver_dr
bind pub - !douayrheims setver_dr
bind pub - !dre setver_dr

proc setver_dr {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver DR
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !nas setver_nasb
bind pub - !nasb setver_nasb

proc setver_nasb {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NASB
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !hnv setver_hnv

proc setver_hnv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver HNV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !web setver_web

proc setver_web {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver WEB
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !akjv setver_akjv

proc setver_akjv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver AKJV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !bbe setver_bbe

proc setver_bbe {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver BBE
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !webster setver_webster

proc setver_webster {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Websters
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !jbc setver_jbc

proc setver_jbc {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ORTHJBC
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !rva setver_rva
bind pub - !spanish setver_rva

proc setver_rva {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SpaRVA
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !lsg setver_lsg
bind pub - !french setver_lsg

proc setver_lsg {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver FreLSG
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !bis setver_bis
bind pub - !indonesian setver_bis

proc setver_bis {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver IndBIS
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !tb setver_tb

proc setver_tb {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver IndTB
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !sve setver_sve
bind pub - !swedish setver_sve

proc setver_sve {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SweSve
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !vnt setver_vnt

proc setver_vnt {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SpaVNT
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !asv setver_asv

proc setver_asv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ASV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !rsv setver_rsv

proc setver_rsv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver RSV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !alt setver_alt

proc setver_alt {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ALT
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !dby setver_dby
bind pub - !darby setver_dby

proc setver_dby {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Darby
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !pr setver_pr
bind pub - !finnish setver_pr

proc setver_pr {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver FinPR
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !lsg setver_lsg
bind pub - !french setver_lsg

proc setver_lsg {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver FreLSG
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !lut setver_lut
bind pub - !luther setver_lut
bind pub - !german setver_lut

proc setver_lut {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver GerLut
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !sch setver_sch

proc setver_sch {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver GerSch
    pub_lookup $nick $uhost $hand $channel $arg
}


bind pub - !kar setver_kar
bind pub - !hungarian setver_kar

proc setver_kar {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver HunKar
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !nrv setver_nrv
bind pub - !italian setver_nrv

proc setver_nrv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ItaNRV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !jps setver_jps

proc setver_jps {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver JPS
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !brp setver_brp
bind pub - !portuguese setver_brp

proc setver_brp {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver PorBRP
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !sev setver_sev

proc setver_sev {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SpaSEV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !swahili setver_swahili

proc setver_swahili {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Swahili
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !wey setver_wey
bind pub - !weymouth setver_wey

proc setver_wey {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Weymouth
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !ylt setver_ylt

proc setver_ylt {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver YLT
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !alb setver_alb
bind pub - !albanian setver_alb

proc setver_alb {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Alb
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !gnpu setver_gnpu
bind pub - !chinese setver_gnpu

proc setver_gnpu {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ChiGNPU
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !dan setver_dan
bind pub - !danish setver_dan

proc setver_dan {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Dan
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !lei setver_lei
bind pub - !dutch setver_lei

proc setver_lei {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver DutLEI
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !lu setver_lu

proc setver_lu {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver DutLU
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !svv setver_svv

proc setver_svv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver DutSVV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !shr setver_shr
bind pub - !equadoran setver_shr

proc setver_shr {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver EquShr
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !crl setver_crl
bind pub - !creole setver_crl

proc setver_crl {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver FreCrl
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !fredrb setver_fredrb

proc setver_fredrb {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver FreDrb
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !ben setver_ben

proc setver_ben {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver GerBen
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !elb setver_elb

proc setver_elb {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver GerElb
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !kj21 setver_kj21

proc setver_kj21 {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver KJ21
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !litv setver_litv

proc setver_litv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver LITV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !lo setver_lo

proc setver_lo {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver LO
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !maori setver_maori

proc setver_maori {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Maori
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !mkjv setver_mkjv

proc setver_mkjv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver MKJV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !nkj setver_nkjv
bind pub - !nkjv setver_nkjv

proc setver_nkjv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NKJV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !ceb setver_ceb
bind pub - !filipino setver_ceb

proc setver_ceb {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver PhiCeb
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !rwebster setver_rwebster

proc setver_rwebster {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver RWebster
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !rv setver_rv

proc setver_rv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SpaRV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !turkish setver_turkish

proc setver_turkish {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Turkish
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !uma setver_uma

proc setver_uma {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Uma
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !vulgate setver_vulgate
bind pub - !vulg setver_vulgate
bind pub - !latin setver_vulgate

proc setver_vulgate {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Vulgate
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !nasb95 setver_nasb95
bind pub - !nas95 setver_nasb95
bind pub - !nau setver_nasb95

proc setver_nasb95 {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NASB95
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !nivbr setver_nivbr
bind pub - !nib setver_nivbr

proc setver_nivbr {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NIVBr
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !nlt setver_nlt

proc setver_nlt {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NLT
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !nrsv setver_nrsv

proc setver_nrsv {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NRSV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !neg setver_neg

proc setver_neg {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver FreNEG
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !lnd setver_lnd

proc setver_lnd {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ItaLND
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !mel setver_mel
bind pub - !melanesian setver_mel

proc setver_mel {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Mel
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !norsk setver_norsk

proc setver_norsk {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Norsk
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !cor setver_cor

proc setver_cor {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver RomCor
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !lba setver_lba

proc setver_lba {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SpaLBA
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !byz setver_byz

proc setver_byz {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ByzX
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !lxx setver_lxx

proc setver_lxx {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver LXXX
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !na26 setver_na26
bind pub - !greek setver_na26

proc setver_na26 {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NA26X
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !scrivner setver_scrivner

proc setver_scrivner {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ScrivnerX
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !stephanus setver_stephanus

proc setver_stephanus {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver StephanusX
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !tischendorf setver_tisch
bind pub - !tisch setver_tisch

proc setver_tisch {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver TischX
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !wh setver_wh

proc setver_wh {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver WHX
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !whnu setver_whnu

proc setver_whnu {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver WHNUX
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !grkbyz setver_grkbyz

proc setver_grkbyz {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Byz
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !grklxx setver_grklxx

proc setver_grklxx {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver LXX
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !grkna26 setver_grkna26
bind pub - !grkgreek setver_na26

proc setver_grkna26 {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NA26
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !grkscrivner setver_grkscrivner

proc setver_grkscrivner {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Scrivner
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !grkstephanus setver_grkstephanus

proc setver_grkstephanus {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Stephanus
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !grktischendorf setver_grktisch
bind pub - !grktisch setver_grktisch

proc setver_grktisch {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Tisch
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !grkwh setver_grkwh

proc setver_grkwh {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver WH
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !grkwhnu setver_grkwhnu

proc setver_grkwhnu {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver WHNU
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !bhs setver_bhs
bind pub - !hebrew setver_bhs

proc setver_bhs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver BHS
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !rst setver_rst

proc setver_rst {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver RST
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !ukrainian setver_ukrainian

proc setver_ukrainian {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Ukrainian
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !arabic setver_arabic

proc setver_arabic {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Arabic
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !n27u4 setver_n27u4
bind pub - !nu setver_n27u4
bind pub - !bgreek setver_n27u4

proc setver_n27u4 {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver N27U4
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !umgreek setver_ukgreek

proc setver_umgreek {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver UMGreek
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !viet setver_viet
bind pub - !vietnamese setver_viet

proc setver_viet {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Viet
    pub_lookup $nick $uhost $hand $channel $arg
}


# ----------------------------------------------------------------------

proc pubsearchword {vlookup} {
    global botnick chan bibver diatheke
    catch {exec $diatheke -s $bibver "$vlookup" >& /tmp/fooout.$botnick}
    catch {set foofile [open /tmp/fooout.$botnick]}

    while {[gets $foofile fooverse] >= 0} {
	set len [string length $fooverse]
	set i 0
	set j 72
	while {$j < $len} {
	    while {[string index $fooverse $j] != ";" && [string index $fooverse $j] != ")" && [string index $fooverse $j] != "\n"} {set j [expr $j + 1]}
	    set foo2 [string range $fooverse $i $j]
	    set foo2 [string trim $foo2]
	    putmsg $chan "$foo2"
	    set i [expr $j + 1]
	    set j [expr $j + 73]
	    if {$j > $len} {set j $len}
	}
	set foo2 [string range $fooverse $i end]
	set foo2 [string trim $foo2]
	putmsg $chan "$foo2"
    }
    catch {close $foofile}
    
    putmsg $chan "$fooverse"
    exec rm /tmp/fooout.$botnick
    return 1
}


proc pub_lookups {nick uhost hand channel arg} {
    global von chan bibver
    set chan $channel
    if {$von==0} {
	putmsg $nick "Verse display is currently off."
	return 0
    }
    if {($von==3)  && (![matchattr $hand 3]) && (![matchattr $hand o]) && (![isvoice $nick $chan])} {
	putmsg $nick "Sorry, only ops and voiced users can do searches right now."
	return 0
    }
    if {($von==2) && (![matchattr $hand 3]) && (![matchattr $hand o])} {
	putmsg $nick "Sorry, only ops can do searches right now."
	return 0
    }
    
    pubsearchword $arg
}

bind pub - !skjv setver_kjvs
bind pub - !senglish setver_kjvs

proc setver_kjvs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver KJV
    pub_lookups $nick $uhost $hand $channel $arg
}


bind pub - !sniv setver_nivs

proc setver_nivs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NIV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sdr setver_drs
bind pub - !sdouayrheims setver_drs
bind pub - !sdre setver_drs

proc setver_drs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver DR
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !snas setver_nasbs
bind pub - !snasb setver_nasbs

proc setver_nasbs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NASB
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !shnv setver_hnvs

proc setver_hnvs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver HNV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sweb setver_webs

proc setver_webs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver WEB
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sbbe setver_bbes

proc setver_bbes {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver BBE
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !swebster setver_websters

proc setver_websters {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Websters
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sjbc setver_jbcs

proc setver_jbcs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ORTHJBC
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !srva setver_rvas
bind pub - !sspanish setver_rvas

proc setver_rvas {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SpaRVA
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sbis setver_biss
bind pub - !sindonesian setver_biss

proc setver_biss {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver IndBIS
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !stb setver_tbs

proc setver_tbs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver IndTB
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !ssve setver_sves
bind pub - !sswedish setver_sves

proc setver_sves {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SweSve
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !svnt setver_vnts

proc setver_vnts {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SpaVNT
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sasv setver_asvs

proc setver_asvs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ASV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !srsv setver_rsvs

proc setver_rsvs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver RSV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !salt setver_alts

proc setver_alts {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ALT
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sdby setver_dbys
bind pub - !sdarby setver_dbys

proc setver_dbys {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Darby
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !spr setver_prs
bind pub - !sfinnish setver_prs

proc setver_prs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver FinPR
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !slsg setver_lsgs
bind pub - !sfrench setver_lsgs

proc setver_lsgs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver FreLSG
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !slut setver_luts
bind pub - !sluther setver_luts
bind pub - !sgerman setver_luts

proc setver_luts {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver GerLut
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !ssch setver_schs

proc setver_schs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver GerSch
    pub_lookups $nick $uhost $hand $channel $arg
}


bind pub - !skar setver_kars
bind pub - !shungarian setver_kars

proc setver_kars {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver HunKar
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !snrv setver_nrvs
bind pub - !sitalian setver_nrvs

proc setver_nrvs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ItaNRV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sjps setver_jpss

proc setver_jpss {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver JPS
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sbrp setver_brps
bind pub - !sportuguese setver_brps

proc setver_brps {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver PorBRP
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !ssev setver_sevs

proc setver_sevs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SpaSEV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sswahili setver_swahilis

proc setver_swahilis {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Swahili
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !swey setver_weys
bind pub - !sweymouth setver_weys

proc setver_weys {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Weymouth
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sylt setver_ylts

proc setver_ylts {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver YLT
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !salb setver_albs
bind pub - !salbanian setver_albs

proc setver_albs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ALB
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sgnpu setver_gnpus
bind pub - !schinese setver_gnpus

proc setver_gnpus {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ChiGNPU
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sdan setver_dans
bind pub - !sdanish setver_dans

proc setver_dans {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Dan
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !slei setver_leis
bind pub - !sdutch setver_leis

proc setver_leis {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver DutLEI
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !slu setver_lus

proc setver_lus {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver DutLU
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !ssvv setver_svvs

proc setver_svvs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver DutSVV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sshr setver_shrs
bind pub - !sequadoran setver_shrs

proc setver_shrs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver EquShr
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !scrl setver_crls
bind pub - !screole setver_crls

proc setver_crls {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver FreCrl
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sfredrb setver_fredrbs

proc setver_fredrbs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver FreDrb
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sben setver_bens

proc setver_bens {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver BerBen
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !selb setver_elbs

proc setver_elbs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver GerElb
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !skj21 setver_kj21s

proc setver_kj21s {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver KJ21
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !slitv setver_litvs

proc setver_litvs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver LITV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !slo setver_los

proc setver_los {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver LO
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !smaori setver_maoris

proc setver_maoris {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Mao
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !smkjv setver_mkjvs

proc setver_mkjvs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver MKJV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !snkjv setver_nkjvs

proc setver_nkjvs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NKJV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sceb setver_cebs
bind pub - !sfilipino setver_cebs

proc setver_cebs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver PhiCeb
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !srwebster setver_rwebsters

proc setver_rwebsters {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver RWebster
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !srv setver_rvs

proc setver_rvs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SpaRV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sturkish setver_turkishs

proc setver_turkishs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Turkish
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !suma setver_umas

proc setver_umas {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Uma
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !svulgate setver_vulgates
bind pub - !svulg setver_vulgates
bind pub - !slatin setver_vulgates

proc setver_vulgates {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Vulgate
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !snasb95 setver_nasb95s
bind pub - !snas95 setver_nasb95s
bind pub - !snau setver_nasb95s

proc setver_nasb95s {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NASB95
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !snivbr setver_nivbrs
bind pub - !snib setver_nivbrs

proc setver_nivbrs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NIVBr
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !snlt setver_nlts

proc setver_nlts {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NLT
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !snrsv setver_nrsvs

proc setver_nrsvs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NRSV
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sneg setver_negs

proc setver_negs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver FreNEG
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !slnd setver_lnds

proc setver_lnds {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ItaLND
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !smel setver_mels
bind pub - !smelanesian setver_mels

proc setver_mels {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Mel
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !snorsk setver_norsks

proc setver_norsks {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Norsk
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !scor setver_cors

proc setver_cors {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver RomCor
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !slba setver_lbas

proc setver_lbas {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver SpaLBA
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sbyz setver_byzs

proc setver_byzs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ByzX
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !slxx setver_lxxs

proc setver_lxxs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver LXXX
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sna26 setver_na26s
bind pub - !sgreek setver_na26s

proc setver_na26s {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NA26X
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sscrivner setver_scrivners

proc setver_scrivners {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ScrivnerX
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sstephanus setver_stephanuss

proc setver_stephanuss {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver StephanusX
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !stischendorf setver_tischs
bind pub - !stisch setver_tischs

proc setver_tischs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver TischX
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !swh setver_whs

proc setver_whs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver WHX
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !swhnu setver_whnus

proc setver_whnus {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver WHNUX
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sgrkbyz setver_grkbyzs

proc setver_grkbyzs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Byz
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sgrklxx setver_grklxxs

proc setver_grklxxs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver LXX
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sgrkna26 setver_grkna26s
bind pub - !sgrkgreek setver_na26s

proc setver_grkna26s {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver NA26
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sgrkscrivner setver_grkscrivners

proc setver_grkscrivners {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Scrivner
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sgrkstephanus setver_grkstephanuss

proc setver_grkstephanuss {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Stephanus
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sgrktischendorf setver_grktischs
bind pub - !sgrktisch setver_grktischs

proc setver_grktischs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Tisch
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sgrkwh setver_grkwhs

proc setver_grkwhs {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver WH
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sgrkwhnu setver_grkwhnus

proc setver_grkwhnus {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver WHNU
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sbhs setver_bhss
bind pub - !shebrew setver_bhss

proc setver_bhss {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver BHS
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !srst setver_rsts

proc setver_rsts {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver RST
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sukrainian setver_ukrainians

proc setver_ukrainians {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Ukrainian
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sarabic setver_arabics

proc setver_arabics {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Arabic
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sn27u4 setver_n27u4s
bind pub - !snu setver_n27u4s
bind pub - !sbgreek setver_n27u4s

proc setver_n27u4s {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver N27U4
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sumgreek setver_ukgreeks

proc setver_umgreeks {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver UMGreek
    pub_lookups $nick $uhost $hand $channel $arg
}

bind pub - !sviet setver_viets
bind pub - !svietnamese setver_viets

proc setver_viets {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Viet
    pub_lookups $nick $uhost $hand $channel $arg
}

#----------------------------------------------------------------------


proc publookupdict {vlookup} {
    global botnick chan bibver diatheke
    catch {exec $diatheke -f plaintext -b $bibver -k "$vlookup" >& /tmp/fooout.$botnick}
    catch {set foofile [open /tmp/fooout.$botnick]}

    while {[gets $foofile fooverse] >= 0} {
	putmsg $chan "$fooverse"
    }
    catch {close $foofile}
  #  exec rm /tmp/fooout.$botnick
    return 1
}


proc pub_lookupd {nick uhost hand channel arg} {
    global von chan bibver
    set chan $channel
    if {$von==0} {
	putmsg $nick "Verse display is currently off."
	return 0
    }
    if {($von==3) && (![matchattr $hand 3]) && (![matchattr $hand o]) && (![isvoice $nick $chan])} {
	putmsg $nick "Sorry, only ops and voiced users can use dictionaries and indices right now."
	return 0
    }
    if {($von==2) && (![matchattr $hand 3]) && (![matchattr $hand o])} {
	putmsg $nick "Sorry, only ops can use dictionaries and indices right now."
	return 0
    }
    
    publookupdict $arg
}

bind pub - !losung setver_losung

proc setver_losung {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver losung_en_99
    set arg [exec date "+%m.%d"]
    pub_lookupd $nick $uhost $hand $channel $arg
}

bind pub - !vines setver_vines

proc setver_vines {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Vines
    pub_lookupd $nick $uhost $hand $channel $arg
}

bind pub - !naves setver_naves

proc setver_naves {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Naves
    pub_lookupd $nick $uhost $hand $channel $arg
}

bind pub - !eastons setver_eastons

proc setver_eastons {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Eastons
    pub_lookupd $nick $uhost $hand $channel $arg
}

bind pub - !strheb setver_strheb

proc setver_strheb {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver StrongsHebrew
    pub_lookupd $nick $uhost $hand $channel $arg
}

bind pub - !strgrk setver_strgrk

proc setver_strgrk {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver StrongsGreek
    pub_lookupd $nick $uhost $hand $channel $arg
}

bind pub - !bdb setver_bdb

proc setver_bdb {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver BDB
    pub_lookupd $nick $uhost $hand $channel $arg
}

bind pub - !thayer setver_thayer

proc setver_thayer {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Thayer
    pub_lookupd $nick $uhost $hand $channel $arg
}

bind pub - !hitch setver_hitch

proc setver_hitch {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Hitch
    pub_lookupd $nick $uhost $hand $channel $arg
}

bind pub - !isbe setver_isbe

proc setver_isbe {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver ISBE
    pub_lookupd $nick $uhost $hand $channel $arg
}

bind pub - !smiths setver_smiths

proc setver_smiths {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Smiths
    pub_lookupd $nick $uhost $hand $channel $arg
}

bind pub - !torrey setver_torrey

proc setver_torrey {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Torrey
    pub_lookupd $nick $uhost $hand $channel $arg
}

#----------------------------------------------------------------------

proc publookupcomm {vlookup} {
    global botnick chan bibver diatheke
    catch {exec $diatheke -c $bibver "$vlookup" >& /tmp/fooout.$botnick}
    catch {set foofile [open /tmp/fooout.$botnick]}
    while {[gets $foofile fooverse] >= 0} {
	set len [string length $fooverse]
	set i 0
	set j 72
	while {$j < $len} {
	    while {[string index $fooverse $j] != " " && [string index $fooverse $j] != "\n"} {set j [expr $j + 1]}
	    set foo2 [string range $fooverse $i $j]
	    set foo2 [string trim $foo2]
	    regsub -all -nocase {(<FI>|<CM>|<FB>)} $foo2 {} foo2
	    regsub -all {<RF>} $foo2 {(footnote: } foo2
	    regsub -all {<Rf>} $foo2 {)} foo2
	    putmsg $chan "$foo2"
	    set i [expr $j + 1]
	    set j [expr $j + 73]
	    if {$j > $len} {set j $len}
	}
	set foo2 [string range $fooverse $i end]
	set foo2 [string trim $foo2]
	regsub -all -nocase {(<FI>|<CM>|<FB>)} $foo2 {} foo2
	regsub -all {<RF>} $foo2 {(footnote: } foo2
	regsub -all {<Rf>} $foo2 {)} foo2
	putmsg $chan "$foo2"
    }
    catch {close $foofile}
    exec rm /tmp/fooout.$botnick
    return 1
}


proc pub_lookupc {nick uhost hand channel arg} {
    global von chan bibver
    set chan $channel
    if {$von==0} {
	putmsg $nick "Verse display is currently off."
	return 0
    }
    if {($von==3) && (![matchattr $hand 3]) && (![matchattr $hand o]) && (![isvoice $nick $chan])} {
	putmsg $nick "Sorry, only ops and voiced users can use commentaries right now."
	return 0
    }
    if {($von==2) && (![matchattr $hand 3]) && (![matchattr $hand o])} {
	putmsg $nick "Sorry, only ops can use commentaries right now."
	return 0
    }

    publookupcomm $arg
}

bind pub - !rwp setver_rwp

proc setver_rwp {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver RWP
    pub_lookupc $nick $uhost $hand $channel $arg
}

bind pub - !mhc setver_mhc

proc setver_mhc {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver MHC
    pub_lookupc $nick $uhost $hand $channel $arg
}

bind pub - !dtn setver_dtn

proc setver_dtn {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver DTN
    pub_lookupc $nick $uhost $hand $channel $arg
}

bind pub - !family setver_family

proc setver_family {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Family
    pub_lookupc $nick $uhost $hand $channel $arg
}

bind pub - !geneva setver_geneva

proc setver_geneva {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Geneva
    pub_lookupc $nick $uhost $hand $channel $arg
}

bind pub - !gill setver_gill

proc setver_gill {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Gill
    pub_lookupc $nick $uhost $hand $channel $arg
}

bind pub - !pnt setver_pnt

proc setver_pnt {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver PNT
    pub_lookupc $nick $uhost $hand $channel $arg
}

bind pub - !tfg setver_tfg

proc setver_tfg {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver TFG
    pub_lookupc $nick $uhost $hand $channel $arg
}

bind pub - !tsk setver_tsk

proc setver_tsk {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver TSK
    pub_lookupc $nick $uhost $hand $channel $arg
}

bind pub - !wesley setver_wesley

proc setver_wesley {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver Wesley
    pub_lookupc $nick $uhost $hand $channel $arg
}

#----------------------------------------------------------------------

bind pub - !dict dictlookup

proc dictlookup {nick uhost hand channel arg} {
    global botnick von dict
    
    if {$von==0} {
	putmsg $nick "Verse display is currently off."
	return 0
    }
    if {($von==3) && (![matchattr $hand 3]) && (![matchattr $hand o]) && (![isvoice $nick $channel])}  {
	putmsg $nick "Sorry, only ops and voiced users can use dictionaries and indices right now."
	return 0
    }
    if {($von==2) && (![matchattr $hand 3]) && (![matchattr $hand o])} {
	putmsg $nick "Only ops can use dictionaries and indices right now."
	return 0
    }
    
    catch {exec $dict "$arg" >& /tmp/fooout.$botnick}
    catch {set foofile [open /tmp/fooout.$botnick]}
    catch {set fooverse [gets $foofile]}
    while {[gets $foofile fooverse] >= 0} {
	set fooverse [string trim $fooverse]
	putmsg $channel "$fooverse"
    }
    catch {close $foofile}
    exec rm /tmp/fooout.$botnick
    return 1
}

#----------------------------------------------------------------------

bind pub - !biblehelp pub_help
bind msg - biblehelp pub_help

proc pub_help {nick uhost hand channel arg} {
    global diaver
    global von
    putserv "NOTICE $nick :Diatheke/Tcl BibleBot version $diaver"

    if {(($von==0) || ($von==2)) && (![matchattr $hand 3]) && (![matchattr $hand o])} {
	putserv "NOTICE $nick :BibleBot displays are currently turned off."
	return 1
    }

    putserv "NOTICE $nick :Supported commands:"
    putserv "NOTICE $nick :Help, using \"!biblehelp\""
    putserv "NOTICE $nick :Book list, using \"!books\" (it's long)"
# Uncomment the next line if you have turned LOGGING on in diatheke to enable
# the history function.
#    putserv "NOTICE $nick :See last 5 calls to BibleBot, using \"!history\""
    putserv "NOTICE $nick :Check display status, using \"!status\""
    putserv "NOTICE $nick :Bible lookups, using \"!<bible version> <book> <chapter>:<verse>\""
    putserv "NOTICE $nick :verse ranges can be specified by adding \"-<last verse>\" to this"
    putserv "NOTICE $nick :To turn Strong's numbers and/or footnotes on, use @ and/or # respectively before the book name.  For example \"!kjv @#Gen 1:4\" will retrieve Genesis 1:3 with the Strong's numbers and footnotes associated with it."

    if {($von==3) && (![matchattr $hand 3]) && (![matchattr $hand o]) && (![isvoice $nick $channel])}  {
	return 1
    }

    putserv "NOTICE $nick :Commentary lookups, using \"!<commentary> <book> <chapter>:<verse>\""
    putserv "NOTICE $nick :Dictionary/index lookups, using \"!<dictionary> <word or number>\""
    putserv "NOTICE $nick :Bible searches, using \"!s<bible version> <word>\""
    putserv "NOTICE $nick :Diatheke/Tcl defaults to PHRASE search mode.  To use MULTI-WORD search mode, preface your search with an @.  To use REGEX mode, preface your search with a #.  For example: \"!skjv @Jesus love\" will print a list of all verses in the KJV containing the words Jesus and love."

    if {(![matchattr $hand 3]) && (![matchattr $hand o])}  {
	return 1
    }

    putserv "NOTICE $nick :To turn verse display off, use \"!verseoff\""
    putserv "NOTICE $nick :To turn all displays on for all users, use \"!verseon\""
    putserv "NOTICE $nick :To turn all displays on for ops only, use \"!verseon o\""
    putserv "NOTICE $nick :To turn verse diaplays on for regular users and all other displays on for ops and voiced users only, use \"!verseon v\" (default)"
}

bind pub - !books pub_books
bind msg - books pub_books

proc pub_books {nick uhost hand channel arg} {
    global von

    if {(($von==0) || ($von==2)) && (![matchattr $hand 3]) && (![matchattr $hand o])} {
	putserv "NOTICE $nick :BibleBot displays are currently turned off."
	return 1
    }

    putserv "NOTICE $nick :English language Bibles (26):"
    putserv "NOTICE $nick :Ameican Standard Version (!asv), Analytical Literal Translation (!alt), Bible in Basic English (!bbe), Darby (!dby), Douay-Rheims Bible (!dr), Green's Literal Translation (!litv), Green's Modern King James Version (!mkjv), Hebrew Names Version (!hnv), Jewish Publication Society 1917 (!jps), King James Version (!kjv), The Living Oracles NT (!lo),"
    putserv "NOTICE $nick :New American Standard Bible (!nasb), New American Standard Bible, 95 Update (!nasb95), New International Version (!niv), New Internation Version, British Edition (!nivbr), New King James Version (!nkjv), New Living Translation (!nlt), New Revised Standard Version (!nrsv), Orthodox Jewish Brit ChadashaNT only (!jbc),"
    putserv "NOTICE $nick :Revised Standard Version (!rsv), Revised 1833 Webster's (!rwebster), World English Bible (!web), Webster's (!webster), Weymouth NT (!wey), Young's Literal Translation (!ylt), 21st Century King James Version (!kj21)"
    putserv "NOTICE $nick :Non-English language Bibles (39):"
    putserv "NOTICE $nick :Albanian Bible (!alb), Chinese GNPU (!gnpu), Danish Bible (!dan), Dutch Leidse Vertaling (!lei), Dutch Lutherse Vertaling (!lu), Dutch Statemvertaling (!svv), Equadoran Shuar NT (!shr), Filipino Nga Cebuano (!ceb),  Finnish Pyhz Raamattu (!pr), French Louis Segond Version (!lsg), French Haitian Creole Version (!crl), French Darby's Version (!fredrb),"
    putserv "NOTICE $nick :French Nouvelle Edition de Geneve (!neg), German Luther Version (!lut), German Schlachter (!sch), German Bengel NT (!ben), German Elberfelder Version (!elb), Hungarian Karoli (!kar), Indonesian Bahasa Indonesia Sehari-hari (!bis), Indonesian Terjemahan Baru (!tb), Italian La Nouva Diodati (!lnd), Italian La Sacra Bibbia Nuova Riveduta (!nrv),"
    putserv "NOTICE $nick :Maori Bible (!maori), Melanesian Pidgin Bible (!mel), Norsk Bible (!norsk), Portuguese A Biblia Sagrada Traduzida em Portugues (!brp), Spanish La Biblia de Las Americas (!lba), Spanish Reina-Valera Actualizada (!rva), Spanish Reina-Valera (!rv), Spanish Sagradas Escrituras (!sev), Spanish Valera NT only (!vnt), Swahili NT (!swahili),"
    putserv "NOTICE $nick :Swedish 1917 Bible NT only (!sve), Turkish NT (!turkish), Uma NT (!uma)"
    putserv "NOTICE $nick :Original Language Bibles (18): (NB, /'s divide transliterated/non-roman versions)"
    putserv "NOTICE $nick :1991 Byzantine/Majority Text (!byz/!grkbyz), Nestle-Aland 26th/27th Ed. (!na26/!grkna26), 1894 Scrivner Textus Receptus (!scrivner/!grkscrivner), 1550 Stephanus Textus Receptus (!stephanus/!grkstephanus), Tischendorf' 8th Ed. GNT (!tisch/!grktisch), 1881 Westcott-Hort GNT (!wh/!grkwh), 1881 Westcott-Hort with NA26 alternate readings (!whnu/!grkwhnu)"
    putserv "NOTICE $nick: Nestle-Aland 27th Ed./UBS 4th Ed. (!n27u4), Septuagint/LXX (!lxx/!grklxx), Biblia Hebraica Stuttgartensia in Hebrew font (!bhs), Jerome's Latin Vulgate (!vulg)"
    putserv "NOTICE $nick :Non-Roman character Bibles (5):"
    putserv "NOTICE $nick :Arabic Bible (!arabic), Russian Synodal Translation (!rst), Ukrainian Bible (!ukrainian), Unaccented Modern Greek Bible (!umgreek), Vietnamese Bible (!viet)"

    if {($von==3) && (![matchattr $hand 3]) && (![matchattr $hand o]) && (![isvoice $nick $channel])}  {
	return 1
    }

    putserv "NOTICE $nick :Dictionaries & Indices (12):"
    putserv "NOTICE $nick :Webster's Dictionary (!dict), Vine's Bible Dictionary(!vines), Easton's Bible Dictionary (!eastons), Nave's Topical Bible (!naves), Smith's Bible Dictionary (!smiths), Hitchcock's Bible Names Dictionary (!hitch), International Standard Bible Dictionary (!isbe), Torrey's New Topical Textbook (!torrey), Strong's Hebrew Bible Dictionary (!strheb), Strong's Greek Bible Dictionary (!strgrk), Brown-Driver-Briggs Hebrew Lexicon (!bdb), Thayer's Greek Lexicon (!thayer)"
    putserv "NOTICE $nick :Commentaries (10):"
    putserv "NOTICE $nick :Darby's Translation Notes (!dtn), Family Bible Notes (!family), Geneva Bible Translation Notes (!geneva), John Gill's Expositor (!gill),  Matthew Henry's Concise (!mhc), The People's New Testament (!pnt), Robertson's Word Pictures (!rwp), The Fourfold Gospel (!tfg), Treasury of Scriptural Knowledge (!tsk), Wesley's Bible Notes (!wesley)"
}

#----------------------------------------------------------------------

bind pub - !status pub_status
bind msg - status pub_status

proc pub_status {nick uhost hand channel arg} {
    global von
    
    if {$von==0} {
	putserv "NOTICE $nick :All BibleBot displays are currently off."
    } elseif {$von==1} {
	putserv "NOTICE $nick :All BibleBot displays are currently on."
    } elseif {$von==2} {
	putserv "NOTICE $nick :All BibleBot displays are currently on for ops only."
    } else {
	putserv "NOTICE $nick :Verse displays are currently on for all users, but other BibleBot displays are currently restricted to ops and voiced users."
    }
    return 1
}


bind pub - !history pub_hist
bind msg - history pub_hist

proc pub_hist {nick uhost hand channel arg} {
    global botnick
    catch {exec tail -n 5 /var/log/diatheke.log >& /tmp/fooout.$botnick}
    catch {set foofile [open /tmp/fooout.$botnick]}
    catch {set fooverse [gets $foofile]}
    putserv "NOTICE $nick :Last 5 calls to Diatheke/Tcl BibleBot"
    putserv "NOTICE $nick :$fooverse"
    while {[gets $foofile fooverse] >= 0} {
	putserv "NOTICE $nick :$fooverse"
    }
    catch {close $foofile}
    exec rm /tmp/fooout.$botnick
    return 1
}

#---------------------------------------------------------------------

proc pub_verseon {nick uhost hand channel arg} {
    global von
    if {![matchattr $hand 3] && ![matchattr $hand o]} {
	return 0
    } elseif {$arg=="v"} {
	set von 3
#	putserv "NOTICE $nick :Long Text Display is now on for voiced only!"
    } elseif {$arg=="o"} {
	set von 2
#	putserv "NOTICE $nick :Verse Display is now on for ops only!"
    } else {
	set von 1
#	putserv "NOTICE $nick :All Display is now on!"
    }
    pub_status $nick $uhost $hand $channel $arg
    return 1
}
bind pub - !verseon pub_verseon
bind msg - verseon pub_verseon

proc pub_verseoff {nick uhost hand channel arg} {
    global von
    
    if {![matchattr $hand 3] && ![matchattr $hand o]} {
	return 0
    }
    set von 0
#    putserv "NOTICE $nick :Verse Display is now off!"
    pub_status $nick $uhost $hand $channel $arg
    return 1
}
bind pub - !verseoff pub_verseoff
bind msg - verseoff pub_verseoff

proc dcc_verseoff {hand idx arg} {
    global von
    global whovoff
    if {![matchattr $hand 3] && ![matchattr $hand o]} {
	return 0
    }
    set von 0
    set whovoff $hand
    return 1
}
bind dcc - verseoff dcc_verseoff

proc dcc_verseon {hand idx arg} {
    global von
    if {![matchattr $hand 3] && ![matchattr $hand o]} {
	return 0
    }
    elseif {$arg=="v"} {
	set von 3
    }
    elseif {$arg=="o"} {
	set von 2
    } else {
	set von 1
    }
    return 1
}
bind dcc - verseon dcc_verseon

#sets default von mode
set von 1















