bind pub - !KJV setver_KJV

proc setver_KJV {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver KJV
    pub_lookup $nick $uhost $hand $channel $arg
}

bind pub - !sKJS setver_sKJV

proc setver_sKJV {nick uhost hand channel arg} {
    global botnick chan bibver
    set bibver KJV
    pub_lookups $nick $uhost $hand $channel $arg
}
