/* Methods that are deprecated in SWORD and should be ignored. */

%ignore sword::SWKey::Error();
%ignore sword::SWKey::Persist() const;
%ignore sword::SWKey::Persist(signed char);

%ignore sword::SWModule::AddEncodingFilter(SWFilter*);
%ignore sword::SWModule::AddOptionFilter(SWOptionFilter*);
%ignore sword::SWModule::AddRawFilter(SWFilter*);
%ignore sword::SWModule::AddRenderFilter(SWFilter*);
%ignore sword::SWModule::AddStripFilter(SWFilter*);
%ignore sword::SWModule::CreateKey;
%ignore sword::SWModule::Description;
%ignore sword::SWModule::Direction;
%ignore sword::SWModule::Disp;
%ignore sword::SWModule::Display();
%ignore sword::SWModule::Encoding;
%ignore sword::SWModule::Error();
%ignore sword::SWModule::getSkipConsecutiveLinks();
%ignore sword::SWModule::Index;
%ignore sword::SWModule::Key;
%ignore sword::SWModule::KeyText;
%ignore sword::SWModule::Lang;
%ignore sword::SWModule::Markup;
%ignore sword::SWModule::Name;
%ignore sword::SWModule::processEntryAttributes(bool) const;
%ignore sword::SWModule::ReplaceEncodingFilter(SWFilter*, SWFilter*);
%ignore sword::SWModule::RemoveEncodingFilter(SWFilter*);
%ignore sword::SWModule::ReplaceRenderFilter(SWFilter*, SWFilter*);
%ignore sword::SWModule::RemoveRenderFilter(SWFilter*);
%ignore sword::SWModule::SetKey;
%ignore sword::SWModule::setKey(SWKey const &);
%ignore sword::SWModule::Type;

%ignore sword::VerseKey::AutoNormalize;
%ignore sword::VerseKey::Book;
%ignore sword::VerseKey::Chapter;
%ignore sword::VerseKey::ClearBounds();
%ignore sword::VerseKey::Headings;
%ignore sword::VerseKey::LowerBound;
%ignore sword::VerseKey::Normalize;
%ignore sword::VerseKey::ParseVerseList;
%ignore sword::VerseKey::Testament;
%ignore sword::VerseKey::TestamentIndex() const;
%ignore sword::VerseKey::UpperBound;
%ignore sword::VerseKey::Verse;
